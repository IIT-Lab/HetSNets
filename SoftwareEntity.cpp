//
// Created by lee on 17-8-28.
//
#include "SoftwareEntity.h"
#include "SystemDriveBus.h"
#include "InterferenceIndex.h"
#include <Eigen/Dense>

using namespace Eigen;

///////////////////////////接收软体类///////////////////////////////




void SoftwareEntityRx::ConnectID(int iID)
{
    dID = iID;
}

void SoftwareEntityRx::ConnectLocation(double dxPoint, double dyPoint) {}

void SoftwareEntityRx::WorkSlotSoftwareEntity()//接收软体的workslot
{
    //设置缓存主服务基站的容器
//    SetMainTxInfo();
    //根据主服务基站和路损表可以计算SINR
    SinrComputing();
    //if (doubleMapSINR.size() != 0)//如果SINR容器不为空，去调用SINR-BLER函数
    //	Sinr2Bler();//统计结果
}

void SoftwareEntityRx::Sinr2Bler()//查询SINR-BLER曲线，得出每个SINR对应的BLER，随机出一个数，判断数据包是否正确接收
{

}

void SoftwareEntityRx::ConnectHardLinkloss(vector<map<int, pair<double, double>>> &TLinklossVecMap)
{

}

void SoftwareEntityRx::SinrComputing()//SINR计算，包含导入BLER曲线，判断包是否正确接收
{
    doubleMapSINR.clear();//每个时隙在使用SINR表之前都要将上一个时隙的内容清空，因为该表只跟当前时隙相关
    double txPow, sinr, rate, thermalNoisePow, signalPow = 0, linkloss = 0, channelGain = 0, interferencePow = 0,totalPow = 0;;
    int RBID, TxID = -1;
    //1个RB,12个连续的载波,12*15000=180000Hz
    double whiteNoise = -174;//-174dBm/Hz
    double noiseFig = 5;//dB
    noiseFig = pow(10, -noiseFig / 10);//线性值
    thermalNoisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

    vecMainTxID.clear();
    vecRBID.clear();
    mapMapLinklossPower.clear();
    GetMainTxID(vecMainTxID);
    GetRBID(vecRBID);
    GetLinklossPower(mapMapLinklossPower);

    for (auto tempMap : mapMapLinklossPower) {
        RBID = tempMap.first;
        if (RBID > -1) {
            totalPow = 0;
            for (auto tempLinklossPower : tempMap.second) {
                txPow = tempLinklossPower.second.second;//dBm
                txPow = pow(10, (txPow - 30) / 10);//W
                linkloss = tempLinklossPower.second.first;//dB
                channelGain = pow(10, -linkloss / 10);//线性值
                totalPow += txPow * channelGain;
                TxID = tempLinklossPower.first;
                for (auto _MainTxID : vecMainTxID) {
                    if (TxID == _MainTxID) {
                        signalPow = txPow * channelGain;
                        cout << "RBID: " << RBID << endl;
                        cout << "TxID: " << TxID << endl;
                        cout << "RxID: " << dID << endl;
                        interferencePow = totalPow - signalPow;
                        sinr = signalPow / (interferencePow + thermalNoisePow); //线性
                        rate = 180000 * RBNUM * log2(1 + sinr);
                        sinr = 10 * log10(sinr);//dB值
                        cout << "SINR: " << sinr << endl;
                        cout << "rate: " << rate << endl;
                        cout << "-----------------------------" << endl;
                        SetSINR(TxID, dID, RBID, SystemDriveBus::iSlot, sinr, rate);
                    }
                }
            }
        }
    }
}

void SoftwareEntityRx::SetTxInfo(SoftwareEntityTx *softTx, pair<double, double>_point)//设置缓存所有可能的发射机信息容器
{
    map<SoftwareEntityTx *, pair<double, double>> mapTxInfo;
    mapTxInfo.insert(pair<SoftwareEntityTx *, pair<double, double>>(softTx, _point));
    vecMapTxInfo.push_back(mapTxInfo);//将某一发射机的软体类指针和坐标（x,y）存放到本接收机的发射机信息容器中
}

void SoftwareEntityRx::GetMainTxID(vector<int> &_vecMainTxID) {
    string RxID = intToString(dID); //接收机的ID
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        mysql->clearResultList();
        string SQLString = "SELECT MainTxID FROM RxID2MainTxID WHERE RxID = " + RxID;
        if(mysql->runSQLCommand(SQLString))
        {
            vector<vector<string> > result = mysql->getResult();
            for(auto & vec : result)
            {
                for(auto &str : vec)
                {
                    string strMainTxID = str.c_str();
//                    cout << "MainTxID：" << strMainTxID << endl;;
                    int MainTxID = string2Int(strMainTxID);
                    _vecMainTxID.push_back(MainTxID);
                }
            }
        }
        else
            cout << "执行失败" << endl;
    }
    mysql->destroyConnection();
}

void SoftwareEntityRx::GetLinklossPower(map<int, map<int, pair<double, double>>> &_mapMapLinklossPower) {
    string RxID = intToString(dID); //接收机的ID
    string slotID = intToString(SystemDriveBus::iSlot);
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        for (auto _RBID : vecRBID) {
            mysql->clearResultList();
            map<int, pair<double, double>> mapLinklossPower;
            string RBID = intToString(_RBID);
            string SQLString = "SELECT * FROM TxIDRxID2RBID WHERE RBID = " + RBID + " AND slotID = " + slotID;
            if(mysql->runSQLCommand(SQLString))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    string strTxID = vec[0].c_str();
                    string strPower = vec[4].c_str();
                    int TxID = string2Int(strTxID);
                    double power = string2Double(strPower);
                    double linkLoss = GetLinkloss(TxID, dID, SystemDriveBus::iSlot);

                    pair<double, double> linklossPower(linkLoss, power);
                    mapLinklossPower.insert(pair<int, pair<double, double>>(TxID, linklossPower));
                }
            }
            else
                cout << "执行失败" << endl;

            mapMapLinklossPower.insert(pair<int, map<int, pair<double, double>>>(_RBID, mapLinklossPower));
            mapLinklossPower.clear();
        }
    }
    mysql->destroyConnection();
}

void SoftwareEntityRx::GetRBID(vector<int> &_vecRBID) {
    string RxID = intToString(dID); //接收机的ID
    string slotID = intToString(SystemDriveBus::iSlot);
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        mysql->clearResultList();
        string SQLString = "SELECT RBID FROM TxIDRxID2RBID WHERE RxID = " + RxID + " AND slotID = " + slotID;
        if(mysql->runSQLCommand(SQLString)) {
            vector<vector<string> > result = mysql->getResult();
            for(auto & vec : result) {
                for(auto &str : vec) {
                    string strRBID = str.c_str();
//                    cout << "RBID：" << strRBID << endl;
                    int RBID = string2Int(strRBID);
                    _vecRBID.push_back(RBID);
                }
            }
        }
        else
            cout << "执行失败" << endl;
    }
    mysql->destroyConnection();
}

//double SoftwareEntityRx::GetLinkloss(int _TxID, int _RxID, int _slotID) {
//    string TxID = intToString(_TxID); //发射机的ID
//    string RxID = intToString(_RxID); //接收机的ID
//    string slotID = intToString(_slotID);
//    double linkLoss = 0;
//
//    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
//    mysql->initConnection();
//    if(mysql->getConnectionStatus()) {
//        mysql->clearResultList();
//        string SQLString = "SELECT linkLoss FROM channelGain WHERE RxID = " + RxID + " AND TxID = " + TxID + " AND slotID = " + slotID;
//        if(mysql->runSQLCommand(SQLString)) {
//            vector<vector<string> > result = mysql->getResult();
//            for(auto & vec : result) {
//                for(auto &str : vec) {
//                    string strlinkLoss = str.c_str();
////                    cout << "linkLoss：" << strlinkLoss << endl;;
//                    linkLoss = string2Double(strlinkLoss);
//                }
//            }
//        }
//        else
//            cout << "执行失败" << endl;
//    }
//    return linkLoss;
//}

///////////////////////////发射软体类///////////////////////////////

void SoftwareEntityTx::ConnectID(int iID)
{
    dID = iID;
}

int SoftwareEntityTx::GetDeviceID()//获取用户的ID
{
    return dID;
}

void SoftwareEntityTx::ConnectHardPower(double dPower)
{
    dTxPower = dPower;
}

void SoftwareEntityTx::ConnectLocation(double dxPoint, double dyPoint)//将坐标取过来
{
    dXPoint = dxPoint;
    dYPoint = dyPoint;
}

void SoftwareEntityTx::TransmitID2AllRx()
{
    //遍历所有可能的接收机，将本发射机的坐标+软体指针发给每个接收
    pair<double, double> point;
    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first >= 30 && _temp.first < 40)
        {
            point.first = this->dXPoint;
            point.second = this->dYPoint;

            if (_temp.second->sGetType() == "class User *") {
                auto rx = dynamic_cast<User*>(_temp.second);
                rx->software.softwareRx.SetTxInfo(this, point);//调用接收软体类的函数设置发射机坐标信息容器
            } else if (_temp.second->sGetType() == "class MacroCell *") {
                auto rx = dynamic_cast<MacroCell*>(_temp.second);
                rx->software.softwareRx.SetTxInfo(this, point);//调用接收软体类的函数设置发射机坐标信息容器
            } else if (_temp.second->sGetType() == "class SmallCell *") {
                auto rx = dynamic_cast<SmallCell*>(_temp.second);
                rx->software.softwareRx.SetTxInfo(this, point);//调用接收软体类的函数设置发射机坐标信息容器
            } else {
                cout << "set rx error!!!" << endl;
            }
        }
    }
}

void SoftwareEntityTx::Scheduler()
{
//    (1)提取扇区中有数据要发的用户参与资源调度

//    (2)对扇区内的用户进行信道资源的分配

//    (3)进行功率分配

}

void SoftwareEntityTx::WorkSlotSoftwareEntity()
{
//    int RxID;
//    for (auto _temp : SystemDriveBus::SlotDriveBus)
//    {
//        if (_temp.first >= 30 && _temp.second->sGetType() == "class User *")
//        {
//            User *_tempUser = dynamic_cast<User *>(_temp.second);
//            RxID = _tempUser->iGetID();
//            ARQ_processes_Tx_buffers* ARQ_processes_Tx_buffersPtr = ARQ_processes_Tx_buffers::Create();
//            ARQ_processes_Tx_buffersPtr->initial(RxID);
//            map_ARQ_processes_Tx_buffers.insert(pair<int, ARQ_processes_Tx_buffers*>(RxID, ARQ_processes_Tx_buffersPtr));
//
//            high_priority_sequence* high_priority_sequencePtr = high_priority_sequence::Create();
//            high_priority_sequencePtr->initial(RxID);
//            map_high_priority_queue.insert(pair<int, high_priority_sequence*>(RxID, high_priority_sequencePtr));
//
//            map<int, message_arq_ack*> map_message_arq_ack;
//            for (int i = 0; i < 3; i++)
//            {
//                message_arq_ack* message_arq_ackPtr = message_arq_ack::Create();
//                message_arq_ackPtr->initial();
//                map_message_arq_ack.insert(pair<int, message_arq_ack*>(i, message_arq_ackPtr));
//            }
//            uplink_ACK_feedback_queues.insert(pair<int, map<int, message_arq_ack*>>(RxID, map_message_arq_ack));
//        }
//    }
//
//    for (auto _temp : SystemDriveBus::SlotDriveBus)
//    {
//        if (_temp.first >= 30 && _temp.second->sGetType() == "class User *")
//        {
//            User *_tempUser = dynamic_cast<User *>(_temp.second);
//            RxID = _tempUser->iGetID();
//            ARQ_processes_modify_for_full_buffer(RxID);
//        }
//    }

    TransmitID2AllRx();	//把该发射机的ID坐标信息发送给每个接收机
//    InterferenceRgister();	//对该发射机随机选择RB块进行数据包的发送，并登记在发射端的干扰登记表里
//    Scheduler();	//进行调度,周期性地发送数据包
}

void SoftwareEntityTx::InterferenceRgister() {}

void SoftwareEntityTx::ARQ_processes_modify_for_full_buffer(int _RxID)
/************************************************************************************************
函数名称：
主要功能：full buffer业务
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    int RxID = _RxID;
    int arq_ack = uplink_ACK_feedback_queues.at(RxID).at(SystemDriveBus::iSlot % 3)->ARQ_ACK;//上行ACK反馈信息
    int number_of_transmission = map_ARQ_processes_Tx_buffers.at(RxID)->map_arq_num_to_block_info.at(uplink_ACK_feedback_queues.at(RxID).at(SystemDriveBus::iSlot % 3)->ARQ_process_id)->number_of_transmission;//重传次数

    //检查用户相应时刻的上行ACK反馈信息
    switch(arq_ack)
    {
        //为0，没有接收得到相应用户的ACK信息，不作处理
        case 0:
//            break;
            //为”-1”，说明该用户数据没有被正确接收，需要重传；git git
        case -1:
            //判断重传次数，如果超过重传次数则丢弃该包
            if (number_of_transmission == NUM_OF_RETRANSMISSION)
            {
//                SystemDriveBus::ID2UserVariable.at(RxID)->packet_drop++;
            }
            //将没有被正确接收的ARQ过程中的data block发送到high priority queue中，等待重传
            //从ARQ Tx buffer中将数据包信息存入高优先级队列，等待重传
            else
            {
                int Queue_Tail = map_high_priority_queue.at(RxID)->Queue_Tail;
                blockInfo* blockInfoPtr = map_ARQ_processes_Tx_buffers.at(RxID)->map_arq_num_to_block_info.at(uplink_ACK_feedback_queues.at(RxID).at(SystemDriveBus::iSlot % 3)->ARQ_process_id);
                map_high_priority_queue.at(RxID)->map_block_info_on_queue.insert(pair<int, blockInfo*>(Queue_Tail, blockInfoPtr));
            }
    }
}

void SoftwareEntityTx::ConnectType(string _TxType) {
    TxType = _TxType;
}

const string &SoftwareEntityTx::getTxType() const {
    return TxType;
}

