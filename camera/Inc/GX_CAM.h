#include "DxImageProc.h"
#include "GxIAPI.h"
#include <opencv2/opencv.hpp>
#include "Timestamp.h"
#include <iostream>
using namespace std;

#define ACQ_TRANSFER_SIZE       (64 * 1024)
#define ACQ_TRANSFER_NUMBER_URB 64          
class GX_CAM
{
public:

    GX_CAM()
    {


        hDevice = NULL;
        status = GX_STATUS_SUCCESS;
        
        open_param_.accessMode = GX_ACCESS_EXCLUSIVE;   
        open_param_.openMode = GX_OPEN_INDEX;           //相机打开方式
        open_param_.pszContent = "1";   
        
        nDeviceNum = 0;     //设备num
        nBufferNum = 10;    //缓冲区num
        bImplementPacketSize = false;
        unPacketSize = 0;
        nPayLoadSize = 0;   

        frame_buffer_ = NULL;

        nWidth = 960;
        nHeight = 800;
        nOffsetX = 480;
        nOffsetY = 200;
        
        nBinningH = 2;
        nBinningV = 2;
        nDecimationH= 2;
        nDecimationV= 2;   

        nAcqMode = GX_ACQ_MODE_CONTINUOUS;  //采集模式
        acq_spd = 200;  //采集帧率

        g_nPayloadSize = 0;

        time = 6000; //曝光时间
    }
    
    ~GX_CAM()
    {
        //status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);   //停采命令，与开采命令相对
        UnPreForAcquisition();
        //free(frame_buffer_->pImgBuf);    //释放缓冲区内存
        status = GXStreamOff(hDevice);  
        status = GXCloseDevice(hDevice);
        status = GXCloseLib();
    }

    bool GX_open()
    {
        //获取库函数版本号
        GXGetLibVersion();
        //初始化库->枚举设备->打开第一个设备
        status = GXInitLib();
        if(status != GX_STATUS_SUCCESS)
        {
            std::cout << "init false" << endl;
            return false;
        }
        
        status = GXUpdateDeviceList(&nDeviceNum, 1000);
        if(status != GX_STATUS_SUCCESS || nDeviceNum == 0)
        {
            std::cout << "update_device_list false" << endl;
            return false;
        }
        
        //打开列表里的第一个设备
        status = GXOpenDeviceByIndex(1, &hDevice);
        if(status != GX_STATUS_SUCCESS)
        {
            std::cout <<"open_device false" << endl;
            return false;
        }

        //获取设备的永久ID信息
        //status = GXGetDevicePersistentIpAddress(hDevice);
        return true;
    }

    //设置相机参数
    bool GX_init()
    {    
        //设置采集模式_默认为连续采集模式
        status = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
        std::cout << "采集模式ini_" << status  << endl;

        //设置采集帧率
        status = GXSetFloat(hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, acq_spd);
        std::cout << "采集帧率ini_" << status  << endl;

        //设置曝光
        status = GXSetFloat(hDevice, GX_FLOAT_EXPOSURE_TIME,time);            
        std::cout << "曝光ini_" << status  << endl;
        
        //设置buffer个数
        // status = GXSetAcqusitionBufferNumber(hDevice, nBufferNum);
        // std::cout << "设置buffer_ini_" << status  << endl;
        
        //设置传输块
        bool bStreamTransferSize = false;
        bool bStreamTransferNumberUrb = false;
        status = GXIsImplemented(hDevice, GX_DS_INT_STREAM_TRANSFER_SIZE, &bStreamTransferSize);
        status = GXIsImplemented(hDevice, GX_DS_INT_STREAM_TRANSFER_NUMBER_URB, &bStreamTransferNumberUrb);
        if(bStreamTransferSize)
        {
            //设置数据块传输大小
            status = GXSetInt(hDevice, GX_DS_INT_STREAM_TRANSFER_SIZE, ACQ_TRANSFER_SIZE);
            std::cout << "传输块大小ini_" << status  << endl;

        }
        if(bStreamTransferNumberUrb)
        {
            //设置数据传输块的数量
            status = GXSetInt(hDevice, GX_DS_INT_STREAM_TRANSFER_NUMBER_URB, ACQ_TRANSFER_NUMBER_URB);
            std::cout << "传输块数量ini_" << status  << endl;

        }

        

        //设置白平衡_关闭
        status = GXSetEnum(hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);
        std::cout << "白平衡ini_" << status  << endl;
        
        //为像素格式转换分配内存
        status = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &g_nPayloadSize);        
        g_pRGBImageBuf = new unsigned char[g_nPayloadSize * 3]; 
        std::cout << "格式转换内存分配ini_" << status  << endl;
        //g_pRaw8Image = new unsigned char[g_nPayloadSize];
        
        //获取颜色通道
        status = GXGetEnum(hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &g_i64ColorFilter);
        std::cout << "获取颜色通道ini_" << status  << endl;

        //设置ROI
        status = GXSetInt(hDevice, GX_INT_WIDTH, nWidth);
        status = GXSetInt(hDevice, GX_INT_HEIGHT, nHeight);
        status = GXSetInt(hDevice, GX_INT_OFFSET_X, nOffsetX);
        status = GXSetInt(hDevice, GX_INT_OFFSET_Y, nOffsetY);

        status = GXStreamOn(hDevice);
        if(status == GX_STATUS_SUCCESS)
            return true;
        else
            return false;
    }
    
    //创建采集线程

    //获取图像
    cv::Mat GX_get_img()
    {   

        status = GXDQBuf(hDevice, &frame_buffer_, 1000);
        status = DxRaw8toRGB24((unsigned char*)frame_buffer_->pImgBuf, g_pRGBImageBuf, frame_buffer_->nWidth, frame_buffer_->nHeight,
                              RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(g_i64ColorFilter), false);        
        src_ = cv::Mat (cv::Size(frame_buffer_->nWidth,frame_buffer_->nHeight),CV_8UC3,g_pRGBImageBuf);
        Mat dst_;
        cv::cvtColor(src_,dst_,cv::COLOR_RGB2BGR);
        status = GXQBuf(hDevice, frame_buffer_);

        time_ = _time.getElapsedTimeInMilliSec();
        std::cout << "read_img_success" << "**" << "time_" << time_ << "ms" << "**" << frame_buffer_->nWidth << 'x' << frame_buffer_->nHeight << endl;
        _time.update();
        
        return dst_;

    }
    
    void PreForAcquisition()
    {

        return;
    }

    void UnPreForAcquisition()
    {
        //Release resources
        if (g_pRGBImageBuf != NULL)
        {
            delete[] g_pRGBImageBuf;
            g_pRGBImageBuf = NULL;
        }

        return;
    }

    void run()
    {   
        thread_status = GX_open();
        thread_status = GX_init();
    }

private:
    GX_DEV_HANDLE                   hDevice;    //句柄        
    GX_STATUS                       status;     //运行状态
    uint32_t                        nDeviceNum; //设备num
    uint64_t                        nBufferNum; //采集buffer
    bool                            bImplementPacketSize;    //流通道包长属性
    uint32_t                        unPacketSize;
    int64_t                         nPayLoadSize;
    bool                            thread_status;
    int64_t                         nAcqMode;   //采集模式
    int                             acq_spd;

    cv::Mat                         src_;
    CELLTimestamp                   _time;
    double                          time_;
    
    //相机参数
    GX_OPEN_PARAM                   open_param_;
    GX_DEVICE_BASE_INFO             device_base_info_;      //相机base信息
    GX_FRAME_DATA                   frame_data_;            //每帧数据
    GX_FRAME_CALLBACK_PARAM         frame_callback_data_;   //回调帧数据
    PGX_FRAME_BUFFER                frame_buffer_;          //缓冲帧数据
    
    //ROI
    int64_t                         nWidth;
    int64_t                         nHeight;
    int64_t                         nOffsetX;
    int64_t                         nOffsetY;
    int64_t                         nBinningH;
    int64_t                         nBinningV;
    int64_t                         nDecimationH;
    int64_t                         nDecimationV;

    int64_t                         g_nPayloadSize;
    unsigned char* g_pRGBImageBuf  = NULL;               ///< Memory for RAW8toRGB24
    unsigned char* g_pRaw8Image = NULL;                 ///< Memory for RAW16toRAW8
    int64_t g_i64ColorFilter = GX_COLOR_FILTER_NONE;    ///< Color filter of device

    float                           time; //曝光时间

};

