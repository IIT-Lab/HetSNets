//
// Created by lee on 17-9-13.
//

#ifndef HETSNETS_SENSE_H
#define HETSNETS_SENSE_H

#include <vector>

using namespace std;

class SenseInterface
{
public:
    static SenseInterface& GetInstance();
    SenseInterface();
    ~SenseInterface();
    void SetSenseInterface();
    void WriteSenseFile(int _slot);
    void addTxPower(double _txPower);
    void addSinr(double _sinr);
    void setNoisePower(double _noisePower);

private:
    static SenseInterface* _Instance;
    int numOfSlot;
    int numOfSample;
    int rankOfSymbol;
    int numOfMainBS;
    int numOfCooperationBS;
    double falseAlarmProbability;
    double minSINR;
    double maxSINR;
    double noisePower; //线性值

    vector<double> vecTxPower; //线性值
    vector<double> vecSinr;
    vector<int> vecSample;
    vector<double> vecChannelGain;
};

#endif //HETSNETS_SENSE_H
