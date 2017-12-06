//
// Created by lee on 17-8-28.
//

#ifndef HETSNETS_HARDWAREENTITY_H
#define HETSNETS_HARDWAREENTITY_H

#include "Interface.h"

/*************************************总硬体类**************************************/

///////////////////////////硬体类///////////////////////////////

class HardwareEntity : public Hardware     //硬体类，与信道对象交互，为接收机软体提供链路损耗
{
public:
    HardwareEntity();

    void CreateHardwareEntity();
    int GetDeviceID();//获取用户的ID
    void ConnectID(int iID);//将用户的ID传入对象
    void ConnectstartSlot(int startSlot);//将起始时隙传入对象
    vector<map<int, pair<double, double>>>& GetLinkloss();//获取链路损耗，方便软体进行SINR计算使用

    void WorkslotHardwareEntityTx();
    void WorkslotHardwareEntityRx();//处理接收机的硬体

//private:
    int dID;//用户的ID
    int iStartSlot;
    double txPower;//存储发射功率
    vector<map<int, pair<double, double>>> TLinklossPowerVecMap;//vector下标是RB块号【0-20】，map的键是发射机号，值是该发射机与接收机的链路损耗
};

///////////////////////////Macro硬体类///////////////////////////////

class MacroHardwareEntity : public HardwareEntity
{
public:
    void InitialHardwareEntity(); //初始化发射功率
    //void WorkslotHardwareEntityTX();
};

///////////////////////////SmallCell硬体类///////////////////////////////

class SmallCellHardwareEntity : public HardwareEntity
{
public:
    void InitialHardwareEntity(); //初始化发射功率
    //void WorkslotHardwareEntityTX();
};

///////////////////////////Wifi硬体类///////////////////////////////

class WifiHardwareEntity : public HardwareEntity
{
public:
//    WifiHardwareEntity();
//    void InitialHardwareEntity();
//    void WorkslotHardwareEntityTx(); //处理发射机的硬体
};

///////////////////////////User硬体类///////////////////////////////

class UserHardwareEntity : public HardwareEntity
{
public:
    void InitialHardwareEntity(); //初始化发射功率
    //void WorkslotHardwareEntityTX();
    void ConnectUserType(string _userType);//将用户的类型传入对象

private:
    string userType;
};

#endif //HETSNETS_HARDWAREENTITY_H
