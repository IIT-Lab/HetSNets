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

#include"Traffic.h"
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

//发送端ARQ过程缓冲区，5表示每条链路有5个ARQ过程，在本程序中各ARQ过程采用"停等"的机制
class ARQ_processes_Tx_buffers
{
    ARQ_processes_Tx_buffers();
    ~ARQ_processes_Tx_buffers();
    static  ARQ_processes_Tx_buffers * Create();//创建函数
    void initial(int _RxID);

    int RxID; //接收机ID
    int ARQ_num;
    block_info block_info;
    int Transmited_Indicator; //元素i表示当前传输块的状态："1"表示已经被发出，正在等待响应；"0"表示等待调度的重传包；"-1"表示buffer为空。
    int Current_Process_id; //当前时刻传输的ARQ process号码【0，4】
};

//数据块信息的封装格式
struct block_info
{
    int L2_index;//层2块索引
    int transport_format;//传输格式
    int transport_block_size;// 发送端已经发出但还没有收到"正确确认"的数据包的传输块大小；
    int number_of_transmission; // 元素i表示相应传输块的发送次数
    //每个数据包的传输时间
    time_struct t_packet;
    time_struct in_quene_delay;//进入高优先级队列的延时结构体
};

//统计时间的结构体
struct time_struct
{
    int time_length;//时间长度
    int time_start;//起始时间
    int time_end;//截至时间
};



#endif //HETSNETS_HARQACKTREATMENT_H
