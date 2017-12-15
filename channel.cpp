//
// Created by lee on 17-8-28.
//
#include "channel.h"
#include "SystemDriveBus.h"
#include "ImtaChannelModel.h"
#include "dBLibrary.h"
#include "Mymath.h"
#include "InterferenceIndex.h"
#include <ctime>

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
    PushChannelGain2MySQL();
}

void channel::UpdateAllTxLinkLossTable()
{
    mLinkLoss.clear();//每次更新前先清除掉容器

    double fc = 2300;
    double angle = 0;
    double angleTemp = 0;
    double angle3dB = 65;
    double maxAttenuation = 20;
    double angleX, angleY;
    double STDDeviation = 1;
    double linkLoss, channelGain, shadowFading = 0;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;

    double antennaGain = 0;//dBi
    double  pathLoss = 0;

    static int i;

    bool losOrNlos;

    Interface *TxPtr = nullptr;

    Interface *receivePtr = SystemDriveBus::ID2Ptr(receiveID);
    if (receivePtr->sGetType() == "class MacroCell *") { //接收机是宏蜂窝基站
        MacroCell *macroRxPtr = dynamic_cast<MacroCell *>(receivePtr);
        x1 = macroRxPtr->GetXPoint();
        y1 = macroRxPtr->GetYPoint();
        for (auto IDtemp : sTxID)//遍历所有与该接收机有关的发射机
        {//1 begain
            //先算路径损耗
            TxPtr = SystemDriveBus::ID2Ptr(IDtemp);
            string TxType = TxPtr->sGetType();
            //选择发射机的类型
            if (TxType == "class User *") {
                User *userTxPtr = dynamic_cast<User *>(TxPtr);
                x2 = userTxPtr->getDXPoint();
                y2 = userTxPtr->getDYPoint();
                losOrNlos = IMTA::losOrNlosSelectUMI(x1, y1, x2, y2);
                if (losOrNlos) {
                    pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);
                }
                else {
                    pathLoss = IMTA::vPathLossUMINlos(x1, y1, x2, y2, STDDeviation, fc);
                }
            } else {
                cout << "set tx Error!!!" << endl;
            }

            //没有涉及相关距离的阴影衰落
            std::default_random_engine generator(i);
            //std::lognormal_distribution<double> Lognormal(0.0, STDDeviation);
            std::lognormal_distribution<double> Lognormal(0.0, STDDeviation);
            shadowFading = dB::Linear2dB(Lognormal(generator));
            i = i + 100;

//        /****Shadow_Fadings****/
//        double temp_double = STD_DEVIATION * randn();
//        shadowFading = sqrt(SHADOW_CORREATION) * (temp_double + STD_DEVIATION * randn());

            shadowFading = 0;
            antennaGain = 0;

            linkLoss = pathLoss + shadowFading + antennaGain;
            mLinkLoss[IDtemp] = linkLoss;

            channelGain = pow(10, -linkLoss / 10);
            mChannelGain[IDtemp] = channelGain;
        }
    } else if (receivePtr->sGetType() == "class User *") {
        User *userRxPtr = dynamic_cast<User *>(receivePtr);
        x1 = userRxPtr->getDXPoint();//目的坐标
        y1 = userRxPtr->getDYPoint();
        for (auto IDtemp : sTxID)//遍历所有与该接收机有关的发射机
        {//1 begain
            //先算路径损耗
            TxPtr = SystemDriveBus::ID2Ptr(IDtemp);
            string TxType = TxPtr->sGetType();

            //选择发射机的类型
            if (TxType == "class MacroCell *")
            {
                MacroCell * macroTxPtr = dynamic_cast<MacroCell *>(TxPtr);
                x2 = macroTxPtr->GetXPoint();
                y2 = macroTxPtr->GetYPoint();

                losOrNlos = IMTA::losOrNlosSelectUMI(x1, y1, x2, y2);
                if (losOrNlos) {
                    pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);
                }
                else {
                    pathLoss = IMTA::vPathLossUMINlos(x1, y1, x2, y2, STDDeviation, fc);
                }

//                pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);

                angleX = x1 - x2;
                angleY = y1 - y2;
                angleTemp = angleCalc(angleX, angleY);//弧度
                angle = angleTemp * 180 / PI;
                angle = angle - 180;

                //天线增益
                antennaGain = -min(12 * pow(angle / angle3dB, 2), maxAttenuation);//天线增益
            }
            else if (TxType == "class SmallCell *")
            {
                SmallCell * smallCellTxPtr = dynamic_cast<SmallCell *>(TxPtr);
                x2 = smallCellTxPtr->GetXPoint();
                y2 = smallCellTxPtr->GetYPoint();

//            pathLoss = IMTA::vPathLoss(x1, y1, x2, y2, STDDeviation, fc);

                losOrNlos = IMTA::losOrNlosSelectUMI(x1, y1, x2, y2);
                if (losOrNlos) {
                    pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);
                }
                else {
                    pathLoss = IMTA::vPathLossUMINlos(x1, y1, x2, y2, STDDeviation, fc);
                }
            }
            else if (TxType == "class User *") {
                User * UserTxPtr = dynamic_cast<User *>(TxPtr);
                x2 = UserTxPtr->getDXPoint();
                y2 = UserTxPtr->getDYPoint();

                losOrNlos = IMTA::losOrNlosSelectUMI(x1, y1, x2, y2);
                if (losOrNlos) {
                    pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);
                }
                else {
                    pathLoss = IMTA::vPathLossUMINlos(x1, y1, x2, y2, STDDeviation, fc);
                }

//                pathLoss = IMTA::vPathLossUMILos(x1, y1, x2, y2, STDDeviation, fc);
            } else {
                cout << "set tx error!!!" << endl;
            }

            //没有涉及相关距离的阴影衰落
            std::default_random_engine generator(i);
            //std::lognormal_distribution<double> Lognormal(0.0, STDDeviation);
            std::lognormal_distribution<double> Lognormal(0.0, STDDeviation);
            shadowFading = dB::Linear2dB(Lognormal(generator));
            i = i + 100;

//        /****Shadow_Fadings****/
//        double temp_double = STD_DEVIATION * randn();
//        shadowFading = sqrt(SHADOW_CORREATION) * (temp_double + STD_DEVIATION * randn());

            shadowFading = 0;
            antennaGain = 0;

            linkLoss = pathLoss + shadowFading + antennaGain;
            mLinkLoss[IDtemp] = linkLoss;

            channelGain = pow(10, -linkLoss / 10);
            mChannelGain[IDtemp] = channelGain;
        }
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
        int index = 0;
        set<int> txUserList;
        set<int> rxUserList;
        InterferenceIndex::GetInstance().GetRelevantUserTxRB(iRB, SystemDriveBus::iSlot, txUserList);
        InterferenceIndex::GetInstance().GetRelevantUserRxRB(iRB, SystemDriveBus::iSlot, rxUserList);
        //对每个RB块上的有效的发射机去路损表里查找对应的路损，存放到vecMapLinkLossPtr
        map<int, pair<double, double>> mLinkLossTemp;
        pair<double, double> linklossPowerTemp;

        for (auto temp : rxUserList)
        {
            if (temp == RxPtr->iGetID())
            {
                if(txUserList.size() != 0)//如果在某RB上，发射用户存在
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
                    index++;
                }
            }
        }
        if (!index)
        {
            //如果在某RB上，发射用户不存在,直接push一个空的map进去
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

const set<int> &channel::getSTxID() const {
    return sTxID;
}

void channel::PushChannelGain2MySQL() {
    MySQLManager *mysql = new MySQLManager("127.0.0.1", "lee", "281217", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        //数字 转 string
        string TxID;
        string RxID = intToString(receiveID);
        string linkLoss;
        double tempChannelGain;
        string channelGain;
        string slotID;
        for (auto tempLinkLoss : mLinkLoss) {
            //数字 转 string
            TxID = intToString(tempLinkLoss.first);
            linkLoss = doubleToString(tempLinkLoss.second);
            tempChannelGain = pow(10, -tempLinkLoss.second / 10);
            channelGain = doubleToString(tempChannelGain);
            slotID = intToString(SystemDriveBus::iSlot);
            //将信道增益信息插入数据库
            //构造MySQL插入语句
            string SQLString = "INSERT INTO channelGain(TxID, RxID, linkLoss, channelGain, slotID) "
                                       "VALUES(" + TxID + "," + RxID + "," + linkLoss + "," + channelGain + "," + slotID + ")";

            if (mysql->insert(SQLString))
            {
//                cout << "插入成功" << endl;
            }
            else cout << "执行失败" << endl;
        }
    }
    mysql->destroyConnection();
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
