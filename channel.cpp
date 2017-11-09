//
// Created by lee on 17-8-28.
//
#include "channel.h"
#include "SystemDriveBus.h"
//#include "InterferenceIndex.h"
#include "ImtaChannelModel.h"
#include "dBLibrary.h"
#include "Mymath.h"
#include "Global.h"
#include "InterferenceIndex.h"

#include <ctime>
#include <string>
#include <set>

using namespace std;

int channel::countTag = 0;

channel::channel()
{
    iID = SystemDriveBus::GetCountDevice();
    iPriority = 20;//设置该信道对象的优先级是20
    //iID = SystemDriveBus::GetCountChannel();
    sType = "class channel *";
}

void channel::initial(default_random_engine dre)
{

}

void channel::ChannelInitial(Interface &_Receiver, vector<Interface*>vTxContainer)
/*信道initial工作流程
1、首先拿到接收用户的ID
2、对每一个和接收用户处于相同频段的ID的发射用户//我们会假设发射机初始化时会在所有其覆盖频段登记
3、计算两个用户之间的大尺度衰落（信道模型选择器）
4、利用额定功率+大尺度衰落进行判断
-需要计算小尺度衰落
-仅需考虑大尺度即可
-可以直接忽略为一个固定值
5、生成需要计算小尺度的多径（UMA是若干组480个数）
6、将小尺度函数进行登记
本函数结束*/
{
    for (auto TRansmitterIndex : vTxContainer)//对于所有的ID对应的发射机进行信道判断
    {
        receiveID = _Receiver.iGetID();
        receiveType  = _Receiver.sGetType();
        sTxID.insert(TRansmitterIndex->iGetID());
    }
}

int channel::GetRxID()
{
    return receiveID;
}

void channel::WorkSlot(default_random_engine dre)
{
    //初始化，把所有可能的发射机对本接收机的路损求出来，存放到容器中
    UpdateAllTxLinkLossTable();
}

void channel::UpdateAllTxLinkLossTable()
{
    mLinkLoss.clear();//每次更新前先清除掉容器

    int Pf = 16;
    double LF = 0;
    double LNLOS = 15;
    double fc = 2000;
    double angle = 0;
    double angleTemp = 0;
    double angle3dB = 65;
    double maxAttenuation = 20;
    double angleX, angleY;
    double STDDeviation = 0;
    double linkLoss, channelGain, shadowFading = 0;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    double antennaGain = 3;//dBi
    double  pathLoss = 0;
    double d1, d2, d;//存储前一时刻发射和接收的距离，当前时刻发射和接收的距离
    int sectorID;

    static int i;

    string losOrNlos;
    Interface *receivePtr = SystemDriveBus::ID2Ptr(receiveID);
    Interface *TXPtr = nullptr;
    MacroCell *macroTxPtr = nullptr;
//    SmallCell *smallCellTxPtr = dynamic_cast<SmallCell *>(receivePtr);
    SmallCell *smallCellTxPtr = nullptr;
    Wifi *wifiTxPtr = nullptr;
    User *userTxPtr = nullptr;
    User *userRxPtr = dynamic_cast<User *>(receivePtr);

//    if (this->getReceiveType() == "class SmallCell *")
//    {
//        x1 = smallCellTxPtr->GetXPoint();//目的坐标
//        y1 = smallCellTxPtr->GetYPoint();
//    }
    if (this->getReceiveType() == "class User *")
    {
        x1 = userRxPtr->getDXPoint();//目的坐标
        y1 = userRxPtr->getDYPoint();
    }
//    else //出错
//    {
//        cout << "仿真配置错误！" << endl;
//    }

    for (auto IDtemp : sTxID)//遍历所有与该接收机有关的发射机
    {//1 begain
        //先算路径损耗
        TXPtr = SystemDriveBus::ID2Ptr(IDtemp);
        string TxType = TXPtr->sGetType();

        //选择发射机的类型
        if (TxType == "class MacroCell *")
        {
            macroTxPtr = dynamic_cast<MacroCell *>(TXPtr);
            x2 = macroTxPtr->GetXPoint();
            y2 = macroTxPtr->GetYPoint();
            ////UMA场景
            pathLoss = IMTA::vPathLoss(x1, y1, x2, y2, STDDeviation, fc);

            angleX = x1 - x2;
            angleY = y1 - y2;
            angleTemp = angleCalc(angleX, angleY);//弧度
            angle = angleTemp * 180 / PI;

            //cout << "发射机：" << macroTxPtr->macroID << "(" << x2 << "," << y2 << ")" << endl;
            if (angle < 90 || (angle < 360 && angle > 330))
            {
                //cout << "第1扇区" << endl;
                sectorID = 1;
            }
            else if (angle < 210 && angle > 90)
            {
                //cout << "第2扇区..................." << endl;
                sectorID = 2;
            }
            else
            {
                //cout << "第3扇区......................................" << endl;
                sectorID = 3;
            }
            angle = angle - 180;

            //天线增益
            antennaGain = -min(12 * pow(angle / angle3dB, 2), maxAttenuation);//天线增益
        }
        else if (TxType == "class SmallCell *")
        {
            smallCellTxPtr = dynamic_cast<SmallCell *>(TXPtr);
            x2 = smallCellTxPtr->GetXPoint();
            y2 = smallCellTxPtr->GetYPoint();
            ////UMA场景
            pathLoss = IMTA::vPathLoss(x1, y1, x2, y2, STDDeviation, fc);
        }
        else if (TxType == "class Wifi *")
        {
//            wifiTxPtr = dynamic_cast<Wifi *>(TXPtr);
//            x2 = wifiTxPtr->getDXPoint();
//            y2 = wifiTxPtr->getDYPoint();
//            d = getDistance(x1, y1, x2, y2);
//            //InH场景
//            losOrNlos = IMTA::losOrNlosSelect("InH", TXPtr, d, mLosOrNlos);
//            if (userRxPtr->getIndoorOrOutdoor() == 1)
//            {
//                pathLoss = 20 * log10(fc) + 26 * log10(d) + Pf - 28;
//            }
//            else if (losOrNlos == "LOS")
//            {
//                pathLoss = 32.4 + 26 * log10(d) + 20 * log10(fc);
//            }
//            else
//            {
//                pathLoss = 32.4 + 20 * log10(d) + 20 * log10(fc) + LF + LNLOS;
//            }
        }
        //没有涉及相关距离的阴影衰落
        std::default_random_engine generator(i);
        //std::lognormal_distribution<double> Lognormal(0.0, STDDeviation);
        std::lognormal_distribution<double> Lognormal(0.0, 1);
        shadowFading = dB::Linear2dB(Lognormal(generator));
        i = i + 100;

        shadowFading = 0;
//        antennaGain = 0;

        linkLoss = pathLoss + shadowFading + antennaGain;
        mLinkLoss[IDtemp] = linkLoss;

        channelGain = pow(10, -linkLoss / 10);
        mChannelGain[IDtemp] = channelGain;
    }
}

void channel::setAllTxPowerMap(int txID, double power)
{
    mTxPower[txID] = power;
}

void channel::setmLinkLossPtr(vector<map<int, pair<double, double>>> *vecMapLinkLossPowerPtr)
{
    Interface *TXPtr = nullptr;
    Interface *RxPtr = nullptr;
    RxPtr = SystemDriveBus::ID2Ptr(receiveID);

    //查询发端的该时隙下干扰登记表，遍历所有的RB块
    for (int iRB = 0; iRB < SUBBNUM; iRB++)
    {
        set<int> txUserList;
        set<int> rxUserList;
        InterferenceIndex::GetInstance().GetRelevantUserTxRB(iRB, SystemDriveBus::iSlot, txUserList);
        InterferenceIndex::GetInstance().GetRelevantUserRxRB(iRB, SystemDriveBus::iSlot, rxUserList);
        //对每个RB块上的有效的发射机去路损表里查找对应的路损，存放到vecMapLinkLossPtr
        map<int, pair<double, double>> mLinkLossTemp;
        pair<double, double> linklossPowerTemp;

        for (auto temp : rxUserList)
        {
            if (temp == RxPtr->iGetID() && txUserList.size() != 0)//如果在某RB上，发射用户存在
            {
                for (auto tx : txUserList)
                {
                    linklossPowerTemp.first = mLinkLoss[tx];
                    linklossPowerTemp.second = mTxPower[tx];
                    //if (mTxPower.find(txSector) != mTxPower.end())//查找发射机功率表，找到对应的发射功率
                    //{
                    //	linklossPowerTemp.second = mTxPower[txSector];
                    //}
                    mLinkLossTemp[tx] = linklossPowerTemp;

                }
                (*vecMapLinkLossPowerPtr).push_back(mLinkLossTemp);
            }
            else//如果在某RB上，发射用户不存在,直接push一个空的map进去
            {
                mLinkLossTemp.clear();
                (*vecMapLinkLossPowerPtr).push_back(mLinkLossTemp);
            }
        }

//        if (txUserList.size() != 0)//如果在某RB上，发射用户存在
//        {
//            for (auto tx : txUserList)
//            {
//                linklossPowerTemp.first = mLinkLoss[tx];
//                linklossPowerTemp.second = mTxPower[tx];
//                //if (mTxPower.find(txSector) != mTxPower.end())//查找发射机功率表，找到对应的发射功率
//                //{
//                //	linklossPowerTemp.second = mTxPower[txSector];
//                //}
//                mLinkLossTemp[tx] = linklossPowerTemp;
//
//            }
//            (*vecMapLinkLossPowerPtr).push_back(mLinkLossTemp);
//
//        }
//        else//如果在某RB上，发射用户不存在,直接push一个空的map进去
//        {
//            (*vecMapLinkLossPowerPtr).push_back(mLinkLossTemp);
//        }
    }
}

void channel::Display()
{

}

double channel::GetLinkLoss(int txID)
{
    return mLinkLoss.at(txID);
}

double channel::GetChannelGain(int txID)
{
    return mChannelGain.at(txID);
}

const string &channel::getReceiveType() const {
    return receiveType;
}

int mobileList::getLOSSelect() const {
    return LOSSelect;
}

void mobileList::setLOSSelect(int LOSSelect) {
    mobileList::LOSSelect = LOSSelect;
}

double mobileList::getR() const {
    return r;
}

void mobileList::setR(double r) {
    mobileList::r = r;
}

double mobileList::getSegma() const {
    return segma;
}

void mobileList::setSegma(double segma) {
    mobileList::segma = segma;
}

double mobileList::getXPR() const {
    return XPR;
}

void mobileList::setXPR(double XPR) {
    mobileList::XPR = XPR;
}

int mobileList::getNumofcluster() const {
    return numofcluster;
}

void mobileList::setNumofcluster(int numofcluster) {
    mobileList::numofcluster = numofcluster;
}

double angleList::getAoD_LOS() const {
    return AoD_LOS;
}

void angleList::setAoD_LOS(double AoD_LOS) {
    angleList::AoD_LOS = AoD_LOS;
}

double angleList::getAoA_LOS() const {
    return AoA_LOS;
}

void angleList::setAoA_LOS(double AoA_LOS) {
    angleList::AoA_LOS = AoA_LOS;
}

double angleList::getV_angle() const {
    return v_angle;
}

void angleList::setV_angle(double v_angle) {
    angleList::v_angle = v_angle;
}

double angleList::getAntennaGainTable() const {
    return antennaGainTable;
}

void angleList::setAntennaGainTable(double antennaGainTable) {
    angleList::antennaGainTable = antennaGainTable;
}

double LSP::getDS() const {
    return DS;
}

void LSP::setDS(double DS) {
    LSP::DS = DS;
}

double LSP::getASD() const {
    return ASD;
}

void LSP::setASD(double ASD) {
    LSP::ASD = ASD;
}

double LSP::getASA() const {
    return ASA;
}

void LSP::setASA(double ASA) {
    LSP::ASA = ASA;
}

double LSP::getSF() const {
    return SF;
}

void LSP::setSF(double SF) {
    LSP::SF = SF;
}

double LSP::getK() const {
    return K;
}

void LSP::setK(double K) {
    LSP::K = K;
}
