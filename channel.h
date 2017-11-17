//
// Created by lee on 17-8-28.
//

#ifndef HETSNETS_CHANNEL_H
#define HETSNETS_CHANNEL_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "Interface.h"
#include "Device.h"

using namespace std;

class channel : public Interface
{
public:
    channel();
    void Display();
    void initial(default_random_engine dre);
    void WorkSlot(default_random_engine dre);
    int GetRxID();
    void UpdateAllTxLinkLossTable();
    void setmLinkLossPtr(vector<map<int, pair<double, double>>> *vecMapLinkLossPowerPtr);//提供给UE的接口，UE的硬体部分传递一个路损容器指针过来，信道直接往这个指针指向的容器中写入路损
    void setAllTxPowerMap(int txID, double power);//获取发射机硬体里的功率值
    void ChannelInitial(Interface &_Receiver, vector<Interface*>vTxContainer);
    double GetLinkLoss(int txID);
    double GetChannelGain(int txID);
    const string &getReceiveType() const;

    const set<int> &getSTxID() const;

    static int countTag;
private:
    struct double2 { double x; double y; };//假的double2，使用的时候会换成CUDA自带的double2，用于存储复数
    map<int, double> mLargeScaleFading;//存储大尺度衰落的map容器，int为用户的ID号，double为大尺度衰落值
    set<int> sTxID;//所有与该信道接收对象相关的发射机ID集
    map<int, string> mLosOrNlos;//存储发射机ID--视距非视距的容器
    string receiveType;
    int receiveID;//存储接收对象的ID
    map<int, double> mTxPower;//存储发射机ID--功率的容器
    map<int, double> mLinkLoss;//存储LinkLoss的map容器，int为发射用户的ID号，double为LinkLoss值
    map<int, double> mChannelGain;
    vector<map<int, pair<double, double>>> TLinklossPowerVecMap;//vector下标是RB块号【0-4】，map的键是发射机号，值是该发射机与接收机的链路损耗
};

class mobileList
{
public:
    int getLOSSelect() const;

    void setLOSSelect(int LOSSelect);

    double getR() const;

    void setR(double r);

    double getSegma() const;

    void setSegma(double segma);

    double getXPR() const;

    void setXPR(double XPR);

    int getNumofcluster() const;

    void setNumofcluster(int numofcluster);

private:
    int LOSSelect; //视距，等于1
    double r; //时延概率分布因子
    double segma; //每径的阴影衰落标准差
    double XPR; //交叉极化比率因子
    int numofcluster; //簇数
};

class angleList
{
public:
    double getAoD_LOS() const;

    void setAoD_LOS(double AoD_LOS);

    double getAoA_LOS() const;

    void setAoA_LOS(double AoA_LOS);

    double getV_angle() const;

    void setV_angle(double v_angle);

    double getAntennaGainTable() const;

    void setAntennaGainTable(double antennaGainTable);

private:
    double AoD_LOS;
    double AoA_LOS;
    double v_angle;
    double antennaGainTable;
};

class LSP
{
public:
    double getDS() const;

    void setDS(double DS);

    double getASD() const;

    void setASD(double ASD);

    double getASA() const;

    void setASA(double ASA);

    double getSF() const;

    void setSF(double SF);

    double getK() const;

    void setK(double K);

private:
    double DS;//时延扩展
    double ASD;//离开角扩展
    double ASA;//到达角扩展
    double SF;
    double K;//莱斯因子
};

#endif //HETSNETS_CHANNEL_H
