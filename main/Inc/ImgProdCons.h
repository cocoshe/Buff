#ifndef IMGPRODCONS_H_
#define IMGPRODCONS_H_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <typeinfo>
#include <semaphore.h>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "ArmorDector.h"
#include "general.h"
#include "ArmorDector.h"
#include "opencv_extend.h"
#include "serial.h"
//#include "log.h"
#include "img_buffer.h"
#include "camera.h"
#include "serial.h"
#include "Buff.h"
#include "PNPSolver.h"
#include "offsetSolve.h"
#include "GX_CAM.h"
#include "state_tab.h"

#include "coordinate.hpp"


class ImgProdCons
{
public:

    static ImgProdCons* getInstance();

    /*
     * @Brief: Receive self state from the serail port, update task mode if commanded
     */
    void Sense();

	/*
    * @Brief: keep reading image from the camera into the buffer
	*/
	void Produce();

	/*
    * @Brief: run tasks
	*/
	void Consume();

    void kalmanFilterInit();

    Point2f  kalmanPredict(Point nowCenter , int measureNum);

    void changeArmorMode(ArmorDetector  &Arm , int type);
    
    thread ProduceThread();

    thread ConsumeThread();

    thread SenseThread();
private:

    ImgProdCons();
    ~ImgProdCons();

private:
    Mat src;
    GX_CAM GX_cam;
    static ImgProdCons * instance;
    /* Camera */
    Mycamera mycamera;

    /* Buffer */
    ImgBuffer _buffer;

    /* Serial */
    Serial serial;

    /* Angle solver */
    PNPSolver p4psolver;

    /* Armor detector */
    ArmorDetector Arm;
    int _trackFlag;
    /* Buff detector */
    Detect detect;


    /* Coordinate */
    Coordinate coordinate;



    sem_t sem_pro;
    sem_t sem_com;

    //1.kalman filter setup     
    MykalmanKF KF;
    Point2f prePoint;
    Point2f center;
    int i = 0;
    int k = 0;
    int state = 0;
    std::mutex _mutex;
    volatile  int _task;
    volatile int _shootTask;
    volatile int _islong;
    volatile float _angleSpeed;
    CELLTimestamp _tTime2;
    CELLTimestamp _tTime;


    STATE_TAB s_TAB;






};
#endif // !IMGPRODCONS_H_

