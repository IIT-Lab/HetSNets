//
// Created by lee on 17-8-28.
//

#ifndef HETSNETS_SOFTWAREENTITY_H
#define HETSNETS_SOFTWAREENTITY_H

#include <vector>
#include <queue>
#include "Traffic.h"
#include "HARQACKTreatment.h"
#include "ControlCenter.h"
#include "psshn_platform/psshn_platform.h"

using namespace std;

///////////////////////////软体类///////////////////////////////

class SoftwareEntity
{

};

///////////////////////////发射软体类///////////////////////////////

class SoftwareEntityTx
{
public:
    int GetDeviceID();//获取用户的ID
    void ConnectID(int iID);//将用户的id传入对象
    void ConnectType(string _TxType);//将设备类型传入对象
    void ConnectHardPower(double dPower);//将硬体里的发射功率传入对象
    void ConnectLocation(double dxPoint, double dyPoint);//将坐标传入对象
    void TransmitID2AllRx();//向所有可能的接收机发射自己的坐标信息
    void Scheduler();//调度，装Txbuffer
    void WorkSlotSoftwareEntity(); //发射软体的workslot
    void InterferenceRgister();//进行干扰登记
    void ARQ_processes_modify_for_full_buffer(int _RxID);//full buffer业务
    const string &getTxType() const;

private:
    double dTxPower;
    int dID;//设备的ID
    string TxType;//发射机的类型
//    map<int, TTxBuffer*> mapTTxBuffer; //接收机ID,发送缓存
    map<int, ARQ_processes_Tx_buffers*> map_ARQ_processes_Tx_buffers; //接收机ID,发送缓存
    map<int, high_priority_sequence*> map_high_priority_queue; //接收机ID,各用户发送端缓存重发包的高优先级队列
    map<int, map<int, message_arq_ack*>> uplink_ACK_feedback_queues; //<接收机ID, <[0, 2], ACK消息>>
    double dXPoint;
    double dYPoint;
};

///////////////////////////接收软体类///////////////////////////////

class SoftwareEntityRx
{
public:
    int GetDeviceID();//获取用户的ID
    void ConnectID(int iID);//将用户的ID传入对象
    void ConnectLocation(double dxPoint, double dyPoint);//将用户的坐标传入对象
    void ConnectMode(string _mode);
    void WorkSlotSoftwareEntity();//接收软体的workslot
    void SetMainTxInfo();//设置缓存主服务基站的容器
    void ConnectHardLinkloss(vector<map<int, pair<double, double>>> &TLinklossPowerVecMap);//将功率和路损取过来
    void Sinr2Bler();//查询SINR-BLER曲线，得出每个SINR对应的BLER，随机出一个数，判断数据包是否正确接收
    void SinrComputing();//SINR计算，包含导入BLER曲线，判断包是否正确接收
    void SetTxInfo(SoftwareEntityTx *softTx, pair<double, double>_point);//设置缓存所有可能的发射机信息容器

    double P_mue[2]; //宏蜂窝发射极化状态
    creal_T P_cbs_data[2]; //小蜂窝发射极化状态
    creal_T P_cue[2]; //小蜂窝用户极化状态
    creal_T H_mbscue[4];
    creal_T H_cbscue[4];
    creal_T H_cbsmue[4];

private:
    int dID;//用户的ID
    string mode;
    vector<map<SoftwareEntityTx *, pair<double, double>>> vecMapTxInfo;//缓存所有可能的发射机信息容器
    vector<map<int, SoftwareEntityTx *>> vecMapMainTxInfo;//缓存主服务基站的容器
    vector<map<int, pair<double, double>>> *vLinklossPowerVecMap;//vector下标是RB块号【0-20】，map的键是发射机号，值是该发射机与接收机的链路损耗
    map<int, map<int, double>> doubleMapSINR;//存放每个RB块下，对应主服务基站的SINR，因为可能在一个RB上有多个主服务基站
    double dXPoint;
    double dYPoint;
};

///////////////////////////Macro发射软体类///////////////////////////////

class MacroSoftwareEntityTx : public SoftwareEntityTx
{

};

///////////////////////////Macro接收软体类///////////////////////////////

class MacroSoftwareEntityRx : public SoftwareEntityRx
{

};

///////////////////////////Macro软体类///////////////////////////////

class MacroSoftwareEntity : public SoftwareEntity
{
public:
    MacroSoftwareEntityTx softwareTx;
    MacroSoftwareEntityRx softwareRx;
};

///////////////////////////SmallCell发射软体类///////////////////////////////

class SmallCellSoftwareEntityTx : public SoftwareEntityTx
{

};

///////////////////////////SmallCell接收软体类///////////////////////////////

class SmallCellSoftwareEntityRx : public SoftwareEntityRx
{

};

///////////////////////////SmallCell软体类///////////////////////////////

class SmallCellSoftwareEntity : public SoftwareEntity
{
public:
    SmallCellSoftwareEntityTx softwareTx;
    SmallCellSoftwareEntityRx softwareRx;
};

///////////////////////////User发射软体类///////////////////////////////

class UserSoftwareEntityTx : public SoftwareEntityTx
{
public:

};

///////////////////////////User接收软体类///////////////////////////////

class UserSoftwareEntityRx : public SoftwareEntityRx
{

};

///////////////////////////User软体类///////////////////////////////

class UserSoftwareEntity : public SoftwareEntity
{
public:
    UserSoftwareEntityTx softwareTx;
    UserSoftwareEntityRx softwareRx;
};




#endif //HETSNETS_SOFTWAREENTITY_H
