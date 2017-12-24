//
// Created by lee on 17-8-28.
//
#include "HardwareEntity.h"
#include "SystemDriveBus.h"

///////////////////////////硬体类///////////////////////////////
HardwareEntity::HardwareEntity()
{

}

void HardwareEntity::CreateHardwareEntity()
{

}



int HardwareEntity::GetDeviceID()//获取用户的ID
{
    return dID;
}

void HardwareEntity::ConnectID(int iID)//将用户的id取过来
{
    dID = iID;
    //SystemDriveBus::ID2Power.insert(pair<int, double>(this->dID, txPower));
}


void HardwareEntity::ConnectstartSlot(int startSlot)
{
    iStartSlot = startSlot;
}

void HardwareEntity::WorkslotHardwareEntityTx()
{
    //////////////////////////////////////////////////////////////发射端的硬体
    //发射机遍历所有的信道对象，将自己的功率登记到信道里
    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first >= 20 && _temp.first < 30)//遍历全部的信道对象
        {
            auto channelObj = dynamic_cast<channel *>(_temp.second);
            channelObj->setAllTxPowerMap(this->GetDeviceID(), this->txPower);
        }
    }
}

void HardwareEntity::WorkslotHardwareEntityRx()
{
    //////////////////////////////////////////////////////////////接收端的硬体

    //首先寻找与接收机自己对应的信道对象
    channel *myChannelPtr = nullptr;
    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first >= 20 && _temp.first < 30)//遍历全部的信道对象
        {
            auto channelObj = dynamic_cast<channel *>(_temp.second);
            if (channelObj->GetRxID() == this->GetDeviceID())
            {
                myChannelPtr = channelObj;
                break;
            }
        }
    }
    //将路损表的指针传给信道对象，在信道里查询干扰登记表，调用信道对象的setmLinkLossPtr函数,写入指针
    TLinklossPowerVecMap.clear();	//每个时隙接收端都会向信道进行设置路损，所以每次时隙循环都要清除掉
//    if (myChannelPtr != nullptr) myChannelPtr->setmLinkLossPtr(&TLinklossPowerVecMap);
}

vector<map<int, pair<double, double>>>& HardwareEntity::GetLinkloss()
{
    return TLinklossPowerVecMap;
}

///////////////////////////Macro硬体类///////////////////////////////

void MacroHardwareEntity::InitialHardwareEntity()
{
    //初始化发射功率
    txPower = SystemDriveBus::ModeID2Par.at(1).get_power();
//    txPower = 40;//dBm

////  测试频谱感知
//    txPower = 30 + (SystemDriveBus::iSlot / 1000);//dBm
//    double _txPower = pow(10, (txPower - 30) / 10);
//    SystemDriveBus::systemSenseInterface.addTxPower(_txPower);
//    cout << "功率:" << txPower << "dBm" << endl;
}

///////////////////////////SmallCell硬体类///////////////////////////////

void SmallCellHardwareEntity::InitialHardwareEntity()
{
    //初始化发射功率
    txPower = SystemDriveBus::ModeID2Par.at(2).get_power();
}

///////////////////////////User硬体类///////////////////////////////

void UserHardwareEntity::InitialHardwareEntity() {
    //初始化发射功率
    if (userType == "D2DTx") {
        txPower = 13;//dBm
    } else if (userType == "MacroCell") {
        txPower = 23;//dBm
    } else {
        cout << "set power error!!!" << endl;
    }
}

void UserHardwareEntity::ConnectUserType(string _userType) {
    userType = _userType;
}
