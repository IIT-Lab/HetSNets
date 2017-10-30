//
// Created by lee on 17-8-1.
//

#ifndef HETSNETS_GLOBAL_H
#define HETSNETS_GLOBAL_H

#define PI                                  3.1415926
#define ARQNUM								5			            //ARQ过程数
#define SUBBNUM								20						//subband总数//for TDD

#define BS_HEIGHT                           10
#define UE_HEIGHT	                        1.5
#define ANTENNAGAIN                         17

#define FI_TILT      12
#define  FI_3DB			15

#define HIGH_QUENE_DELAY  10 //进入高优先级队列需要等待10个延时
#define HIGH_QUENE_LENGTH 4+HIGH_QUENE_DELAY//高优先级队列的长度

#endif //HETSNETS_GLOBAL_H
