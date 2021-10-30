#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
using namespace cv;
using namespace std;

class Coordinate {
public:
    Coordinate();
    Coordinate(int dx, int dy, int u0, int v0, int f);

    // 旋转矩阵 3*4
    vector<vector<double>> rotation_matrix;

    /** TODO:已留出参数接口
     * @brief 从电控收取编码器的值计算云台坐标
     * @param
     * @return 3*4的矩阵，直接用rotation_matrix接收
    */
    vector<vector<double>> calculate_head();

    /**
     * @brief 计算世界坐标
     * @param center 目标在图像中的坐标
     * @param distance 目标距离
     * @param rotation_matrix 旋转矩阵
     * @return 返回目标的世界坐标
    */
    vector<double> pc_to_pw(Point2f center, double distance);


private:
    // 判断是否能解算
    bool flag;

    // 分别为像素在XY轴方向下的物理尺寸，单位为mm，图像坐标的主点
    double dx, dy, u0, v0;

    // f为有效焦距
    double f;

};



#endif
