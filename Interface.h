//
// Created by lee on 17-8-1.
//

#ifndef HETSNETS_INTERFACE_H
#define HETSNETS_INTERFACE_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include <random>

using namespace std;

class Interface                                                             //公共接口类
{
protected:
    int iID;//每个设备的ID
    int iPriority = 10;//每个设备的优先级
    string sType;//每个设备的类型

public:
    Interface() {};                                                          //构造函数
    virtual int iGetID() { return iID; };
    virtual int iGetPriority() { return iPriority; };
    virtual string sGetType() { return sType; };
    virtual void Display() {};                                               //虚函数，用于子类继承并打印出对象的位置
    virtual void WorkSlot(default_random_engine dre) {};
    virtual void initial(default_random_engine dre) {}; //初始化函数
};

class Hardware
{
public:
    //构造函数
    Hardware() {};
    ~Hardware() {};

    //硬件类型的读写接口
    void SetHardwareType(string) {};
    string GetHardwareType() {};

    //硬件参数的读写接口
    void SetParameter(string, double) {};
    double GetParameter(string) {};

    //对硬件的基础操作
    void Print() {};
    void Work() {};
    void Initial() {};

protected:
    string sHardwareType;          //硬件类型
    map<string, double> mParameter;//存储硬件参数：类型-值
};

#endif //HETSNETS_INTERFACE_H
