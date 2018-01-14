//
// Created by lee on 18-1-10.
//
#include "ResourceAllocation.h"
#include "MySQLManager.h"
#include "SystemDriveBus.h"

/**************************RR::MUser**************************/

RR::MUser::MUser(int _uID, double _power, double _channelGain, double _cqi) {
    uID = _uID;
    power = _power;
    channelGain = _channelGain;
    cqi = _cqi;
}

RR::MUser::~MUser() {

}

int RR::MUser::getUID() const {
    return uID;
}

double RR::MUser::getPower() const {
    return power;
}

double RR::MUser::getChannelGain() const {
    return channelGain;
}

double RR::MUser::getCqi() const {
    return cqi;
}

int RR::MUser::getRBID() const {
    return RBID;
}

void RR::MUser::setRBID(int RBID) {
    MUser::RBID = RBID;
}

void MUserRBAllocation(map<int, RR::MUser *> mapID2MUserPtr, int RBNum) {
    int RBID = 0;
    for (auto temp : mapID2MUserPtr) {
        if (RBID < RBNum) {
            temp.second->setRBID(RBID);
            RBID++;
        } else {
            cout << "error!!!" << endl;
        }
    }
}

/**************************RR*************************/

void D2DPairRBAllocation(map<int, RR::MUser *> mapID2MUserPtr, map<int, RR::D2DPair *> mapID2D2DPairPtr) {
    vector<RR::MUser*> vecMUserPtr;
    vector<RR::D2DPair*> vecD2DPairPtr;
    for (auto temp : mapID2MUserPtr) {
        vecMUserPtr.push_back(temp.second);
    }
    for (auto temp : mapID2D2DPairPtr) {
        vecD2DPairPtr.push_back(temp.second);
    }

    //蜂窝用户按CQI降序排序
    sort(vecMUserPtr.begin(), vecMUserPtr.end(), comMUserCqi);
    //D2D pair 按 channel gain 降序排序
    sort(vecD2DPairPtr.begin(), vecD2DPairPtr.end(), comD2DPairChannelGain);

    int MUserNum = (int)vecMUserPtr.size();
    for (int i = 0; i < MUserNum; ++i) {
        RR::MUser * tempMUser = vecMUserPtr[i];
        int MUserID = tempMUser->getUID();
        int BSID = 0;
        int RBID = tempMUser->getRBID(); //选出CQI最大的蜂窝用户的RB
        double MUserPower = tempMUser->getPower();
        MUserPower = pow(10, (MUserPower - 30) / 10);//W
        double MUserChannelGain = tempMUser->getChannelGain();

        int D2DPairNum = (int)vecD2DPairPtr.size();
        for (int j = 0; j < D2DPairNum; ++j) {
            RR::D2DPair * tempD2DPair = vecD2DPairPtr[j];
            if (tempD2DPair->getRBID() == -1) { //如果没有被分配
                //计算SINR
                double MUserSinr, D2DSinr;
                int D2DTxID = tempD2DPair->getTxID();
                int D2DRxID = tempD2DPair->getRxID();
                double D2DTxPower = tempD2DPair->getPower();
                D2DTxPower = pow(10, (D2DTxPower - 30) / 10);//W
                double D2DPairChannelGain = tempD2DPair->getChannelGain();
                double M2Dlinkloss = GetLinkloss(MUserID, D2DRxID, SystemDriveBus::iSlot);
                double M2DchannelGain = pow(10, -M2Dlinkloss / 10);//线性值
                double D2Blinkloss = GetLinkloss(D2DTxID, BSID, SystemDriveBus::iSlot);
                double D2BchannelGain = pow(10, -D2Blinkloss / 10);//线性值

                double whiteNoise = -174;//-174dBm/Hz
                double noiseFig = 5;//dB
                noiseFig = pow(10, noiseFig / 10);//线性值
                double noisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

                MUserSinr = MUserPower * MUserChannelGain / (D2DTxPower * D2BchannelGain + noisePow);
                MUserSinr = 10 * log10(MUserSinr);//dB值
                D2DSinr = D2DTxPower * D2DPairChannelGain / (MUserPower * M2DchannelGain + noisePow);
                D2DSinr = 10 * log10(D2DSinr);//dB值

                if (MUserSinr > 10 && D2DSinr > 10) {
                    tempD2DPair->setRBID(RBID);
                    break;
                } else {
//                continue;
                }
            }
        }
    }
}

bool comMUserCqi(RR::MUser *MUser1, RR::MUser *MUser2) {
    return MUser1->getCqi() > MUser2->getCqi();
}

bool comD2DPairChannelGain(RR::D2DPair *D2DPair1, RR::D2DPair *D2DPair2) {
    return D2DPair1->getChannelGain() > D2DPair2->getChannelGain();
}

/**************************RR::D2DPair**************************/

RR::D2DPair::D2DPair(int _ID, int _TxID, int _RxID, double _power, double _channelGain) {
    ID = _ID;
    TxID = _TxID;
    RxID = _RxID;
    power = _power;
    channelGain = _channelGain;
    RBID = -1;
}

RR::D2DPair::~D2DPair() {

}

int RR::D2DPair::getID() const {
    return ID;
}

int RR::D2DPair::getTxID() const {
    return TxID;
}

int RR::D2DPair::getRxID() const {
    return RxID;
}

double RR::D2DPair::getPower() const {
    return power;
}

double RR::D2DPair::getChannelGain() const {
    return channelGain;
}

int RR::D2DPair::getRBID() const {
    return RBID;
}

void RR::D2DPair::setRBID(int RBID) {
    D2DPair::RBID = RBID;
    cout << "D2D pair: " << ID << " 分配 RB: " << D2DPair::RBID << endl;
}
