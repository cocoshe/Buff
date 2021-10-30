#include <iostream>
#include <math.h>
#include <cmath>
#include "serial.h"
#include "DxImageProc.h"
#include "GxIAPI.h"
#include "Timestamp.h"
#include "thread"


using namespace std;

struct Cam_pose
{
    float x = 0;
    float y = 0;
    float z = 0;

    float radius;
    
    float yaw;
    float pitch;

    float pre_yaw;
    float pre_pitch;
};

struct Shoot_pose
{
    float x;
    float y;
    float z;
    
    float radius;

    float yaw;
    float pitch;

    float dis;    
};

struct Target_pose
{
    float x;
    float y;
    float z;
    
    float cir_spd;
    float line_spd;
    float dis;
};


class Spin
{
public:
    Spin();
    ~Spin();

    Shoot_pose cal_target_pose();
    void sent_spin_info();
    void get_spin_info();
    void test();

    std::thread Sense();
    std::thread Test();
    std::thread Sent_info();

private:
    Serial serial;
    Cam_pose cam_pose;
    Shoot_pose shoot_pose;
    Target_pose target_pose;
    float cam_shoot_dis;
    CELLTimestamp _tTime2;
    PGX_FRAME_BUFFER pFrameBuffer;
    GX_DEV_HANDLE hDevice = NULL;
    GX_STATUS status;

};



