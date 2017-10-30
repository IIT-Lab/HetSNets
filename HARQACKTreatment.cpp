//
// Created by lee on 17-8-31.
//
#include"HARQACKTreatment.h"

const int HARQStateWatingForACKFeedback = 1;
const int HARQStateWatingForTxbufferSent = 0;
const int HARQStateIdle = -1;

/////////////////////////////////////////////////////////Txbuffer类实现///////////////////////////////////////////////////////////////////////////////////

Txbuffer::Txbuffer()
{
    TPacketPtr = new Traffic::TrafficPacket;
}

Txbuffer::Txbuffer(Traffic::TrafficPacket &_TPacketPtr, string  _sBlockType, int  _iSEIndex, int _CodeLength,
                   int  _iTransNumber, int  _iIsRetransblock, int _OptionalAttributesNum)
{
    TPacketPtr = &_TPacketPtr;
    //TPacketPtr->vTransmit(_CodeLength);  //将缓存包装入HARQ进程发出后，才做此操作
    sBlockType = _sBlockType;           //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    iSEIndex = _iSEIndex;	           //该包的SE序号，对应mcs等级
    CodeLength = _CodeLength;
    iTransNumber = _iTransNumber;         //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = _iIsRetransblock;      //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vOptionalAttributes.resize(0);          //有可选属性需要初始化时可更改。
}

void Txbuffer::initialTxbuffer(Traffic::TrafficPacket* _TPacketPtr, string  _sBlockType, int  _iSEIndex, int _CodeLength,
                               int  _iTransNumber, int  _iIsRetransblock, int _OptionalAttributesNum)
{
    TPacketPtr = _TPacketPtr;
    //TPacketPtr->vTransmit(_CodeLength);  //将缓存包装入HARQ进程发出后，才做此操作
    sBlockType = _sBlockType;           //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    iSEIndex = _iSEIndex;	           //该包的SE序号，对应mcs等级
    CodeLength = _CodeLength;
    iTransNumber = _iTransNumber;         //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = _iIsRetransblock;      //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vOptionalAttributes.resize(0);          //有可选属性需要初始化时可更改。
}

void Txbuffer::operator = (Txbuffer txbuffer)
{
    this->TPacketPtr = txbuffer.TPacketPtr;
    //TPacketPtr->vTransmit(_CodeLength); //这句话考虑是否需要，存疑
    this->sBlockType = txbuffer.sBlockType;
    this->iSEIndex = txbuffer.iSEIndex;
    this->CodeLength = txbuffer.CodeLength;
    this->iTransNumber = txbuffer.iTransNumber;
    this->iIsRetransblock = txbuffer.iIsRetransblock;
    this->vOptionalAttributes = txbuffer.vOptionalAttributes;
}

void Txbuffer::TransmitSuccess()
{
    TPacketPtr->vTransmitSuccess(CodeLength);
}
void Txbuffer::TransmitFail()
{
    TPacketPtr->vTransmitFail(CodeLength);
}
void Txbuffer::clear()                                          //清除数据，生成空包，用于发送缓存删除时的数据包清除
{
    TPacketPtr = NULL;
    sBlockType = "bllockUnDefined";           //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    iSEIndex = -1;	           //该包的SE序号，对应mcs等级
    CodeLength = 0;
    iTransNumber = -1;         //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = -10;      //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vOptionalAttributes.resize(0);
}

////////////////////////////////////////////////Rxbuffer类实现//////////////////////////////////////////////////////////////////////////

Rxbuffer::Rxbuffer(int  _iSEIndex, int _CodeLength, int _iTransNumber, int  _iIsRetransblock)
{
    iSEIndex        = _iSEIndex;	                     //该包的SE序号，对应mcs等级
    CodeLength      = _CodeLength;                     //信道上实际传输的包的大小
    iTransNumber    = _iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = _iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包

}

void Rxbuffer::initialRxbuffer(int  _iSEIndex, int _CodeLength, int _iTransNumber, int  _iIsRetransblock)
{
    iSEIndex = _iSEIndex;	                     //该包的SE序号，对应mcs等级
    CodeLength = _CodeLength;                     //信道上实际传输的包的大小
    iTransNumber = _iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = _iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
}

void Rxbuffer::operator=(Rxbuffer rxbuffer)
{
    this->iSEIndex = rxbuffer.iSEIndex;	                     //该包的SE序号，对应mcs等级
    this->CodeLength = rxbuffer.CodeLength;                     //信道上实际传输的包的大小
    this->iTransNumber = rxbuffer.iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    this->iIsRetransblock = rxbuffer.iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    //暂时未考虑可选属性的深浅拷贝
    //return *this;
}
Rxbuffer::Rxbuffer(const Rxbuffer& rxbuffer)
{
    this->iSEIndex        = rxbuffer.iSEIndex;	                     //该包的SE序号，对应mcs等级
    this->CodeLength      = rxbuffer.CodeLength;                     //信道上实际传输的包的大小
    this->iTransNumber    = rxbuffer.iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    this->iIsRetransblock = rxbuffer.iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    //暂时未考虑可选属性的深浅拷贝
}

void Rxbuffer::clear()                                          //清除数据，生成空包，用于发送缓存删除时的数据包清除
{
    sBlockType = "bllockUnDefined";           //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    iSEIndex = -1;	           //该包的SE序号，对应mcs等级
    CodeLength = 0;
    iTransNumber = -1;         //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    iIsRetransblock = -1;      //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vOptionalAttributes.resize(0);
}

TTxBuffer::TTxBuffer()
{

}

TTxBuffer::~TTxBuffer()
{

}

TTxBuffer *TTxBuffer::Create()
{
    TTxBuffer *TTxBufferPtr;
    TTxBufferPtr = new TTxBuffer();
    return TTxBufferPtr;
}

void TTxBuffer::initial(int _RxID)
{
    int tempNum=0;//数据包个数的计数
    for (int ARQNum=0;ARQNum<ARQNUM;ARQNum++)
    {
        TBlockInfo* TBlockInfoPtr = TBlockInfo::Create();
        TBlockInfoPtr->initial(ARQNum);
        mapTBlockInfo.insert(pair<int, TBlockInfo*>(ARQNum, TBlockInfoPtr));

        isWaitingACKFlag[ARQNum]=0;		//初始化为ARQ过程等待发送
        NACKTime[ARQNum]=-10;				//初始化为-10,以防调度时误判
        LatestNACKTime[ARQNum]=-10;
        allocateRBNum[ARQNum]=0;			//初始化发送缓存中资源块为空
        for (int tempRB=0;tempRB<SUBBNUM;tempRB++)
        {
            RBIdx[ARQNum][tempRB]=-1;		//发送缓存中RB号码为空
        }
        //初始化给每个发送缓存都填充数据
        mapTBlockInfo.at(ARQNum)->blockIndexL2=tempNum;
        mapTBlockInfo.at(ARQNum)->blockIndexL3=0;
        mapTBlockInfo.at(ARQNum)->blockType=1;
        mapTBlockInfo.at(ARQNum)->CodeLength=0;
        mapTBlockInfo.at(ARQNum)->isRetransblock=0;
        ARQID[ARQNum]=-1;
        isRetransUser=-1;
        tempNum=tempNum+1;
    }
    RxID = _RxID;
}

/****************************根据2006 LTE DL存档代码编写****************************/

ARQ_processes_Tx_buffers::ARQ_processes_Tx_buffers()
{

}

ARQ_processes_Tx_buffers::~ARQ_processes_Tx_buffers()
{

}

ARQ_processes_Tx_buffers *ARQ_processes_Tx_buffers::Create()
{
    ARQ_processes_Tx_buffers *ARQ_processes_Tx_buffersPtr;
    ARQ_processes_Tx_buffersPtr = new ARQ_processes_Tx_buffers();
    return ARQ_processes_Tx_buffersPtr;
}

void ARQ_processes_Tx_buffers::initial(int _RxID)
{
    //初始化用户的ARQ过程发送缓存
    ARQ_num = 0;
    Current_Process_id = -1;
    Transmited_Indicator = -1;
    block_info.t_packet.time_start = 0;
    block_info.t_packet.time_end = 0;
    block_info.t_packet.time_length = 0;

    RxID = _RxID;
}
