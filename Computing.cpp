//
// Created by lee on 17-9-27.
//
#include <math.h>
#include "Computing.h"

sinrComputing* sinrComputing::_Instance = nullptr;

sinrComputing::sinrComputing() {
    this->interferencePow = 0;
    this->noisePow = 0;
    this->signalPow = 0;
    this->sinr = 0;
}

sinrComputing& sinrComputing::GetInstance()
{
    if (_Instance == nullptr)
    {
        _Instance = new sinrComputing();
    }
    return *_Instance;
}

void sinrComputing::addSignalPow(double _signalPow) {
    signalPow += _signalPow;
}

void sinrComputing::addNoisePow(double _noisePow) {
    noisePow += _noisePow;
}

void sinrComputing::addInterferencePow(double _interferencePow) {
    interferencePow += _interferencePow;
}

double sinrComputing::getSinr() {
    return sinr;
}

void sinrComputing::updateSinr() {
    sinr = signalPow / (noisePow + interferencePow);
    sinr = 10 * log10(sinr);//dB值
}

void sinrComputing::clearSinr() {
    this->interferencePow = 0;
    this->noisePow = 0;
    this->signalPow = 0;
    this->sinr = 0;
}
