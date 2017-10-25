//
// Created by lee on 17-7-28.
//
#include "ImportExport.h"
#include "Device.h"
#include "SystemDriveBus.h"
//#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

//总线声明
shape_par SystemDriveBus::system_shape;
map<int, mode_par> SystemDriveBus::Priority2Par;
map<int, mode_par> SystemDriveBus::ModeID2Par;
sense_par SystemDriveBus::system_sense;//存储系统频谱感知参数
map<int, deviceLocation> SystemDriveBus::cellPosition;//存放所有的Macro基站的位置
map<int, deviceLocation> SystemDriveBus::SmallCellPosition;//存放所有的SmallCell基站的位置
map<int, deviceLocation> SystemDriveBus::WifiPosition;//存放所有的Wifi基站的位置

map<int, Interface*> SystemDriveBus::ID2PtrBus;//ID到基类指针映射的登记表，所有系统中的地形和设备都需要登记
multimap<string, Interface*>SystemDriveBus::Type2PtrBus; //存储生成对象的类型和指针的容器
multimap<int, Interface*> SystemDriveBus::SlotDriveBus;//时隙驱动总线登记表，int是设备的优先级，0-9为系统保留优先级，10-19为发射机优先级，20-29为信道优先级，30-39为接收机优先级
multimap<int, Interface*> SystemDriveBus::DisplayDriveBus;//显示函数优先级，int是设备优先级，所有需要显示的设备都要挂在到这个驱动总线上

ImportExport* ImportExport::_Instance = nullptr;
ofstream ImportExport::fout;                                                                         //静态成员初始化，类型+类名+作用域

ImportExport::ImportExport()
{

}

ImportExport& ImportExport::GetInstance()
{
    if (_Instance == nullptr)
    {
        _Instance = new ImportExport();
    }
    return *_Instance;
}

void int2string(int time_int, string &time_string)
/************************************************************************************************
函数名称：int2string
主要功能：将int类型转化为字符串
输入参数：系统时间（int类型和string类型）
输出参数：
其他信息：
*************************************************************************************************/
{
    stringstream time_temp;
    time_temp << time_int;
    time_temp >> time_string;
}

void ImportExport::GenerateFileName()
/************************************************************************************************
函数名称：GenerateFileName
主要功能：根据系统时间生成文件名
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    const time_t t = time(NULL);
    struct tm current_time;
    localtime_r(&t, &current_time);

    string temp;
    int2string(current_time.tm_year + 1900, temp);
    string t1 = temp;
    string stra;
    stra = stra + t1;
    int2string(current_time.tm_mon + 1, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_mday, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_hour, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_min, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_sec, temp);
    t1 = temp;
    stra = stra + t1;                                                                          //读取系统时间，并拼接成字符串

    string sRelativeFileName = "HetSnets_matlab/PlotMap" + stra + ".m";                                         //把.m文件命名成带系统时间的，方便存储每次运行的结果
    fout.open(sRelativeFileName);                                                               //打开文件，若不存在则新建一个
}

void ImportExport::GenerateSenseFile()
/************************************************************************************************
函数名称：GenerateSenseFile
主要功能：根据系统时间生成频谱感知matlab文件名
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    const time_t t = time(NULL);
    struct tm current_time;
    localtime_r(&t, &current_time);

    string temp;
    int2string(current_time.tm_year + 1900, temp);
    string t1 = temp;
    string stra;
    stra = stra + t1;
    int2string(current_time.tm_mon + 1, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_mday, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_hour, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_min, temp);
    t1 = temp;
    stra = stra + t1;
    int2string(current_time.tm_sec, temp);
    t1 = temp;
    stra = stra + t1;                                                                          //读取系统时间，并拼接成字符串

    string sRelativeFileName = "HetSnets_matlab/SenseMain" + stra + ".m";                                         //把.m文件命名成带系统时间的，方便存储每次运行的结果
    fout.open(sRelativeFileName);                                                               //打开文件，若不存在则新建一个
}

void ImportExport::SetScene()
/************************************************************************************************
函数名称：SetScene
主要功能：将构建好的对象指针存放到总线上，并调用总线里对象指针对应的display函数，由matlab打印出地图
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    cout << "*****************总线构造开始*****************" << endl;

    GenerateFileName();
    Interface* _InterfaceTemp;

    for (auto temp_mode_par : SystemDriveBus::ModeID2Par)
    {
        if (temp_mode_par.first == 1)//LTE
        {
            //读取LTE（MacroCell）系统参数
            mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
            int Macro_num = Macro_mode_par.get_numOfTx();
            double Macro_radius = Macro_mode_par.get_radius();
            int Macro_user_num = Macro_mode_par.get_numOfRx();
            MacroCell::CellularSysInit(Macro_num, Macro_radius);

            for (int i = 0; i < Macro_num; i++)//创建宏蜂窝小区对象
            {
                _InterfaceTemp = MacroCell::Create();
                SetPtr2Bus(_InterfaceTemp);
            }

            for (int i = 0; i < Macro_user_num; i++)
            {
                _InterfaceTemp = User::Create("MacroCell");
                SetPtr2Bus(_InterfaceTemp);
            }
        }
        if (temp_mode_par.first == 2)//SmallCell
        {
            mode_par SmallCell_mode_par = SystemDriveBus::ModeID2Par.at(2);
            int SmallCell_num = SmallCell_mode_par.get_numOfTx();
            int SmallCell_user_num = SmallCell_mode_par.get_numOfRx();

            for (int i = 0; i < SmallCell_num; i++)
            {
                _InterfaceTemp = SmallCell::Create();
                SetPtr2Bus(_InterfaceTemp);
            }

            for (int j = 0; j < SmallCell_user_num; ++j)
            {
                _InterfaceTemp = User::Create("SmallCell");
                SetPtr2Bus(_InterfaceTemp);
            }
        }
        if (temp_mode_par.first == 3)//Wifi
        {
            mode_par Wifi_mode_par = SystemDriveBus::ModeID2Par.at(3);
            int Wifi_num = Wifi_mode_par.get_numOfTx();
            int Wifi_user_num = Wifi_mode_par.get_numOfRx();

            for (int i = 0; i < Wifi_num; ++i)
            {
                _InterfaceTemp = Wifi::Create();
                SetPtr2Bus(_InterfaceTemp);
            }

            for (int j = 0; j < Wifi_user_num; ++j)
            {
                _InterfaceTemp = User::Create("Wifi");
                SetPtr2Bus(_InterfaceTemp);
            }
        }
        if (temp_mode_par.first == 4)//D2D
        {
            mode_par D2D_mode_par = SystemDriveBus::ModeID2Par.at(4);
            int D2DTx_num = D2D_mode_par.get_numOfTx();
            int D2DRx_num = D2D_mode_par.get_numOfRx();

            for (int i = 0; i < D2DTx_num; i++)
            {
                _InterfaceTemp = User::Create("D2DTx");
                SetPtr2Bus(_InterfaceTemp);
            }
            for (int i = 0; i < D2DRx_num; i++)
            {
                _InterfaceTemp = User::Create("D2DRx");
                SetPtr2Bus(_InterfaceTemp);
            }
        }
    }

    PushSceneToMySQL(); //将总线上的对象信息存入数据库

    for (auto vDisplayBusElement : SystemDriveBus::DisplayDriveBus)                                //遍历总线里的指针，调用Display函数，画出地图
    {
        vDisplayBusElement.second->Display();
    }

    if (SystemDriveBus::system_shape.get_shape() == "circle")
    {
        ImportExport::fout << "hold on;" << endl;
        ImportExport::fout << "alpha = 0:pi / 50 : 2 * pi; " << endl;
        ImportExport::fout << "x = 0 + " << SystemDriveBus::system_shape.get_radius() << " * cos(alpha);" << endl;
        ImportExport::fout << "y = 0 + " << SystemDriveBus::system_shape.get_radius() << " * sin(alpha);" << endl;
        ImportExport::fout << "p = plot(x, y, '-');" << endl;
        ImportExport::fout << "set(p, 'Color', [0.5 0.5 0.5], 'LineWidth', 0.5);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    ImportExport::fout << "axis equal" << endl;
    ImportExport::fout << "title('基站和用户位置分布图');" << endl;
    //ImportExport::fout << "legend([h1, h2, h3, h4], 'Macro基站', 'SmallCell基站', 'Wifi基站', '用户');" << endl;
    ImportExport::fout << "legend([h1, h2], 'Macro基站', 'SmallCell基站');" << endl;

    ImportExport::fout.close();
}

void ImportExport::PushSceneToMySQL()
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "lee", "281217", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        //清空数据库中的数据表
        string cleanMacroBaseStation = "DELETE FROM MacroBaseStation";
        if (mysql->insert(cleanMacroBaseStation))
        {
            cout << "删除成功" << endl;
        }
        else cout << "删除失败" << endl;

        string cleanSmallCellBaseStation = "DELETE FROM SmallCellBaseStation";
        if (mysql->insert(cleanSmallCellBaseStation))
        {
            cout << "删除成功" << endl;
        }
        else cout << "删除失败" << endl;

        string cleanUser = "DELETE FROM User";
        if (mysql->insert(cleanSmallCellBaseStation))
        {
            cout << "删除成功" << endl;
        }
        else cout << "删除失败" << endl;

        //将基站和用户信息插入数据库
        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.second->sGetType() == "class MacroCell *")
            {
                MacroCell *_tempMacroCell = dynamic_cast<MacroCell *>(_temp.second);

                //数字 转 string
                string iID = intToString(_tempMacroCell->iGetID());
                string macroID = intToString(_tempMacroCell->GetmacroID());
                string dXPoint = doubleToString(_tempMacroCell->GetXPoint());
                string dYPoint = doubleToString(_tempMacroCell->GetYPoint());

                //构造MySQL插入语句
                string SQLString = "INSERT INTO MacroBaseStation(iID, macroID, dXPoint, dYPoint) "
                                           "VALUES(" + iID + "," + macroID + "," + dXPoint + "," + dYPoint + ")";

                if (mysql->insert(SQLString))
                {
                    cout << "插入成功" << endl;
                }
                else cout << "执行失败" << endl;
            }

            if (_temp.second->sGetType() == "class SmallCell *")
            {
                SmallCell *_tempSmallCell = dynamic_cast<SmallCell *>(_temp.second);

                //数字 转 string
                string iID = intToString(_tempSmallCell->iGetID());
                string SmallCellID = intToString(_tempSmallCell->getSmallCellID());
                string dXPoint = doubleToString(_tempSmallCell->GetXPoint());
                string dYPoint = doubleToString(_tempSmallCell->GetYPoint());
                string relativedXPoint = doubleToString(_tempSmallCell->getRelativeXPoint());
                string relativedYPoint = doubleToString(_tempSmallCell->getRelativeYPoint());
                string cellID = intToString(_tempSmallCell->getCellID());

                //构造MySQL插入语句
                string SQLString = "INSERT INTO SmallCellBaseStation(iID, SmallCellID, dXPoint, dYPoint, relativedXPoint, relativedYPoint, cellID) "
                                           "VALUES(" + iID + "," + SmallCellID + "," + dXPoint + "," + dYPoint + "," + relativedXPoint + "," + relativedYPoint + "," + cellID + ")";

                if (mysql->insert(SQLString))
                {
                    cout << "插入成功" << endl;
                }
                else cout << "执行失败" << endl;
            }

            if (_temp.second->sGetType() == "class User *")
            {
                User *_tempUser = dynamic_cast<User *>(_temp.second);

                //数字 转 string
                string iID = intToString(_tempUser->iGetID());
                string UserID = intToString(_tempUser->getUserID());
                string dXPoint = doubleToString(_tempUser->getDXPoint());
                string dYPoint = doubleToString(_tempUser->getDYPoint());
                string relativedXPoint = doubleToString(_tempUser->getRelativeXPoint());
                string relativedYPoint = doubleToString(_tempUser->getRelativeYPoint());
                string cellID = intToString(_tempUser->getCellID());
                string SmallCellID = intToString(_tempUser->getSmallCellID());

                //构造MySQL插入语句
                string SQLString = "INSERT INTO User(iID, UserID, dXPoint, dYPoint, relativedXPoint, relativedYPoint, cellID, SmallCellID) "
                                           "VALUES(" + iID + "," + UserID + "," + dXPoint + "," + dYPoint + "," + relativedXPoint + "," + relativedYPoint + "," + cellID + "," + SmallCellID + ")";
                if (mysql->insert(SQLString))
                {
                    cout << "插入成功" << endl;
                }
                else cout << "执行失败" << endl;
            }
        }
    } else cout << "连接未建立" << endl;
}

void ImportExport::PushChannelToMySQL()
{

}

void ImportExport::SetSceneByMySQL()
{

}

void ImportExport::system_config()
/************************************************************************************************
函数名称：system_config
主要功能：读取数据库中的配置信息，配置系统仿真区域参数，通信系统参数
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    cout << "*****************仿真区域*****************" << endl;
    shape_par _system_shape = shape_par::GetInstance();
    _system_shape.set_shape();
    SystemDriveBus::system_shape = _system_shape;

    cout << "*****************仿真模式*****************" << endl;
    vector<string> all_mode = mode_par::GetInstance().get_all_mode();
    for (auto temp_mode : all_mode)
    {
        cout << "------------" << temp_mode << "------------" << endl;
        mode_par _mode_par = mode_par::GetInstance(temp_mode);
        _mode_par.set_mode();
        SystemDriveBus::ModeID2Par.insert(pair<int, mode_par>(_mode_par.get_modeID(), _mode_par));
        SystemDriveBus::Priority2Par.insert(pair<int, mode_par>(_mode_par.get_priority(), _mode_par));
    }

//    cout << "*****************频谱感知*****************" << endl;
    sense_par _sense_par = sense_par::GetInstance();
    _sense_par.set_sense();
    SystemDriveBus::system_sense = _sense_par;
}

void ImportExport::SetPtr2Bus(Interface *_InterfaceTemp)
/************************************************************************************************
函数名称：SetPtr2Bus
主要功能：将设备对象的指针存入总线
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    SystemDriveBus::ID2PtrBus.insert(pair<int, Interface*>(_InterfaceTemp->iGetID(), _InterfaceTemp));
    SystemDriveBus::DisplayDriveBus.insert(pair<int, Interface*>(_InterfaceTemp->iGetPriority(), _InterfaceTemp));
    SystemDriveBus::SlotDriveBus.insert(pair<int, Interface*>(_InterfaceTemp->iGetPriority(), _InterfaceTemp));
}


