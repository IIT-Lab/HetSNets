//
// Created by lee on 17-8-28.
//
#include "SoftwareEntity.h"
#include "SystemDriveBus.h"
#include "InterferenceIndex.h"

///////////////////////////接收软体类///////////////////////////////

int SoftwareEntityRx::GetDeviceID()//获取用户的ID
{
    return dID;
}


void SoftwareEntityRx::ConnectID(int iID)
{
    dID = iID;
}

void SoftwareEntityRx::ConnectLocation(double dxPoint, double dyPoint)//将坐标取过来
{
    dXPoint = dxPoint;
    dYPoint = dyPoint;
}

void SoftwareEntityRx::ConnectMode(string _mode)
{
    mode = _mode;
}

void SoftwareEntityRx::SetMainTxInfo()//设置缓存主服务基站的容器
{
    //由坐标判断发射机距离自己的距离是否在（a,b）范围内，并存储主服务基站
    //先遍历所有可能的发射机信息表
    vecMapMainTxInfo.clear();//每个时隙在使用vecMapMainTxInfo表之前都要将上一个时隙的内容清空，因为该表只跟当前时隙相关
    int txID = -1;
    int rxID = this->dID;
    Interface *rxPtr = SystemDriveBus::ID2PtrBus.at(rxID);
//    SmallCell *rxSmallCellPtr = dynamic_cast<SmallCell *>(rxPtr);
    User *rxUserPtr = dynamic_cast<User *>(rxPtr);
    if (rxUserPtr->getUser_type() == "MacroCell")
    {
        int MacroCellID = rxUserPtr->getCellID();
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.second->sGetType() == "class MacroCell *")
            {
                MacroCell *MacroCellPtr = dynamic_cast<MacroCell *>(_temp.second);
                if (MacroCellPtr->GetmacroID() == MacroCellID) txID = MacroCellPtr->iGetID();
            }
        }
    }
    else if (rxUserPtr->getUser_type() == "SmallCell")
    {
        int SmallCellID = rxUserPtr->getSmallCellID();
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.second->sGetType() == "class SmallCell *")
            {
                SmallCell *SmallCellPtr = dynamic_cast<SmallCell *>(_temp.second);
                if (SmallCellPtr->getSmallCellID() == SmallCellID) txID = SmallCellPtr->iGetID();
            }
        }
    }

    for (auto vTemp : vecMapTxInfo)
    {
        map<int, SoftwareEntityTx *> mainTxTemp;
        auto mapPtr = vTemp.begin();
        if (txID == mapPtr->first->GetDeviceID())
        {
            auto softTxPtr = mapPtr->first;
            mainTxTemp[txID] = softTxPtr;
            vecMapMainTxInfo.push_back(mainTxTemp);//将主服务基站存储在容器中
        }
    }
    vecMapTxInfo.clear();//每个时隙在使用vecMapTxInfo表之前都要将上一个时隙的内容清空，因为该表只跟当前时隙相关
}

void SoftwareEntityRx::WorkSlotSoftwareEntity()//接收软体的workslot
{
    //设置缓存主服务基站的容器
    SetMainTxInfo();
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
    vLinklossPowerVecMap = &TLinklossVecMap;
}

void SoftwareEntityRx::SinrComputing()//SINR计算，包含导入BLER曲线，判断包是否正确接收
{
    //查找TLinklossVecMap表，取出每个RB块上的所有发射用户，去遍历vecMapMainTxInfo主服务基站容器
    doubleMapSINR.clear();//每个时隙在使用SINR表之前都要将上一个时隙的内容清空，因为该表只跟当前时隙相关
    double txPow, sinr, noiseFig, thermalNoisePow, signalPow, channelGain = 0, interferencePow = 0,rate = 0;
    map<int, double> mSINRTempNull;//定义一个空的map，用于无主服务基站或者无发射信号时赋空值
    int RBNo = 0;//用于存储RB块内有SINR值得map，所以把RB号作为键来存储
    thermalNoisePow = -111;//算在10个RB块，即2MHZ带宽上的热噪声，-174dBm/HZ + 10log(2*10^6)
    thermalNoisePow = pow(10, (thermalNoisePow - 30) / 10);//线性值
//    thermalNoisePow = -174 + 10 * log10(4000000);//算在20个RB块，即4MHZ带宽上的热噪声，-174dBm/HZ + 10log(4*10^6)
//    thermalNoisePow = -174 + 10 * log10(18000);//算在1个RB块，即0.18MHZ带宽上的热噪声，-174dBm/HZ + 10log(0.18*10^6)

    for (auto vTemp : *vLinklossPowerVecMap)
    {//RB begin
        map<int, double> mSINRTemp;

        //对每个RB块，算SINR，因为没有接收端干扰登记表，接收机不知道要在哪些RB块上接收信号，只能挨个RB查找自己的主服务基站有没有在RB上，若有，算SINR
        if (vTemp.size() != 0 && vecMapMainTxInfo.size() != 0)//如果map中有值，即当前RB下有发射用户存在
        {//if1 begin
            //先算出每个RB上总的信号+干扰功率
            double totalPow = 0;
            bool haveMainTx = false;//若该RB上虽然有发射信号，但没有一个是有用的主服务基站信号，则将vecMapSINR内层的map置为空，false--无主服务基站
            auto mapPtr1 = vTemp.begin();
            for (; mapPtr1 != vTemp.end(); mapPtr1++)
            {
                txPow = mapPtr1->second.second;//dBm
                txPow = pow(10, (txPow - 30) / 10);//W
                channelGain = pow(10, -mapPtr1->second.first / 10);//线性值
                totalPow += txPow * channelGain;
            }
            auto mapPtr = vTemp.begin();
            for (; mapPtr != vTemp.end(); mapPtr++)
            {
                //查找主服务基站表，看当前RB块的发射用户上是否有主服务基站，若没有，则SINR置为0
                for (auto txTemp : vecMapMainTxInfo)
                {
                    auto txPtr = txTemp.begin();
                    if (txPtr->first == mapPtr->first)//当前RB上存在主服务基站
                    {
                        haveMainTx = true;//将是否有主服务基站标记为真
                        //把该发射机作为主服务基站，其余的当成干扰，计算当前RB块上的SINR =信号功率/干扰功率+白噪功率，分子分母都是线性值
                        //channelGain = pow(10, -mapPtr1->second.first / 10);
                        txPow = mapPtr->second.second;//dBm
                        txPow = pow(10, (txPow - 30) / 10);//W
                        channelGain = pow(10, -mapPtr->second.first / 10);//线性值
                        signalPow = txPow * channelGain;//算出信号功率
                        if (totalPow == signalPow)
                        {
                            interferencePow = 0;
                        }
                        else
                        {
//                            interferencePow = 10 * log10(totalPow - signalPow);//算出干扰功率,dB值
//                            interferencePow = pow(10, interferencePow / 10);//线性值
                            interferencePow = totalPow - signalPow;
                        }

//                        noiseFig = thermalNoisePow * 180000 / 2;//线性值
//                        SystemDriveBus::systemSenseInterface.setNoisePower(noiseFig);

                        sinr = signalPow / (interferencePow + thermalNoisePow); //线性
                        sinr = 10 * log10(sinr);//dB值

                        //更新系统SINR
                        SystemDriveBus::systemSinrComputing.addSignalPow(signalPow);
                        SystemDriveBus::systemSinrComputing.addInterferencePow(interferencePow);
                        SystemDriveBus::systemSinrComputing.addNoisePow(thermalNoisePow + noiseFig);
                        SystemDriveBus::systemSinrComputing.updateSinr();

//                        noiseFig = 9;//算出噪声功率,dB值

//                        signalPow = 10 * log10(signalPow);//dB值

//                        sinr = signalPow - (interferencePow + thermalNoisePow); //单位dB

                        cout << "用户[" << this->dID << "]在[" << RBNo << "]号RB上的SINR为：" << sinr << endl;
                        mSINRTemp[mapPtr->first] = sinr;//将该主服务基站对应的SINR存起来
                        sinr = pow(10, sinr / 10);//线性值
                        rate+= 180 * log2(1 + sinr);
                    }
                }
            }
            if (!haveMainTx)//即该RB上有信号，但是无主服务信号,则直接将vecMapSINR内层的map置为空
            {
                cout << RBNo << "号RB上有信号，但是无主服务信号" << endl;
//                vecMapSINR.push_back(mSINRTempNull);
            }
            else
            {
                doubleMapSINR[RBNo] = mSINRTemp;
            }

        }//if1 end
        else//如果map中没有值，即当前RB下发射用户不存在，则直接将vecMapSINR内层的map置为空
        {
            //cout << RBNo << "号RB下发射用户不存在" << endl;
            //vecMapSINR.push_back(mSINRTempNull);
        }
        RBNo++;
    }//RB end
}

void SoftwareEntityRx::SetTxInfo(SoftwareEntityTx *softTx, pair<double, double>_point)//设置缓存所有可能的发射机信息容器
{
    map<SoftwareEntityTx *, pair<double, double>> mapTxInfo;
    mapTxInfo.insert(pair<SoftwareEntityTx *, pair<double, double>>(softTx, _point));
    vecMapTxInfo.push_back(mapTxInfo);//将某一发射机的软体类指针和坐标（x,y）存放到本接收机的发射机信息容器中
}

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
//            auto rx = dynamic_cast<SmallCell*>(_temp.second); //测试频谱感知
            auto rx = dynamic_cast<User*>(_temp.second);
            point.first = this->dXPoint;
            point.second = this->dYPoint;
            rx->software.softwareRx.SetTxInfo(this, point);//调用接收软体类的函数设置发射机坐标信息容器
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
    InterferenceRgister();	//对该发射机随机选择RB块进行数据包的发送，并登记在发射端的干扰登记表里
    Scheduler();	//进行调度,周期性地发送数据包
}

void SoftwareEntityTx::InterferenceRgister()
{
////    测试频谱感知，20个小蜂窝作为接收用户，分配20个RB
//    int selectedRB;
//    for (selectedRB = 0; selectedRB < SUBBNUM; selectedRB++)
//    {
//        InterferenceIndex::GetInstance().RFRegisterOneRB(this->GetDeviceID(), SystemDriveBus::iSlot, selectedRB);
//    }
//
//    selectedRB = 0;
//    for (auto _temp : SystemDriveBus::SlotDriveBus)
//    {
//        if (_temp.first >=30)
//        {
//            InterferenceIndex::GetInstance().RFRxRegisterOneRB(_temp.second->iGetID(), SystemDriveBus::iSlot, selectedRB);
//            selectedRB++;
//        }
//    }

    ////测试极化频谱共享，分配5个RB
    int selectedRB;
    //如果发射机是宏蜂窝基站，将5个RB都分配给唯一的蜂窝用户
    if (this->getTxType() == "class MacroCell *")
    {
        //登记发射机占用RB
        for (selectedRB = 0; selectedRB < SUBBNUM; selectedRB++)
        {
            InterferenceIndex::GetInstance().RFRegisterOneRB(this->GetDeviceID(), SystemDriveBus::iSlot, selectedRB);
        }
        //登记接收机占用RB
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.first >=30)
            {
                User *_tempUser = dynamic_cast<User *>(_temp.second);
                if (_tempUser->getUser_type() == "MacroCell")
                {
                    for (selectedRB = 0; selectedRB < SUBBNUM; selectedRB++)
                    {
                        InterferenceIndex::GetInstance().RFRxRegisterOneRB(_tempUser->iGetID(), SystemDriveBus::iSlot, selectedRB);
                    }
                }
            }
        }
    }

    //如果发射机是小蜂窝基站，每个小蜂窝用户分配1个RB
    if (this->getTxType() == "class SmallCell *")
    {
        //登记发射机占用RB
        for (selectedRB = 0; selectedRB < SUBBNUM; selectedRB++)
        {
            InterferenceIndex::GetInstance().RFRegisterOneRB(this->GetDeviceID(), SystemDriveBus::iSlot, selectedRB);
        }
        //登记接收机占用RB
        selectedRB = 0;
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.first >=30)
            {
                User *_tempUser = dynamic_cast<User *>(_temp.second);
                if (_tempUser->getUser_type() == "SmallCell")
                {
                    InterferenceIndex::GetInstance().RFRxRegisterOneRB(_temp.second->iGetID(), SystemDriveBus::iSlot, selectedRB);
                    selectedRB++;
                }
            }
        }
    }
}

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
                SystemDriveBus::ID2UserVariable.at(RxID)->packet_drop++;
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

