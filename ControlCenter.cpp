//
// Created by lee on 17-11-17.
//
#include "ControlCenter.h"
#include "psshn_platform/psshn_platform.h"
#include "SystemDriveBus.h"

ControCenter::ControCenter() {
    iID = 99999;
    iPriority = 50;//设置该对象的优先级是50
    sType = "class ControlCenter *";
}

ControCenter::~ControCenter() {

}

Interface * ControCenter::Create()
/************************************************************************************************
函数名称：Create
主要功能：调用BS的构造函数，并返回BS的对象指针
输入参数：
输出参数：BSPtr，BS的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *ControCenterPtr;
    ControCenterPtr = new ControCenter();
    return ControCenterPtr;
}

void ControCenter::WorkSlot(default_random_engine dre) {
    double P_mue[2];
    creal_T P_cue[2];
    creal_T P_cbs_data[2];
    int P_cbs_size[2];
    creal_T H_mbscue[4];
    creal_T H_cbscue[4];
    creal_T H_cbsmue[4];

    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first >= 30 && _temp.second->sGetType() == "class User *")
        {
            User *_tempUser = dynamic_cast<User *>(_temp.second);
            if (_tempUser->getUser_type() == "SmallCell")
            {
                psshn_platform(P_mue, P_cue, P_cbs_data, P_cbs_size, H_mbscue, H_cbscue, H_cbsmue);

                for (int i = 0; i < 2; ++i) {
                    _tempUser->software.softwareRx.P_mue[i] = P_mue[i];
                    _tempUser->software.softwareRx.P_cue[i] = P_cue[i];
                    _tempUser->software.softwareRx.P_cbs_data[i] = P_cbs_data[i];
                }

                for (int i = 0; i < 4; ++i) {
                    _tempUser->software.softwareRx.H_mbscue[i] = H_mbscue[i];
                    _tempUser->software.softwareRx.H_cbscue[i] = H_cbscue[i];
                    _tempUser->software.softwareRx.H_cbsmue[i] = H_cbsmue[i];
                }
            }
        }
    }
}
