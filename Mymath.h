//
// Created by lee on 17-8-1.
//

#ifndef HETSNETS_MYMATH_H
#define HETSNETS_MYMATH_H

#include "Device.h"

double  randn();
void NormalTable(double *FTable, double * fTable, int Length);
double NormalRNG(double * Gauss_F, int Length);

deviceLocation complexIntMul(deviceLocation m, double n);
deviceLocation complexMul(deviceLocation m, deviceLocation n);
deviceLocation complexAdd(deviceLocation m, deviceLocation n);
deviceLocation complexSub(deviceLocation m, deviceLocation n);

double getDistance(double x1, double y1, double x2, double y2);

double min(double a, double b);

double angleCalc(double x, double y);

#endif //HETSNETS_MYMATH_H
