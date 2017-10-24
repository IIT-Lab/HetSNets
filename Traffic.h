//
// Created by lee on 17-8-31.
//

#ifndef HETSNETS_TRAFFIC_H
#define HETSNETS_TRAFFIC_H

#include <vector>

using namespace std;

namespace Traffic
{
    //用户数据包信息
    class  TrafficPacket      //业务数据包
    {
    public:
        TrafficPacket();                                                //显式定义默认无参构造函数
        TrafficPacket(const TrafficPacket &tblockinfo);                           //复制构造函数
        TrafficPacket& operator=(const TrafficPacket &tblockinfo);                       //等号运算符重载
        ~TrafficPacket();

        TrafficPacket(int _iPacketType, int  _iPacketNumber, double  _dBitSizeRemain, double  dBitSizeSentAlready,
                      double  dArriveTime, double dCompleteTime, vector<int>& _vOptionalAttributes);                                          //有基本属性初始化，无可选属性

        void Initial(int _iPacketType, int  _iPacketNumber, double  _dBitSizeRemain, double  dBitSizeSentAlready,
                     double  dArriveTime, double dCompleteTime, vector<int>& _vOptionalAttributes);

        void vTransmit(double size);
        void vTransmitSuccess(double size);
        void vTransmitFail(double size);

//        void SimpleInitial(int vectorSize);                                  //无基本属性、无可选属性
//        void clear();

        int iPacketType;           //数据包类型，1：表示语音数据包类型，2：数据，3：视频
        int  iPacketNumber;        //数据包的标号；

        double  dBitSizeRemain;          //表明本数据包传输后剩余的数据量大小
        double  dBitSizeSentAlready;     //表明本数据包已经传输出去的数据量大小
        double  dArriveTime;
        double dCompleteTime;

        vector<int>   vOptionalAttributes;//每个用户根据需要设置可选属性,属性种类数可扩展。不同的属性对应不同的数据包类型，1，2，3
        //（属性与位置对应：int  blockIndexL3 //数据包的L3号;int  blockIndexL2;int  CodeLength;
        //这三个属性后可以添加可选属性，需要在对象中说明属性的具体含义
//        int blockIndexL3;						 //数据包的L3号,FullBuffer()中写入
//        int blockIndexL2;					     //数据包的L2号,FullBuffer()中写入
//        int CodeLength;
    };
}

class TBlockInfo
{
public:
    TBlockInfo();
    ~TBlockInfo();
    static  TBlockInfo * Create();//创建函数
    void initial(int _ARQNum);

    int blockIndexL3;						 //数据包的L3号,FullBuffer()中写入
    int blockIndexL2;					     //数据包的L2号,FullBuffer()中写入
    int SEIndex;							// SE对应的序号，即第几个SE；
    int CodeLength;
    int transNumber;		//首次发送,传输次数为0;重传时,标识分别为1,2,……MAXRETRANSTIME。最多发送MAXRETRANSTIME+1次
    int blockType;							 //仅在新包时写入。1：表示单流数据包；20：双流的0层；21：双流的1层
    int isRetransblock;            //1：表示为重传包，0：表示为非重传包，-1：表示为既无重传包也无新包

    int ARQNum;
};

#endif //HETSNETS_TRAFFIC_H
