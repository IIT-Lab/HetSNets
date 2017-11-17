//
// Created by lee on 17-8-1.
//
#include "Device.h"
#include "SystemDriveBus.h"
#include "Mymath.h"
#include "ImportExport.h"
#include "Global.h"

int MacroCell::countTag = 0;
int SmallCell::countTag = 0;
int Wifi::countTag = 0;
int User::countTag = 0;
int SystemDriveBus::iCountDevice = 0;

MacroCell::MacroCell()
/************************************************************************************************
函数名称：构造函数
主要功能：初始化对象参数
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 10;//设置该对象的优先级是10
    sType = "class MacroCell *";
    macroID = SystemDriveBus::GetCountMacro();
    mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
    CellRadius = Macro_mode_par.get_radius();
    dXPoint = SystemDriveBus::cellPosition.at(macroID).dXPoint;
    dYPoint = SystemDriveBus::cellPosition.at(macroID).dYPoint;

    numOfSample = SystemDriveBus::system_sense.getNumOfSample();
}

Interface * MacroCell::Create()
/************************************************************************************************
函数名称：Create
主要功能：调用BS的构造函数，并返回BS的对象指针
输入参数：
输出参数：BSPtr，BS的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *MacroCellPtr;
    MacroCellPtr = new MacroCell();
    return MacroCellPtr;
}

void MacroCell::initial(default_random_engine dre)
{
    //依次调硬体的初始化函数，软体的初始化函数
    hardware.InitialHardwareEntity();
    JoinSection2TransData();
}

void MacroCell::JoinSection2TransData()
{
    /*完成软件与硬件相关数据的套接*/
    //UserLocation.ConnectXYPoint(dXPoint, dYPoint);//获取D2DUser坐标
    software.softwareTx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
    //software.softwareRx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
    software.softwareTx.ConnectID(iID);//获取D2DUserID
    //software.softwareRx.ConnectID(iID);//获取D2DUserID
    hardware.ConnectID(iID);//获取D2DUserID
    software.softwareTx.ConnectType(sGetType());
    //software.softwareRx.ConnectHardLinkloss(hardware.GetLinkloss());//接收软体获取硬体里的路损
    software.softwareTx.ConnectHardPower(hardware.txPower);//发射软体获取硬体里的功率
    //hardware.ConnectstartSlot(software.softwareTx.startSlot);//硬体从软体发射类里获取起始发射时间
}

void MacroCell::Display()
/************************************************************************************************
函数名称：Display
主要功能：输出BS的位置信息到.m文件中
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    //cout << "画出基站位置" << endl;
    Out2MatlabFile();
}

void MacroCell::Out2MatlabFile()
{
    ImportExport::fout << "h1 = scatter(" << dXPoint << "," << dYPoint << ",200,'Marker','^','LineWidth',1,'MarkerEdgeColor',[0 0 0]/255,'MarkerFaceColor',[0 0 0]/255);" << endl;//打印Macro基站的坐标
    ImportExport::fout << "hold on;" << endl;

    if (SystemDriveBus::system_shape.get_shape() == "cell")
    {
        ImportExport::fout << "A = [" << dXPoint << "," << dYPoint << "];" << endl;
        ImportExport::fout << "N = " << CellRadius << ";" << endl;
        ImportExport::fout << "B = [A(1) + N, A(1) + N/2, A(1) - N/2, A(1) - N, A(1) - N/2, A(1) + N/2, A(1) + N];" << endl;
        ImportExport::fout << "C = [A(2), A(2) + N*sqrt(3)/2, A(2) + N*sqrt(3)/2, A(2), A(2) - N*sqrt(3)/2, A(2) - N*sqrt(3)/2, A(2)];" << endl;
        ImportExport::fout << "p  =  plot(B,C,'k');" << endl;
        ImportExport::fout << "set(p,'Color',[0 0 0],'LineWidth',0.5)" << endl;
        ImportExport::fout << "hold on;" << endl;

        ImportExport::fout << "A = [" << dXPoint + CellRadius / 2 << "," << dYPoint + CellRadius / sqrt(3) / 2 << "];" << endl;
        ImportExport::fout << "N = " << CellRadius / sqrt(3) << ";" << endl;
        ImportExport::fout << "B = [A(1) + N*sqrt(3)/2, A(1), A(1) - N*sqrt(3)/2, A(1) - N*sqrt(3)/2, A(1), A(1) + N*sqrt(3)/2, A(1) + N*sqrt(3)/2];" << endl;
        ImportExport::fout << "C = [A(2) + N/2, A(2) + N, A(2) + N/2, A(2) - N/2, A(2) - N, A(2) - N/2, A(2) + N/2];" << endl;
        ImportExport::fout << "p = plot(B,C,'k:','LineWidth',0.6);" << endl;
        ImportExport::fout << "set(p,'Color',[0.5 0.5 0.5],'LineWidth',1)" << endl;
        ImportExport::fout << "hold on;" << endl;

        ImportExport::fout << "A = [" << dXPoint - CellRadius / 2 << "," << dYPoint + CellRadius / sqrt(3) / 2 << "];" << endl;
        ImportExport::fout << "N = " << CellRadius / sqrt(3) << ";" << endl;
        ImportExport::fout << "B = [A(1) + N*sqrt(3)/2, A(1), A(1) - N*sqrt(3)/2, A(1) - N*sqrt(3)/2, A(1), A(1) + N*sqrt(3)/2, A(1) + N*sqrt(3)/2];" << endl;
        ImportExport::fout << "C = [A(2) + N/2, A(2) + N, A(2) + N/2, A(2) - N/2, A(2) - N, A(2) - N/2, A(2) + N/2];"  << endl;
        ImportExport::fout << "p = plot(B,C,'k:','LineWidth',0.6);" << endl;
        ImportExport::fout << "set(p,'Color',[0.5 0.5 0.5],'LineWidth',1)" << endl;
        ImportExport::fout << "hold on;" << endl;

        ImportExport::fout << "A = [" << dXPoint << "," << dYPoint - CellRadius / sqrt(3) << "];" << endl;
        ImportExport::fout << "N = " << CellRadius / sqrt(3) << ";" << endl;
        ImportExport::fout << "B = [A(1) + N*sqrt(3)/2, A(1), A(1) - N*sqrt(3)/2, A(1) - N*sqrt(3)/2, A(1), A(1) + N*sqrt(3)/2, A(1) + N*sqrt(3)/2];" << endl;
        ImportExport::fout << "C = [A(2) + N/2, A(2) + N, A(2) + N/2, A(2) - N/2, A(2) - N, A(2) - N/2, A(2) + N/2];"  << endl;
        ImportExport::fout << "p = plot(B,C,'k:','LineWidth',0.6);" << endl;
        ImportExport::fout << "set(p,'Color',[0.5 0.5 0.5],'LineWidth',1)" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
}

void MacroCell::CellularSysInit(int cell_num, double cell_radius)
{
/************************************************************************************************
函数名称：CellularSysInit
主要功能：生成Macro基站位置，并存放在总线上的cellPosition中
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
    {
        if (cell_num > 0)
        {
            deviceLocation horizontal_unit_vector, vertical_unit_vector, tempCellPosition;//定义水平和垂直方向的两个复向量
            horizontal_unit_vector.dXPoint = 1.5 * cell_radius;						//水平方向的矢量为3*R/2
            horizontal_unit_vector.dYPoint = 0;
            vertical_unit_vector.dYPoint = 0.5 * pow(3, 0.5) * cell_radius;				//垂直方向的矢量为j*3^(0.5)*R/2
            vertical_unit_vector.dXPoint = 0;

            //确定0-3号小区的位置坐标
            tempCellPosition.dXPoint = 0;
            tempCellPosition.dYPoint = 0;
            if (cell_num > 0)
            {
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(0, tempCellPosition));
            }
            if (cell_num > 1)
            {
                tempCellPosition = complexAdd(horizontal_unit_vector, vertical_unit_vector);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(1, tempCellPosition));
                tempCellPosition = complexIntMul(vertical_unit_vector, 2);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(2, tempCellPosition));
                tempCellPosition = complexSub(vertical_unit_vector, horizontal_unit_vector);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(3, tempCellPosition));

                //通过对称关系求的4-6号小区的位置坐标
                for (int cellNum = 4; cellNum < 7; cellNum++)
                {
                    //cellPosition[cellNum] = complexIntMul(cellPosition[cellNum - 3], -1);
                    tempCellPosition = complexIntMul(SystemDriveBus::cellPosition.at(cellNum - 3), -1);
                    SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(cellNum, tempCellPosition));
                }
            }
            if (cell_num > 7)
            {
                //确定7-13号小区的位置坐标
                tempCellPosition = complexIntMul(horizontal_unit_vector, 2);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(7, tempCellPosition));
                tempCellPosition = complexIntMul(SystemDriveBus::cellPosition.at(1), 2);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(8, tempCellPosition));
                tempCellPosition = complexAdd(SystemDriveBus::cellPosition.at(1), SystemDriveBus::cellPosition.at(2));
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(9, tempCellPosition));
                tempCellPosition = complexIntMul(vertical_unit_vector, 4);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(10, tempCellPosition));
                tempCellPosition = complexAdd(SystemDriveBus::cellPosition.at(2), SystemDriveBus::cellPosition.at(3));
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(11, tempCellPosition));
                tempCellPosition = complexIntMul(SystemDriveBus::cellPosition.at(3), 2);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(12, tempCellPosition));
                tempCellPosition = complexIntMul(horizontal_unit_vector, -2);
                SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(13, tempCellPosition));

                //通过对称关系求的14-18小区的位置坐标
                for (int cellNum = 14; cellNum < 19; cellNum++)
                {
                    tempCellPosition = complexIntMul(SystemDriveBus::cellPosition.at(cellNum - 6), -1);
                    SystemDriveBus::cellPosition.insert(pair<int, deviceLocation>(cellNum, tempCellPosition));
                }
            }
        }
    }
}

double MacroCell::GetXPoint()
{
    return dXPoint;
}

double MacroCell::GetYPoint()
{
    return dYPoint;
}

void MacroCell::WorkSlot(default_random_engine dre)
{
    //更新功率
    if (!(SystemDriveBus::iSlot % 1000)) hardware.InitialHardwareEntity();
    //调用发射软体类的workslot
    software.softwareTx.WorkSlotSoftwareEntity();
    //调用硬体类的发射workslot，将发射机的功率指针传给所有的信道登记下来
    hardware.WorkslotHardwareEntityTx();
}

void MacroCell::GenerateSample()
{
    for (int i = 0; i < numOfSample; i++)
    {
        int tempSample = 0;
        double temp = (double)rand() / RAND_MAX;
        if (temp > 0.5) tempSample = 1;
        vecSample.push_back(tempSample);
    }
}

const vector<int> &MacroCell::getVecSample() const {
    return vecSample;
}

int MacroCell::GetmacroID() {
    return macroID;
}

MacroCell::~MacroCell() {

}

///////////////////////////SmallCell类///////////////////////////////

SmallCell::SmallCell()
/************************************************************************************************
函数名称：构造函数
主要功能：初始化对象参数
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 10; //发射优先级
    if (SystemDriveBus::system_sense.get_sence())
    {
        iPriority = 30; //做频谱感知，作为接收对象,设置为接收优先级
    }
    sType = "class SmallCell *";
    SmallCellID = SystemDriveBus::GetCountSmallCell();
    mode_par SmallCell_mode_par = SystemDriveBus::ModeID2Par.at(2);
    SmallCellRadius = SmallCell_mode_par.get_radius();

    if (SystemDriveBus::system_shape.get_shape() == "rectangle")
    {
        dXPoint = ((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_length() - SmallCellRadius * 2) + SmallCellRadius;
        dYPoint = ((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_width() - SmallCellRadius * 2) + SmallCellRadius;
    }
    else if (SystemDriveBus::system_shape.get_shape() == "circle")
    {
        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_radius() - SmallCellRadius);
        dXPoint = temp_radius * sin(temp_angle);
        dYPoint = temp_radius * cos(temp_angle);
    }
    else //蜂窝形仿真区域
    {
        double x, y, tempx, tempy;
        int randCell;
        mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
        int cell_num = Macro_mode_par.get_numOfTx();
        double cell_radius = Macro_mode_par.get_radius();
        double inter_side_distance = cell_radius * sqrt(3) - SmallCellRadius;

        x = ((double)rand() / RAND_MAX - 0.5)*sqrt(3)*inter_side_distance / 2.0;
        y = ((double)rand() / RAND_MAX - 0.5)*inter_side_distance;

        randCell = (int)(((double)rand() / RAND_MAX) * cell_num);

        if (fabs(y)>(inter_side_distance - sqrt(3)*fabs(x)))  //如果处于边缘的三角区域，映射到扇区中；
        {

            tempx = sqrt(3)*inter_side_distance / 2.0 - fabs(x);
            tempy = inter_side_distance / 2 - fabs(y);
            if (x>0 && y>0)
            {
                x = tempx;
                y = tempy;
            }
            else if (x>0 && y<0)
            {
                y = -tempy;
                x = tempx;
            }
            else if (x<0 && y<0)
            {
                x = -tempx;
                y = -tempy;
            }
            else if (x<0 && y>0)
            {
                x = -tempx;
                y = tempy;
            }
        }
        relativeXPoint = x;//radius * cos(alfa);					//用户的x轴相对坐标,需要累加绝对坐标
        relativeYPoint = y;//radius * sin(alfa);					//用户的y轴相对坐标

        cellID = randCell;
        dXPoint = SystemDriveBus::cellPosition.at(cellID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::cellPosition.at(cellID).dYPoint + relativeYPoint;
    }
    deviceLocation SmallCellLocation;
    SmallCellLocation.dXPoint = dXPoint;
    SmallCellLocation.dYPoint = dYPoint;
    SystemDriveBus::SmallCellPosition.insert(pair<int, deviceLocation>(SmallCellID, SmallCellLocation));
}

Interface * SmallCell::Create()
/************************************************************************************************
函数名称：Create
主要功能：调用SmallCell的构造函数，并返回SmallCell的对象指针
输入参数：
输出参数：SmallCellPtr，SmallCell的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *SmallCellPtr;
    SmallCellPtr = new SmallCell();
    return SmallCellPtr;
}

void SmallCell::initial(default_random_engine dre)
{
    //依次调硬体的初始化函数，软体的初始化函数
    hardware.InitialHardwareEntity();
    JoinSection2TransData();
}

void SmallCell::JoinSection2TransData()
{
    /*完成软件与硬件相关数据的套接*/
//    UserLocation.ConnectXYPoint(dXPoint, dYPoint);//获取D2DUser坐标
    software.softwareTx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
//    software.softwareRx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
    software.softwareTx.ConnectID(iID);//获取D2DUserID
//    software.softwareRx.ConnectID(iID);//获取D2DUserID
    software.softwareTx.ConnectType(sGetType());
    hardware.ConnectID(iID);//获取D2DUserID
//    software.softwareRx.ConnectMode(mode);
//    software.softwareRx.ConnectHardLinkloss(hardware.GetLinkloss());//接收软体获取硬体里的路损
    software.softwareTx.ConnectHardPower(hardware.txPower);//发射软体获取硬体里的功率
//    hardware.ConnectstartSlot(software.softwareTx.startSlot);//硬体从软体发射类里获取起始发射时间
}

void SmallCell::Display()
/************************************************************************************************
函数名称：Display
主要功能：输出BS的位置信息到.m文件中
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    //cout << "画出基站位置" << endl;
    Out2MatlabFile();
}

void SmallCell::Out2MatlabFile()
{
    ImportExport::fout << "hold on;" << endl;
    ImportExport::fout << "h2 = scatter(" << dXPoint << "," << dYPoint << ",'Marker','^','MarkerEdgeColor',[0 0 255]/255,'MarkerFaceColor',[0 0 255]/255);" << endl;
    ImportExport::fout << "alpha = 0:pi / 50 : 2 * pi; " << endl;
    ImportExport::fout << "x = " << dXPoint << " + " << SmallCellRadius << " * cos(alpha);" << endl;
    ImportExport::fout << "y = " << dYPoint << " + " << SmallCellRadius << " * sin(alpha);" << endl;
    ImportExport::fout << "p = plot(x, y, '-');" << endl;
    ImportExport::fout << "set(p, 'Color', [0.5 0.5 0.5], 'LineWidth', 0.5);" << endl;
    ImportExport::fout << "hold on;" << endl;
}

void SmallCell::WorkSlot(default_random_engine dre)
{
    //测试频谱感知，SmallCell作为接收用户
    //调用硬体类的接收workslot，将路损指针传给信道，直接写入接收硬体，便于接收软体进行SINR计算
    hardware.WorkslotHardwareEntityTx();
    //调用接收软体类的workslot
    software.softwareTx.WorkSlotSoftwareEntity();
}

double SmallCell::GetXPoint()
{
    return dXPoint;
}

double SmallCell::GetYPoint()
{
    return dYPoint;
}

int SmallCell::getCellID() const {
    return cellID;
}

double SmallCell::getRelativeXPoint() const {
    return relativeXPoint;
}

double SmallCell::getRelativeYPoint() const {
    return relativeYPoint;
}

int SmallCell::getSmallCellID() const {
    return SmallCellID;
}

double SmallCell::getSmallCellRadius() const {
    return SmallCellRadius;
}

/*************************************WiFi网络类**************************************/

///////////////////////////WiFi类///////////////////////////////

Wifi::Wifi()
/************************************************************************************************
函数名称：构造函数
主要功能：初始化对象参数
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 10;//设置该信道对象的优先级是50
    WifiID = SystemDriveBus::GetCountWifi();
    sType = "class Wifi *";
    mode_par Wifi_mode_par = SystemDriveBus::ModeID2Par.at(3);
    WifiRadius = Wifi_mode_par.get_radius();

    if (SystemDriveBus::system_shape.get_shape() == "rectangle")
    {
        dXPoint = ((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_length() - WifiRadius * 2) + WifiRadius;
        dYPoint = ((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_width() - WifiRadius * 2) + WifiRadius;
    }
    else if (SystemDriveBus::system_shape.get_shape() == "circle")
    {
        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_radius() - WifiRadius);
        dXPoint = temp_radius * sin(temp_angle);
        dYPoint = temp_radius * cos(temp_angle);
    }
    else //蜂窝形仿真区域
    {
        double x, y, tempx, tempy;
        int randCell;
        mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
        int cell_num = Macro_mode_par.get_numOfTx();
        double cell_radius = Macro_mode_par.get_radius();
        double inter_side_distance = cell_radius * sqrt(3) - WifiRadius;

        x = ((double) rand() / RAND_MAX - 0.5) * sqrt(3) * inter_side_distance / 2.0;
        y = ((double) rand() / RAND_MAX - 0.5) * inter_side_distance;

        randCell = (int) (((double) rand() / RAND_MAX) * cell_num);

        if (fabs(y) > (inter_side_distance - sqrt(3) * fabs(x)))  //如果处于边缘的三角区域，映射到扇区中；
        {

            tempx = sqrt(3) * inter_side_distance / 2.0 - fabs(x);
            tempy = inter_side_distance / 2 - fabs(y);
            if (x > 0 && y > 0) {
                x = tempx;
                y = tempy;
            } else if (x > 0 && y < 0) {
                y = -tempy;
                x = tempx;
            } else if (x < 0 && y < 0) {
                x = -tempx;
                y = -tempy;
            } else if (x < 0 && y > 0) {
                x = -tempx;
                y = tempy;
            }
        }
        relativeXPoint = x;//radius * cos(alfa);					//用户的x轴相对坐标,需要累加绝对坐标
        relativeYPoint = y;//radius * sin(alfa);					//用户的y轴相对坐标

        cellID = randCell;
        dXPoint = SystemDriveBus::cellPosition.at(cellID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::cellPosition.at(cellID).dYPoint + relativeYPoint;
    }
    deviceLocation WifiLocation;
    WifiLocation.dXPoint = dXPoint;
    WifiLocation.dYPoint = dYPoint;
    SystemDriveBus::WifiPosition.insert(pair<int, deviceLocation>(WifiID, WifiLocation));
}

Interface * Wifi::Create()
/************************************************************************************************
函数名称：Create
主要功能：调用BS的构造函数，并返回BS的对象指针
输入参数：
输出参数：BSPtr，BS的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *WifiPtr;
    WifiPtr = new Wifi();
    return WifiPtr;
}

void Wifi::Display()
/************************************************************************************************
函数名称：Display
主要功能：输出BS的位置信息到.m文件中
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    //cout << "画出基站位置" << endl;
    Out2MatlabFile();
}

void Wifi::Out2MatlabFile()
{
    ImportExport::fout << "h3 = scatter(" << dXPoint << "," << dYPoint << ",'Marker','^','MarkerEdgeColor',[255 0 0]/255,'MarkerFaceColor',[255 0 0]/255);" << endl;
    ImportExport::fout << "alpha = 0:pi / 50 : 2 * pi; " << endl;
    ImportExport::fout << "x = " << dXPoint << " + " << WifiRadius << " * cos(alpha);" << endl;
    ImportExport::fout << "y = " << dYPoint << " + " << WifiRadius << " * sin(alpha);" << endl;
    ImportExport::fout << "p = plot(x, y, '-');" << endl;
    ImportExport::fout << "set(p, 'Color', [0.5 0.5 0.5], 'LineWidth', 0.5);" << endl;
    ImportExport::fout << "hold on;" << endl;
}

double Wifi::getDXPoint() const {
    return dXPoint;
}

double Wifi::getDYPoint() const {
    return dYPoint;
}

///////////////////////////总用户大类///////////////////////////////

User::User(string _user_type)
/************************************************************************************************
函数名称：构造函数
主要功能：根据不同的用户类型，初始化对象参数
输入参数：用户类型
输出参数：
其他信息：
*************************************************************************************************/
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 30;
    UserID = SystemDriveBus::GetCountUser();
    sType = "class User *";
    user_type = _user_type;
    if (user_type == "MacroCell")
    {
        double x, y, tempx, tempy;
        int randCell;
        mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
        int cell_num = Macro_mode_par.get_numOfTx();
        double cell_radius = Macro_mode_par.get_radius();
        double inter_side_distance = cell_radius * sqrt(3);

        x = ((double) rand() / RAND_MAX - 0.5) * sqrt(3) * inter_side_distance / 2.0;
        y = ((double) rand() / RAND_MAX - 0.5) * inter_side_distance;

        randCell = (int) (((double) rand() / RAND_MAX) * cell_num);

        if (fabs(y) > (inter_side_distance - sqrt(3) * fabs(x)))  //如果处于边缘的三角区域，映射到扇区中；
        {

            tempx = sqrt(3) * inter_side_distance / 2.0 - fabs(x);
            tempy = inter_side_distance / 2 - fabs(y);
            if (x > 0 && y > 0) {
                x = tempx;
                y = tempy;
            } else if (x > 0 && y < 0) {
                y = -tempy;
                x = tempx;
            } else if (x < 0 && y < 0) {
                x = -tempx;
                y = -tempy;
            } else if (x < 0 && y > 0) {
                x = -tempx;
                y = tempy;
            }
        }
        relativeXPoint = x;//radius * cos(alfa);					//用户的x轴相对坐标,需要累加绝对坐标
        relativeYPoint = y;//radius * sin(alfa);					//用户的y轴相对坐标

        cellID = randCell;
        dXPoint = SystemDriveBus::cellPosition.at(cellID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::cellPosition.at(cellID).dYPoint + relativeYPoint;
    }
    if (user_type == "SmallCell")
    {
        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::ModeID2Par.at(2).get_radius());
        relativeXPoint = temp_radius * sin(temp_angle);
        relativeYPoint = temp_radius * cos(temp_angle);
        int SmallCell_num = SystemDriveBus::ModeID2Par.at(2).get_numOfTx();
        int randSmallCell = (int) (((double) rand() / RAND_MAX) * SmallCell_num);
        SmallCellID = randSmallCell;
        dXPoint = SystemDriveBus::SmallCellPosition.at(SmallCellID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::SmallCellPosition.at(SmallCellID).dYPoint + relativeYPoint;
    }
    if (user_type == "Wifi")
    {
        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::ModeID2Par.at(3).get_radius());
        relativeXPoint = temp_radius * sin(temp_angle);
        relativeYPoint = temp_radius * cos(temp_angle);
        int Wifi_num = SystemDriveBus::ModeID2Par.at(3).get_numOfTx();
        int randSmallCell = (int) (((double) rand() / RAND_MAX) * Wifi_num);
        WifiID = randSmallCell;
        dXPoint = SystemDriveBus::WifiPosition.at(WifiID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::WifiPosition.at(WifiID).dYPoint + relativeYPoint;
    }
    if (user_type == "D2DTx" || user_type == "D2DRx")
    {
        if (SystemDriveBus::system_shape.get_shape() == "rectangle")
        {
            dXPoint = ((double) rand() / RAND_MAX) * SystemDriveBus::system_shape.get_length();
            dYPoint = ((double) rand() / RAND_MAX) * SystemDriveBus::system_shape.get_width();
        }
        else if (SystemDriveBus::system_shape.get_shape() == "circle")
        {
            double temp_angle = (double) rand() / RAND_MAX * PI;
            double temp_radius = ((double) rand() / RAND_MAX) * SystemDriveBus::system_shape.get_radius();
            dXPoint = temp_radius * sin(temp_angle);
            dYPoint = temp_radius * cos(temp_angle);
        }
        else //蜂窝形仿真区域
        {
            double x, y, tempx, tempy;
            int randCell;
            mode_par Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
            int cell_num = Macro_mode_par.get_numOfTx();
            double cell_radius = Macro_mode_par.get_radius();
            double inter_side_distance = cell_radius * sqrt(3);

            x = ((double) rand() / RAND_MAX - 0.5) * sqrt(3) * inter_side_distance / 2.0;
            y = ((double) rand() / RAND_MAX - 0.5) * inter_side_distance;

            randCell = (int) (((double) rand() / RAND_MAX) * cell_num);

            if (fabs(y) > (inter_side_distance - sqrt(3) * fabs(x)))  //如果处于边缘的三角区域，映射到扇区中；
            {

                tempx = sqrt(3) * inter_side_distance / 2.0 - fabs(x);
                tempy = inter_side_distance / 2 - fabs(y);
                if (x > 0 && y > 0) {
                    x = tempx;
                    y = tempy;
                } else if (x > 0 && y < 0) {
                    y = -tempy;
                    x = tempx;
                } else if (x < 0 && y < 0) {
                    x = -tempx;
                    y = -tempy;
                } else if (x < 0 && y > 0) {
                    x = -tempx;
                    y = tempy;
                }
            }
            relativeXPoint = x;//radius * cos(alfa);					//用户的x轴相对坐标,需要累加绝对坐标
            relativeYPoint = y;//radius * sin(alfa);					//用户的y轴相对坐标

            cellID = randCell;
            dXPoint = SystemDriveBus::cellPosition.at(cellID).dXPoint + relativeXPoint;
            dYPoint = SystemDriveBus::cellPosition.at(cellID).dYPoint + relativeYPoint;
        }
    }
}

Interface * User::Create(string _user_type)
/************************************************************************************************
函数名称：Create
主要功能：调用BS的构造函数，并返回BS的对象指针
输入参数：
输出参数：BSPtr，BS的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *UserPtr;
    UserPtr = new User(_user_type);
    return UserPtr;
}

void User::initial(default_random_engine dre)
{
    //依次调硬体的初始化函数，软体的初始化函数
//    hardware.InitialHardwareEntity();
    JoinSection2TransData();
}

void User::JoinSection2TransData()
{
    /*完成软件与硬件相关数据的套接*/
//    UserLocation.ConnectXYPoint(dXPoint, dYPoint);//获取D2DUser坐标
//    software.softwareTx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
    software.softwareRx.ConnectLocation(dXPoint, dYPoint);//获取D2DUser坐标
//    software.softwareTx.ConnectID(iID);//获取D2DUserID
    software.softwareRx.ConnectID(iID);//获取D2DUserID
//    software.softwareTx.ConnectType(sGetType());
    hardware.ConnectID(iID);//获取D2DUserID
//    software.softwareRx.ConnectMode(mode);
    software.softwareRx.ConnectHardLinkloss(hardware.GetLinkloss());//接收软体获取硬体里的路损
//    software.softwareTx.ConnectHardPower(hardware.txPower);//发射软体获取硬体里的功率
//    hardware.ConnectstartSlot(software.softwareTx.startSlot);//硬体从软体发射类里获取起始发射时间
}

void User::WorkSlot(default_random_engine dre)
{
    //调用硬体类的接收workslot，将路损指针传给信道，直接写入接收硬体，便于接收软体进行SINR计算
    hardware.WorkslotHardwareEntityRx();
    //调用接收软体类的workslot
    software.softwareRx.WorkSlotSoftwareEntity();
}

void User::Display()
/************************************************************************************************
函数名称：Display
主要功能：输出User的位置信息到.m文件中
输入参数：
输出参数：
其他信息：
*************************************************************************************************/
{
    //cout << "画出基站位置" << endl;
    Out2MatlabFile();
}

void User::Out2MatlabFile()
{
    if (user_type == "MacroCell")
    {
        ImportExport::fout << "h3 = scatter(" << dXPoint << "," << dYPoint << ",'o', 'CData', [0 0 0] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    if (user_type == "SmallCell")
    {
        ImportExport::fout << "h4 = scatter(" << dXPoint << "," << dYPoint << ",'x', 'CData', [0 0 255] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    if (user_type == "Wifi")
    {
        ImportExport::fout << "h4 = scatter(" << dXPoint << "," << dYPoint << ",'.', 'CData', [255 0 0] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    if (user_type == "D2DTx")
    {
        ImportExport::fout << "h4 = scatter(" << dXPoint << "," << dYPoint << ",'o', 'CData', [0 0 0] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    if (user_type == "D2DRx")
    {
        ImportExport::fout << "h4 = scatter(" << dXPoint << "," << dYPoint << ",'o', 'CData', [0 0 255] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
}

double User::getDXPoint() const {
    return dXPoint;
}

double User::getDYPoint() const {
    return dYPoint;
}

int User::getIndoorOrOutdoor() const {
    return indoorOrOutdoor;
}

double User::getRelativeXPoint() const {
    return relativeXPoint;
}

double User::getRelativeYPoint() const {
    return relativeYPoint;
}

int User::getUserID() const {
    return UserID;
}

int User::getSmallCellID() const {
    return SmallCellID;
}

int User::getCellID() const {
    return cellID;
}

const string &User::getUser_type() const {
    return user_type;
}

