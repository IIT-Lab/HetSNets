//
// Created by lee on 17-9-13.
//
#include "Sense.h"
#include "SystemDriveBus.h"
#include "ImportExport.h"

SenseInterface* SenseInterface::_Instance = nullptr;

SenseInterface &SenseInterface::GetInstance() {
    if (_Instance == nullptr)
    {
        _Instance = new SenseInterface();
    }
    return *_Instance;
}

SenseInterface::SenseInterface()
{
    this->numOfSlot = SystemDriveBus::system_sense.get_numOfSlot();
    this->numOfSample = SystemDriveBus::system_sense.getNumOfSample();
    this->rankOfSymbol = SystemDriveBus::system_sense.getRankOfSymbol();
    this->numOfMainBS = SystemDriveBus::system_sense.getNumOfMainBS();
    this->numOfCooperationBS = SystemDriveBus::system_sense.getNumOfCooperationBS();
    this->falseAlarmProbability = SystemDriveBus::system_sense.getFalseAlarmProbability();
    this->minSINR = SystemDriveBus::system_sense.getMinSINR();
    this->maxSINR = SystemDriveBus::system_sense.getMaxSINR();
}

SenseInterface::~SenseInterface() {

}

void SenseInterface::SetSenseInterface()
{
    vecSample.clear();
    vecChannelGain.clear();
    //测试：一个Macro基站作为主机站
    MacroCell *_tempMacroCell = dynamic_cast<MacroCell *>(SystemDriveBus::ID2PtrBus.at(0));
    this->vecSample = _tempMacroCell->getVecSample();

    for (auto _temp : SystemDriveBus::SlotDriveBus)
    {
        if (_temp.first > 10 && _temp.first < 30)//信道对象
        {
            channel *_tempChannel = dynamic_cast<channel *>(_temp.second);
            vecChannelGain.push_back(_tempChannel->GetChannelGain(0)); //主机站Macro基站的ID是0
        }
    }
}

void SenseInterface::WriteSenseFile(int _slot)
{
    if (!_slot)
    {
        ImportExport::fout << "M = " << numOfSlot % 20 << ";" << endl;//蒙特卡洛仿真次数
        ImportExport::fout << "N = " << numOfSample << ";" << endl;//样本数
        ImportExport::fout << "K = " << numOfCooperationBS << ";" << endl;//合作用户数（Small Cell数）
        ImportExport::fout << "P = " << rankOfSymbol << ";" << endl;//发送信号的秩
        ImportExport::fout << "Pf = " << falseAlarmProbability << ";" << endl;//虚警概率
        ImportExport::fout << "HHH = zeros(K,20*M);" << endl;
        ImportExport::fout << "HH = zeros(K,M);" << endl;
    }

    ImportExport::fout << "H = [" << endl;
    for (int i = 0; i < numOfCooperationBS; ++i)
    {
        ImportExport::fout << vecChannelGain[i] << ";" << endl;
    }

    ImportExport::fout << "];" << endl;
    ImportExport::fout << "HH(:," << (_slot % 1000 + 1) << ") = H;" << endl;

    if (!_slot % 1000 && _slot != 0)
    {
        ImportExport::fout << "HHH(:," << (_slot - 1000 + 1) << ":" << (_slot + 1) << ") = HH;" << endl;
    }

    if (_slot == numOfSlot - 1)
    {
        ImportExport::fout << "SNR = [" << endl;
        for (auto _SNR : vecSinr)
        {
            ImportExport::fout << _SNR << ";" << endl;
        }
        ImportExport::fout << "];" << endl;

        ImportExport::fout << "txPower = [" << endl; //发射功率
        for (auto _txPower : vecTxPower)
        {
            ImportExport::fout << _txPower << ";" << endl;
        }
        ImportExport::fout << "];" << endl;

        ImportExport::fout << "noisePower = " << noisePower << ";" << endl;//1个RB上的噪声功率

        ImportExport::fout << "Pdsnr_oas1_1(M, N, K, P, Pf, SNR, txPower, noisePower, HHH);" << endl;
    }
}

void SenseInterface::addTxPower(double _txPower)
{
    vecTxPower.push_back(_txPower);
}

void SenseInterface::addSinr(double _sinr)
{
    vecSinr.push_back(_sinr);
}

void SenseInterface::setNoisePower(double _noisePower)
{
    noisePower = _noisePower;
}
