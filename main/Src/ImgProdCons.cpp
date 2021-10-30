#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <typeinfo>
#include <string>
#include "thread"

#include "ArmorDector.h"
#include "general.h"
#include "ArmorDector.h"
#include "opencv_extend.h"
//#include "log.h"
#include "ImgProdCons.h"
#include "img_buffer.h"
#include "general.h"
#include "Timestamp.h"
using namespace std;
using namespace cv;
Point2f getSymmetry(Point2f center ,Point2f point ,double radius)
{
    Point2f aimPoint = Point2f(0,0);
    double dis = sqrt((center.x - point.x) * (center.x - point.x) + (center.y - point.y) * (center.y - point.y));
    double angle  = abs(atan((point.y - center.y) / (point.x - center.x)));
    if((point.x - center.x) < 0 && (point.y - center.y) < 0)
    {
        aimPoint.x =center.x + dis * cos(angle);
        aimPoint.y = center.y + dis * sin(angle);
    }
    else  if((point.x - center.x) > 0 && (point.y - center.y) < 0)
    {
        aimPoint.x =center.x - dis * cos(angle);
        aimPoint.y = center.y + dis * sin(angle);
    }
    else if((point.x - center.x) > 0 && (point.y - center.y) > 0)
    {
        aimPoint.x =center.x - dis * cos(angle);
        aimPoint.y = center.y - dis * sin(angle);
    }
    else if((point.x - center.x) < 0 && (point.y - center.y) > 0)
    {
        aimPoint.x =center.x + dis * cos(angle);
        aimPoint.y = center.y - dis * sin(angle);
    }
    else
    {
        aimPoint = center;
    }
    
    return aimPoint;
}
ImgProdCons::ImgProdCons()
{
    sem_init(&sem_pro , 0 , 0 );
    sem_init(&sem_com , 0 , 1 );
    _trackFlag = 0;
    prePoint=Point2f(0,0);
    center = Point2f(0,0);
    _angleSpeed = 0;
    _task = Serial::AUTO_SHOOT;
    _shootTask = Serial::BUFF_SHOOT;
    _islong=Serial::SHOOT_LONG;
    bool err = serial.InitPort();
    if(err == Serial::USB_CANNOT_FIND)
    {
        //LOG_ERROR << "USB_CANNOT_FIND";
    }
    KF.kalmanInit();



    //初始化相机参数
    //p4psolver.SetCameraMatrix(1432.8,1436.1,352.4,295.1);
    p4psolver.SetCameraMatrix(1787.9,1775.6,555.4,481.4);
    //设置畸变参数
    // p4psolver.SetDistortionCoefficients(-0.0653 , 0.4867 , 0,0,0);
    p4psolver.SetDistortionCoefficients(-0.0711 , -0.3714 , 0,0,0);
    //small armor
    p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1三维坐标的单位是毫米
    p4psolver.Points3D.push_back(cv::Point3f(135, 0, 0));	//P2
    p4psolver.Points3D.push_back(cv::Point3f(60, 135, 0));	//P3
    //p4psolver.Points3D.push_back(cv::Point3f(135, 60, 0));	//P3
    p4psolver.Points3D.push_back(cv::Point3f(0, 60, 0));	//P4
    //cout << "装甲版世界坐标 = " << endl << p4psolver.Points3D << endl;


    // //初始化相机参数
    // //p4psolver.SetCameraMatrix(1432.8,1436.1,352.4,295.1);
    // p4psolver.SetCameraMatrix(1787.9,1775.6,555.4,481.4);
    // //设置畸变参数
    // //p4psolver.SetDistortionCoefficients(-0.0653 , 0.4867 , 0,0,0);
    // p4psolver.SetDistortionCoefficients(-0.0711 , -0.3714 , 0,0,0);
    // //small armor
    // p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1三维坐标的单位是毫米
    // p4psolver.Points3D.push_back(cv::Point3f(0, 400, 0));	//P2
    // p4psolver.Points3D.push_back(cv::Point3f(150, 0, 0));	//P3
    // //p4psolver.Points3D.push_back(cv::Point3f(135, 60, 0));	//P3
    // p4psolver.Points3D.push_back(cv::Point3f(0, 100, 105));	//P4
    // cout << "装甲版世界坐标 = " << endl << p4psolver.Points3D << endl;


}
ImgProdCons::~ImgProdCons()
{
    sem_destroy(&sem_pro);
    sem_destroy(&sem_com);
}
 ImgProdCons* ImgProdCons::getInstance()
{
    instance = new ImgProdCons();
    return instance;
}

/*换摄像头之前的API
void ImgProdCons::Produce()
{
    GX_cam.run();
	while (1)
	{
        src = GX_cam.GX_get_img();
        if(src.empty())
        {
            //LOG_ERROR << "src empty";
            continue;
        }
        else
        {
            sem_wait(&sem_com);
            try
            {
                _buffer.ImgEnterBuffer(src);
            }
            catch (...)
            {
                std::cout << "照片读如出错" << std::endl;
                throw;
            }
            sem_post(&sem_pro);
        }
	}
}
*/

//换摄像头之后的API
void ImgProdCons::Produce()
{
    cv::Mat src;
    //while(true){
        // VideoCapture cap;
        // cap.open("/home/coshe/视频/3.mp4");
       while (!mycamera.open());
       if (!mycamera.setVideoparam()) {cout << "set param fail" << endl; return;}
       while (!mycamera.startStream());
        while (1)
        {
           if (!mycamera.getVideoimage()) continue;
           if (!mycamera.rgbtocv()) continue;
           src = mycamera.getiamge();
            // cap >> src;
            if (src.empty()) {
                break;
            }
            else
            {
                sem_wait(&sem_com);
                try
                {
                    _buffer.ImgEnterBuffer(src);
                }
                catch (...)
                {
                    std::cout << "照片读如出错" << std::endl;
                    throw;
                }
                sem_post(&sem_pro);
            }
        }
    
    //}
	while (!mycamera.closeStream());
}

unsigned short int decode(unsigned char *buff)
{
    if (buff[0] == 'a' && buff[3] == 'b')
    {
        return (unsigned short int)(buff[2] << 8 | buff[1]);  
    }
}

void ImgProdCons::Sense()
{
    while(1)
    {
        unsigned char buff[3];
        fd_set fdRead;
        FD_ZERO(&fdRead);
        FD_SET(serial.getFd(), &fdRead);
        int fdReturn = select(serial.getFd() + 1, &fdRead, 0, 0, nullptr);
        if(fdReturn < 0)
        {
            cout << "select 失败"<<endl;
            exit(1);
        }
        usleep(1000);
        if (FD_ISSET(serial.getFd(), &fdRead))
        {
            bool is_read = serial.ReadData(buff, 3);
            // std::cout << decode(buff) << std::endl;
            // if(buff[0]!= 'a' || buff[2] != 'b')
            // {
            //     std::cout << "串口接收数据错误"  << std::endl;
            //     continue;
            // }
            //puts((const char *)buff);
            if (is_read == false)
            {
                cout  << "读取串口失败" << endl;
                continue;
            }
            for (int i = 0; i < 3; i++)
            {
                // cout << buff[i] << " ";
            }
            // if(buff[0] != 'S' || buff[2] != 'E')
            // {
            //     std::cout << "接受数据错误" <<std::endl;
            //     continue;
            // }
            // if(buff[1]=='B')
            //     cout << "*************************************" << endl;
            _islong = Serial::SHOOT_SHORT;
        }

        // 从电控取得旋转矩阵
        // coordinate.rotation_matrix = coordinate.calculate_head();
        

    }
}

void ImgProdCons::Consume()
{
    bool SOS;
    char shoot_state;

    float pitch;
    float pre_pitch;
    float pre_pre_pitch;

    int buffindex;

    Arm.setEnemyColor(BLUE);

    int cnt = 0;
    double fps = 0;

    while (1)
    {

        fps = (double)getTickCount();

        Mat src;
        sem_wait(&sem_pro);
        try {
            _buffer.GetImage(src);
            cnt++;
        } catch(...) { 
            std::cout << "读取相机图片出错" << std::endl;
            exit(-1);
        }
        sem_post(&sem_com);

/*添加的文本应该是影响到角落里面的识别了
        switch (_task)
        {
            case  Serial::AUTO_SHOOT:
                putText( src, "AUTO_SHOOT", Point(10,30),
		            FONT_HERSHEY_SIMPLEX,0.5, Scalar (0,255,0),2);
                    switch(_shootTask)
                    {
                        case Serial::ARMOR_SHOOT:
                            putText(src, "ARMOR_SHOOT", Point(10,50),
		                        FONT_HERSHEY_SIMPLEX,0.5, Scalar (80,150,80),2);
                            break;
                        case Serial::BUFF_SHOOT:
                            putText(src, "BUFF_SHOOT", Point(10,50),
                                    FONT_HERSHEY_SIMPLEX,0.5, Scalar (80,150,80),2);
                            break;
                    }
                    switch(_islong)
                    {
                        case Serial::SHOOT_LONG:
                            putText(src, "SHOOT_LONG", Point(10,70),
		                        FONT_HERSHEY_SIMPLEX,0.5, Scalar (80,150,80),2);
                            break;
                        case Serial::SHOOT_SHORT:
                            putText(src, "SHOOT_SHORT", Point(10,70),
                                    FONT_HERSHEY_SIMPLEX,0.5, Scalar (80,150,80),2);
                            break;
                    }
                    break;
            case Serial::NO_SHOOT :
                putText( src, "NO_TASK", Point(10,30),
		            FONT_HERSHEY_SIMPLEX,0.5, Scalar (255,0,0),2);
                    break;
        }
*/

        // if (src.size().width != 512 || src.size().height != 384)
        // {
        //     // LOG_ERROR << "size error";
        //     cv::waitKey(1000);
        //     continue;
        // }



        if (!src.empty() && _buffer.get_headIdx() != buffindex)
        {
            buffindex = _buffer.get_headIdx();
            if(_task == Serial::AUTO_SHOOT)
            {
                if(_shootTask == Serial::ARMOR_SHOOT)
                {
                    int findEnemy;
                    Arm.loadImg(src);
                    int type = Arm.getArmorType();
                    findEnemy = Arm.detect();

                    if(findEnemy <= 1)
                    {
                        SOS = 1;
                        uint8_t buff[18];
                        buff[0] = 's';
                        for(int i = 1; i < 17; i++)
                        {
                            buff[i] = '0';
                        }
                        buff[17] = 'e';
                        serial.WriteData(buff, sizeof(buff));
                    }
                    else
                    {

                    // waitKey(0);

                        Point offset = cv::Point(0,0);
                        std::vector<cv::Point2f> t = Arm.getArmorVertex();

                        /*输出点的坐标
                        for (int i = 0; i < t.size(); i++)
                        {
                            cout << "t[" << i << "]" << t[i] << endl;
                        }
                        */

                        cv::Rect r(t[0].x, t[0].y, t[1].x-t[0].x, t[2].y-t[1].y);
                        changeArmorMode(Arm, type);
                        p4psolver.Points2D.push_back(t[0]);	//P1
                        p4psolver.Points2D.push_back(t[1]);	//P2
                        p4psolver.Points2D.push_back(t[2]);	//P3
                        p4psolver.Points2D.push_back(t[3]);	//P4
                        //cout << "test1:图中特征点坐标 = " << endl << p4psolver.Points2D << endl;
                        if (p4psolver.Solve(PNPSolver::METHOD::CV_P3P) != 0)
                        {
                            cout << "距离解算错误" << endl;
                        }
                        double distance = -p4psolver.Position_OcInW.z / 1000;
                        if(Arm.getArmorType() == BIG_ARMOR)
                        {
                            distance = 2 * distance -0.1 + 0.8;
                        }
                        else
                        {
                            distance = distance;
                        }
                        distance *= 2.7;

                        /*更改pnp解算距离的参数*/
                        distance /= 1.6;
                        /**/
                        // 计算世界坐标
                        //vector<double> pw = coordinate.pc_to_pw(center, distance);


                        if (p4psolver.Solve(PNPSolver::METHOD::CV_P3P) == 0)
                            cout << "目标距离=" << distance << "米" << endl;
                        prePoint = center;
                        center = Arm.getCenterPoint(Arm);
                        shoot_state = s_TAB.tab_State(center, 1, 0, Arm.get_angle(), 1, SOS);
                        cout << "SOS " << SOS << endl;

                        // offsetSolve solver(center, distance);
                        // offset = solver.getoffset();


                        Point2f predictPoint = KF.kalmanPredict(center.x, center.y);
                        float r1 = abs(predictPoint.x - center.x);
                        float r2 = abs(predictPoint.y - center.y);
                        if (sqrt(r1 * r1 + r2 * r2) > Arm.getArmorLength(Arm))
                        {
                            predictPoint = prePoint;  
                        } 
                        if (sqrt(r1 * r1 + r2 * r2) < Arm.getArmorLength(Arm) / 1.5)
                        {
                            predictPoint = center;
                        }
                        if (prePoint.x !=0 && prePoint.y != 0)
                            circle(src, prePoint, 4, Scalar(0, 255, 255), 2);
                        if (center.x !=0 && center.y != 0)
                            circle(src, center, 1/*Arm.getArmorLength(Arm)*/, Scalar(0, 255, 0), 2);
                        cout << center << endl;
                        // if (predictPoint.x != 0 && predictPoint.y != 0)
                        //     circle(src, getSymmetry(center, predictPoint, Arm.getArmorLength(Arm)), 4, Scalar(0, 0, 255), 4);
                        int armorMode = Arm.getArmorType();

                        if (Arm.get_angle() < 2.5 || Arm.get_angle() > 177.5)
                            pitch = center.y;

                        if (SOS) pre_pitch = pre_pre_pitch;
                        else pre_pre_pitch = pitch;

                        // if (shoot_state == '4' || shoot_state == '5')
                        // {
                        //     if(Arm.get_angle() < 2.5 || Arm.get_angle() > 177.5)
                        //     {
                        //         serial.sendBoxPosition(center, serial, 1, distance, armorMode, offset, shoot_state);
                        //     }
                        //     else 
                        //     {
                        //         serial.sendBoxPosition(cv::Point2f(480 + 15, pre_pitch), serial, 1, distance, armorMode, offset, shoot_state);
                        //     }
                        // }
                        // else serial.sendBoxPosition(center, serial, 1, distance, armorMode, offset, shoot_state);
                        
                        serial.sendBoxPosition(center, serial, 1, distance, armorMode, offset, shoot_state);


                        SOS = 0;

                        //debug
                        string dis = "distance : ";
                        switch(Arm.getArmorType())
                        {
                            case BIG_ARMOR:
                                putText(src, "BIG_ARMOR", Point(80, 368),
                                    FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
                                break;
                            case SMALL_ARMOR:
                                putText(src, "SMALL_ARMOR", Point(80, 368),
                                    FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
                                break;
                        }
                        //circle(src,center, 2, Scalar(255, 0, 255), 2);
                        dis += to_string(distance);
                        putText(src, dis.c_str(), Point(240, 368),
                            FONT_HERSHEY_SIMPLEX, 0.7, Scalar (0, 0, 255), 3);
                        p4psolver.Points2D.clear();
                            cv::rectangle(src, r, Scalar(0, 255, 255), 3);
                    }

                    /**/
                    // cout << endl << endl;
                    /**/
                    
                }
                else if (_shootTask == Serial::BUFF_SHOOT)
                {
                   // Detect detect;
                   if (detect.detect_new(src) == false)
                   {
                    //    continue;
                   }
                }
            }
            else if (_task == Serial::NO_TASK)
            {

                //std::cout << "change mode to NO_TASK" << std:: endl;

            }
            // waitKey(1);
            cv::imshow("result", src);
            cv::waitKey(1);
            // if (cnt <= 1600)
            // {
            //     cv::waitKey(1);
            // }
            // else 
            // {
            //     cv::waitKey(20);
            // }
            
            
        }
        fps = ((double)cv::getTickCount() - fps) / cv::getTickFrequency();
        fps = 1 / fps;
        cout << "**********" << endl << "fps=" << fps << "**********" << endl;
    }
}

void ImgProdCons::changeArmorMode(ArmorDetector &Arm, int type)
{
     if(Arm.getArmorType() != type)
     {
        switch(Arm.getArmorType())
        {
            case SMALL_ARMOR:
                std::cout << "change" << endl;
                p4psolver.Points3D.clear();
                p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1三维坐标的单位是毫米
                p4psolver.Points3D.push_back(cv::Point3f(135, 0, 0));	//P2
                p4psolver.Points3D.push_back(cv::Point3f(60, 135, 0));	//P3
                //p4psolver.Points3D.push_back(cv::Point3f(135, 60, 0));	//P3
                p4psolver.Points3D.push_back(cv::Point3f(0, 60, 0));	//P4
                break;
            case BIG_ARMOR:
                std::cout << "change" << endl;
                p4psolver.Points3D.clear();
                p4psolver.Points3D.push_back(cv::Point3f(0, 0, 0));		//P1三维坐标的单位是毫米
                p4psolver.Points3D.push_back(cv::Point3f(230, 0, 0));	//P2
                p4psolver.Points3D.push_back(cv::Point3f(60, 230, 0));	//P3
                //p4psolver.Points3D.push_back(cv::Point3f(230, 60, 0));	//P3
                p4psolver.Points3D.push_back(cv::Point3f(0, 60, 0));	//P4
                break;
        }
    }
}
thread ImgProdCons::ConsumeThread()
{
    return thread(&ImgProdCons::Consume, this);
}
thread ImgProdCons::ProduceThread()
{
    return thread(&ImgProdCons::Produce, this);
}
thread ImgProdCons::SenseThread()
{
    return thread(&ImgProdCons::Sense, this);
}
bool distanceSolve(double dis)
{
    if (dis < 0.4 || dis > 4) return false;
}

