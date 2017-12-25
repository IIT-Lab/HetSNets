//
// Created by lee on 17-8-1.
//
#include "Device.h"
#include "SystemDriveBus.h"
#include "Mymath.h"
#include "ImportExport.h"
#include "Global.h"
#include "InterferenceIndex.h"
#include "graphColoring.h"

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
    if (SystemDriveBus::ULorDL == "UL") {
        iPriority = 30;//上行链路　接收优先级
    } else {
        iPriority = 10;//下行链路　发射优先级
    }

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
    //按照发射和接收优先级区分上下行链路
    if (iPriority >= 30) { //接收优先级　上行链路
        Scheduler();	//进行调度 对该发射机随机选择RB块进行数据包的发送，并登记在发射端的干扰登记表里

        //调用硬体类的接收workslot，将路损指针传给信道，直接写入接收硬体，便于接收软体进行SINR计算
        hardware.WorkslotHardwareEntityRx();
        //调用接收软体类的workslot
        software.softwareRx.WorkSlotSoftwareEntity();
    } else { //发射优先级　下行链路
        //更新功率
        hardware.InitialHardwareEntity();
        //调用发射软体类的workslot
        software.softwareTx.WorkSlotSoftwareEntity();
        //调用硬体类的发射workslot，将发射机的功率指针传给所有的信道登记下来
        hardware.WorkslotHardwareEntityTx();

        Scheduler();	//进行调度 对该发射机随机选择RB块进行数据包的发送，并登记在发射端的干扰登记表里
    }
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

void MacroCell::Scheduler() {
    if (iPriority >= 30) { //接收优先级　上行链路
        double cellRadius = SystemDriveBus::ModeID2Par.at(1).get_radius();
        double MacroUserTxPower = SystemDriveBus::ModeID2Par.at(1).get_power(); //dBm
        double D2DTxPower = SystemDriveBus::ModeID2Par.at(4).get_power(); //dBm
//        MacroUserTxPower = pow(10, (MacroUserTxPower - 30) / 10);//W
//        D2DTxPower = pow(10, (D2DTxPower - 30) / 10);//W
//        MacroUserTxPower = MacroUserTxPower / RBNUM;//W
//        D2DTxPower = D2DTxPower / RBNUM;//W
//        MacroUserTxPower = 10 * log10(MacroUserTxPower) + 30;//dBm
//        D2DTxPower = 10 * log10(D2DTxPower) + 30;//dBm

        /********************************图构建*********************************/
        SetVecMacroUserID();
        SetMapD2DUserID();
        double threshold = 20;
        /********************************贪婪图着色*********************************/
        map<int, hyperNode*> mapNodeID2HyperNodePtr;
        if (SystemDriveBus::iSlot >= 0 && SystemDriveBus::iSlot < 1) {
            cout << "*****************图构建*****************" << endl;
            SetGraph(threshold);
            int nodeNum = (int)graph.size();

            for (int i = 0; i < nodeNum; ++i) {
                hyperNode* hyperNodePtr = new hyperNode(i);
                hyperNodePtr->initial(graph);
                mapNodeID2HyperNodePtr.insert(pair<int, hyperNode*>(i, hyperNodePtr));
            }
            cout << "*****************图着色*****************" << endl;
            hypergraghColoring(mapNodeID2HyperNodePtr, RBNUM);
            cout << "*****************着色结束*****************" << endl;
        }
        /********************************贪婪图着色*********************************/

        /********************************超图着色*********************************/
        if (SystemDriveBus::iSlot >= 1 && SystemDriveBus::iSlot < 2) {
            cout << "*****************超图构建*****************" << endl;
            SetHypergraph(threshold);
            int nodeNum = (int)hypergraph.size();

            for (int i = 0; i < nodeNum; ++i) {
                hyperNode* hyperNodePtr = new hyperNode(i);
                hyperNodePtr->initial(hypergraph);
                mapNodeID2HyperNodePtr.insert(pair<int, hyperNode*>(i, hyperNodePtr));
            }
            cout << "*****************超图着色*****************" << endl;
            hypergraghColoring(mapNodeID2HyperNodePtr, RBNUM);
            cout << "*****************着色结束*****************" << endl;
        }
        /********************************超图着色*********************************/

        /********************************干扰区域超图着色*********************************/
        if (SystemDriveBus::iSlot >= 2) {
            cout << "*****************干扰区域构建*****************" << endl;
            map<int, macroUser*> mapID2MUEPtr;
            for (int macroUserID : vecMacroUserID) {
                double linkloss = GetLinkloss(macroUserID, 0, SystemDriveBus::iSlot);
                double channelGain = pow(10, -linkloss / 10);//线性值
                macroUser* macroUserPtr = new macroUser(macroUserID, MacroUserTxPower, D2DTxPower, channelGain, cellRadius);
                mapID2MUEPtr.insert(pair<int, macroUser*>(macroUserID, macroUserPtr));
            }

            SLAComputing(mapID2MUEPtr);

            cout << "*****************蜂窝用户着色*****************" << endl;
            macroUserColoring(mapID2MUEPtr, RBNUM);

            int TxID, RxID, RBID;
            for (auto temp : mapID2MUEPtr) {
                TxID = temp.second->getUID();
                RxID = 0;
                RBID = temp.second->getColor();
                MacroUserTxPower = temp.second->getPower();
                PushRBAllocation2MySQL(TxID, RxID, RBID, SystemDriveBus::iSlot, MacroUserTxPower);
            }

            cout << "*****************超图构建*****************" << endl;
            map<int, D2DPair*> mapID2D2DPairPtr;
            vector<vector<int>> D2DHypergraph; //表示超图的矩阵incidence Matrix

            //构建 D2D pair ID 对 D2D pair 指针登记表
            int D2DPairID = 0;
            for (auto temp : mapD2DUserID) {
                D2DPair* D2DPairPtr = new D2DPair(D2DPairID, temp.first, temp.second);
                //初始化initial 添加用户发射功率和坐标
                int D2DTxID = temp.first;
                User * D2DTxPtr = dynamic_cast<User *>(SystemDriveBus::ID2PtrBus.at(D2DTxID));
                double dXPoint = D2DTxPtr->getDXPoint();
                double dYPoint = D2DTxPtr->getDYPoint();
                D2DPairPtr->initial(D2DTxPower, dXPoint, dYPoint);

                mapID2D2DPairPtr.insert(pair<int, D2DPair*>(D2DPairID, D2DPairPtr));
                D2DPairID++;
            }

            //根据干扰区域初始化 D2D pair 的候选颜色集
            SetD2DPair(mapID2MUEPtr, mapID2D2DPairPtr, RBNUM);

            //构造以 D2D pair 为节点的超图
            SetD2DHypergraph(mapID2D2DPairPtr, D2DHypergraph);

            cout << "*****************D2D超图着色*****************" << endl;
            D2DHypergraphColoring(mapID2D2DPairPtr, D2DHypergraph, RBNUM, mapID2MUEPtr);

            for (auto temp : mapID2D2DPairPtr) {
                TxID = temp.second->getTxID();
                RxID = temp.second->getRxID();;
                RBID = temp.second->getColor();
                D2DTxPower = temp.second->getPower();
                PushRBAllocation2MySQL(TxID, RxID, RBID, SystemDriveBus::iSlot, D2DTxPower);
            }

            cout << "*****************D2D着色结束*****************" << endl;
        }
        /********************************干扰区域超图着色*********************************/

        /********************************资源分配*********************************/
        int MacroUserNum = (int)vecMacroUserID.size();
        int D2DPairNum = (int)mapD2DUserID.size();

        int TxID, RxID, RBID;

        for (auto temp : mapNodeID2HyperNodePtr) {
            int NodeID = temp.first;
            if (NodeID < MacroUserNum) { //Macro User
                TxID = NodeID + 1;
                RxID = 0;
                RBID = temp.second->getColor();
                PushRBAllocation2MySQL(TxID, RxID, RBID, SystemDriveBus::iSlot, MacroUserTxPower);
            } else { //D2DUser
                TxID = NodeID + 1;
                RxID = NodeID + D2DPairNum + 1;
                RBID = temp.second->getColor();
                PushRBAllocation2MySQL(TxID, RxID, RBID, SystemDriveBus::iSlot, D2DTxPower);
            }
        }

    } else { //发射优先级　下行链路
        ////资源分配
    }
}

void MacroCell::PushRBAllocation2MySQL(int _TxID, int _RxID, int _RBID, int _slotID, double _power) {
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "lee", "281217", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        string TxID = intToString(_TxID);
        string RxID = intToString(_RxID);
        string RBID = intToString(_RBID);
        string slotID = intToString(_slotID);
        string power = doubleToString(_power);
        //构造MySQL插入语句
        string SQLString = "INSERT INTO TxIDRxID2RBID(TxID, RxID, RBID, slotID, power) "
                                   "VALUES(" + TxID + "," + RxID + "," + RBID + "," + slotID + "," + power + ")";

        if (mysql->insert(SQLString))
        {
//                cout << "TxIDRxID2RBID插入成功" << endl;
        }
        else cout << "TxIDRxID2RBID插入失败" << endl;
    } else cout << "连接未建立" << endl;
    mysql->destroyConnection();
}

void MacroCell::SetGraph(double _threshold) {
    graph.clear();
    int MacroUserNum = (int)vecMacroUserID.size();
    int D2DPairNum = (int)mapD2DUserID.size();
    int nodeNum = MacroUserNum + D2DPairNum;

    double MacroUserTxPower = SystemDriveBus::ModeID2Par.at(1).get_power(); //dBm
    double D2DTxPower = SystemDriveBus::ModeID2Par.at(4).get_power(); //dBm
    MacroUserTxPower = pow(10, (MacroUserTxPower - 30) / 10);//W
    D2DTxPower = pow(10, (D2DTxPower - 30) / 10);//W
//    MacroUserTxPower = MacroUserTxPower / RBNUM;
//    D2DTxPower = D2DTxPower / RBNUM;

    //将所有代表宏蜂窝用户的节点相连
    int edgeID = 0;
    for (int MacroUserID1 : vecMacroUserID) {
        for (int MacroUserID2 = MacroUserID1 + 1; MacroUserID2 <= MacroUserNum; MacroUserID2++) {
            if (MacroUserID1 != MacroUserID2) {
                vector<int> vecNodes;
                vecNodes.push_back(MacroUserID1 - 1);//因为有一个宏小区基站　MacroUserID从1开始计数　所以导入节点vec时要减一
                vecNodes.push_back(MacroUserID2 - 1);
                mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                cout << "set edge: " << edgeID << endl;
                edgeID++;
            }
        }
    }

    double threshold = _threshold;//dB 干扰阈值
    double M2Dsir = 0;
    double D2Bsir = 0;
    //计算蜂窝用户和对D2DRx的干扰 计算D2DTx对基站的干扰 D2B M2D
    for (int MacroUserID : vecMacroUserID) {
        for (auto D2DPair : mapD2DUserID) {
            int D2DTxID = D2DPair.first;
            int D2DRxID = D2DPair.second;
            double M2BLinkloss = GetLinkloss(MacroUserID, 0, SystemDriveBus::iSlot);
            double D2BLinkloss = GetLinkloss(D2DTxID, 0, SystemDriveBus::iSlot);
            double D2DLinkloss = GetLinkloss(D2DTxID, D2DRxID, SystemDriveBus::iSlot);
            double M2DLinkloss = GetLinkloss(MacroUserID, D2DRxID, SystemDriveBus::iSlot);
            double M2BGain = pow(10, -M2BLinkloss / 10);//线性值
            double D2BGain = pow(10, -D2BLinkloss / 10);//线性值
            double D2DGain = pow(10, -D2DLinkloss / 10);//线性值
            double M2DGain = pow(10, -M2DLinkloss / 10);//线性值
            D2Bsir = (D2DTxPower * D2DGain) / (MacroUserTxPower * M2DGain);
            D2Bsir = 10 * log10(D2Bsir);//dB值
            M2Dsir = (MacroUserTxPower * M2BGain) / (D2DTxPower * D2BGain);
            M2Dsir = 10 * log10(M2Dsir);//dB值
            if (M2Dsir > threshold && D2Bsir > threshold) {
//                cout << "no edge" << endl;
            } else {
                cout << "set edge: " << edgeID << endl;
                vector<int> vecNodes;
                vecNodes.push_back(MacroUserID - 1);
                vecNodes.push_back(D2DTxID - 1);
                mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                edgeID++;
            }
        }
    }

    double F2Ssir = 0;
    double S2Fsir = 0;
    //计算D2D pair1 First 和　D2D pair2 Second 之间的干扰
    for (auto D2DPair1 : mapD2DUserID) {
        int D2DTx1ID = D2DPair1.first;
        int D2DRx1ID = D2DPair1.second;
        for (auto D2DPair2 : mapD2DUserID) {
            int D2DTx2ID = D2DPair2.first;
            int D2DRx2ID = D2DPair2.second;
            if (D2DRx1ID < D2DRx2ID) {
                double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                F2Ssir = (D2DTxPower * F2FGain) / (D2DTxPower * S2FGain);
                F2Ssir = 10 * log10(F2Ssir);//dB值
                S2Fsir = (D2DTxPower * S2SGain) / (D2DTxPower * F2SGain);
                S2Fsir = 10 * log10(S2Fsir);//dB值
                if (F2Ssir > threshold && S2Fsir > threshold) {
//                    cout << "no edge" << endl;
                } else {
                    cout << "set edge: " << edgeID << endl;
                    vector<int> vecNodes;
                    vecNodes.push_back(D2DTx1ID - 1);
                    vecNodes.push_back(D2DTx2ID - 1);
                    mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                    edgeID++;
                }
            }
        }
    }

    //写入incidence Matrix
    for (int nodeID = 0; nodeID < nodeNum; ++nodeID) {
        vector<int> vecNode2Edge;
        for (auto temp : mapEdgeVecNodes) {
//            edgeID = temp.first;
            bool haveEdge = false;
            for (auto tempNodeID : temp.second) {
                if (nodeID == tempNodeID) haveEdge = true;
            }
            if (haveEdge) {
                vecNode2Edge.push_back(1);
            } else {
                vecNode2Edge.push_back(0);
            }
        }
        graph.push_back(vecNode2Edge);
    }

//    //输出图
//    for (int i = 0; i < graph.size(); ++i) {
//        for (int j = 0; j < graph[0].size(); ++j) {
//            cout << graph[i][j] << ",";
//        }
//        cout << endl;
//    }
//    cout << endl;
}

void MacroCell::SetVecMacroUserID() {
    vecMacroUserID.clear();
    for (auto _temp : SystemDriveBus::SlotDriveBus) {
        if (_temp.second->sGetType() == "class User *") {
            User* tempUser = dynamic_cast<User *>(_temp.second);
            if (tempUser->getUser_type() == "MacroCell") {
                vecMacroUserID.push_back(tempUser->iGetID());
            }
        }
    }
}

void MacroCell::SetMapD2DUserID() {
    mapD2DUserID.clear();
    for (auto _temp : SystemDriveBus::SlotDriveBus) {
        if (_temp.second->sGetType() == "class User *") {
            User* tempUser = dynamic_cast<User *>(_temp.second);
            if (tempUser->getUser_type() == "D2DRx") {
                mapD2DUserID.insert(pair<int, int>(tempUser->getD2DTxID(), tempUser->iGetID()));
            }
        }
    }
}

void MacroCell::SetHypergraph(double _threshold) {
    hypergraph.clear();
    int MacroUserNum = (int)vecMacroUserID.size();
    int D2DPairNum = (int)mapD2DUserID.size();
    int nodeNum = MacroUserNum + D2DPairNum;

    double MacroUserTxPower = SystemDriveBus::ModeID2Par.at(1).get_power(); //dBm
    double D2DTxPower = SystemDriveBus::ModeID2Par.at(4).get_power(); //dBm
    MacroUserTxPower = pow(10, (MacroUserTxPower - 30) / 10);//W
    D2DTxPower = pow(10, (D2DTxPower - 30) / 10);//W
//    MacroUserTxPower = MacroUserTxPower / RBNUM;
//    D2DTxPower = D2DTxPower / RBNUM;

    //将所有代表宏蜂窝用户的节点相连
    int edgeID = 0;
    for (int MacroUserID1 : vecMacroUserID) {
        for (int MacroUserID2 = MacroUserID1 + 1; MacroUserID2 <= MacroUserNum; MacroUserID2++) {
            if (MacroUserID1 != MacroUserID2) {
                vector<int> vecNodes;
                vecNodes.push_back(MacroUserID1 - 1);//因为有一个宏小区基站　MacroUserID从1开始计数　所以导入节点vec时要减一
                vecNodes.push_back(MacroUserID2 - 1);
                mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                cout << "set edge: " << edgeID << endl;
                edgeID++;
            }
        }
    }

    double threshold = _threshold;//dB 干扰阈值
    double M2Dsir = 0;
    double D2Bsir = 0;
    //计算蜂窝用户和对D2DRx的干扰 计算D2DTx对基站的干扰 D2B M2D
    for (int MacroUserID : vecMacroUserID) {
        for (auto D2DPair : mapD2DUserID) {
            int D2DTxID = D2DPair.first;
            int D2DRxID = D2DPair.second;
            double M2BLinkloss = GetLinkloss(MacroUserID, 0, SystemDriveBus::iSlot);
            double D2BLinkloss = GetLinkloss(D2DTxID, 0, SystemDriveBus::iSlot);
            double D2DLinkloss = GetLinkloss(D2DTxID, D2DRxID, SystemDriveBus::iSlot);
            double M2DLinkloss = GetLinkloss(MacroUserID, D2DRxID, SystemDriveBus::iSlot);
            double M2BGain = pow(10, -M2BLinkloss / 10);//线性值
            double D2BGain = pow(10, -D2BLinkloss / 10);//线性值
            double D2DGain = pow(10, -D2DLinkloss / 10);//线性值
            double M2DGain = pow(10, -M2DLinkloss / 10);//线性值
            D2Bsir = (D2DTxPower * D2DGain) / (MacroUserTxPower * M2DGain);
            D2Bsir = 10 * log10(D2Bsir);//dB值
            M2Dsir = (MacroUserTxPower * M2BGain) / (D2DTxPower * D2BGain);
            M2Dsir = 10 * log10(M2Dsir);//dB值
            if (M2Dsir > threshold && D2Bsir > threshold) {
//                cout << "no edge" << endl;
            } else {
                cout << "set edge: " << edgeID << endl;
                vector<int> vecNodes;
                vecNodes.push_back(MacroUserID - 1);
                vecNodes.push_back(D2DTxID - 1);
                mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                edgeID++;
            }
        }
    }

    double F2Ssir = 0;
    double S2Fsir = 0;
    //计算D2D pair1 First 和　D2D pair2 Second 之间的干扰
    for (auto D2DPair1 : mapD2DUserID) {
        int D2DTx1ID = D2DPair1.first;
        int D2DRx1ID = D2DPair1.second;
        for (auto D2DPair2 : mapD2DUserID) {
            int D2DTx2ID = D2DPair2.first;
            int D2DRx2ID = D2DPair2.second;
            if (D2DRx1ID < D2DRx2ID) {
                double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                F2Ssir = (D2DTxPower * F2FGain) / (D2DTxPower * S2FGain);
                F2Ssir = 10 * log10(F2Ssir);//dB值
                S2Fsir = (D2DTxPower * S2SGain) / (D2DTxPower * F2SGain);
                S2Fsir = 10 * log10(S2Fsir);//dB值
                if (F2Ssir > threshold && S2Fsir > threshold) {
//                    cout << "no edge" << endl;
                } else {
                    cout << "set edge: " << edgeID << endl;
                    vector<int> vecNodes;
                    vecNodes.push_back(D2DTx1ID - 1);
                    vecNodes.push_back(D2DTx2ID - 1);
                    mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                    edgeID++;
                }
            }
        }
    }

    //遍历三个节点的累计干扰
    //1个Macro User 2个D2D pair
    double M2FSsir = 0;
    double F2MSsir = 0;
    double S2MFsir = 0;
    for (int MacroUserID : vecMacroUserID) {
        for (auto D2DPair1 : mapD2DUserID) {
            int D2DTx1ID = D2DPair1.first;
            int D2DRx1ID = D2DPair1.second;
            for (auto D2DPair2 : mapD2DUserID) {
                int D2DTx2ID = D2DPair2.first;
                int D2DRx2ID = D2DPair2.second;
                if (D2DRx1ID < D2DRx2ID) {
                    //M2FSsir
                    double M2MLinkloss = GetLinkloss(MacroUserID, 0, SystemDriveBus::iSlot);
                    double F2MLinkloss = GetLinkloss(D2DTx1ID, 0, SystemDriveBus::iSlot);
                    double S2MLinkloss = GetLinkloss(D2DTx2ID, 0, SystemDriveBus::iSlot);
                    double M2MGain = pow(10, -M2MLinkloss / 10);//线性值
                    double F2MGain = pow(10, -F2MLinkloss / 10);//线性值
                    double S2MGain = pow(10, -S2MLinkloss / 10);//线性值
                    M2FSsir = (MacroUserTxPower * M2MGain) / (D2DTxPower * F2MGain +D2DTxPower * S2MGain);
                    M2FSsir = 10 * log10(M2FSsir);//dB值

                    //F2MSsir
                    double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double M2FLinkloss = GetLinkloss(MacroUserID, D2DRx1ID, SystemDriveBus::iSlot);
                    double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                    double M2FGain = pow(10, -M2FLinkloss / 10);//线性值
                    double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                    F2MSsir = (D2DTxPower * F2FGain) / (MacroUserTxPower * M2FGain +D2DTxPower * S2FGain);
                    F2MSsir = 10 * log10(F2MSsir);//dB值

                    //S2MFsir
                    double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double M2SLinkloss = GetLinkloss(MacroUserID, D2DRx2ID, SystemDriveBus::iSlot);
                    double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                    double M2SGain = pow(10, -M2SLinkloss / 10);//线性值
                    double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                    S2MFsir = (D2DTxPower * S2SGain) / (MacroUserTxPower * M2SGain +D2DTxPower * F2SGain);
                    S2MFsir = 10 * log10(S2MFsir);//dB值

                    if (M2FSsir > threshold && F2MSsir > threshold && S2MFsir > threshold) {
//                    cout << "no edge" << endl;
                    } else {
                        cout << "set edge: " << edgeID << endl;
                        vector<int> vecNodes;
                        vecNodes.push_back(MacroUserID - 1);
                        vecNodes.push_back(D2DTx1ID - 1);
                        vecNodes.push_back(D2DTx2ID - 1);
                        mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                        edgeID++;
                    }
                }
            }
        }
    }
    //3个D2D pair First Second Third
    double F2STsir = 0;
    double S2FTsir = 0;
    double T2FSsir = 0;
    for (auto D2DPair1 : mapD2DUserID) {
        int D2DTx1ID = D2DPair1.first;
        int D2DRx1ID = D2DPair1.second;
        for (auto D2DPair2 : mapD2DUserID) {
            int D2DTx2ID = D2DPair2.first;
            int D2DRx2ID = D2DPair2.second;
            for (auto D2DPair3 : mapD2DUserID) {
                int D2DTx3ID = D2DPair3.first;
                int D2DRx3ID = D2DPair3.second;
                if (D2DRx1ID < D2DRx2ID && D2DRx1ID < D2DRx3ID && D2DRx2ID < D2DRx3ID) {
                    //F2STsir
                    double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double T2FLinkloss = GetLinkloss(D2DTx3ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                    double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                    double T2FGain = pow(10, -T2FLinkloss / 10);//线性值
                    F2STsir = (D2DTxPower * F2FGain) / (D2DTxPower * T2FGain +D2DTxPower * S2FGain);
                    F2STsir = 10 * log10(F2STsir);//dB值

                    //S2FTsir
                    double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double T2SLinkloss = GetLinkloss(D2DTx3ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                    double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                    double T2SGain = pow(10, -T2SLinkloss / 10);//线性值
                    S2FTsir = (D2DTxPower * S2SGain) / (D2DTxPower * T2SGain +D2DTxPower * F2SGain);
                    S2FTsir = 10 * log10(S2FTsir);//dB值

                    //T2FSsir
                    double T2TLinkloss = GetLinkloss(D2DTx3ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double F2TLinkloss = GetLinkloss(D2DTx1ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double S2TLinkloss = GetLinkloss(D2DTx2ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double T2TGain = pow(10, -T2TLinkloss / 10);//线性值
                    double F2TGain = pow(10, -F2TLinkloss / 10);//线性值
                    double S2TGain = pow(10, -S2TLinkloss / 10);//线性值
                    T2FSsir = (D2DTxPower * T2TGain) / (D2DTxPower * F2TGain +D2DTxPower * S2TGain);
                    T2FSsir = 10 * log10(T2FSsir);//dB值

                    if (F2STsir > threshold && S2FTsir > threshold && T2FSsir > threshold) {
//                    cout << "no edge" << endl;
                    } else {
                        cout << "set edge: " << edgeID << endl;
                        vector<int> vecNodes;
                        vecNodes.push_back(D2DTx1ID - 1);
                        vecNodes.push_back(D2DTx2ID - 1);
                        vecNodes.push_back(D2DTx3ID - 1);
                        mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                        edgeID++;
                    }
                }
            }
        }
    }

    //写入incidence Matrix
    for (int nodeID = 0; nodeID < nodeNum; ++nodeID) {
        vector<int> vecNode2Edge;
        for (auto temp : mapEdgeVecNodes) {
//            edgeID = temp.first;
            bool haveEdge = false;
            for (auto tempNodeID : temp.second) {
                if (nodeID == tempNodeID) haveEdge = true;
            }
            if (haveEdge) {
                vecNode2Edge.push_back(1);
            } else {
                vecNode2Edge.push_back(0);
            }
        }
        hypergraph.push_back(vecNode2Edge);
    }

//    //输出图
//    for (int i = 0; i < hypergraph.size(); ++i) {
//        for (int j = 0; j < hypergraph[0].size(); ++j) {
//            cout << hypergraph[i][j] << ",";
//        }
//        cout << endl;
//    }
//    cout << endl;
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

    if (SystemDriveBus::ULorDL == "UL") {
        iPriority = 30;//上行链路　接收优先级
    } else {
        iPriority = 10;//下行链路　发射优先级
    }

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
        part1:
        double M2SRadius = 0;//相对宏基站的半径
        double S2SRadius = 0;//相对小基站的半径
        while (M2SRadius < 100)
        {
            double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
            M2SRadius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_radius() - SmallCellRadius);
            dXPoint = M2SRadius * sin(temp_angle);
            dYPoint = M2SRadius * cos(temp_angle);
        }

        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.second->sGetType() == "class SmallCell *" && this->iID != _temp.second->iGetID())
            {
                SmallCell *tempSmallCellPtr = dynamic_cast<SmallCell*>(_temp.second);
                double tempXPoint = tempSmallCellPtr->GetXPoint();
                double tempYPoint = tempSmallCellPtr->GetYPoint();
                S2SRadius = getDistance(dXPoint, dYPoint, tempXPoint, tempYPoint);
                if (S2SRadius < 50) goto part1;
            }
        }
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

    if (SystemDriveBus::ULorDL == "UL") {
        iPriority = 30;//上行链路　接收优先级
    } else {
        iPriority = 10;//下行链路　发射优先级
    }

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
    //LTE（MacroCell）系统参数
    mode_par Macro_mode_par;
    int Macro_user_num;
    int cell_num;

    //读取Small Cell）系统参数
    mode_par SmallCell_mode_par;
    int SmallCell_user_num;

    iID = SystemDriveBus::GetCountDevice();

    if (SystemDriveBus::ULorDL == "UL") {
        iPriority = 10;//上行链路　发射优先级
    } else {
        iPriority = 30;//下行链路　接收优先级
    }

    UserID = SystemDriveBus::GetCountUser();
    sType = "class User *";
    user_type = _user_type;
    mainTxID = -1;
    D2DTxID = -1;
    if (user_type == "MacroCell")
    {
        //读取LTE（MacroCell）系统参数
        Macro_mode_par = SystemDriveBus::ModeID2Par.at(1);
        Macro_user_num = Macro_mode_par.get_numOfRx();
        cell_num = Macro_mode_par.get_numOfTx();
        part1:
        double U2SRadius = 0; //用户到小蜂窝基站的距离
        double x, y, tempx, tempy;
        int randCell;
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

        for (auto _temp : SystemDriveBus::SlotDriveBus)
        {
            if (_temp.second->sGetType() == "class SmallCell *" && this->iID != _temp.second->iGetID())
            {
                SmallCell *tempSmallCellPtr = dynamic_cast<SmallCell*>(_temp.second);
                double tempXPoint = tempSmallCellPtr->GetXPoint();
                double tempYPoint = tempSmallCellPtr->GetYPoint();
                U2SRadius = getDistance(dXPoint, dYPoint, tempXPoint, tempYPoint);
                if (U2SRadius < 50) goto part1;
            }
        }

        mainTxID = cellID;
    }
    else if (user_type == "SmallCell")
    {
        //读取Small Cell）系统参数
        SmallCell_mode_par = SystemDriveBus::ModeID2Par.at(2);
        SmallCell_user_num = SmallCell_mode_par.get_numOfRx();

        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::ModeID2Par.at(2).get_radius());
        relativeXPoint = temp_radius * sin(temp_angle);
        relativeYPoint = temp_radius * cos(temp_angle);
        int SmallCell_num = SystemDriveBus::ModeID2Par.at(2).get_numOfTx();
//        int randSmallCell = (int) (((double) rand() / RAND_MAX) * SmallCell_num);
        int randSmallCell = (iID - cell_num - SmallCell_num - Macro_user_num) / (SmallCell_user_num / SmallCell_num);
        SmallCellID = randSmallCell;
        dXPoint = SystemDriveBus::SmallCellPosition.at(SmallCellID).dXPoint + relativeXPoint;
        dYPoint = SystemDriveBus::SmallCellPosition.at(SmallCellID).dYPoint + relativeYPoint;

        mainTxID = cell_num + SmallCellID;
    }
    else if (user_type == "Wifi")
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
    else if (user_type == "D2D")
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
    else if (user_type == "D2DTx") {
        if (SystemDriveBus::system_shape.get_shape() == "circle") {
            double Tx2BSRadius = 0;
            while (Tx2BSRadius < 100) {
                double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
                double temp_radius = sqrt((double) rand() / RAND_MAX) * (SystemDriveBus::system_shape.get_radius());
                dXPoint = temp_radius * sin(temp_angle);
                dYPoint = temp_radius * cos(temp_angle);
                Tx2BSRadius = getDistance(dXPoint, dYPoint, 0, 0);
            }
            iPriority = 10; //发射优先级
            deviceLocation D2DTxLocation;
            D2DTxLocation.dXPoint = dXPoint;
            D2DTxLocation.dYPoint = dYPoint;
            SystemDriveBus::D2DTxPosition.insert(pair<int, deviceLocation>(iID, D2DTxLocation));
        }
        else {
            cout << "error!!!!!" << endl;
        }
    }
    else {
        cout << "error!!!!!" << endl;
    }
}

User::User(Interface * _D2DRxPtr)
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 30;//接收优先级
    UserID = SystemDriveBus::GetCountUser();
    sType = "class User *";
    user_type = "D2DRx";
    mainTxID = -1;
    D2DTxID = -1;

    double MaxRadius = 20; //D2D发射机和接收机之间的最大间距

    double cellRadius = SystemDriveBus::system_shape.get_radius();
    double Rx2BSRadius = cellRadius + 1;//D2DRx相对宏基站的距离
    User *_tempUser = dynamic_cast<User *>(_D2DRxPtr);
    while (Rx2BSRadius > cellRadius) {
        double temp_angle = (double) rand() / RAND_MAX * 2 * PI;
        double temp_radius = sqrt((double) rand() / RAND_MAX) * MaxRadius;
        double relativeX = temp_radius * sin(temp_angle);
        double relativeY = temp_radius * cos(temp_angle);
        dXPoint = _tempUser->getDXPoint() + relativeX;
        dYPoint = _tempUser->getDYPoint() + relativeY;
        Rx2BSRadius = getDistance(dXPoint, dYPoint, 0, 0);
    }
    D2DTxID = _tempUser->iGetID();
    mainTxID = _tempUser->iGetID();
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

Interface * User::Create(Interface * _D2DRxPtr)
/************************************************************************************************
函数名称：Create
主要功能：调用BS的构造函数，并返回BS的对象指针
输入参数：
输出参数：BSPtr，BS的对象指针
其他信息：
*************************************************************************************************/
{
    Interface *UserPtr;
    UserPtr = new User(_D2DRxPtr);
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
    software.softwareTx.ConnectType(user_type);
    hardware.ConnectID(iID);//获取D2DUserID
//    software.softwareRx.ConnectMode(mode);
    software.softwareRx.ConnectHardLinkloss(hardware.GetLinkloss());//接收软体获取硬体里的路损
//    software.softwareTx.ConnectHardPower(hardware.txPower);//发射软体获取硬体里的功率
//    hardware.ConnectstartSlot(software.softwareTx.startSlot);//硬体从软体发射类里获取起始发射时间
    hardware.ConnectUserType(user_type);
}

void User::WorkSlot(default_random_engine dre)
{
    //按照发射和接收优先级区分D2DTx D2DRx
    if (iPriority >= 30) { //接收优先级　D2DRx
        //调用硬体类的接收workslot，将路损指针传给信道，直接写入接收硬体，便于接收软体进行SINR计算
        hardware.WorkslotHardwareEntityRx();
        //调用接收软体类的workslot
        software.softwareRx.WorkSlotSoftwareEntity();
    } else { //发射优先级　D2DTx
        //更新功率
        hardware.InitialHardwareEntity();
        hardware.WorkslotHardwareEntityTx();
        software.softwareTx.WorkSlotSoftwareEntity();
    }

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
        ImportExport::fout << "h5 = scatter(" << dXPoint << "," << dYPoint << ",'^', 'CData', [0 0 255] / 255);" << endl;
        ImportExport::fout << "hold on;" << endl;
    }
    if (user_type == "D2DRx")
    {
        int TxID = mainTxID;
        Interface * D2DTxPtr = SystemDriveBus::ID2PtrBus.at(TxID);
        User * D2DTxUserPtr = dynamic_cast<User *>(D2DTxPtr);
        double TxXPoint = D2DTxUserPtr->getDXPoint();
        double TxYPoint = D2DTxUserPtr->getDYPoint();
        ImportExport::fout << "h6= scatter(" << dXPoint << "," << dYPoint << ",'x', 'CData', [0 0 255] / 255);" << endl;
        ImportExport::fout << "plot([" << TxXPoint << "," << dXPoint << "],[" << TxYPoint << "," << dYPoint << "],'b-')" << endl;
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

int User::getMainTxID() const {
    return mainTxID;
}

int User::getD2DTxID() const {
    return D2DTxID;
}

