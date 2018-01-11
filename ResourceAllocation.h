//
// Created by lee on 18-1-10.
//

#ifndef HETSNETS_RESOURCEALLOCATION_H
#define HETSNETS_RESOURCEALLOCATION_H

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

namespace RR {
    class MUser {
    public:
        MUser(int _uID, double _power, double _channelGain, double _cqi);
        ~MUser();

        void setRBID(int RBID);

        int getUID() const;

        double getPower() const;

        double getChannelGain() const;

        double getCqi() const;

        int getRBID() const;

    private:
        int uID;
        double power; //宏蜂窝用户发射功率
        double channelGain; //宏蜂窝用户到宏基站的信道增益
        double cqi; //上一个时隙反馈的CQI 暂时用上一个时隙的SINR代替
        int RBID;
    };

    class D2DPair {
    public:
        D2DPair(int _ID, int _TxID, int _RxID, double _power, double _channelGain);
        ~D2DPair();

    private:
        int ID; //编号 从0开始
        int TxID;
        int RxID;
        double power; //D2D用户发射功率
        double channelGain; //D2D pair 之间的信道增益
        int RBID;
    };
}

bool comMUserCqi(RR::MUser* MUser1, RR::MUser* MUser2);


void MUserRBAllocation(map<int, RR::MUser*> mapID2MUserPtr, int RBNum);
void D2DPairRBAllocation(map<int, RR::MUser*> mapID2MUserPtr, map<int, RR::D2DPair*> mapID2D2DPairPtr);

#endif //HETSNETS_RESOURCEALLOCATION_H
