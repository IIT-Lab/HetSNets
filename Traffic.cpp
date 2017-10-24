//
// Created by lee on 17-8-31.
//
#include"Traffic.h"
namespace Traffic
{
    //************************************TBlockInfo类中的各函数实现**********************************************

    TrafficPacket::TrafficPacket() {};
    TrafficPacket::~TrafficPacket() {};

    TrafficPacket& TrafficPacket::operator=(const TrafficPacket &tblockinfo)
    {
        // 首先检测等号右边的是否就是左边的对象本身，若是本对象本身,则直接返回
        if (this == &tblockinfo)
        {
            return *this;
        }
        // 复制等号右边的成员到左边的对象中
        this->iPacketType = tblockinfo.iPacketType;           //数据包类型，1：表示语音数据包类型，2：数据，3：视频
        this->iPacketNumber = tblockinfo.iPacketNumber;        //数据包的标号；
        this->dBitSizeRemain = tblockinfo.dBitSizeRemain;          //表明本数据包传输后剩余的数据量大小
        this->dBitSizeSentAlready = tblockinfo.dBitSizeSentAlready;     //表明本数据包已经传输出去的数据量大小
        this->dArriveTime = tblockinfo.dArriveTime;
        this->dCompleteTime = tblockinfo.dCompleteTime;
        this->vOptionalAttributes = tblockinfo.vOptionalAttributes;	//可选属性涉及到深拷贝浅拷贝，需要注意，未完成

        // 把等号左边的对象再次传出
        // 目的是为了支持连等 eg:    a=b=c 系统首先运行 b=c
        // 然后运行 a= ( b=c的返回值,这里应该是复制c值后的b对象)
        return *this;
    }

    TrafficPacket::TrafficPacket(const TrafficPacket &tblockinfo)             //复制构造函数
    {
        this->iPacketType = tblockinfo.iPacketType;           //数据包类型，1：表示语音数据包类型，2：数据，3：视频
        this->iPacketNumber = tblockinfo.iPacketNumber;        //数据包的标号；
        this->dBitSizeRemain = tblockinfo.dBitSizeRemain;          //表明本数据包传输后剩余的数据量大小
        this->dBitSizeSentAlready = tblockinfo.dBitSizeSentAlready;     //表明本数据包已经传输出去的数据量大小
        this->dArriveTime = tblockinfo.dArriveTime;
        this->dCompleteTime = tblockinfo.dCompleteTime;
        this->vOptionalAttributes = tblockinfo.vOptionalAttributes;//可选属性涉及到深拷贝浅拷贝，需要注意，未完成
    }

    TrafficPacket::TrafficPacket(int _iPacketType, int  _iPacketNumber, double  _dBitSizeRemain, double  _dBitSizeSentAlready,
                                 double  _dArriveTime, double _dCompleteTime, vector<int>& _vOptionalAttributes)
    //有基本属性初始化，无可选属性
    {
        iPacketType = _iPacketType;           //数据包类型，1：表示语音数据包类型，2：数据，3：视频
        iPacketNumber = _iPacketNumber;        //数据包的标号；
        dBitSizeRemain = _dBitSizeRemain;          //表明本数据包传输后剩余的数据量大小
        dBitSizeSentAlready = _dBitSizeSentAlready;     //表明本数据包已经传输出去的数据量大小
        dArriveTime = _dArriveTime;
        dCompleteTime = _dCompleteTime;
        vOptionalAttributes = _vOptionalAttributes;
    }
    void TrafficPacket::Initial(int _iPacketType, int  _iPacketNumber, double  _dBitSizeRemain, double  _dBitSizeSentAlready,
                                double  _dArriveTime, double _dCompleteTime, vector<int>& _vOptionalAttributes)
    {
        iPacketType = _iPacketType;           //数据包类型，1：表示语音数据包类型，2：数据，3：视频
        iPacketNumber = _iPacketNumber;        //数据包的标号；
        dBitSizeRemain = _dBitSizeRemain;          //表明本数据包传输后剩余的数据量大小
        dBitSizeSentAlready = _dBitSizeSentAlready;     //表明本数据包已经传输出去的数据量大小
        dArriveTime = _dArriveTime;
        dCompleteTime = _dCompleteTime;
        vOptionalAttributes = _vOptionalAttributes;
    }

    void TrafficPacket::vTransmit(double _dBufferSize)
    {
        dBitSizeRemain -= _dBufferSize;
        dBitSizeSentAlready += _dBufferSize;
    }
    void TrafficPacket::vTransmitSuccess(double _dBufferSize)
    {
        dBitSizeSentAlready -= _dBufferSize;
    }
    void TrafficPacket::vTransmitFail(double _dBufferSize)
    {
        dBitSizeRemain += _dBufferSize;
        dBitSizeSentAlready -= _dBufferSize;
    }
}

TBlockInfo::TBlockInfo()
{

}

TBlockInfo::~TBlockInfo()
{

}

TBlockInfo *TBlockInfo::Create()
{
    TBlockInfo *TBlockPtr;
    TBlockPtr = new TBlockInfo();
    return TBlockPtr;
}

void TBlockInfo::initial(int _ARQNum)
{
    blockIndexL3 = -1;
    blockIndexL2 = -1;
    SEIndex = -1;
    CodeLength = -1;
    transNumber = -1;
    blockType = 0;
    isRetransblock = 0;

    ARQNum = _ARQNum;
}
