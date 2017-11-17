//
// Created by lee on 17-8-1.
//

#ifndef HETSNETS_DEVICE_H
#define HETSNETS_DEVICE_H

#include "Interface.h"
#include "HardwareEntity.h"
#include "SoftwareEntity.h"
#include "ControlCenter.h"
#include "psshn_platform/psshn_platform.h"

struct deviceLocation
{
    double dXPoint;
    double dYPoint;
};

///////////////////////////蜂窝类///////////////////////////////

class MacroCell : public Interface
{
public:
    MacroCell();

    virtual ~MacroCell();

    static Interface * Create();
    void initial(default_random_engine dre);
    void Display();//打印函数
    void Out2MatlabFile();//将对象的位置坐标信息输出到.m文件
    void WorkSlot(default_random_engine dre);
    void JoinSection2TransData();
    static void CellularSysInit(int cell_num, double cell_radius); //生成所有基站的位置信息，存在cellPosition中
    double GetXPoint();
    double GetYPoint();
    int GetmacroID();

    void GenerateSample();
    const vector<int> &getVecSample() const;

    static int countTag;

    MacroHardwareEntity hardware;
    MacroSoftwareEntity software;

    double P_mue[2]; //宏蜂窝发射极化状态

private:
    double dXPoint;
    double dYPoint;
    double CellRadius;
    int macroID;

    vector<int> vecSample;
    int numOfSample;
};

///////////////////////////SmallCell类///////////////////////////////

class SmallCell : public Interface
{
public:
    SmallCell();
    static  Interface * Create();//创建函数
    void initial(default_random_engine dre);
    void JoinSection2TransData();
    void Display();//打印函数
    void Out2MatlabFile();//将对象的位置坐标信息输出到.m文件
    void WorkSlot(default_random_engine dre);
    double GetXPoint();
    double GetYPoint();

    int getCellID() const;

    static int countTag;

    double getRelativeXPoint() const;

    double getRelativeYPoint() const;

    int getSmallCellID() const;

    double getSmallCellRadius() const;

    SmallCellHardwareEntity hardware;
    SmallCellSoftwareEntity software;

    creal_T P_cbs_data[2]; //小蜂窝发射极化状态

private:
    double dXPoint;
    double dYPoint;
    double relativeXPoint;
    double relativeYPoint;
    int SmallCellID;
    int cellID;
    double SmallCellRadius;
};

///////////////////////////Wifi类///////////////////////////////

class Wifi : public Interface
{
public:
    Wifi();
    static  Interface * Create();//创建函数
    //void initial(default_random_engine dre);
    void Display();//打印函数
    void Out2MatlabFile();//将对象的位置坐标信息输出到.m文件
    double getDXPoint() const;
    double getDYPoint() const;

    static int countTag;
private:
    double dXPoint;
    double dYPoint;
    double relativeXPoint;
    double relativeYPoint;
    int WifiID;
    int cellID;
    double WifiRadius;
};

///////////////////////////总用户大类///////////////////////////////

class User :public Interface
{
public:
    User(string _user_type);
    static  Interface * Create(string user_type);//创建函数
    void initial(default_random_engine dre);
    void JoinSection2TransData();
    void WorkSlot(default_random_engine dre);
    void Display();//打印函数
    void Out2MatlabFile();//将对象的位置坐标信息输出到.m文件
    double getDXPoint() const;
    double getDYPoint() const;
    int getIndoorOrOutdoor() const;

    double getRelativeXPoint() const;

    double getRelativeYPoint() const;

    int getUserID() const;

    int getSmallCellID() const;

    int getCellID() const;

    const string &getUser_type() const;

    static int countTag;

    UserHardwareEntity hardware;
    UserSoftwareEntity software;

private:
    string user_type;
    int txID;
    double dXPoint;
    double dYPoint;
    double relativeXPoint;
    double relativeYPoint;
    int UserID;
    int WifiID;
    int SmallCellID;
    int cellID;
    int indoorOrOutdoor;
//室内室外标志, 1室内，0室外
};

#endif //HETSNETS_DEVICE_H
