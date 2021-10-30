#include "state_tab.h"
using namespace std;

void STATE_TAB::clear()
{
    s_Flag.lock_time = 0;
    s_Flag.lock_buffer = 0;
    
    s_Flag. inv_buffer_l = 0;
    s_Flag. inv_buffer_r = 0;
    s_Flag.inv_buffer = 0;

    s_Flag.lock_inv_buffer = 0;

    s_Flag.is_center = 0;
    
    s_Flag.is_center_lock = 0;
    s_Flag.enemy = -1;

    s_Flag.lock_move = 0;   
    s_Flag.lock_pre_move = 0;

    s_Flag.lock_inv_move = 0;
    s_Flag.is_inv = 0;     
    
    s_Flag.armor_angle = 0;
    s_Flag.pre_armor_angle = 0;

}

int STATE_TAB::quene_sum(int n,int*yaw_smooth,int item)
{
	//均值滤波
	int cs_yaw_total=0;
	int cs_yaw_after=0;
	static int smoothf=0;
	for(int i=0;i<n;i++)
	{
	if(smoothf==i)
		{
			yaw_smooth[i]=item;
			if(i==n-1)
			{
				smoothf=0;
			}
			else
			{
			smoothf++;
			}
			
		  break;
		}
	}
	 for(int j=0;j<n;j++)
	{
		cs_yaw_total+=yaw_smooth[j];
	}     
	return cs_yaw_total;
}

bool STATE_TAB::is_Center(cv::Point2f center_point)
{
    int over_dis = 35;
    cv::Point2f cur_point = cv::Point2f(MAT_WIDTH/2,center_point.y);
    float center_distance = cvex::distance(center_point,cur_point);
    if (center_distance > over_dis) return false;
    else return true;
}

int STATE_TAB::lock_Time(int lock_time,bool is_center)
{
    if(is_center)   lock_time ++;
    else lock_time = 0;
    return lock_time;
}

int STATE_TAB::lock_Move(cv::Point2f cur_center)
{
    int lock_move_ = 10;
    float move = cur_center.x - MAT_WIDTH/2;
    if(move > lock_move_) return 2;
    else if(move  <  -lock_move_)   return 1;
    else return 0;
}

bool STATE_TAB::return_Move(int lock_move,int pre_find_enemy, int find_enemy)
{
    if(find_enemy = 0 && pre_find_enemy > 0)    return true;
    else return false;
}

bool STATE_TAB::unlock_Buff(int lock_buff)
{
    int nice_buff = 35;
    if(lock_buff > nice_buff)   return true;
    else return false;
}

//返回状态
/*
    0 / 无敌人
    取负数为反陀螺——-1/-2
    取正数为平滑跟踪——1/2/3
*/
char STATE_TAB::tab_State(cv::Point2f center,int find_enemy,int lock_move,float rotation, bool control,bool SOS)
{
    if(!control) {clear(); return '2';}
    //data
    s_Flag.enemy = find_enemy;
    if(rotation > 175 && rotation < 5 && s_Flag.is_inv == 0 )    s_Flag.lock_inv_move = lock_move;

    //插入陀螺判定条件
    
    if(rotation != 0)    
    {
        s_Flag.pre_armor_angle = s_Flag.armor_angle;
        s_Flag.armor_angle = rotation;
        float angle_if = abs(s_Flag.pre_armor_angle - s_Flag.armor_angle);
        cout <<  "angle_if"   << angle_if << endl;

        if(s_Flag.lock_inv_buffer > 50)
        {
                s_Flag.lock_inv_buffer = 0;
                s_Flag.inv_buffer = 0;

        }

        if(s_Flag.lock_inv_buffer <= 50 && s_Flag.is_inv == 0)
        {
            s_Flag.lock_inv_buffer++;
            if(angle_if > 100 && s_Flag.pre_armor_angle < 90 && s_Flag.armor_angle > 90)
            {
                if(SOS) s_Flag.inv_buffer++;
                else    s_Flag.inv_buffer--;               
                s_Flag.lock_inv_buffer = 0;
            }
            if(s_Flag.inv_buffer > 3)
            {
                    s_Flag.is_inv = -1;
                    s_Flag.inv_buffer= 0;
                    s_Flag.lock_inv_buffer = 0;
            }

            if(angle_if > 100 && s_Flag.pre_armor_angle >  90 && s_Flag.armor_angle < 90)
            {
                if(SOS) s_Flag.inv_buffer--;
                else    s_Flag.inv_buffer++;
                s_Flag.lock_inv_buffer = 0;
            }
            if(s_Flag.inv_buffer  < -3)
            {
                    s_Flag.is_inv = 1;
                    s_Flag.inv_buffer = 0;
                    s_Flag.lock_inv_buffer = 0;
            }

        }
    

        if(s_Flag.is_inv != 0)
        {
            if(angle_if < 100)
            {
                s_Flag.inv_buffer++;
            }
            else s_Flag.inv_buffer = 0;
            
            if(s_Flag.inv_buffer > 100)
            {
                s_Flag.is_inv = 0;
                s_Flag.inv_buffer = 0;
            }
        }
    }

    if(s_Flag.is_inv == 0)
    {
        s_Flag.lock_pre_move = s_Flag.lock_move;
        s_Flag.lock_move = lock_move;
        if(s_Flag.is_center_lock == false)
        {
            s_Flag.is_center = is_Center(center);   
            s_Flag.lock_time = lock_Time(s_Flag.lock_time ,is_Center(center));
            if(s_Flag.lock_time > 30)    s_Flag.is_center_lock = true;
        }

        if(s_Flag.is_center_lock)
        {
            //debug
            s_Flag.lock_time++;

            bool is_unlock = unlock_Buff(s_Flag.lock_buffer);

            if(is_unlock)   s_Flag.lock_move = lock_move;
            
            if(s_Flag.lock_pre_move !=  s_Flag.lock_move)       s_Flag.lock_buffer = 0;
            s_Flag.lock_buffer++;    
        }               
        else    s_Flag.lock_buffer = 0;
    }

    //debug
    cout <<  "enemy" << s_Flag.enemy << endl;
    cout <<  "inv_buffer" << s_Flag.inv_buffer<< endl;
    cout <<  "lock_inv_buffer" << s_Flag.lock_inv_buffer << endl;
    cout <<  "is_inv" << s_Flag.is_inv << endl;
    cout <<  "pre_armor_angle" << s_Flag.pre_armor_angle << endl;
    cout <<  "armor_angle" << s_Flag.armor_angle << endl;
    cout <<  "lock_inv_move" << s_Flag.lock_inv_move << endl;
    cout <<  "lock_move" << lock_move << endl;



    //return
    if(s_Flag.enemy > 0)
    {
        if(s_Flag.is_inv == 0)
        {
            if(s_Flag.lock_move == 1)    return '3';
            if(s_Flag.lock_move == 0)    return '2';
            if(s_Flag.lock_move == -1)    return '1';
        }
        else
        {
            if(s_Flag.is_inv == -1)     return '4';
            if(s_Flag.is_inv == 1)      return '5';
        }
    }
    else if(s_Flag.is_inv != 0) return s_Flag.is_inv;
}

cv::Point2f STATE_TAB::shoot_TAB(cv::Point2f center,char state,int is_move,float arm_len,float angle,float distance)
{
    cv::Point2f target = center;
    if(state == '5')     
    {
        // if(is_move == -1)
        //     target.x = center.x + arm_len * 0.5;
        // else if(is_move == 1)
        //     target.x = center.x + arm_len * 0.8;
        // else
            target.x = center.x + arm_len *  0.65;
    }
    if(state == '4')
    {
    //     if(is_move == -1)
    //         target.x = center.x - arm_len * 0.8;
    //     else if(is_move == 1)
    //         target.x = center.x - arm_len * 0.5;
    //     else
            target.x = center.x - arm_len *  0.65;
    }
    // if(state == '1')      target.x = center.x - arm_len * 0.75;
    // if(state == '3')      target.x = center.x + arm_len * 0.75;
    return target;
}