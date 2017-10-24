//
// Created by lee on 17-9-26.
//

#include "InterferenceIndex.h"
#include <iostream>
using namespace std;

InterferenceIndex* InterferenceIndex::_Instance = nullptr;//静态成员初始化

InterferenceIndex& InterferenceIndex::GetInstance()
{
    if (_Instance == nullptr) {
        _Instance = new InterferenceIndex(8);
    }
    return *_Instance;
}

InterferenceIndex::InterferenceIndex(int iSlotN)
/************************************************************************************************
函数名称：构造函数
主要功能：将时隙存储占用表初始化为iSlotN个调度周期
输入参数：iSlotN（调度周期），如：调度周期是8，则相对时隙在0-7之间
输出参数：无输出参数
其他信息：本版本由王洁在韩秉君版本基础上编写，模块完成时间2015年3月18日上午9:00，经过初步测试
*************************************************************************************************/
{
    map<int, set<int>>mRFOccupationListTemp;
    iSlotNum = iSlotN;
    for (int _iSlotNum = 0; _iSlotNum != iSlotN; ++_iSlotNum)
    {
        vTxOccupationList.push_back(mRFOccupationListTemp);//做初始化
        vRxOccupationList.push_back(mRFOccupationListTemp);
    }
}

void InterferenceIndex::RFRegisterOneRB(int const iEquipmentID, int  iSlotID, int RB)
/************************************************************************************************
函数名称：RFRegister
主要功能：将某ID用户在某时隙的频谱占用情况存入时隙占用表上；如：用户1在时隙1上占用RB2、7、10，则在时隙1下，在RB2、7、10里分别填上用户ID1
输入参数：iEquipmentID，用户ID
iSlotID，时隙编号，输入的是绝对时隙
sRFOccupation，集合内的每一个int均代表一个被占用的RB
输出参数：无输出参数
其他信息：本版本由王洁在韩秉君版本基础上编写，模块完成时间2015年3月18日上午9:00，经过初步测试
*************************************************************************************************/
{
    iSlotID %= iSlotNum;                   //将绝对时隙变成相对时隙
    vTxOccupationList[iSlotID][RB].insert(iEquipmentID);   //将每个RB对应的时隙号，发射机号存储到大容器里
}

void InterferenceIndex::RFRxRegisterOneRB(int const iEquipmentID, int  iSlotID, int RB)
{
    iSlotID %= iSlotNum;                                     //将绝对时隙变成相对时隙
    vRxOccupationList[iSlotID][RB].insert(iEquipmentID);     //将每个RB对应的时隙号，接收机号存储到大容器里
}


void InterferenceIndex::GetRelevantUserTxRB(int const iRBIndex, int  iSlotID, set<int> &sRelevantUserList)
/******************************************************************************************************
函数名称：GetRelevantUserRB
主要功能：获取对某时隙某RB上所有用户的ID,如：给定时隙1，RB1，将查找到的用户列表存入sRelevantUserList
输入参数：iRBIndex（RB号），iSlotID（时隙编号），输入的是绝对时隙
输出参数：RelevantUserList
其他信息：本版本由王洁在韩秉君版本基础上编写，模块完成时间2015年3月18日上午9:00，经过初步测试
*********************************************************************************************************/
{
    iSlotID %= iSlotNum;                                                                         //将绝对时隙变成相对时隙
    auto RBTag = false;                                                                          //标记要查找的RB是否存在，初始化为不存在
    auto MapEnd = &vTxOccupationList[iSlotID];                                                   //MapEnd是时隙占用表内层map的引用
    if ((*MapEnd).find(iRBIndex) != (*MapEnd).end())                                             //若查找到RB
    {
        sRelevantUserList = (*MapEnd).at(iRBIndex);                                              //直接把RB对应的set<int>用户ID的引用拷给sRelevantUserList
        RBTag = true;                                                                            //将其标记为存在
    }
    if (!RBTag)                                                                                  //若不存在RB
    {
        std::cout << "时隙 " << iSlotID << " 下要查找的RB " << iRBIndex << " 不存在！退出GetRelevantUserRB函数！" << endl;
        return;
    }
}

void InterferenceIndex::GetRelevantUserRxRB(int const iRBIndex, int  iSlotID, set<int> &sRelevantUserList)
/******************************************************************************************************
函数名称：GetRelevantUserRB
主要功能：获取对某时隙某RB上所有用户的ID,如：给定时隙1，RB1，将查找到的用户列表存入sRelevantUserList
输入参数：iRBIndex（RB号），iSlotID（时隙编号），输入的是绝对时隙
输出参数：RelevantUserList
其他信息：本版本由王洁在韩秉君版本基础上编写，模块完成时间2015年3月18日上午9:00，经过初步测试
*********************************************************************************************************/
{
    iSlotID %= iSlotNum;                                                                         //将绝对时隙变成相对时隙
    auto RBTag = false;                                                                          //标记要查找的RB是否存在，初始化为不存在
    auto MapEnd = &vRxOccupationList[iSlotID];                                                   //MapEnd是时隙占用表内层map的引用
    if ((*MapEnd).find(iRBIndex) != (*MapEnd).end())                                             //若查找到RB
    {
        sRelevantUserList = (*MapEnd).at(iRBIndex);                                              //直接把RB对应的set<int>用户ID的引用拷给sRelevantUserList
        RBTag = true;                                                                            //将其标记为存在
    }
    if (!RBTag)                                                                                  //若不存在RB
    {
        std::cout << "时隙 " << iSlotID << " 下要查找的RB " << iRBIndex << " 不存在！退出GetRelevantUserRB函数！" << endl;
        return;
    }
}