//
// Created by lee on 18-1-10.
//
#include "ResourceAllocation.h"

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
    for (auto temp : mapID2MUserPtr) {
        vecMUserPtr.push_back(temp.second);
    }
    //蜂窝用户按CQI降序排序
    sort(vecMUserPtr.begin(), vecMUserPtr.end(), comMUserCqi);
    int MUserNum = (int)mapID2MUserPtr.size();
    for (int i = 0; i < MUserNum; ++i) {
        int RBID = vecMUserPtr[i]->getRBID(); //选出CQI最大的蜂窝用户的RB

    }
}

bool comMUserCqi(RR::MUser *MUser1, RR::MUser *MUser2) {
    return MUser1->getCqi() > MUser2->getCqi();
}

/**************************RR::D2DPair**************************/

RR::D2DPair::D2DPair(int _ID, int _TxID, int _RxID, double _power, double _channelGain) {
    ID = _ID;
    TxID = _TxID;
    RxID = _RxID;
}

RR::D2DPair::~D2DPair() {

}
