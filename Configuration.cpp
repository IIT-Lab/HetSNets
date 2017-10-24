//
// Created by lee on 17-7-27.
//

#include "Configuration.h"
#include <sstream>

/************************************shape_par*****************************************/
shape_par* shape_par::_Instance = nullptr;
mode_par* mode_par::_Instance = nullptr;
sense_par* sense_par::_Instance = nullptr;

shape_par::shape_par()
{
    this->shape = "";
    this->length = -1;
    this->width = -1;
    this->radius = -1;
}

shape_par::~shape_par() {

}

shape_par &shape_par::GetInstance() {
    if (_Instance == nullptr)
    {
        _Instance = new shape_par();
    }
    return *_Instance;
}

void shape_par::set_shape()
/************************************************************************************************
函数名称：
主要功能：读取数据库中的仿真区域参数，配置shape_par对象
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus())
    {
        //仿真区域
        mysql->clearResultList();
        if(mysql->runSQLCommand("SELECT shape FROM shape"))
        {
            vector<vector<string> > result = mysql->getResult();
            for(auto & vec : result)
            {
                for(auto &str : vec)
                {
                    shape = str.c_str();
                    cout << "仿真区域：" << shape;
                }
                cout << endl;
            }
        }
        else
            cout << "执行失败" << endl;

        if (shape == "rectangle")
        {
            //矩形区域长
            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT length FROM shape"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        length = atof(str.c_str());
                        cout << "矩形区域长：" << length;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            //矩形区域宽
            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT width FROM shape"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        width = atof(str.c_str());
                        cout << "矩形区域宽：" << width;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;
        }
        else if (shape == "circle")
        {
            //圆形区域半径
            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT radius FROM shape"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        radius = atof(str.c_str());
                        cout << "圆形区域半径：" << radius;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;
        }
    }
    else cout << "连接未建立" << endl;
}

string shape_par::get_shape() {
    return shape;
}

double shape_par::get_length() {
    return length;
}

double shape_par::get_width() {
    return width;
}

double shape_par::get_radius() {
    return radius;
}

/************************************mode_par*****************************************/
mode_par::mode_par()
/************************************************************************************************
函数名称：无参构造函数
主要功能：读取数据库中的通信系统参数，查看有哪几种通信模式
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "lee", "281217", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        //仿真模式
        mysql->clearResultList();
        if (mysql->runSQLCommand("SELECT mode FROM mode")) {
            vector<vector<string> > result = mysql->getResult();
            for (auto &vec : result) {
                for (auto &str : vec) {
                    string temp_mode = str.c_str();
                    all_mode.push_back(temp_mode);
                }
            }
        } else cout << "执行失败" << endl;
    } else cout << "连接未建立" << endl;
}

mode_par::mode_par(string mode)
/************************************************************************************************
函数名称：有参构造函数
主要功能：根据不同的通信模式初始化mode_par对象
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    this->mode = mode;
    this->all_mode_par.push_back("priority");
    this->all_mode_par.push_back("numOfTx");
    this->all_mode_par.push_back("radius");
    this->all_mode_par.push_back("power");
    this->all_mode_par.push_back("numOfRx");

    this->priority = -1;
    this->numOfTx = -1;
    this->radius = -1;
    this->power = -1;
    this->numOfRx = -1;
}

mode_par& mode_par::GetInstance()
{
    if (_Instance == nullptr)
    {
        _Instance = new mode_par();
    }
    return *_Instance;
}
mode_par& mode_par::GetInstance(string mode)
{
    _Instance = nullptr;
    if (_Instance == nullptr)
    {
        _Instance = new mode_par(mode);
    }
    return *_Instance;
}

void mode_par::set_mode()
/************************************************************************************************
函数名称：
主要功能：读取数据库中的通信系统参数，配置mode_par对象
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "lee", "281217", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        int line_int = -1;
        //仿真模式
        mysql->clearResultList();
        if (mysql->runSQLCommand("SELECT mode FROM mode"))
        {
            vector<vector<string> > result = mysql->getResult();
            int i = 0;//行号
            for (auto &vec : result) {
                for (auto &str : vec) {
                    string temp_mode = str.c_str();
                    if (temp_mode == mode)
                    {
                        line_int = i;
                        break;
                    }
                    i++;
                }
            }
        } else cout << "执行失败" << endl;

        //设置modeID
        if (mode == "LTE") modeID = 1;
        if (mode == "SmallCell") modeID = 2;
        if (mode == "Wifi") modeID = 3;
        if (mode == "D2D") modeID = 4;

        //行号设置
        string line_string;
        stringstream line_temp;
        line_temp << line_int;
        line_temp >> line_string;

        for (auto temp_par : all_mode_par)
        {
            mysql->clearResultList();
            string SQLCommand = "SELECT ";
            string SQLCommand_middle = " FROM mode LIMIT ";
            string SQLCommand_end = ",1";
            SQLCommand = SQLCommand + temp_par + SQLCommand_middle + line_string + SQLCommand_end;
            if (mysql->runSQLCommand(SQLCommand))
            {
                vector<vector<string> > result = mysql->getResult();
                for (auto &vec : result)
                {
                    for (auto &str : vec)
                    {
                        if (temp_par == "priority")
                        {
                            priority = atoi(str.c_str());
                            cout << mode << "优先级:" << priority;
                        }
                        else if (temp_par == "numOfTx")
                        {
                            numOfTx = atoi(str.c_str());
                            cout << mode << "发射机数:" << numOfTx;
                        }
                        else if (temp_par == "radius")
                        {
                            if (mode != "D2D") radius = atoi(str.c_str());
                            cout << mode << "半径:" << radius;
                        }
                        else if (temp_par == "power")
                        {
                            power = atoi(str.c_str());
                            cout << mode << "功率:" << power;
                        }
                        else if (temp_par == "numOfRx")
                        {
                            numOfRx = atoi(str.c_str());
                            cout << mode << "接收机数:" << numOfRx;
                        }
                    }
                    cout << endl;
                }
            } else cout << "执行失败" << endl;
        }
    }
    else cout << "连接未建立" << endl;
}

int mode_par::get_priority() {
    return priority;
}

vector<string> mode_par::get_all_mode() {
    return all_mode;
}

mode_par::~mode_par()
{

}

string mode_par::get_mode() {
    return mode;
}

double mode_par::get_radius() {
    return radius;
}

int mode_par::get_numOfTx() {
    return numOfTx;
}

double mode_par::get_power() {
    return power;
}

int mode_par::get_numOfRx() {
    return numOfRx;
}

int mode_par::get_modeID() {
    return modeID;
}

/************************************sense_par*****************************************/
sense_par::sense_par()
{
    this->sense = 0; //初始化为0
    this->numOfSlot = -1;
    this->numOfSample = -1;
    this->rankOfSymbol = -1;
    this->SenseMainBS = "";
    this->numOfMainBS = -1;
    this->SenseCooperationBS = "";
    this->numOfCooperationBS = -1;
    this->falseAlarmProbability = -1;
    this->minSINR = -1;
    this->maxSINR = -1;
}

sense_par::~sense_par() {

}

sense_par &sense_par::GetInstance() {
    if (_Instance == nullptr)
    {
        _Instance = new sense_par();
    }
    return *_Instance;
}

void sense_par::set_sense()
/************************************************************************************************
函数名称：
主要功能：读取数据库中的频谱感知参数，配置sense_par对象
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus())
    {
        //频谱感知
        mysql->clearResultList();
        if(mysql->runSQLCommand("SELECT sense FROM sense"))
        {
            vector<vector<string> > result = mysql->getResult();
            for(auto & vec : result)
            {
                for(auto &str : vec)
                {
                    sense = atoi(str.c_str());
                }
                cout << endl;
            }
        }
        else
            cout << "执行失败" << endl;

        if (sense)
        {
            cout << "*****************频谱感知*****************" << endl;
            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT numOfSlot FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        numOfSlot = atoi(str.c_str());
                        cout << "蒙特卡洛仿真次数：" << numOfSlot;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT numOfSample FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        numOfSample = atoi(str.c_str());
                        cout << "样本数：" << numOfSample;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT rankOfSymbol FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        rankOfSymbol = atoi(str.c_str());
                        cout << "发送信号的秩：" << rankOfSymbol;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT SenseMainBS FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        SenseMainBS = str.c_str();
                        cout << "主基站：" << SenseMainBS;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT numOfMainBS FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        numOfMainBS = atoi(str.c_str());
                        cout << "主基站数目：" << numOfMainBS;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT SenseCooperationBS FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        SenseCooperationBS = str.c_str();
                        cout << "合作基站：" << SenseCooperationBS;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT numOfCooperationBS FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        numOfCooperationBS = atoi(str.c_str());
                        cout << "合作基站数目：" << numOfCooperationBS;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT falseAlarmProbability FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        falseAlarmProbability = atof(str.c_str());
                        cout << "虚警概率：" << falseAlarmProbability;
                    }
                    cout << endl;
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT minSINR FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        minSINR = atof(str.c_str());
                    }
                }
            }
            else cout << "执行失败" << endl;

            mysql->clearResultList();
            if(mysql->runSQLCommand("SELECT maxSINR FROM sense"))
            {
                vector<vector<string> > result = mysql->getResult();
                for(auto & vec : result)
                {
                    for(auto &str : vec)
                    {
                        maxSINR = atof(str.c_str());
                    }
                }
            }
            else cout << "执行失败" << endl;

            if (minSINR && maxSINR) cout << "SINR取值范围(dBm):" << minSINR << "~" << maxSINR << endl;
        }
    }
    else cout << "连接未建立" << endl;
}

int sense_par::get_sence() const  {
    return sense;
}

int sense_par::get_numOfSlot() const {
    return numOfSlot;
}

int sense_par::getNumOfSlot() const {
    return numOfSlot;
}

int sense_par::getNumOfSample() const {
    return numOfSample;
}

int sense_par::getRankOfSymbol() const {
    return rankOfSymbol;
}

int sense_par::getNumOfMainBS() const {
    return numOfMainBS;
}

int sense_par::getNumOfCooperationBS() const {
    return numOfCooperationBS;
}

double sense_par::getFalseAlarmProbability() const {
    return falseAlarmProbability;
}

double sense_par::getMinSINR() const {
    return minSINR;
}

double sense_par::getMaxSINR() const {
    return maxSINR;
}




