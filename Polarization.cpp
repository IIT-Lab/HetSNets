//
// Created by lee on 17-11-10.
//

#include <iostream>
#include "Polarization.h"
#include "Global.h"

using namespace std;


void SimplePolarizationChannel(double x,double r,double r1, Matrix2cd& H)
/************************************************************************************************
函数名称：
主要功能：生成简化双极化瑞利信道
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    Matrix4d P;  //4*4矩阵
    P << 1, 0, 0, r,
            0, x, x*r1, 0,
            0, x*r1, x, 0,
            r, 0, 0, 1;
//    cout << "P =" << endl << P << endl;

    Matrix4d L = P.llt().matrixL();  //Cholesky分解
//    cout << "L =" << endl << L << endl;  //L为4*4下三角矩阵

    double f1 = 2 * PI * rand() / RAND_MAX;
    double f2 = 2 * PI * rand() / RAND_MAX;
    double f3 = 2 * PI * rand() / RAND_MAX;
    double f4 = 2 * PI * rand() / RAND_MAX;
//    cout << "f1 = " << f1 <<endl;
//    cout << "f2 = " << f2 <<endl;
//    cout << "f3 = " << f3 <<endl;
//    cout << "f4 = " << f4 <<endl;

    complex<double> x1(0, -f1);
    complex<double> x2(0, -f2);
    complex<double> x3(0, -f3);
    complex<double> x4(0, -f4);
//    cout << "x1 = " << x1 <<endl;

    complex<double> y1 = exp(x1);
    complex<double> y2 = exp(x2);
    complex<double> y3 = exp(x3);
    complex<double> y4 = exp(x4);
//    cout << "y1 = " << y1 <<endl;

    Vector4cd Y;
    Y << y1, y2, y3, y4;
//    cout << "Y = " << Y <<endl;

    Vector4cd L1 = L * Y;
//    cout << "L1 = " << L1 <<endl;

    //Matrix2cd H;
    H << conj(L1(0, 0)), conj(L1(1, 0)), conj(L1(2, 0)), conj(L1(3, 0));
    cout << "简化信道去极化效应矩阵：" << endl;
    cout << "H = " << endl << H <<endl;

//    std::default_random_engine dre((unsigned)time(NULL));
//    normal_distribution<double> normal(0.0, 1);
    //cout << "正态分布随机数： " << normal(e) <<endl;

//    complex<double> Z1(sqrt(0.5) * normal(dre), sqrt(0.5) * normal(dre));
//    cout << "Z1 = " << Z1 <<endl;

//    H = Z1 * H;
//    cout << "H = " << H <<endl;
}

void polarizationChannel(double cpr, double xpr, double RCorrCoefficients, double TCorrCoefficients, double cpc, double apc, Matrix2cd &matrixDepolarization)
/************************************************************************************************
函数名称：polarizationChannel
主要功能：生成信道去极化效应矩阵
输入参数：
double cpr：co-polar ratios
double xpr：cross-polar ratios
double RCorrCoefficients：receive correlation coefficients for co-located antennas
double TCorrCoefficients：transmit correlation coefficients for co-located antennas
double cpc：co-polar correlation coefficients
double apc：anti-polar correlation coefficients
输出参数：
Matrix2cd &matrixDepolarization：信道去极化效应矩阵
其他信息：
*************************************************************************************************/
{
    Matrix4d matrixPolarizationCorr;  //4*4极化相关矩阵
    matrixPolarizationCorr << 1, sqrt(cpr * xpr) * TCorrCoefficients, sqrt(xpr) * RCorrCoefficients, sqrt(cpr) * cpc,
            sqrt(cpr * xpr) * TCorrCoefficients, cpr * xpr, sqrt(cpr) * xpr * apc, cpr * sqrt(xpr) * RCorrCoefficients,
            sqrt(xpr) * RCorrCoefficients, sqrt(cpr) * xpr * apc, xpr, sqrt(cpr * xpr) * TCorrCoefficients,
            sqrt(cpr) * cpc, cpr * sqrt(xpr) * RCorrCoefficients, sqrt(cpr * xpr) * TCorrCoefficients, cpr;

    cout << "极化相关矩阵：" << endl;
    cout << "matrixPolarizationCorr =" << endl << matrixPolarizationCorr << endl;

    Matrix4d L = matrixPolarizationCorr.llt().matrixL();  //Cholesky分解
//    cout << "L =" << endl << L << endl;  //L为4*4下三角矩阵

    double randomAngle1 = 2 * PI * rand() / RAND_MAX;
    double randomAngle2 = 2 * PI * rand() / RAND_MAX;
    double randomAngle3 = 2 * PI * rand() / RAND_MAX;
    double randomAngle4 = 2 * PI * rand() / RAND_MAX;

    complex<double> x1(0, -randomAngle1);
    complex<double> x2(0, -randomAngle2);
    complex<double> x3(0, -randomAngle3);
    complex<double> x4(0, -randomAngle4);
//    cout << "x1 = " << x1 <<endl;

    complex<double> y1 = exp(x1);
    complex<double> y2 = exp(x2);
    complex<double> y3 = exp(x3);
    complex<double> y4 = exp(x4);
//    cout << "y1 = " << y1 <<endl;

    Vector4cd Y;
    Y << y1, y2, y3, y4;
//    cout << "Y = " << Y <<endl;

    Vector4cd L1 = L * Y;
//    cout << "L1 = " << L1 <<endl;

    matrixDepolarization << conj(L1(0, 0)), conj(L1(1, 0)), conj(L1(2, 0)), conj(L1(3, 0));
    cout << "信道去极化效应矩阵：" << endl;
    cout << "matrixDepolarization = " << endl << matrixDepolarization <<endl;

}
