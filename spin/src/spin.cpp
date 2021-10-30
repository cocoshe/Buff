#include "spin.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include "serial.h"

using namespace std;

Spin::Spin()
{
    //serial.InitPort();
}

Spin::~Spin()
{
    GXStreamOff(hDevice);
    GXCloseDevice(hDevice);
}



//通信
void Spin::get_spin_info()
{
    serial.InitPort();
    while(1)
    {   
        unsigned char buff[7];
        fd_set fdRead;
        FD_ZERO(&fdRead);
        FD_SET(serial.getFd(), &fdRead);
        int fdReturn = select(serial.getFd() + 1, &fdRead, 0, 0, nullptr);
        if (fdReturn <0)
        {
            cout << "select 失败" << endl;
            continue;
        }
        if (FD_ISSET(serial.getFd(), &fdRead))
        {
            bool is_read=serial.ReadData(buff, 7);
            if (is_read==false)
            {
                cout  << "读取串口失败" << endl;
                continue;
            }
            if (buff[0] == 0xAA && buff[7] == 0xBB)  
            {
                cam_pose.yaw = buff[1];
                cam_pose.pitch = buff[3];
                target_pose.dis = buff[5]; 
            }
            else            
            {
                std::cout << "接受数据错误" << std::endl;
                continue;
            }    
        }
    }
}

//两种预测方法：    uint8_t buff[17];
//一种根据target的实时位姿变化对其运动轨迹进行预测
//第二种方法比较精准，但同时运算量应该也相对较大（待检验），对测距要求也比较高
Shoot_pose Spin::cal_target_pose()
{
    target_pose.dis = 1;
    target_pose.x = cos(cam_pose.pitch) * target_pose.dis * cos(cam_pose.yaw);
    target_pose.y = cos(cam_pose.pitch) * target_pose.dis * sin(cam_pose.yaw);
    target_pose.z = -target_pose.dis * sin(cam_pose.pitch);

    Shoot_pose shoot_pose;    
    float dis_XY = sqrt((shoot_pose.x - target_pose.x)*(shoot_pose.x - target_pose.x) + (shoot_pose.y - target_pose.y)*(shoot_pose.y - target_pose.y)); 
    shoot_pose.yaw = atan((float)(target_pose.y / (target_pose.x -  shoot_pose.x)));
    shoot_pose.pitch = atan((float)(dis_XY/(-target_pose.z)));
    return shoot_pose;
}


void Spin::sent_spin_info()
{   while(1)
    {
        //int is_succ_serial = serial.sendTarget(serial,shoot_pose.yaw, shoot_pose.pitch,shoot_pose.dis, 1 , 1);
        int is_succ_serial = serial.sendTarget(serial,3500, 3000, 8, 1 , 1);

        if (is_succ_serial == 0)
        {
            // cout << "false is_serial" << endl;
            continue;
        }
    }
}


void Spin::test()
{
    cv::Mat src;
    GX_STATUS status = GX_STATUS_SUCCESS;
    uint32_t nDeviceNum = 0;
    //初始化库
    status = GXInitLib();
    if (status != GX_STATUS_SUCCESS)
    {
        return;
    }
    //枚举设备列表
    status = GXUpdateDeviceList(&nDeviceNum, 1000);
    if ((status != GX_STATUS_SUCCESS) || (nDeviceNum <= 0))
    {
        return;
    }

    uint32_t entrynums;
    status = GXGetEnumEntryNums(hDevice, GX_ENUM_PIXEL_FORMAT, &entrynums);
    // cout << entrynums << endl;
    int64_t nPixelFormat = 0;
    status = GXGetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, &nPixelFormat);
    //cout << nPixelFormat << endl;
    //nPixelFormat = GX_PIXEL_FORMAT_BAYER_BG10;
    //status = GXSetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, nPixelFormat);

    //打开第一个设备
    status = GXOpenDeviceByIndex(1, &hDevice);
    if (status == GX_STATUS_SUCCESS) 
    {
        std::cout << "打开设备_ok" << std::endl;        
    }
    else 
    {   
        cout << "打开设备失败" << endl;
        return;
    }

    while(1)
    {
        //读取图片
        double time = _tTime2.getElapsedTimeInMilliSec();
        cout << "time2 :  " << time << "ms" << endl;
        _tTime2.update();

        status = GXStreamOn(hDevice);
        if (status == GX_STATUS_SUCCESS)
        {
            //调用 GXDQBuf 取一帧图像
            status = GXDQBuf(hDevice, &pFrameBuffer, 1000);
            if (status == GX_STATUS_SUCCESS)
            {
                if (pFrameBuffer->nStatus == GX_FRAME_STATUS_SUCCESS)
                {
                    int width = pFrameBuffer->nWidth;
                    int height = pFrameBuffer->nHeight;
                    src = cv::Mat(cv::Size(width, height), CV_8UC1, (void*)pFrameBuffer->pImgBuf, cv::Mat::AUTO_STEP);//转化为OpenCV格式便于处理和显示
                    cv::imshow("a",src);
                    cv::waitKey(1);
                }
            }
        
        }
    }
}

thread Spin::Sense()
{
    return thread(&Spin::get_spin_info,this);
}

thread Spin::Test()
{
    return thread(&Spin::test,this);
}

thread Spin::Sent_info()
{
    return thread(&Spin::sent_spin_info,this);
}