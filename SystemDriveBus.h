//
// Created by lee on 17-7-31.
//

#ifndef HETSNETS_SYSTEMDRIVEBUS_H
#define HETSNETS_SYSTEMDRIVEBUS_H

#include <iostream>
#include <map>
#include <string>
#include "Configuration.h"
#include "Device.h"
#include "channel.h"
#include "Computing.h"
#include "Sense.h"
#include "HARQACKTreatment.h"

using namespace std;

namespace SystemDriveBus
{
#ifdef MAIN
    shape_par system_shape;
    map<int, mode_par> Priority2Par;
    map<int, mode_par> ModeID2Par;
    sense_par system_sense;//存储系统频谱感知参数
    map<int, deviceLocation> cellPosition;//存放所有的Macro基站的位置
    map<int, deviceLocation> SmallCellPosition;//存放所有的SmallCell基站的位置
    map<int, deviceLocation> WifiPosition;//存放所有的Wifi基站的位置
    sinrComputing systemSinrComputing;//存储系统SINR参数
    SenseInterface systemSenseInterface;//频谱感知接口

    map<int, Interface*> ID2PtrBus;//ID到基类指针映射的登记表，所有系统中的地形和设备都需要登记
	multimap<string, Interface*>Type2PtrBus; //存储生成对象的类型和指针的容器
	multimap<int, Interface*> SlotDriveBus;//时隙驱动总线登记表，int是设备的优先级，0-9为系统保留优先级，10-19为发射机优先级，20-29为信道优先级，30-39为接收机优先级
	multimap<int, Interface*> DisplayDriveBus;//显示函数优先级，int是设备优先级，所有需要显示的设备都要挂在到这个驱动总线上

	int iCountDevice = 0;//给生成的设备计数
    int iSlot;//时隙号

#else
    extern shape_par system_shape;//存储系统仿真区域参数
    extern map<int, mode_par> Priority2Par;//通信模式优先级——通信模式参数
    extern map<int, mode_par> ModeID2Par; //通信模式ID——通信模式参数，modeID,1:Macro,2:SmallCell,3:Wifi,4:D2D
    extern sense_par system_sense;//存储系统频谱感知参数
    extern map<int, deviceLocation> cellPosition;//存放所有的Macro基站的位置
    extern map<int, deviceLocation> SmallCellPosition;//存放所有的SmallCell基站的位置
    extern map<int, deviceLocation> WifiPosition;//存放所有的Wifi基站的位置
    extern sinrComputing systemSinrComputing;//存储系统SINR参数
    extern SenseInterface systemSenseInterface;//频谱感知接口
    extern map<int, statistic_variable*> ID2UserVariable;//用户ID——用户的响应统计量

    extern map<int, Interface*> ID2PtrBus;//ID到基类指针映射的登记表，所有系统中的地形和设备都需要登记
    extern multimap<string, Interface*>Type2PtrBus; //存储生成对象的类型和指针的容器
    extern multimap<int, Interface*> SlotDriveBus;//时隙驱动总线登记表，int是设备的优先级，0-9为系统保留优先级，10-19为发射机优先级，20-29为信道优先级，30-39为接收机优先级
    extern multimap<int, Interface*> DisplayDriveBus;//显示函数优先级，int是设备优先级，所有需要显示的设备都要挂在到这个驱动总线上

    extern int iCountDevice;//给生成的设备计数
    extern int iSlot;//时隙号

#endif

    static int GetCountDevice()
    {
        return iCountDevice++;
    }

    static int GetCountChannel()
    {
        static int iCountChannel = 0;
        if (channel::countTag != 0)
        {
            return iCountChannel++;
        }
        else
        {
            channel::countTag = 1;
            iCountChannel = 0;
            return iCountChannel++;
        }
    }

    static int GetCountMacro()
    {
        static int iCountMacro = 0;
        if (MacroCell::countTag != 0)
        {
            return iCountMacro++;
        }
        else
        {
            MacroCell::countTag = 1;
            iCountMacro = 0;
            return iCountMacro++;
        }
    }

    static int GetCountSmallCell()
    {
        static int iCountSmallCell = 0;
        if (SmallCell::countTag != 0)
        {
            return iCountSmallCell++;
        }
        else
        {
            SmallCell::countTag = 1;
            iCountSmallCell = 0;
            return iCountSmallCell++;
        }
    }

    static int GetCountWifi()
    {
        static int iCountWifi = 0;
        if (Wifi::countTag != 0)
        {
            return iCountWifi++;
        }
        else
        {
            Wifi::countTag = 1;
            iCountWifi = 0;
            return iCountWifi++;
        }
    }

    static int GetCountUser()
    {
        static int iCountUser = 0;
        if (User::countTag != 0)
        {
            return iCountUser++;
        }
        else
        {
            User::countTag = 1;
            iCountUser = 0;
            return iCountUser++;
        }
    }

    static Interface* ID2Ptr(int ID)
    {
        if (ID2PtrBus.find(ID) != ID2PtrBus.end())
        {
            return ID2PtrBus.at(ID);
        }
        else
            return nullptr;
    }
}

namespace ElementConstructionSet
{
    static void channelManager()
    {
        vector<Interface*>vTxContainer;
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.first >= 10 && _temp.first < 20)
            {
                vTxContainer.push_back(_temp.second);//按顺序先把发射容器填满
            }
            if (_temp.first >= 30 && _temp.first < 40)//开始遍历接收容器
            {
                //调用信道对象构造函数,对每个接收端构建信道对象
                channel *channelObj = new channel;
                //ChannelModelTool::LoadLargeScaleFadingModel();
                channelObj->ChannelInitial(*_temp.second, vTxContainer);
                SystemDriveBus::SlotDriveBus.insert(pair<int, Interface*>(channelObj->iGetPriority(), channelObj));
            }
        }
    }
}

#endif //HETSNETS_SYSTEMDRIVEBUS_H
