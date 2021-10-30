#include "coordinate.hpp"
#include <iostream>
using namespace std;
using namespace std;

Coordinate::Coordinate() {
    cout << "no info, can't calculate" << endl;
    flag = false;
}

Coordinate::Coordinate(int dx, int dy, int u0, int v0, int f) {
    this->dx = dx;
    this->dy = dy;
    this->u0 = u0;
    this->v0 = v0;
    this->f  = f;
    flag = true;
}

vector<vector<double>> Coordinate::calculate_head() {

    if (flag == false)
    {
        // return ;
    }
    

    vector<vector<double>> tmp;

    vector<double> rows;
    rows.push_back(1);
    rows.push_back(1);
    rows.push_back(1);
    rows.push_back(1);
    
    tmp.push_back(rows);
    tmp.push_back(rows);
    tmp.push_back(rows);
    return tmp;
}

vector<double> Coordinate::pc_to_pw(Point2f center, double distance) {
    // 强转数据类型
    double u, v;
    u = (double)center.x;
    v = (double)center.y;

    // 世界坐标 TODO:根据敌我的高度解算Zw，当前只是根据pnp解算的距离当成Zw
    double Xw, Yw, Zw;
    Zw = distance;

    // 解算方程前面的系数
    double tmp11, tmp12, tmp13, tmp14;
    double tmp21, tmp22, tmp23, tmp24;

    vector<vector<double>> &r = rotation_matrix;

    tmp11 = (f / dx) * r[1 - 1][1 - 1] + (u0 - u) * r[3 - 1][1 - 1];
    tmp12 = (f / dx) * r[1 - 1][2 - 1] + (u0 - u) * r[3 - 1][2 - 1];
    tmp13 = (f / dx) * r[1 - 1][3 - 1] + (u0 - u) * r[3 - 1][3 - 1];
    tmp14 = (f / dx) * r[1 - 1][4 - 1] + (u0 - u) * r[3 - 1][4 - 1];

    tmp21 = (f / dy) * r[2 - 1][1 - 1] + (v0 - v) * r[3 - 1][1 - 1];
    tmp22 = (f / dy) * r[2 - 1][2 - 1] + (v0 - v) * r[3 - 1][2 - 1];
    tmp23 = (f / dy) * r[2 - 1][3 - 1] + (v0 - v) * r[3 - 1][3 - 1];
    tmp24 = (f / dy) * r[2 - 1][4 - 1] + (v0 - v) * r[3 - 1][4 - 1];

    Xw = -((tmp13 * tmp22 - tmp12 * tmp23) * Zw) - (tmp14 * tmp22 - tmp12 * tmp24) / (tmp11 * tmp22 - tmp12 * tmp21);
    Yw = -((tmp13 * tmp21 - tmp11 * tmp23) * Zw) - (tmp14 * tmp21 - tmp11 * tmp24) / (tmp12 * tmp21 - tmp11 * tmp22);

    vector<double> pw;
    pw.push_back(Xw);
    pw.push_back(Yw);
    pw.push_back(Zw);
    return pw;
}

