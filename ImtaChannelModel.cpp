//
// Created by lee on 17-8-28.
//
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <string>
#include <math.h>

#include "dBLibrary.h"//这种封装方式非常危险，以后设立专门的dB运算对象
#include "ImtaChannelModel.h"
#include "Mymath.h"
#include "SystemDriveBus.h"

using namespace std;
using namespace dB;

namespace IMTA
{
/*************************************************************************************************/
    double DS_Correlationdistance_LOS[4]={8,7,30,50};
    double DS_Correlationdistance_NLOS[4]={5,10,40,36};
    double ASD_Correlationdistance_LOS[4]={7,8,18,25};
    double ASD_Correlationdistance_NLOS[4]={3,10,50,30};
    double ASA_Correlationdistance_LOS[4]={5,8,15,35};
    double ASA_Correlationdistance_NLOS[4]={3,9,50,40};
    double SF_Correlationdistance_LOS[4]={10,10,37,37};
    double SF_Correlationdistance_NLOS[4]={6,13,50,120};
    double K_Correlationdistance_LOS[4]={4,15,12,40};
    double DS_Miu_LOS[4]={-7.70,-7.19,-7.03,-7.49};
    double DS_Miu_NLOS[4]={-7.41,-6.89,-6.44,-7.43};
    double DS_Dieta_LOS[4]={0.18,0.40,0.66,0.55};
    double DS_Dieta_NLOS[4]={0.14,0.54,0.39,0.48};
    double ASD_Miu_LOS[4]={1.60,1.20,1.15,0.90};
    double ASD_Miu_NLOS[4]={1.62,1.41,1.41,0.95};
    double ASD_Dieta_LOS[4]={0.18,0.43,0.28,0.38};
    double ASD_Dieta_NLOS[4]={0.25,0.17,0.28,0.45};
    double ASA_Miu_LOS[4]={1.62,1.75,1.81,1.52};
    double ASA_Miu_NLOS[4]={1.77,1.84,1.87,1.52};
    double ASA_Dieta_LOS[4]={0.22,0.19,0.20,0.24};
    double ASA_Dieta_NLOS[4]={0.16,0.15,0.11,0.13};
    double SF_Dieta_LOS[4]={3,3,4,4};
    double SF_Dieta_NLOS[4]={4,4,6,8};
    double K_Miu_LOS[4]={7,9,9,7};
    double K_Dieta_LOS[4]={4,5,3.5,4};

    double DS_Correlationdistance_UMIindoor=10;
    double ASD_Correlationdistance_UMIindoor=11;
    double ASA_Correlationdistance_UMIindoor=17;
    double SF_Correlationdistance_UMIindoor=7;
    double DS_Dieta_UMIindoor=0.32;
    double DS_Miu_UMIindoor=-6.62;
    double ASD_Dieta_UMIindoor=0.42;
    double ASD_Miu_UMIindoor=1.25;
    double ASA_Dieta_UMIindoor=0.16;
    double ASA_Miu_UMIindoor=1.76;
    double SF_Dieta_UMIindoor=7;
    double LSP_cross_correlations_UMIindoor[4][4]=
            {

                    {1.0000   ,      0 ,        0      ,   0},
                    {-0.5000   , 0.8660  ,       0   ,      0},
                    {0.2000  ,  0.5774 ,   0.7916    ,     0},
                    {    0  ,  0.4619  , -0.3369 ,   0.8205},
            };
///////////////////////////////////////////////////////////////

    double LSP_cross_correlations_InH_LOS_rms[5][5]=
            {
                    {  1.0000  ,       0   ,      0 ,        0    ,     0},
                    {-0.8000   , 0.6000    ,     0  ,       0     ,    0},
                    { -0.4000  ,  0.4667   , 0.7888 ,        0     ,    0},
                    { -0.5000  ,  0.6667  , -0.1409 ,   0.5345    ,     0},
                    { 0.5000   ,-0.1667   , 0.3521  ,  0.7684   , 0.0884},
            };
    double LSP_cross_correlations_InH_NLOS_rms[4][4]=
            {
                    {1.0000      ,   0   ,      0    ,     0},
                    {-0.5000  ,  0.8660   ,      0   ,      0},
                    {    0   , 0.4619   , 0.8869     ,    0},
                    {-0.4000 ,  -0.2309 ,   0.1203   , 0.8788}
            };
    double LSP_cross_correlations_UMi_LOS_rms[5][5]=
            {
                    {1.0000     ,    0     ,    0       ,  0     ,    0},
                    {-0.4000   , 0.9165    ,     0     ,    0    ,     0},
                    {-0.5000   , 0.3273 ,   0.8018    ,     0    ,     0},
                    {-0.4000   , 0.6983  , -0.0356   , 0.5925    ,     0},
                    {0.5000   ,-0.5455   , 0.2851   , 0.4913  ,  0.3602}
            };
    double LSP_cross_correlations_UMi_NLOS_rms[4][4]=
            {
                    {1.0000     ,    0   ,      0    ,     0},
                    {-0.7000  ,  0.7141  ,       0    ,     0},
                    {    0     ,    0   , 1.0000      ,   0},
                    {-0.4000   , 0.1680  ,       0  ,  0.9010}
            };
    double LSP_cross_correlations_UMa_LOS_rms[5][5]=
            {
                    {1.0000     ,    0     ,    0      ,   0    ,     0},
                    {-0.4000   , 0.9165    ,     0     ,    0   ,      0},
                    {-0.5000  ,  0.2182  ,  0.8381     ,    0   ,      0},
                    {-0.5000  ,  0.6547  , -0.4688  ,  0.3189    ,     0},
                    {    0   ,-0.4364   ,0.1136   , 0.4358  ,  0.7789}
            };
    double LSP_cross_correlations_UMa_NLOS_rms[4][4]=
            {
                    {1.0000       ,  0    ,     0    ,     0},
                    {-0.4000   , 0.9165  ,       0   ,      0},
                    {-0.6000   , 0.1746  ,  0.7807   ,    0},
                    {    0   , 0.6547   , 0.3660   , 0.6614},
            };
    double LSP_cross_correlations_RMa_LOS_rms[5][5]=
            {
                    {1.0000    ,     0     ,    0   ,      0  ,       0},
                    {-0.5000   , 0.8660    ,     0  ,       0 ,        0},
                    {    0     ,    0   , 1.0000  ,      0   ,      0},
                    {    0     ,    0   ,      0  ,  1.0000  ,       0},
                    {    0     ,    0   ,      0   ,      0  ,  1.0000},
            };
    double LSP_cross_correlations_RMa_NLOS_rms[4][4]=
            {
                    {1.0000    ,     0      ,   0      ,   0},
                    {-0.5000   , 0.8660     ,    0     ,    0},
                    {0.6000  , -0.1155   , 0.7916    ,     0},
                    {    0   ,      0    ,     0  ,  1.0000}
            };
/*************************************************************************************************/

    double vPathLossUMALos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc)
    {
        //_x1,_y1,_x2,_y2这两个坐标是wrapround扭曲变换后的坐标
        const double dMinDistance = 3.0;//PPT上写的最小距离是3米
        double hBS, hMS, hBSEFF, hMSEFF, dBP1, d, PL = 0;
        hBS = 25;
        hMS = 1.5;
        hBSEFF = hBS - 1;
        hMSEFF = hMS - 1;
        dBP1 = 4 * hBSEFF*hMSEFF*fc * 10 / 3;//p31
        d = max(dMinDistance, sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)));
        if (d<dBP1)
        {
            PL = 22*log10(d) + 28 + 20 * log10(fc);
        }
        else if (d>dBP1 && d<5000)
        {
            PL = 40 * log10(d) + 7.8 - 18 * log10(hBSEFF) - 18 * log10(hMSEFF) + 2 * log10(fc);
        }
        STDDeviation = 3;
        //cout << PL << endl;
        return PL;
    }

    double vPathLossUMANlos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc)
    {
        const double dMinDistance = 3.0;//PPT上写的最小距离是3米
        double hBS, hMS, W, h, d, PL = 0;
        hBS = 25;
        hMS = 1.5;
        W = 20;
        h = 20;
        d = max(dMinDistance, sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)));

        PL = 161.04-7.1*log10(W)+7.5*log10(h) - (24.37 - 3.7 * pow((h / hBS), 2)) * log10(hBS) + (43.42 - 3.1*log10(hBS))*(log10(d) - 3) + 20*log10(fc) - (3.2*pow((log10(11.75*hMS)),2) - 4.97);

        STDDeviation = 3;
        //cout << PL << endl;
        return PL;
    }

    double vPathLossUMILos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc)
    {
        //_x1,_y1,_x2,_y2这两个坐标是wrapround扭曲变换后的坐标
        const double dMinDistance = 3.0;//PPT上写的最小距离是3米
        double hBS, hMS, hBSEFF, hMSEFF, dBP1, d, PL = 0;
        hBS = 10;
        hMS = 1.5;
        hBSEFF = hBS - 1;
        hMSEFF = hMS - 1;
        dBP1 = 4 * hBSEFF*hMSEFF*fc * 10 / 3;//p31
        d = max(dMinDistance, sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)));
        if (d<dBP1)
        {
            PL = 22.7*log10(d) + 27.0 + 20 * log10(fc);
        }
        else if (d>dBP1 && d<5000)
        {
            PL = 40 * log10(d) + 7.56 - 17.3 * log10(hBSEFF) - 17.3 * log10(hMSEFF) + 2.7 * log10(fc);
        }
        STDDeviation = 3;
        //cout << PL << endl;
        return PL;
    }
    double vPathLossUMINlos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc)
    {
        //_x1,_y1,_x2,_y2这两个坐标是wrapround扭曲变换后的坐标
        double  w, d1, d2, n1, n2, PL, PLd1d2, PLd2d1;
        const double dMinDistance = 1.0;
        w = 20;
        d1 = max(dMinDistance, abs(_x1 - _x2));
        d2 = max(dMinDistance, abs(_y1 - _y2));
        if (d1<5000 && d2 < 2000)
        {
            n1 = max(2.8 - 0.0024*d1, 1.84);
            n2 = max(2.8 - 0.0024*d2, 1.84);
            PLd1d2 = vPathLossUMILos(_x1, 0, _x2, 0, STDDeviation, 2) + 17.3 - 12.5*n1 + 10 * n1*log10(d2) + 3 * log10(fc);
            PLd2d1 = vPathLossUMILos(0, _y1, 0, _y2, STDDeviation, 2) + 17.3 - 12.5*n2 + 10 * n2*log10(d1) + 3 * log10(fc);
            PL = min(PLd1d2, PLd2d1);
            STDDeviation = 4;
            //cout << PL << endl;
            return PL;
        }
        return 0;
    }

    double vPathLossInHLos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc)
    /**************************************************************************************************
    函数名称：vPathLossInHLos
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    **************************************************************************************************/
    {
        //_x1,_y1,_x2,_y2这两个坐标是wrapround扭曲变换后的坐标
        const double dMinDistance = 3.0;//PPT上写的最小距离是3米
        double hBS, hMS, hBSEFF, hMSEFF, dBP1, d, PL = 0;

        d = max(dMinDistance, sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)));

        PL = 16.9*log10(d) + 32.8 + 20 * log10(fc);

        STDDeviation = 3;
        //cout << PL << endl;
        return PL;
    }

    double vPathLoss(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc) {
        STDDeviation = 8;
        double hBS, d, PL = 0;
        hBS = 25;
        d = sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2));

        PL = 40 * (1 - 4 / 1000 * hBS) * log10(d / 1000) - 18 * log10(hBS) + 21 * log10(fc) + 80;

        return PL;
    }

//    string losOrNlosSelect(string _sScenario, Interface* _TXPtr, double _d, map<int, string>& _mLosOrNlos)
//    {
//        double tempRand = (double)rand() / RAND_MAX;
//        double p;
//        int LOS_NLOS = 0;
//        string losOrNlos = "NLOS";
//        if (_sScenario == "InH")
//        {
//            if (_d <= 18)
//            {
//                LOS_NLOS = 1;
//                losOrNlos = "LOS";
//            }
//            else if (_d > 18 && _d <= 37)
//            {
//                p = exp(-(_d - 18) / 27);
//                if (p > tempRand)
//                {
//                    LOS_NLOS = 1;
//                    losOrNlos = "LOS";
//                }
//            }
//            else
//            {
//                p = 0.5;
//                if (p > tempRand)
//                {
//                    LOS_NLOS = 1;
//                    losOrNlos = "LOS";
//                }
//            }
//        }
//        else if (_sScenario == "UMA")
//        {
//            p = min(18 / _d, 1.0) * (1 - exp(-_d / 63)) + exp(-_d / 63);
//            if (p > tempRand)
//            {
//                LOS_NLOS = 1;
//                losOrNlos = "LOS";
//            }
//        }
//        _mLosOrNlos.insert(pair<int, string>(_TXPtr->iGetID(), losOrNlos));
//        return losOrNlos;
//    }

    void losOrNlosSelect(mobileList& _mobileList, string _TxType, string _Scenario, double _x1, double _y1, double _x2, double _y2)
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        double r, segma, XPR, d, p;
        int numofcluster, LOS_NLOS;
        double tempRand = (double)rand() / RAND_MAX;
        d = sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)); //发射机到接收机的距离

        //发射机是Macro
        if (_TxType == "class MacroCell *")
        {
            if (_Scenario == "UMA")
            {
                p = min(18/d, 1) * (1 - exp(-d/63)) + exp(-d/63);
                if(p > tempRand)
                {
                    LOS_NLOS = 1;
                    numofcluster = 12;
                    r = 2.5;
                    segma = 3;
                    XPR = sqrt(1 / dB2Linear(8.0));
                }
                else
                {
                    LOS_NLOS = 0;
                    numofcluster = 20;
                    r = 2.4;
                    segma = 3;
                    XPR = sqrt(1 / dB2Linear(7.0));
                }
            }

            else if(_Scenario == "UMI")
            {
                if(d <= 10)
                {
                    LOS_NLOS = 1;
                    numofcluster = 15;
                    r = 2.4;
                    segma = 3;
                    XPR = sqrt(1 / dB2Linear(8.0));
                }
                else
                {
                    p = exp(-(d-10)/200);
                    if(p > tempRand)
                    {
                        LOS_NLOS = 1;
                        numofcluster = 15;
                        r = 2.4;
                        segma = 3;
                        XPR = sqrt(1/dB2Linear(8.0));
                    }
                    else
                    {
                        LOS_NLOS = 0;
                        numofcluster = 14;
                        r = 1.5;
                        segma = 3;
                        XPR = sqrt(1/dB2Linear(4.0));
                    }
                }
            }
        }

        _mobileList.setLOSSelect(LOS_NLOS);
        _mobileList.setNumofcluster(numofcluster);
        _mobileList.setR(r);
        _mobileList.setSegma(segma);
        _mobileList.setXPR(XPR);
    }

    void anatennaGain(angleList& _angleList, double _x1, double _y1, double _x2, double _y2, double _RxAntennaAngle, int _SectorID)
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        double angleInPI,angleTemp;
        double UE_CELL_d;
        double angle_cell_UE;
        double AoD_LOS;
        double AoA_LOS;
        double v_angle;
        double antennaGainTable;

        angle_cell_UE = atan2((_y1 - _y2), (_x1 - _x2));
        if (angle_cell_UE < 0)
            angleTemp = fabs(2 * PI + angle_cell_UE - _RxAntennaAngle);
        else
            angleTemp=fabs(angle_cell_UE - _RxAntennaAngle);
        AoA_LOS = angleTemp;

        UE_CELL_d = sqrt(pow((_x1 - _x2), 2) + pow((_y1 - _y2), 2)); //发射机到接收机的距离
        v_angle = atan((BS_HEIGHT - UE_HEIGHT) / UE_CELL_d); //待修改，基站高度和用户高度可配置

        angleInPI = angleCalc((_x1 - _x2), (_y1 - _y2));
        angleTemp = angleInPI - PI / 6 - 2 * PI / 3 * _SectorID;//对边
        if (angleTemp > PI)
        {
            angleTemp = 2 * PI - angleTemp;
        }
        else if (angleTemp < -PI)
        {
            angleTemp = 2 * PI + angleTemp;
        }
        AoD_LOS = angleTemp;

        antennaGainTable = ANTENNAGAIN - antenna(AoD_LOS, v_angle);

        _angleList.setAoA_LOS(AoA_LOS);
        _angleList.setAoD_LOS(AoD_LOS);
        _angleList.setV_angle(v_angle);
        _angleList.setAntennaGainTable(antennaGainTable);
    }

    double antenna(double in_angle , double v_angle)
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        double temp_angle_in_degree, h_gain, v_gain, gain;
        if (in_angle<-PI)
        {
            in_angle=2*PI+in_angle;
        }
        else if (in_angle>PI)
        {
            in_angle=2*PI-in_angle;
        }
        temp_angle_in_degree = 180 * in_angle / PI;
        h_gain = 12 * pow(temp_angle_in_degree/70,2);
        if(h_gain > 20)
            h_gain = 20;
        temp_angle_in_degree = 180 * v_angle / PI;
        v_gain = 12 * pow((temp_angle_in_degree-FI_TILT)/FI_3DB,2);
        if(v_gain > 20)
            v_gain = 20;
        gain = v_gain+h_gain;
        if(gain > 20)
            gain = 20;
        return gain;
    }

    double PenetrationLoss(string _Scenario)
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        return 20;
    }

    void distance_correlated_N(double inter_side_distance,double correlation_distance,int coordinate_number,
                               double *coordinate_x,double *coordinate_y,double *LSP)
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        double max_cell_x;
        double max_cell_y;//?????????
        max_cell_x=4/sqrt(3)*inter_side_distance;
        max_cell_y=5.0/2*inter_side_distance;

        int max_square_x;
        int max_square_y;
        max_square_x=(int)(max_cell_x/correlation_distance)+2;
        max_square_y=(int)((max_cell_y-correlation_distance/2)/correlation_distance)+3;

        //??????????N(0,1)??????
        int i,j;
        //double random_num[2*max_square_x+1][2*max_square_y];
        double *random_num=new double[2000*2000];//0408
        //double random_num[2000][2000];
        for (i=0;i<2*max_square_x+1;i++)
        {
            for (j=0;j<2*max_square_y;j++)
            {
                random_num[i*(2*max_square_x+1)+j]=randn();//0408
            }
        }

        //????????????

        int coordinate_no;//??????
        int x[4];//?????4????x???,...,-2,-1,0,1,2,...
        int y[4];//?????4????y???,...,-2,-1,1,2,...
        double x_pos,y_pos;

        for (coordinate_no=0;coordinate_no<coordinate_number;coordinate_no++)
        {
            //??x???x_pos
            if (coordinate_x[coordinate_no]>=0)
            {
                //??????????
                x[0]=(int)(coordinate_x[coordinate_no]/correlation_distance);
                x[1]=(int)(coordinate_x[coordinate_no]/correlation_distance)+1;
            }
            else
            {
                x[1]=(int)(coordinate_x[coordinate_no]/correlation_distance);
                x[0]=(int)(coordinate_x[coordinate_no]/correlation_distance)-1;
            }
            x[2]=x[1];
            x[3]=x[0];
            x_pos=coordinate_x[coordinate_no]-x[0]*correlation_distance;

            //??y???y_pos
            if ((coordinate_y[coordinate_no]<correlation_distance/2 && coordinate_y[coordinate_no]>0)||
                (coordinate_y[coordinate_no]>-1*correlation_distance/2 && coordinate_y[coordinate_no]<0))
            {
                y[0]=1;
                y[1]=1;
                y[2]=-1;
                y[3]=-1;
                y_pos=coordinate_y[coordinate_no]+correlation_distance/2;
            }
            else if (coordinate_y[coordinate_no]>0)
            {
                y[3]=(int)((coordinate_y[coordinate_no]-correlation_distance/2)/correlation_distance)+1;
                y[2]=y[3];
                y[0]=y[3]+1;
                y[1]=y[0];
                y_pos=coordinate_y[coordinate_no]-(y[3]-1)*correlation_distance-correlation_distance/2;
            }
            else
            {
                y[0]=(int)((coordinate_y[coordinate_no]+correlation_distance/2)/correlation_distance-1);
                y[1]=y[0];
                y[2]=y[0]-1;
                y[3]=y[2];
                y_pos=coordinate_y[coordinate_no]-y[3]*correlation_distance-correlation_distance/2;
            }

            int sx[4];//???????????,???????,0,1,2,...
            int sy[4];//???????????,???????,0,1,2,...
            double s[4];//???[sx[4],sy[4]]?????????
            for (i=0;i<4;i++)
            {
                sx[i]=x[i]+max_square_x;
                if (y[i]<0)
                {
                    sy[i]=y[i]+max_square_y;
                }
                else
                {
                    sy[i]=y[i]+max_square_y-1;
                }

                s[i]=random_num[sx[i]*(2*max_square_x+1)+sy[i]];//0408
            }

            LSP[coordinate_no]=sqrt(1-x_pos/correlation_distance)*(s[0]*sqrt(y_pos/correlation_distance)
                                                                   +s[3]*sqrt(1-y_pos/correlation_distance))+(s[1]*sqrt(y_pos/correlation_distance)+
                                                                                                              s[2]*sqrt(1-y_pos/correlation_distance))*sqrt(x_pos/correlation_distance);

        }
        delete []random_num;
    }

    void SFInitial()
    /************************************************************************************************
    函数名称：
    主要功能：
    输入参数：
    输出参数：
    其他信息：
    *************************************************************************************************/
    {
        //测试UMI
        int scenario = 1;

        double x, y, inter_side_distance,max_cell_x,max_cell_y;
        int x_num, y_num, i,j,k;
        double *SF_wo_std;
        double *coordinate_X, *coordinate_Y;

        inter_side_distance = sqrt(3) * SystemDriveBus::ModeID2Par.find(1)->second.get_radius();
        max_cell_x = 4.0 / sqrt(3) * inter_side_distance;
        max_cell_y = 5.0 / 2.0 * inter_side_distance;

        //测试UMI
        x_num = int(2*max_cell_x)+1;
        y_num = int(2*max_cell_y)+1;
        coordinate_X = new double[x_num*y_num];
        coordinate_Y = new double[x_num*y_num];
        SF_wo_std = new double[x_num*y_num];
        x = - max_cell_x;
        i=0;
        while(x<=max_cell_x)
        {
            y = - max_cell_y;
            while(y<=max_cell_y)
            {
                coordinate_X[i]=x;
                coordinate_Y[i]=y;
                y++;
                i++;
            }
            x++;
        }

        distance_correlated_N(inter_side_distance,SF_Correlationdistance_LOS[scenario],x_num*y_num,coordinate_X,coordinate_Y,SF_wo_std);
    }
}


