//
// Created by lee on 17-8-28.
//

#ifndef HETSNETS_IMTACHANNELMODEL_H
#define HETSNETS_IMTACHANNELMODEL_H

#include <vector>
#include <string>

#include "channel.h"
#include "Interface.h"

using namespace std;

namespace IMTA
{
    //大尺度衰落
    double vPathLossUMALos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc);
    double vPathLossUMANlos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc);
    double vPathLossUMILos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc = 2.0);
    double vPathLossUMINlos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc = 2.0);
    double vPathLossInHLos(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc = 2.0);

    double vPathLoss(double _x1, double _y1, double _x2, double _y2, double &STDDeviation, double fc); //根据算法论文

//    string losOrNlosSelect(string _sScenario,Interface* _TXPtr, double _d, map<int, string>& _mLosOrNlos);
    void losOrNlosSelect(mobileList& _mobileList, string _TxType, string _Scenario, double _x1, double _y1, double _x2, double _y2);
    void anatennaGain(angleList& _angleList, double _x1, double _y1, double _x2, double _y2, double _RxAntennaAngle, int _SectorID);
    double antenna(double in_angle , double v_angle);

    double PenetrationLoss(string _Scenario);

    void distance_correlated_N(double inter_side_distance,double correlation_distance,int coordinate_number,
                               double *coordinate_x,double *coordinate_y,double *LSP);

    void SFInitial();

    bool losOrNlosSelectUMI(double _x1, double _y1, double _x2, double _y2);//UMI场景视距非视距选择

    //小尺度计算
    //void LSP_Generation(string _Scenario, );
}
#endif //HETSNETS_IMTACHANNELMODEL_H
