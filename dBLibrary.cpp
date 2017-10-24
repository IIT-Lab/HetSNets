//
// Created by lee on 17-8-28.
//

#include "dBLibrary.h"

#include <math.h>

double dB::Linear2dB(double _dLinearValue)//线性值转化为dB值
{
    return 10 * log10(_dLinearValue);
}
double dB::dB2Linear(double _dDBValue)//dB值转化为线性值
{
    return pow(10, 0.1*(_dDBValue));
}