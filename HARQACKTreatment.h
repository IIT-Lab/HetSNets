//
// Created by lee on 17-8-31.
//

#ifndef HETSNETS_HARQACKTREATMENT_H
#define HETSNETS_HARQACKTREATMENT_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include<iostream>

#include "Traffic.h"
#include "Global.h"

using namespace std;

/*发端：发送缓存*/
class Txbuffer                           //送入信道的发送缓存的数据包
{
public:
    Traffic::TrafficPacket* TPacketPtr;  //业务数据包指针；
    string  sBlockType;                  //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    int  iSEIndex;	                     //该包的SE序号，对应mcs等级
    int  CodeLength;                     //信道上实际传输的包的大小
    int  iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    int  iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vector<int>   vOptionalAttributes;//每个用户根据需要设置可选属性,属性种类数可扩展。不同的属性对应不同的数据包类型，1，2，3

    //这三个属性后可以添加可选属性，需要在对象中说明属性的具体含义）
    Txbuffer();
    Txbuffer(Traffic::TrafficPacket &_TPacketPtr, string  sBlockType, int  iSEIndex, int CodeLength,
             int iTransNumber, int  iIsRetransblock, int OptionalAttributesNum);

    void operator=(Txbuffer txbuffer);
    ~Txbuffer(){};
    void initialTxbuffer(Traffic::TrafficPacket* _TPacketPtr, string  sBlockType, int  iSEIndex, int CodeLength,
                         int iTransNumber, int  iIsRetransblock, int OptionalAttributesNum);
    void TransmitSuccess();
    void TransmitFail();
    void clear();
};

/*收端：根据接收的THARQProcessInfo生成接收数据包信息*/
class Rxbuffer
{
public:
    string  sBlockType;                  //HARQ进程待传输包的类型，仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    int  iSEIndex;	                     //该包的SE序号，对应mcs等级
    int  CodeLength;                     //信道上实际传输的包的大小
    int  iTransNumber;                   //发送次数，首次发送的传输次数为0，重传时,标识分别为1,2,…MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    int  iIsRetransblock;                //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包
    vector<int>   vOptionalAttributes;//每个用户根据需要设置可选属性,属性种类数可扩展。不同的属性对应不同的数据包类型，1，2，3

    //这三个属性后可以添加可选属性，需要在对象中说明属性的具体含义）
    Rxbuffer(){ };
    Rxbuffer(int  iSEIndex, int CodeLength, int iTransNumber, int  iIsRetransblock);
    Rxbuffer(const Rxbuffer& rxbuffer);
    void operator=(Rxbuffer rxbuffer);

    // Rxbuffer& operator=(const Rxbuffer& rxbuffer);
    ~Rxbuffer(){};

    void initialRxbuffer(int  iSEIndex, int CodeLength, int iTransNumber, int  iIsRetransblock);
//    void ReceiveSuccess();
//    void ReceiveFail();
    void clear();
};

class TTxBuffer
{
public:
    TTxBuffer();
    ~TTxBuffer();
    static  TTxBuffer * Create();//创建函数
    void initial(int _RxID);

    //TBlockInfo blockInfo[ARQNUM];	//一个ARQ过程待发送或已发送的数据包，只用到modulateType,codeRateType和transNumber变量
    map<int, TBlockInfo*> mapTBlockInfo; //ARQNUM,TBlockInfo
    int NACKTime[ARQNUM];			//记录收到NACK的时刻,只有当收到正确ACK确认后才将其清空
    int LatestNACKTime[ARQNUM];
    int RBIdx[ARQNUM][SUBBNUM];     //用于发送重传包时确定发送的RB位置，只用到前面的allocateRBNum个
    int allocateRBNum[ARQNUM];	    //某ARQ过程占用的RB数
    int isWaitingACKFlag[ARQNUM];	//记录每个ARQ过程的ACK等待状态;1:等待ACK;0:等待发送;-1:空闲
    int ARQID[ARQNUM];
    int isRetransUser;// -1表示该用户不是重传用户，1表示该用户是重传用户

    int RxID; //接收机ID
    int layerNum;
};

/****************************根据2006 LTE DL存档代码编写****************************/
class blockInfo;
class timeStruct;

//发送端ARQ过程缓冲区，5表示每条链路有5个ARQ过程，在本程序中各ARQ过程采用"停等"的机制
class ARQ_processes_Tx_buffers
{
public:
    ARQ_processes_Tx_buffers();
    ~ARQ_processes_Tx_buffers();
    static  ARQ_processes_Tx_buffers * Create();//创建函数
    void initial(int _RxID);

    int RxID; //接收机ID
    int ARQ_num;
    map<int, blockInfo*> map_arq_num_to_block_info;//BLOCK_INFO block_info[ARQ_NUM]
    map<int, int> map_arq_num_to_Transmited_Indicator; //表示当前传输块的状态："1"表示已经被发出，正在等待响应；"0"表示等待调度的重传包；"-1"表示buffer为空。
    int Current_Process_id; //当前时刻传输的ARQ process号码【0，4】
};

//数据块信息的封装格式
class blockInfo
{
public:
    blockInfo();
    virtual ~blockInfo();
    static  blockInfo * Create();//创建函数
    void initial();

    int L2_index;//层2块索引
    int transport_format;//传输格式
    int transport_block_size;// 发送端已经发出但还没有收到"正确确认"的数据包的传输块大小；
    int number_of_transmission; // 元素i表示相应传输块的发送次数
    //每个数据包的传输时间
    timeStruct* t_packet_ptr;
    timeStruct* in_quene_delay_ptr;//进入高优先级队列的延时结构体
};

//统计时间的结构体
class timeStruct
{
public:
    timeStruct();
    virtual ~timeStruct();
    static  timeStruct * Create();//创建函数
    void initial();

    int time_length;//时间长度
    int time_start;//起始时间
    int time_end;//截至时间
};

//系统中每个用户等待重传的高优先级队列
class high_priority_sequence
{
public:
    high_priority_sequence();
    ~high_priority_sequence();
    static  high_priority_sequence * Create();//创建函数
    void initial(int _RxID);

    map<int, blockInfo*> map_block_info_on_queue;//队列长度为4
    int Queue_Head;
    int Queue_Tail;
    int RxID; //接收机ID
};

class statistic_variable
{
public:
    statistic_variable();
    ~statistic_variable();
    static  statistic_variable * Create();//创建函数
    void initial(int _RxID);

    int RxID; //接收机ID

    //每个文件的传输时间,此时的time_length表示用户所有文件的传输时间之和
    timeStruct* t_file_ptr;
    //每个用户的丢包数
    int packet_drop;
    //每个用户正确接收的包数
    int right_packet;
    //每个用户正确接收的bit数
    int right_bit_num;
    //每个用户错误接收的包数
    int error_packet;
    //平均包呼叫吞吐量(对每个用户而言)  一个包呼叫过程即一个文件传输过程
    double average_packet_call_throught;
    //每个用户误帧率
    double FER_session;
    //每个用户的丢包率
    double FER_residual;
    //每个用户的包时延
    double packet_delay;
};

//ARQ过程的ACK消息
class message_arq_ack
{
public:
    message_arq_ack();
    ~message_arq_ack();
    static  message_arq_ack * Create();//创建函数
    void initial();

    int ARQ_ACK;//ARQ过程的ACK指示，初始化时设为0，"1"表示ACK，-1表示NACK，"0"表示无内容。
    int ARQ_process_id; //表示上述的ARQ 反馈信息对应的ARQ process ID。
};


#endif //HETSNETS_HARQACKTREATMENT_H
