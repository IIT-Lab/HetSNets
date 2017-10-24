//
// Created by lee on 17-8-1.
//
#include <math.h>
#include <iostream>

#include "Mymath.h"
#include "Global.h"

//定义实现复数乘实数的函数
deviceLocation complexIntMul(deviceLocation m, double n)
{
    deviceLocation z;
    z.dXPoint = m.dXPoint*n;
    z.dYPoint = m.dYPoint*n;
    return z;
}

//定义复数乘法函数
deviceLocation complexMul(deviceLocation m, deviceLocation n)
{
    deviceLocation u;
    u.dXPoint = ((m.dXPoint * n.dXPoint) - (m.dYPoint * n.dYPoint));
    u.dYPoint = ((m.dYPoint * n.dXPoint) + (m.dXPoint * n.dYPoint));
    return u;
}

//定义复数加用算函数
deviceLocation complexAdd(deviceLocation m, deviceLocation n)
{
    deviceLocation z;
    z.dXPoint = m.dXPoint + n.dXPoint;
    z.dYPoint = m.dYPoint + n.dYPoint;
    return z;
}

//定义复数减法函数
deviceLocation complexSub(deviceLocation m, deviceLocation n)
{
    deviceLocation z;
    z.dXPoint = m.dXPoint - n.dXPoint;
    z.dYPoint = m.dYPoint - n.dYPoint;
    return z;
}

/*生成一个服从高斯分布的随机数*/
double  randn()
{
    double *FTable, *fTable;
    double Gauss_F[256];
    double Gauss_f[256];
    double gauss_array;
    FTable = Gauss_F;//正态分布函数
    fTable = Gauss_f;//概率密度函数
    NormalTable(FTable, fTable, 256);//生成正态分布表
    gauss_array = NormalRNG(Gauss_F, 256);//产生正态分布随机数
    return gauss_array;
}

/*
生成正态分布函数和正态分布概率密度函数表，表宽度为3倍方差
正态分布函数均值为0，方差为1
FTable: 正态分布函数表
fTable: 正态分布概率密度表
Length: 表的长度
*/
void NormalTable(double *FTable, double * fTable, int Length)


{
    int i;
    double h; /* 步长 */
    double x, temp;
    double C;

    x = -3;
    h = 6.0 / Length / 2;
    FTable[0] = 0;
    C = 1 / sqrt(2 * 3.1415927);
    /* 初始参数设置 */

    fTable[0] = exp(-x*x / 2)*C;
    /* 起始点的概率密度 ，exp(-x*x/2)*C为概率密度函数 */

    for (i = 1; i<Length; i++)
    {
        x += h;
        temp = exp(-x*x / 2)*C;
        x += h;
        fTable[i] = exp(-x*x / 2)*C;
        /* 计算正态分布概率密度函数 */

        FTable[i] = FTable[i - 1] + (fTable[i - 1] + 4 * temp + fTable[i])*h / 3;
        /* 辛普森数值积分公式计算正态分布函数 */
    }
}

/*
生成均值为0,方差为1的高斯随机数
Gauss_F: 正态分布函数表，规格如上，可以由NormalTable函数生成
Length: 正态分布函数表的长度
返回值：均值为0,方差为1的高斯分布随机数
*/
double NormalRNG(double * Gauss_F, int Length)


{
    double RandomNumber;
    double temp, h;
    int i;

    h = 6.0 / Length;
    /* 正态分布表的步长 */


    temp = rand();
    temp = temp / 32767.0;
    /*产生一个[0,1)区间内均匀随机数 */

    if (temp == 0)
        return 0;

    for (i = 0; i<Length; i++)
    {
        if (temp <= Gauss_F[i]) /*计算随机数落入正态分布表的哪个区间 */
        {
            RandomNumber = (-3 + h*i) + (temp - Gauss_F[i - 1]) / (Gauss_F[i] - Gauss_F[i - 1])*h;
            /* 进行线性差值 */
            break;
        }

    }
    if (i == Length)
    {
        temp = Gauss_F[i - 1];
        RandomNumber = (-3 + h*i) + (temp - Gauss_F[i - 1]) / (Gauss_F[i] - Gauss_F[i - 1])*h;
    }
    return RandomNumber;
}

double getDistance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double min(double a, double b)
{
    double c = (a > b) ? b : a;
    return c;
}

double angleCalc(double x, double y)
{
    double angle;
    if (x == 0)
    {
        if (y>0)
            angle = PI / 2;
        else if (y<0)
            angle = 3 * PI / 2;
        else angle = 0;
    }
    else if (y<0)
        angle = -atan2(-y, x) + 2 * PI;
    else angle = atan2(y, x);
    return angle;
}
