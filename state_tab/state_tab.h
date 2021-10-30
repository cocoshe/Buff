#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <serial.h>
#include "opencv_extend.h"
#include"ArmorDector.h"
#include "camera.h"

class STATE_TAB
{

private:
    struct state_flag
    {
        //time
        int lock_time ;
        int lock_buffer;
        int inv_buffer;
        int inv_buffer_l;
        int inv_buffer_r;

        int lock_inv_buffer;

        //state_ing
        bool is_center ;
        int lock_inv_move;

        //state
        bool is_center_lock;
        int enemy;
        int lock_pre_move;
        int lock_move ;
        int is_inv;
        
        float armor_angle;
        float pre_armor_angle;


    };

    int quene_sum(int n,int*yaw_smooth,int item);
    bool is_Center(cv::Point2f center_point);
    int  lock_Time(int lock_time,bool is_center);
    int  lock_Move(cv::Point2f cur_center);
    bool return_Move(int lock_move,int pre_find_enemy, int find_enemy);
    bool unlock_Buff(int lock_buff);
    void clear();

public:
    STATE_TAB(){clear();}
    ~STATE_TAB(){}
    char tab_State(cv::Point2f center,int find_enemy,int lock_move,float rotation , bool control ,bool SOS);
    cv::Point2f shoot_TAB(cv::Point2f center,char state,int is_move,float arm_len,float angle,float distance);

private:
    state_flag s_Flag;
};