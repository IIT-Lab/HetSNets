#include <iostream>

#include "ImportExport.h"
#include "SystemDriveBus.h"

using namespace std;

//总线声明
int SystemDriveBus::iSlot;
string SystemDriveBus::ULorDL;//系统属于上行链路还是下行链路

int main()
{
    if (0) {
        double sinr = 0;
        double rate = 0;
        double systemCapacity = 0;
        int macroUserNum = 10;
        int D2DNum = 30;
        int RBNum = 10;
        int slotNum = 16;

        /******************************系统容量统计*******************************/
        cout << "CUENUM:" << macroUserNum << endl;
        cout << "D2DNUM:" << D2DNum << endl;
        cout << "RBNUM:" << RBNum << endl;

        cout << "----------普通图----------" << endl;
        systemCapacity = GetSystemCapacity(macroUserNum, D2DNum, RBNum, 0);
        cout << "平均系统容量:" << systemCapacity << endl;

        cout << "----------超图----------" << endl;
        systemCapacity = GetSystemCapacity(macroUserNum, D2DNum, RBNum, 1);
        cout << "平均系统容量:" << systemCapacity << endl;

        cout << "----------经典贪婪----------" << endl;
        systemCapacity = GetSystemCapacity(macroUserNum, D2DNum, RBNum, 2);
        cout << "平均系统容量:" << systemCapacity << endl;

        cout << "----------干扰区域超图----------" << endl;
        systemCapacity = GetSystemCapacity(macroUserNum, D2DNum, RBNum, 3);
        cout << "平均系统容量:" << systemCapacity << endl;
        /******************************系统容量统计*******************************/

        cout << endl;
        cout << "/////////////////////普通图：" << endl;
        cout << "*********************蜂窝用户***********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = 1; TxID <= macroUserNum; ++TxID) {
                sinr = GetSinr(slot * 4, TxID);

//            if (sinr == 0) continue;
////            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }
        cout << "**********************D2D**********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = macroUserNum + 1; TxID <= macroUserNum + D2DNum; ++TxID) {
                sinr = GetSinr(slot * 4, TxID);

//            if (sinr == 0) continue;
////            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }

        cout << endl;
        cout << "/////////////////////超图：" << endl;
        cout << "*********************蜂窝用户***********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = 1; TxID <= macroUserNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 1, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }
        cout << "**********************D2D**********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = macroUserNum + 1; TxID <= macroUserNum + D2DNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 1, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }

        cout << endl;
        cout << "/////////////////////经典贪婪：" << endl;
        cout << "*********************蜂窝用户***********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = 1; TxID <= macroUserNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 2, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }
        cout << "**********************D2D**********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = macroUserNum + 1; TxID <= macroUserNum + D2DNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 2, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }

        cout << endl;
        cout << "/////////////////////干扰区域超图：" << endl;
        cout << "*********************蜂窝用户***********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = 1; TxID <= macroUserNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 3, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }
        cout << "**********************D2D**********************" << endl;
        for (int slot = 0; slot < slotNum / 4; slot++) {
            for (int TxID = macroUserNum + 1; TxID <= macroUserNum + D2DNum; ++TxID) {
                sinr = GetSinr(slot * 4 + 3, TxID);

//            if (sinr == 0) continue;
//            cout << "TxID: " << TxID << " , SINR: " << sinr << endl;
//            cout << sinr << ";" << endl;

                if (sinr == 0) {
                    rate = 0;
                } else {
                    sinr = pow(10, sinr / 10);
                    rate = log2(1 + sinr);
                }
                cout << rate << ";" << endl;
            }
        }

        return 0;
    }

    SystemDriveBus::ULorDL = "UL"; //上行链路

    //实例化一个系统对象
    ImportExport system = ImportExport::GetInstance();
    system.ClearMySQL();

    //配置仿真区域参数和仿真模式参数
    system.system_config();

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

    int slot = 100;

//    SystemDriveBus::iSlot = -1; //测试!!!!!!!!!!!!!!!!!!!!!!

    //开始进行时隙循环
    cout << "*****************时隙循环开始*****************" << endl;
    while (SystemDriveBus::iSlot != slot)
    {
        i += 100;//为了使两次随机数变化更大一点
        std::default_random_engine dre(i); //保证循环每次的随机数引擎的初始值不相同

        cout << "当前第 " << SystemDriveBus::iSlot << " 个时隙 " << endl;

        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            _temp.second->WorkSlot(dre);
        }

        //每次循环结束　统计一次系统容量
        double sumRatePerSlot;
        sumRatePerSlot = GetSumRate(SystemDriveBus::iSlot);
        sumRatePerSlot = sumRatePerSlot / 180000 / RBNUM;
        cout << "********************************************" << endl;
        cout << "第" << SystemDriveBus::iSlot << "个时隙的系统容量:" << sumRatePerSlot << endl;
        cout << "********************************************" << endl;
        int cueNum = SystemDriveBus::ModeID2Par.at(1).get_numOfRx();
        int D2DNum = SystemDriveBus::ModeID2Par.at(4).get_numOfRx();
        if (SystemDriveBus::iSlot / 4 > 0) {
            pushSumRate(cueNum, D2DNum, RBNUM, sumRatePerSlot, SystemDriveBus::iSlot % 4);
        }

        SystemDriveBus::iSlot++;
    }

//    ////统计
//    cout << endl << endl;
//    cout << "//////////////////////////////////////////////////////////////////" << endl;
//    int cueNum = SystemDriveBus::ModeID2Par.at(1).get_numOfRx();
//    int D2DNum = SystemDriveBus::ModeID2Par.at(4).get_numOfRx();
//    cout << "CUE NUM: " << cueNum << endl;
//    cout << "D2D NUM: " << D2DNum << endl;
//    cout << "RB NUM: " << RBNUM << endl;
//
//    double graphSumRate;
//    graphSumRate = GetSumRate(0);
//    graphSumRate = graphSumRate / 180000 / RBNUM;
//    cout << "********************************************" << endl;
//    cout << "图着色的系统容量:" << graphSumRate << endl;
////    pushSumRate(cueNum, D2DNum, RBNUM, sumRate, 0);
//    cout << "********************************************" << endl;
//
//    double hypergraphSumRate;
//    hypergraphSumRate = GetSumRate(1);
//    hypergraphSumRate = hypergraphSumRate / 180000 / RBNUM;
//    cout << "********************************************" << endl;
//    cout << "超图着色的系统容量:" << hypergraphSumRate << endl;
////    pushSumRate(cueNum, D2DNum, RBNUM, sumRate, 1);
//    cout << "********************************************" << endl;
//
//    double SLAgraphSumRate;
//    SLAgraphSumRate = GetSumRate(2);
//    SLAgraphSumRate = SLAgraphSumRate / 180000 / RBNUM;
//    cout << "********************************************" << endl;
//    cout << "干扰区域图着色的系统容量:" << SLAgraphSumRate << endl;
////    pushSumRate(cueNum, D2DNum, RBNUM, sumRate, 2);
//    cout << "********************************************" << endl;
//
//    double SLAhypergraphSumRate;
//    SLAhypergraphSumRate = GetSumRate(3);
//    SLAhypergraphSumRate = SLAhypergraphSumRate / 180000 / RBNUM;
//    cout << "********************************************" << endl;
//    cout << "干扰区域超图着色的系统容量:" << SLAhypergraphSumRate << endl;
////    pushSumRate(cueNum, D2DNum, RBNUM, sumRate, 3);
//    cout << "********************************************" << endl;
//
////    if (graphSumRate < hypergraphSumRate && graphSumRate < SLAhypergraphSumRate && SLAhypergraphSumRate < hypergraphSumRate) {
//    if (graphSumRate <= hypergraphSumRate) {
//        if(1) {
////        if (graphSumRate <= SLAhypergraphSumRate) {
////            if(1) {
//            if (SLAgraphSumRate <= SLAhypergraphSumRate) {
//                pushSumRate(cueNum, D2DNum, RBNUM, graphSumRate, 0);
//                pushSumRate(cueNum, D2DNum, RBNUM, hypergraphSumRate, 1);
//                pushSumRate(cueNum, D2DNum, RBNUM, SLAgraphSumRate, 2);
//                pushSumRate(cueNum, D2DNum, RBNUM, SLAhypergraphSumRate, 3);
//            }
//        }
//    }

    return 0;
}

/************************************************************************************************
函数名称：
主要功能：
输入参数：
输出参数：
其他信息：
*************************************************************************************************/