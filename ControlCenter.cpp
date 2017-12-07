//
// Created by lee on 17-11-17.
//
#include "ControlCenter.h"
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

}
