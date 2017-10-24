//
// Created by lee on 17-9-27.
//

#ifndef HETSNETS_COMPUTING_H
#define HETSNETS_COMPUTING_H

class sinrComputing
{
public:
    sinrComputing();
    static sinrComputing& GetInstance();
    void addSignalPow(double _signalPow);
    void addNoisePow(double _noisePow);
    void addInterferencePow(double _interferencePow);
    void updateSinr();
    double getSinr();
    void clearSinr();

private:
    static sinrComputing* _Instance;
    double sinr; //线性值
    double signalPow;
    double noisePow;
    double interferencePow;
};

#endif //HETSNETS_COMPUTING_H
