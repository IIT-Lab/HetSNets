#include <iostream>

#include "ImportExport.h"
#include "SystemDriveBus.h"
#include "Sense.h"
#include "Computing.h"

using namespace std;

//总线声明
int SystemDriveBus::iSlot;
sinrComputing SystemDriveBus::systemSinrComputing;
SenseInterface SystemDriveBus::systemSenseInterface;
map<int, statistic_variable*> SystemDriveBus::ID2UserVariable;

int main()
{
    //实例化一个系统对象
    ImportExport system = ImportExport::GetInstance();

    //配置仿真区域参数和仿真模式参数
    system.system_config();

    //统计系统SINR
    sinrComputing _systemSinrComputing = sinrComputing::GetInstance();
    SystemDriveBus::systemSinrComputing = _systemSinrComputing;

    srand((unsigned int)(time(NULL)));
    //构建发送和接收设备
    system.SetScene();

    //根据收发设备属性，构建信道对象，对接收机循环
    cout << "信道构建开始 " << endl;
    ElementConstructionSet::channelManager();
    cout << "信道构建结束" << endl;

    //调用所有的初始化函数，所有基站把自己写入到干扰登记表中
    SystemDriveBus::iSlot = 0;
    srand((unsigned int)time(0)); //保证每次运行程序产生的随机数不一样
    double i = rand(); //随机数引擎的初始化种子

    cout << "初始化函数开始 " << endl;
    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        i += 100;//为了使两次随机数变化更大一点
        std::default_random_engine dre(i);//保证循环每次的随机数引擎的初始值不相同
        _temp.second->initial(dre);
    }
    cout << "初始化函数结束" << endl;

    //配置参数，测试用
    int sense_test = SystemDriveBus::system_sense.get_sence(); //测试频谱感知
    if (sense_test)
    {
        system.GenerateSenseFile();//创建存储频谱感知参数的matlab文件
        SenseInterface tempSenseInterface = SenseInterface::GetInstance();
        SystemDriveBus::systemSenseInterface = tempSenseInterface;
    }

    system.GenerateChannelGainFile();//创建存储信道增益的matlab文件

    //初始化用户的响应统计量
    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first >= 30 && _temp.second->sGetType() == "class User *")
        {
            User *_tempUser = dynamic_cast<User *>(_temp.second);
            int RxID = _tempUser->iGetID();
            statistic_variable* statistic_variablePtr = statistic_variable::Create();
            statistic_variablePtr->initial(RxID);
            SystemDriveBus::ID2UserVariable.insert(pair<int, statistic_variable*>(RxID, statistic_variablePtr));
        }
    }

//    int slot = SystemDriveBus::system_sense.get_numOfSlot();//总时隙数
    int slot = 1;

    //开始进行时隙循环
    cout << "时隙循环开始" << endl;
    while (SystemDriveBus::iSlot != slot)
    {
//        if (!(SystemDriveBus::iSlot % 1000)) SystemDriveBus::systemSinrComputing.clearSinr(); //1000个时隙统计一次SINR，清空SINR

        i += 100;//为了使两次随机数变化更大一点
        std::default_random_engine dre(i); //保证循环每次的随机数引擎的初始值不相同

        cout << "当前第 " << SystemDriveBus::iSlot << " 个时隙 " << endl;

        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            _temp.second->WorkSlot(dre);
        }

//        if (sense_test)
//        {
//            SystemDriveBus::systemSenseInterface.SetSenseInterface();
//            if (!(SystemDriveBus::iSlot % 1000)) //1000个时隙统计一次SINR
//            {
//                cout << "SINR:" << SystemDriveBus::systemSinrComputing.getSinr() << endl;
//                SystemDriveBus::systemSenseInterface.addSinr(SystemDriveBus::systemSinrComputing.getSinr());
//            }
//            SystemDriveBus::systemSenseInterface.WriteSenseFile(SystemDriveBus::iSlot);
//        }

        if (1)
        {
            set<int> sTxID;
            int rxID;
            double ChannelGain;
            ImportExport::fout << "H = [" << endl;
            for (auto _temp : SystemDriveBus::SlotDriveBus)
            {
                if (_temp.second->sGetType() == "class channel *")
                {
                    channel *_tempChannel = dynamic_cast<channel *>(_temp.second);
                    sTxID = _tempChannel->getSTxID();
                    rxID = _tempChannel->GetRxID();
                    for (auto txID : sTxID)
                    {
                        ChannelGain = _tempChannel->GetChannelGain(txID);
                        ImportExport::fout << ChannelGain << ",";
                    }
                    ImportExport::fout << ";" << endl;
                }
            }
            ImportExport::fout << "];" << endl;
        }

        SystemDriveBus::iSlot++;
    }

//    cout<< "系统平均SINR:" << SystemDriveBus::systemSinrComputing.getSinr() << endl;

    return 0;
}

/************************************************************************************************
函数名称：
主要功能：
输入参数：
输出参数：
其他信息：
*************************************************************************************************/