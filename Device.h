//
// Created by lee on 17-8-1.
//

#ifndef HETSNETS_DEVICE_H
#define HETSNETS_DEVICE_H

#include "Interface.h"
#include "HardwareEntity.h"
#include "SoftwareEntity.h"
#include "ControlCenter.h"

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

    void Scheduler();//调度 进行干扰登记
    void PushRBAllocation2MySQL(int _TxID, int _RxID, int _RBID, int _slotID, double _power);
    void SetVecMacroUserID();
    void SetMapD2DUserID();
    void SetGraph(double _threshold);
    void SetHypergraph(double _threshold);

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

    vector<vector<int>> graph; //表示普通图的矩阵incidence Matrix
    vector<vector<int>> hypergraph; //表示超图的矩阵incidence Matrix
    vector<int> vecMacroUserID; //宏蜂窝用户的ID
    map<int, int> mapD2DUserID; //D2D用户的ID
    map<int, vector<int>> mapEdgeVecNodes;//map的键是边序号　vector储存对应的节点号
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
    User(Interface * _D2DRxPtr); //构造函数重载 D2D用户的构造函数
    static  Interface * Create(string user_type);//创建函数
    static  Interface * Create(Interface * _D2DRxPtr);//创建函数重载 D2D用户的构造函数
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

    int getMainTxID() const;

    int getD2DTxID() const;

    UserHardwareEntity hardware;
    UserSoftwareEntity software;

private:
    string user_type;
    int mainTxID;
    double dXPoint;
    double dYPoint;
    double relativeXPoint;
    double relativeYPoint;
    int UserID;
    int WifiID;
    int SmallCellID;
    int cellID;
    int indoorOrOutdoor;
    int D2DTxID; //若用户为D2DRx 该变量保存对应D2DTx的ID
//室内室外标志, 1室内，0室外
};

#endif //HETSNETS_DEVICE_H
