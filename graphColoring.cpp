//
// Created by lee on 17-12-8.
//


#include "graphColoring.h"
#include "Mymath.h"
#include "MySQLManager.h"
#include "SystemDriveBus.h"

/********************************node类*********************************/

node::node(int _ID) {
    ID = _ID;
    degree = -1;
    color = -1;
    order = -1;
}

node::~node() {

}

void node::initial(vector<vector<int>> _incidenceMatrix) {
    //初始化vecEdgeID
    vector<int> vecEdge = _incidenceMatrix[ID];
    for (int edgeID = 0; edgeID < vecEdge.size(); edgeID++) {
        if (vecEdge[edgeID]) {
            vecEdgeID.push_back(edgeID);
        }
    }

    //初始化mapEdgeID2NodeID
    for (int edgeID : vecEdgeID) {
        vector<int> tempvecAdjacencyNodeID; //1条边对应的所有相邻节点ID的集合
        for (int AdjacencyNodeID = 0; AdjacencyNodeID < _incidenceMatrix.size(); ++AdjacencyNodeID) {
            if (AdjacencyNodeID != ID && _incidenceMatrix[AdjacencyNodeID][edgeID]) {
                tempvecAdjacencyNodeID.push_back(AdjacencyNodeID);
            }
        }
        mapEdgeID2AdjacencyNodeID.insert(pair<int, vector<int>>(edgeID, tempvecAdjacencyNodeID));
    }

    //初始化vecAdjacencyNodeID
    for (auto tempvecAdjacencyNodeID : mapEdgeID2AdjacencyNodeID) {
        if (tempvecAdjacencyNodeID.second.size() == 1) {
            vecAdjacencyNodeID.push_back(tempvecAdjacencyNodeID.second[0]);
        }
    }

    //初始化degree
    degree = (int)vecEdgeID.size();
}

int node::getDegree() const {
    return degree;
}

void node::setOrder(int order) {
    node::order = order;
}

const map<int, vector<int>> &node::getMapEdgeID2AdjacencyNodeID() const {
    return mapEdgeID2AdjacencyNodeID;
}

void node::update(int _edgeID) {
    //更新vecEdgeID //更新mapEdgeID2AdjacencyNodeID
    vector<int> tempvecEdgeID = vecEdgeID; //节点相邻边的ID 从0开始
    map<int, vector<int>> tempmapEdgeID2AdjacencyNodeID = mapEdgeID2AdjacencyNodeID; //相邻边的ID对相邻节点ID的映射表
    vecEdgeID.clear();
    mapEdgeID2AdjacencyNodeID.clear();
    for (int i = 0; i < tempvecEdgeID.size(); ++i) {
        if (tempvecEdgeID[i] != _edgeID) {
            vecEdgeID.push_back(tempvecEdgeID[i]);
            mapEdgeID2AdjacencyNodeID.insert(pair<int, vector<int>>(tempvecEdgeID[i], tempmapEdgeID2AdjacencyNodeID.at(tempvecEdgeID[i])));
        }
    }

    //更新degree
    degree = (int)vecEdgeID.size();
}

int node::getID() const {
    return ID;
}

int node::getOrder() const {
    return order;
}

int node::getColor() const {
    return color;
}

void node::setVecCandidateColor(int _colorNum, map<int, node *> _mapNodeID2NodePtr) {
    for (int colorID = 0; colorID < _colorNum; ++colorID) {
        int pushColor = 1;
        for (auto temp : vecAdjacencyNodeID) {
            if (_mapNodeID2NodePtr.at(temp)->getColor() == colorID) pushColor = 0;
        }
        if (pushColor) vecCandidateColor.push_back(colorID);
    }
}

void node::setColor() {
    if (vecCandidateColor.size()) color = vecCandidateColor[0];
    cout << "节点" << ID + 1 << ": " << "颜色 " << color << endl;
}

/********************************功能函数*********************************/

bool comDegree(node *_node1, node *_node2) { //按度从高到低排序
    return _node1->getDegree() > _node2->getDegree();
}

bool comOrder(node *_node1, node *_node2) { //按着色顺序排序
    return _node1->getOrder() < _node2->getOrder();
}

bool comhyperDegree(hyperNode *_node1, hyperNode *_node2) { //按度从高到低排序
    return _node1->getDegree() > _node2->getDegree();
}

bool comhyperOrder(hyperNode *_node1, hyperNode *_node2) { //按着色顺序排序
    return _node1->getOrder() < _node2->getOrder();
}

void graghColoring(map<int, node *> _mapNodeID2NodePtr, int _colorNum) {
    int numOfNodes = (int)_mapNodeID2NodePtr.size();
    vector<node*> nodes;
    for (auto _temp : _mapNodeID2NodePtr) {
        nodes.push_back(_temp.second);
    }

    for (int i = 0; i < numOfNodes; ++i) {
        //排序
        sort(nodes.begin(), nodes.end(), comDegree); //按度从高到低排序
        node * tempNode = nodes[0]; //度最大的节点
        tempNode->setOrder(i); //设置着色序号

        //去除节点 去除边 更新子图
        map<int, vector<int>> mapBreakEdgeID2AdjacencyNodeID = tempNode->getMapEdgeID2AdjacencyNodeID();
        for (auto tempVec : mapBreakEdgeID2AdjacencyNodeID) {
            for (auto AdjacencyNodeID : tempVec.second) {
                _mapNodeID2NodePtr.at(AdjacencyNodeID)->update(tempVec.first);
            }
        }

        vector<node*> tempNodes = nodes;
        nodes.clear();
        for (int j = 0; j < tempNodes.size(); ++j) {
            if (tempNodes[j]->getID() != tempNode->getID()) {
                nodes.push_back(nodes[j]);
            }
        }
    }

    //按节点着色顺序给节点着色
    nodes.clear();
    for (auto _temp : _mapNodeID2NodePtr) {
        nodes.push_back(_temp.second);
    }

    sort(nodes.begin(), nodes.end(), comOrder); //按着色顺序排序
    for (int k = 0; k < nodes.size(); ++k) {
        nodes[k]->setVecCandidateColor(_colorNum, _mapNodeID2NodePtr);
        nodes[k]->setColor();
    }
}

void hypergraghColoring(map<int, hyperNode *> _mapNodeID2HyperNodePtr, int _colorNum) {
    int numOfNodes = (int)_mapNodeID2HyperNodePtr.size();
    vector<hyperNode*> nodes;
    for (auto _temp : _mapNodeID2HyperNodePtr) {
        nodes.push_back(_temp.second);
    }

    for (int i = 0; i < numOfNodes; ++i) {
        //排序
        sort(nodes.begin(), nodes.end(), comhyperDegree); //按度从高到低排序
        hyperNode *tempNode = nodes[0]; //度最大的节点
        tempNode->setOrder(i); //设置着色序号

        //去除节点 去除边 更新子图
        map<int, vector<int>> mapBreakEdgeID2AdjacencyNodeID = tempNode->getMapEdgeID2AdjacencyNodeID();
        for (auto tempVec : mapBreakEdgeID2AdjacencyNodeID) {
            for (auto AdjacencyNodeID : tempVec.second) {
                _mapNodeID2HyperNodePtr.at(AdjacencyNodeID)->update(tempVec.first);
            }
        }

        vector<hyperNode*> tempNodes = nodes;
        nodes.clear();
        for (int j = 0; j < tempNodes.size(); ++j) {
            if (tempNodes[j]->getID() != tempNode->getID()) {
                nodes.push_back(nodes[j]);
            }
        }
    }

    //按节点着色顺序给节点着色
    nodes.clear();
    for (auto _temp : _mapNodeID2HyperNodePtr) {
        nodes.push_back(_temp.second);
    }

    sort(nodes.begin(), nodes.end(), comhyperOrder); //按着色顺序排序
    for (int k = 0; k < nodes.size(); ++k) {
        nodes[k]->setVecCandidateColor(_colorNum, _mapNodeID2HyperNodePtr);
        nodes[k]->setColor();
    }
}

/********************************hyperNode类*********************************/

hyperNode::hyperNode(int _ID) {
    ID = _ID;
    degree = -1;
    color = -1;
    order = -1;
}

hyperNode::~hyperNode() {

}

void hyperNode::initial(vector<vector<int>> _incidenceMatrix) {
    //初始化vecEdgeID
    vector<int> vecEdge = _incidenceMatrix[ID];
    for (int edgeID = 0; edgeID < vecEdge.size(); edgeID++) {
        if (vecEdge[edgeID]) {
            vecEdgeID.push_back(edgeID);
        }
    }

    //初始化mapEdgeID2NodeID //初始化 vecNormalEdgeID vecHyperEdgeID mapHyperEdgeID2AdjNodeID
    for (int edgeID : vecEdgeID) {
        vector<int> tempvecAdjacencyNodeID; //1条边对应的所有相邻节点ID的集合
        for (int AdjacencyNodeID = 0; AdjacencyNodeID < _incidenceMatrix.size(); ++AdjacencyNodeID) {
            if (AdjacencyNodeID != ID && _incidenceMatrix[AdjacencyNodeID][edgeID]) {
                tempvecAdjacencyNodeID.push_back(AdjacencyNodeID);
            }
        }
        mapEdgeID2AdjacencyNodeID.insert(pair<int, vector<int>>(edgeID, tempvecAdjacencyNodeID));
        if (tempvecAdjacencyNodeID.size() > 1) {
            vecHyperEdgeID.push_back(edgeID);
            mapHyperEdgeID2AdjNodeID.insert(pair<int, vector<int>>(edgeID, tempvecAdjacencyNodeID));
        }
        else {
            vecNormalEdgeID.push_back(edgeID);
        }
    }

    //初始化vecNorAdjNodeID vecHyperAdjNodeID
    for (auto tempvecAdjacencyNodeID : mapEdgeID2AdjacencyNodeID) {
        if (tempvecAdjacencyNodeID.second.size() == 1) {
            vecNorAdjNodeID.push_back(tempvecAdjacencyNodeID.second[0]);
        }
        else {
            vecHyperAdjNodeID.push_back(tempvecAdjacencyNodeID.second);
        }
    }

    //初始化degree
    degree = (int)vecEdgeID.size();
}

void hyperNode::update(int _edgeID) {
    //更新vecEdgeID //更新mapEdgeID2AdjacencyNodeID
    vector<int> tempvecEdgeID = vecEdgeID; //节点相邻边的ID 从0开始
    map<int, vector<int>> tempmapEdgeID2AdjacencyNodeID = mapEdgeID2AdjacencyNodeID; //相邻边的ID对相邻节点ID的映射表
    vecEdgeID.clear();
    mapEdgeID2AdjacencyNodeID.clear();
    for (int i = 0; i < tempvecEdgeID.size(); ++i) {
        if (tempvecEdgeID[i] != _edgeID) {
            vecEdgeID.push_back(tempvecEdgeID[i]);
            mapEdgeID2AdjacencyNodeID.insert(pair<int, vector<int>>(tempvecEdgeID[i], tempmapEdgeID2AdjacencyNodeID.at(tempvecEdgeID[i])));
        }
    }

    //更新degree
    degree = (int)vecEdgeID.size();
}

int hyperNode::getID() const {
    return ID;
}

int hyperNode::getDegree() const {
    return degree;
}

int hyperNode::getOrder() const {
    return order;
}

void hyperNode::setOrder(int order) {
    hyperNode::order = order;
    cout << "设置节点" << ID + 1 << ": " << "着色序号： " << order << endl;
}

void hyperNode::setColor() {
    if (vecCandidateColor.size()) color = vecCandidateColor[0];
    cout << "节点" << ID + 1 << ": " << "颜色 " << color << endl;
}

const map<int, vector<int>> &hyperNode::getMapEdgeID2AdjacencyNodeID() const {
    return mapEdgeID2AdjacencyNodeID;
}

void hyperNode::setVecCandidateColor(int _colorNum, map<int, hyperNode *> _mapNodeID2HyperNodePtr) {
    for (int colorID = 0; colorID < _colorNum; ++colorID) {
        int pushNormalColor = 1; //该颜色是否加入候选颜色集的标志位
        for (auto temp : vecNorAdjNodeID) { //普通边相连的两个节点不能同色
            if (_mapNodeID2HyperNodePtr.at(temp)->getColor() == colorID) pushNormalColor = 0;
        }

        int pushHyperColor = 1;
        for (auto temp : mapHyperEdgeID2AdjNodeID) { //超边相连的多个节点不能同色
            pushHyperColor = 0;
            for (auto AdjNodeID :temp.second) {
                if (_mapNodeID2HyperNodePtr.at(AdjNodeID)->getColor() != colorID) {
                    pushHyperColor = 1;
                }
            }
        }

        if (pushNormalColor && pushHyperColor) vecCandidateColor.push_back(colorID);
    }
}

int hyperNode::getColor() const {
    return color;
}

/********************************macroUser类*********************************/

macroUser::macroUser(int _uID, double _power, double _D2DTxPower, double _channelGain, double _cellRadius) {
    uID = _uID;
    power = _power;
    D2DTxPower = _D2DTxPower;
    channelGain = _channelGain;
    cellRadius = _cellRadius;
    color = -1;
}

macroUser::~macroUser() {

}

void macroUser::SetSLARadius() {
    double targetSinr = 20; //dB
    targetSinr = pow(10, targetSinr / 10); //线性值
    double targetOutageProbability = 0.01; //目标中断概率
    double PLExponent = 4;

    //1个RB,12个连续的载波,12*15000=180000Hz
    double whiteNoise = -174;//-174dBm/Hz
    double noiseFig = 5;//dB
    noiseFig = pow(10, noiseFig / 10);//线性值
    double noisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

    double D2DTxPow = pow(10, (D2DTxPower - 30) / 10); //线性
    double MacroUserPow = pow(10, (power - 30) / 10); //线性

//    double pathLoss_0 = -10 * log10(targetOutageProbability / D2DTxPower * (power * channelGain / targetSinr - noisePow));//dB
//    pathLoss_0 = pow(10, -pathLoss_0 / 10);//线性值
    double pathLoss_0 = targetOutageProbability / D2DTxPow * (MacroUserPow * channelGain / targetSinr - noisePow);//线性
    double d_0; //干扰保护区域半径
    if (pathLoss_0 < 0) {
        d_0 = cellRadius;
    } else {
//        d_0 = pow(1 / pathLoss_0, 1.0 / PLExponent);
        pathLoss_0 = -10 * log10(pathLoss_0);
        d_0 = 1000 * pow(10, (pathLoss_0 - 128.1) / 37.6);
    }

    if (d_0 > cellRadius) { //d_0 大于小区半径
        d_0 = 500;
        SLARadius.push_back(d_0);
    } else {
        SLARadius.push_back(d_0);
    }
    cout << "macroUser: " << uID << " 干扰保护区域半径：" << d_0 << endl;

    if (d_0 < 500) {
        double d_k = 0; //离散干扰区域半径
        double pathLoss_k;
        pathLoss_k = targetOutageProbability / D2DTxPow / 2 * (MacroUserPow * channelGain / targetSinr - noisePow);

        if (pathLoss_k < 0) {
            d_k = cellRadius;
        } else {
            d_k = pow(1 / pathLoss_k, 1.0 / PLExponent);
        }

        if (d_k > cellRadius) { //d_0 大于小区半径
            d_k = 500;
            SLARadius.push_back(d_k);
        } else {
            SLARadius.push_back(d_k);
        }
    }
}

void macroUser::SetColor(int _colorID) {
    color = _colorID;
    cout << "macroUser: " << uID << ", 着色：" << color << endl;
}

const vector<double> &macroUser::getSLARadius() const {
    return SLARadius;
}

int macroUser::getColor() const {
    return color;
}

int macroUser::getUID() const {
    return uID;
}

double macroUser::getPower() const {
    return power;
}

void macroUser::SetSIRRadius() {
    double targetSinr = 20; //dB
    targetSinr = pow(10, targetSinr / 10); //线性值
    double PLExponent = 7;

    //1个RB,12个连续的载波,12*15000=180000Hz
    double whiteNoise = -174;//-174dBm/Hz
    double noiseFig = 5;//dB
    noiseFig = pow(10, noiseFig / 10);//线性值
    double noisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

    double pathLoss_0 = 1 / D2DTxPower * (power * channelGain / targetSinr);//线性
    double d_0; //干扰保护区域半径
    if (pathLoss_0 < 0) {
        d_0 = cellRadius;
    } else {
        d_0 = pow(1 / pathLoss_0, 1.0 / PLExponent);
    }

    cout << "macroUser: " << uID << " 干扰保护区域半径：" << d_0 << endl;

    if (d_0 > cellRadius) { //d_0 大于小区半径
        d_0 = 500;
        SLARadius.push_back(d_0);
    } else {
        SLARadius.push_back(d_0);
    }
}

double macroUser::getSIRRadius() const {
    return SIRRadius;
}

/********************************macroUser类*********************************/

/********************************干扰区域超图着色*********************************/

void SLAComputing(map<int, macroUser*> &_mapID2MUEPtr) {
    for (auto temp : _mapID2MUEPtr) {
        temp.second->SetSLARadius(); //计算每个宏蜂窝用户的干扰区域半径
    }
}

void macroUserColoring(map<int, macroUser *> &_mapID2MUEPtr, int _colorNum) {
    int colorID = 0;
    for (auto temp : _mapID2MUEPtr) {
        if (colorID < _colorNum) {
            temp.second->SetColor(colorID);
            colorID++;
        } else {
            cout << "error!!!" << endl;
        }
    }
}

void SetD2DPair(map<int, macroUser *> _mapID2MUEPtr, map<int, D2DPair *> &_mapID2D2DPairPtr, int _colorNum) {
    //根据干扰区域初始化 D2D pair 的候选颜色集
    for (auto tempD2DPair : _mapID2D2DPairPtr) {
        double dXPoint = tempD2DPair.second->getDXPoint();
        double dYPoint = tempD2DPair.second->getDYPoint();
        double D2DTx2BSRadius = getDistance(dXPoint, dYPoint, 0, 0);
        vector<int> vecNoColor;//存储不能被着色的颜色序号
        //更新 vecNoColor
        for (auto tempMUE : _mapID2MUEPtr) {
            vector<double> SLARadius = tempMUE.second->getSLARadius();
            if (D2DTx2BSRadius < SLARadius[0]) { //D2D pair 在干扰保护区域范围内 不能着色
                vecNoColor.push_back(tempMUE.second->getColor());
            }
        }
        //更新候选颜色集
        for (int colorID = 0; colorID < _colorNum; ++colorID) {
            bool colorOrNot = true;
            for (int noColorID : vecNoColor) {
                if (colorID == noColorID) colorOrNot = false;
            }
            if (colorOrNot) tempD2DPair.second->addCandidateColor(colorID);
        }
    }
}

void SetD2DHypergraph(map<int, D2DPair*> _mapID2D2DPairPtr, vector<vector<int>> &_D2DHypergraph) {
    int nodeNum = (int)_mapID2D2DPairPtr.size();
    double D2DTxPower = 0;
    if (_mapID2D2DPairPtr.size()) {
        D2DTxPower = _mapID2D2DPairPtr.at(0)->getPower();
    }
    double threshold = 20; //dB
    int edgeID = 0;

    map<int, vector<int>> mapEdgeVecNodes;//map的键是边序号　vector储存对应的节点号

    //普通边
    double F2Ssir = 0;
    double S2Fsir = 0;
    //计算D2D pair1 First 和　D2D pair2 Second 之间的干扰
    for (auto D2DPair1 : _mapID2D2DPairPtr) {
        int D2DTx1ID = D2DPair1.second->getTxID();
        int D2DRx1ID = D2DPair1.second->getRxID();
        for (auto D2DPair2 : _mapID2D2DPairPtr) {
            if (D2DPair1.first < D2DPair2.first) { //避免重复计算
                int D2DTx2ID = D2DPair2.second->getTxID();
                int D2DRx2ID = D2DPair2.second->getRxID();
                double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                F2Ssir = (D2DTxPower * F2FGain) / (D2DTxPower * S2FGain);
                F2Ssir = 10 * log10(F2Ssir);//dB值
                S2Fsir = (D2DTxPower * S2SGain) / (D2DTxPower * F2SGain);
                S2Fsir = 10 * log10(S2Fsir);//dB值
                if (F2Ssir > threshold && S2Fsir > threshold) {
//                    cout << "no edge" << endl;
                } else {
                    cout << "set edge: " << edgeID << endl;
                    vector<int> vecNodes;
                    vecNodes.push_back(D2DPair1.first);
                    vecNodes.push_back(D2DPair2.first);

                    //更新 D2D pair 节点对象
                    D2DPair1.second->addEdgeID(edgeID);
                    D2DPair2.second->addEdgeID(edgeID);
                    D2DPair1.second->addEdgeID2NodeID(edgeID, vecNodes);
                    D2DPair2.second->addEdgeID2NodeID(edgeID, vecNodes);

                    mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                    edgeID++;
                }
            }
        }
    }

    //超边
    //3个D2D pair First Second Third
    double F2STsir = 0;
    double S2FTsir = 0;
    double T2FSsir = 0;
    for (auto D2DPair1 : _mapID2D2DPairPtr) {
        int D2DTx1ID = D2DPair1.second->getTxID();
        int D2DRx1ID = D2DPair1.second->getRxID();
        for (auto D2DPair2 : _mapID2D2DPairPtr) {
            int D2DTx2ID = D2DPair2.second->getTxID();
            int D2DRx2ID = D2DPair2.second->getRxID();
            for (auto D2DPair3 : _mapID2D2DPairPtr) {
                int D2DTx3ID = D2DPair3.second->getTxID();
                int D2DRx3ID = D2DPair3.second->getRxID();
                if (D2DPair1.first < D2DPair2.first && D2DPair1.first < D2DPair3.first && D2DPair2.first < D2DPair3.first) {
                    //F2STsir
                    double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double T2FLinkloss = GetLinkloss(D2DTx3ID, D2DRx1ID, SystemDriveBus::iSlot);
                    double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                    double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                    double T2FGain = pow(10, -T2FLinkloss / 10);//线性值
                    F2STsir = (D2DTxPower * F2FGain) / (D2DTxPower * T2FGain +D2DTxPower * S2FGain);
                    F2STsir = 10 * log10(F2STsir);//dB值

                    //S2FTsir
                    double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double T2SLinkloss = GetLinkloss(D2DTx3ID, D2DRx2ID, SystemDriveBus::iSlot);
                    double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                    double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                    double T2SGain = pow(10, -T2SLinkloss / 10);//线性值
                    S2FTsir = (D2DTxPower * S2SGain) / (D2DTxPower * T2SGain +D2DTxPower * F2SGain);
                    S2FTsir = 10 * log10(S2FTsir);//dB值

                    //T2FSsir
                    double T2TLinkloss = GetLinkloss(D2DTx3ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double F2TLinkloss = GetLinkloss(D2DTx1ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double S2TLinkloss = GetLinkloss(D2DTx2ID, D2DRx3ID, SystemDriveBus::iSlot);
                    double T2TGain = pow(10, -T2TLinkloss / 10);//线性值
                    double F2TGain = pow(10, -F2TLinkloss / 10);//线性值
                    double S2TGain = pow(10, -S2TLinkloss / 10);//线性值
                    T2FSsir = (D2DTxPower * T2TGain) / (D2DTxPower * F2TGain +D2DTxPower * S2TGain);
                    T2FSsir = 10 * log10(T2FSsir);//dB值

                    if (F2STsir > threshold && S2FTsir > threshold && T2FSsir > threshold) {
//                    cout << "no edge" << endl;
                    } else {
                        cout << "set edge: " << edgeID << endl;
                        vector<int> vecNodes;
                        vecNodes.push_back(D2DPair1.first);
                        vecNodes.push_back(D2DPair2.first);
                        vecNodes.push_back(D2DPair3.first);

                        //更新 D2D pair 节点对象
                        D2DPair1.second->addEdgeID(edgeID);
                        D2DPair2.second->addEdgeID(edgeID);
                        D2DPair3.second->addEdgeID(edgeID);
                        D2DPair1.second->addEdgeID2NodeID(edgeID, vecNodes);
                        D2DPair2.second->addEdgeID2NodeID(edgeID, vecNodes);
                        D2DPair3.second->addEdgeID2NodeID(edgeID, vecNodes);

                        mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                        edgeID++;
                    }
                }
            }
        }
    }

    //写入incidence Matrix
    for (int nodeID = 0; nodeID < nodeNum; ++nodeID) {
        vector<int> vecNode2Edge;
        for (auto temp : mapEdgeVecNodes) {
//            edgeID = temp.first;
            bool haveEdge = false;
            for (auto tempNodeID : temp.second) {
                if (nodeID == tempNodeID) haveEdge = true;
            }
            if (haveEdge) {
                vecNode2Edge.push_back(1);
            } else {
                vecNode2Edge.push_back(0);
            }
        }
        _D2DHypergraph.push_back(vecNode2Edge);
    }

    //输出图
    for (int i = 0; i < _D2DHypergraph.size(); ++i) {
        for (int j = 0; j < _D2DHypergraph[0].size(); ++j) {
            cout << _D2DHypergraph[i][j] << ",";
        }
        cout << endl;
    }
    cout << endl;

    //更新 D2D pair

}

void D2DHypergraphColoring(map<int, D2DPair *> &_mapID2D2DPairPtr, vector<vector<int>> _D2DHypergraph, int _colorNum, map<int, macroUser*> _mapID2MUEPtr) {
    for (int colorID = 0; colorID < _colorNum; ++colorID) { //遍历系统候选颜色集
        //构建关于某个颜色的子图
        map<int, D2DPair *> mapSubGraphOneColor; //存储子图中包含的 D2D pair
        vector<D2DPair *> vecSubGraphOneColor; //存储子图中包含的 D2D pair
        for (auto tempD2DPair : _mapID2D2DPairPtr) {
            vector<int> vecCandidateColor = tempD2DPair.second->getVecCandidateColor();
            for (int candidateColorID : vecCandidateColor) {
                if (candidateColorID == colorID) {
                    mapSubGraphOneColor.insert(tempD2DPair);
                    vecSubGraphOneColor.push_back(tempD2DPair.second);
                }
            }
        }

        //检查子图是否为空
        while (vecSubGraphOneColor.size()) {
            //更新 D2D pair 关于某个颜色的子图
            for (auto nodePtr : vecSubGraphOneColor) {
                //将某个节点子图中的所有边的ID存入vecEdgeID
                vector<int> vecEdgeID;
                for (auto temp : nodePtr->getMapEdgeID2NodeID()) { //遍历节点的所有边 筛选出属于子图的边
                    int edgeID = temp.first;
                    int nodeNum = (int)temp.second.size();
                    if (nodeNum == 1) {
                        //判断边对应节点的候选颜色集是否有该颜色
                        bool pushEdgeID = true;
                        for (auto nodeID : temp.second) {
                            if (mapSubGraphOneColor.find(nodeID) == mapSubGraphOneColor.end()) { //节点不存在
                                pushEdgeID = false;
                            }
                        }
                        if (pushEdgeID) vecEdgeID.push_back(edgeID);
                    }
                }
                nodePtr->addColorID2EdgeID(colorID, vecEdgeID);
            }

            //计算所有节点关于某种颜色的节点度
            for (auto nodePtr : vecSubGraphOneColor) {
                nodePtr->updateDegree(colorID);
            }

            //////计算着色顺序
            //统计已经着色 colorID 的所有节点
            macroUser * coloredMacroUser = nullptr;
            vector<D2DPair *> vecColoredNode;
            //找出着色 colorID 的蜂窝用户
            for (auto temp : _mapID2MUEPtr) {
                if (temp.second->getColor() == colorID) {
                    coloredMacroUser = temp.second;
                }
            }
            //找出着色 colorID 的D2D pair
            for (auto temp : _mapID2D2DPairPtr) {
                if (temp.second->getColor() == colorID) vecColoredNode.push_back(temp.second);
            }
            //计算 colorID 的和速率
            double rateBefore, rateAfter, macroSinr, D2DSinr, macroUserPower, D2DTxPower, linkloss, channelGain, signalPow;
            double macroRate = 0;
            double D2DRate = 0;
            vector<double> vecD2DRate;
            double interferencePow = 0;
            double whiteNoise = -174;//-174dBm/Hz
            double noiseFig = 5;//dB
            noiseFig = pow(10, noiseFig / 10);//线性值
            double thermalNoisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

            /////////////////////////////////////////////////////////////////////////////////////////////

            /********************rateBefore**********************/
            ////计算蜂窝用户速率
            if (coloredMacroUser != nullptr) {
                //计算目标信号功率
                macroUserPower = coloredMacroUser->getPower();
                linkloss = GetLinkloss(coloredMacroUser->getUID(), 0, SystemDriveBus::iSlot);
                channelGain = pow(10, -linkloss / 10);//线性值
                signalPow = macroUserPower * channelGain;
                for (auto temp : vecColoredNode) { //计算干扰信号功率
                    D2DTxPower = temp->getPower();
                    linkloss = GetLinkloss(temp->getTxID(), 0, SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    interferencePow = interferencePow + D2DTxPower * channelGain;
                }
                macroSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                macroRate = 180000 * RBNUM * log2(1 + macroSinr);
            }

            ////计算D2D用户速率
            for (auto temp : vecColoredNode) {
                //计算目标信号功率
                D2DTxPower = temp->getPower();
                linkloss = GetLinkloss(temp->getTxID(), temp->getRxID(), SystemDriveBus::iSlot);
                channelGain = pow(10, -linkloss / 10);//线性值
                signalPow = D2DTxPower * channelGain;
                interferencePow = 0;
                //计算蜂窝用户的干扰
                if (coloredMacroUser != nullptr) {
                    macroUserPower = coloredMacroUser->getPower();
                    linkloss = GetLinkloss(coloredMacroUser->getUID(), temp->getRxID(), SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    interferencePow = macroUserPower * channelGain;
                }

                for (auto interNode : vecColoredNode) {
                    if (interNode->getID() != temp->getID()) { //计算干扰信号功率
                        D2DTxPower = interNode->getPower();
                        linkloss = GetLinkloss(interNode->getTxID(), temp->getRxID(), SystemDriveBus::iSlot);
                        channelGain = pow(10, -linkloss / 10);//线性值
                        interferencePow = interferencePow + D2DTxPower * channelGain;
                    }
                }
                D2DSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                D2DRate = 180000 * RBNUM * log2(1 + D2DSinr);
                vecD2DRate.push_back(D2DRate);
            }
            rateBefore = macroRate;
            for (double _D2DRate : vecD2DRate) {
                rateBefore = rateBefore + _D2DRate;
            }
            /********************rateBefore**********************/

            /********************rateAfter**********************/
            vector<D2DPair *> vecColoredNodeAfter = vecColoredNode;
            for (auto nodePtr : vecSubGraphOneColor) {
                vecColoredNodeAfter.push_back(nodePtr); //将准备着色的节点加入已着色节点集合
                ////计算蜂窝用户速率
                if (coloredMacroUser != nullptr) {
                    //计算目标信号功率
                    macroUserPower = coloredMacroUser->getPower();
                    linkloss = GetLinkloss(coloredMacroUser->getUID(), 0, SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    signalPow = macroUserPower * channelGain;
                    for (auto temp : vecColoredNodeAfter) { //计算干扰信号功率
                        D2DTxPower = temp->getPower();
                        linkloss = GetLinkloss(temp->getTxID(), 0, SystemDriveBus::iSlot);
                        channelGain = pow(10, -linkloss / 10);//线性值
                        interferencePow = interferencePow + D2DTxPower * channelGain;
                    }
                    macroSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                    macroRate = 180000 * RBNUM * log2(1 + macroSinr);
                }
                ////计算D2D用户速率
                for (auto temp : vecColoredNodeAfter) {
                    //计算目标信号功率
                    D2DTxPower = temp->getPower();
                    linkloss = GetLinkloss(temp->getTxID(), temp->getRxID(), SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    signalPow = D2DTxPower * channelGain;
                    //计算蜂窝用户的干扰
                    if (coloredMacroUser != nullptr) {
                        macroUserPower = coloredMacroUser->getPower();
                        linkloss = GetLinkloss(coloredMacroUser->getUID(), temp->getRxID(), SystemDriveBus::iSlot);
                        channelGain = pow(10, -linkloss / 10);//线性值
                        interferencePow = macroUserPower * channelGain;
                    } else {
                        interferencePow = 0;
                    }

                    for (auto interNode : vecColoredNode) {
                        if (interNode->getID() != temp->getID()) { //计算干扰信号功率
                            D2DTxPower = interNode->getPower();
                            linkloss = GetLinkloss(interNode->getTxID(), temp->getRxID(), SystemDriveBus::iSlot);
                            channelGain = pow(10, -linkloss / 10);//线性值
                            interferencePow = interferencePow + D2DTxPower * channelGain;
                        }
                    }
                    D2DSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                    D2DRate = 180000 * RBNUM * log2(1 + D2DSinr);
                    vecD2DRate.push_back(D2DRate);
                }
                rateAfter = macroRate;
                for (double _D2DRate : vecD2DRate) {
                    rateAfter = rateAfter + _D2DRate;
                }
                double order = (rateAfter - rateBefore);
                nodePtr->setOrder(order);
            }

            /********************rateAfter**********************/

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//            vector<D2DPair *> vecColoredNodeAfter = vecColoredNode;
//            for (auto nodePtr : vecSubGraphOneColor) {
//                vecColoredNodeAfter.push_back(nodePtr); //将准备着色的节点加入已着色节点集合
//                ////计算蜂窝用户速率
//                if (coloredMacroUser != nullptr) {
//                    //计算目标信号功率
//                    macroUserPower = coloredMacroUser->getPower();
//                    linkloss = GetLinkloss(coloredMacroUser->getUID(), 0, SystemDriveBus::iSlot);
//                    channelGain = pow(10, -linkloss / 10);//线性值
//                    signalPow = macroUserPower * channelGain;
//                    for (auto temp : vecColoredNodeAfter) { //计算干扰信号功率
//                        D2DTxPower = temp->getPower();
//                        linkloss = GetLinkloss(temp->getTxID(), 0, SystemDriveBus::iSlot);
//                        channelGain = pow(10, -linkloss / 10);//线性值
//                        interferencePow = interferencePow + D2DTxPower * channelGain;
//                    }
//                    macroSinr = signalPow / (interferencePow + thermalNoisePow); //线性
//                    macroRate = 180000 * RBNUM * log2(1 + macroSinr);
//                }
//                ////计算D2D用户速率
//                if (nodePtr != nullptr) {
//                    //计算目标信号功率
//                    D2DTxPower = nodePtr->getPower();
//                    linkloss = GetLinkloss(nodePtr->getTxID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
//                    channelGain = pow(10, -linkloss / 10);//线性值
//                    signalPow = D2DTxPower * channelGain;
//                    //计算蜂窝用户的干扰
//                    if (coloredMacroUser != nullptr) {
//                        macroUserPower = coloredMacroUser->getPower();
//                        linkloss = GetLinkloss(coloredMacroUser->getUID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
//                        channelGain = pow(10, -linkloss / 10);//线性值
//                        interferencePow = macroUserPower * channelGain;
//                    } else {
//                        interferencePow = 0;
//                    }
//
//                    for (auto interNode : vecColoredNode) {
//                        if (interNode->getID() != nodePtr->getID()) { //计算干扰信号功率
//                            D2DTxPower = interNode->getPower();
//                            linkloss = GetLinkloss(interNode->getTxID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
//                            channelGain = pow(10, -linkloss / 10);//线性值
//                            interferencePow = interferencePow + D2DTxPower * channelGain;
//                        }
//                    }
//                    D2DSinr = signalPow / (interferencePow + thermalNoisePow); //线性
//                    D2DRate = 180000 * RBNUM * log2(1 + D2DSinr);
//                }
//
////                double order = (macroRate + D2DRate);
//                double order = (macroRate + D2DRate) / (nodePtr->getDegree() + 1);
//                nodePtr->setOrder(order);
//            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////////

//            //按节点度排序
//            sort(vecSubGraphOneColor.begin(), vecSubGraphOneColor.end(), comD2DDegree); //按度从高到低排序

            //按节点着色序号 order 排序
            sort(vecSubGraphOneColor.begin(), vecSubGraphOneColor.end(), comD2DOrder); //按order从高到低排序

            //给着色序号最先的节点着色
            vecSubGraphOneColor[0]->SetColor(colorID);
            vecSubGraphOneColor[0]->clearCandidateColor(); //清除已着色节点的候选颜色集

            //在相邻节点的候选颜色集中去掉该颜色
            map<int, vector<int>> mapEdgeID2NodeID = vecSubGraphOneColor[0]->getMapEdgeID2NodeID();
            for (auto temp : mapEdgeID2NodeID) {
                if ((int)temp.second.size() < 3) { //如果是普通边
                    int NodeID1 = temp.second[0];
                    int NodeID2 = temp.second[0];
                    _mapID2D2DPairPtr.at(NodeID1)->deleteCandidateColor(colorID);
                    _mapID2D2DPairPtr.at(NodeID2)->deleteCandidateColor(colorID);
                } else { //如果是超边
                    int NodeID1 = temp.second[0];
                    int NodeID2 = temp.second[1];
                    int NodeID3 = temp.second[2];
                    if (_mapID2D2DPairPtr.at(NodeID1)->getColor() == colorID && _mapID2D2DPairPtr.at(NodeID2)->getColor() == colorID) {
                        _mapID2D2DPairPtr.at(NodeID3)->deleteCandidateColor(colorID);
                    }
                    if (_mapID2D2DPairPtr.at(NodeID1)->getColor() == colorID && _mapID2D2DPairPtr.at(NodeID3)->getColor() == colorID) {
                        _mapID2D2DPairPtr.at(NodeID2)->deleteCandidateColor(colorID);
                    }
                    if (_mapID2D2DPairPtr.at(NodeID2)->getColor() == colorID && _mapID2D2DPairPtr.at(NodeID3)->getColor() == colorID) {
                        _mapID2D2DPairPtr.at(NodeID1)->deleteCandidateColor(colorID);
                    }
                }
            }

            //根据离散干扰区域更新候选颜色集
//            if (coloredMacroUser != nullptr) { //如果有蜂窝用户着色该颜色
//                vector<double> vecSLARadius = coloredMacroUser->getSLARadius();
//                if (vecSLARadius.size() > 1) {
//                    double d_k = vecSLARadius[1]; //离散干扰区域半径
//                    //筛选已经着该颜色的D2D节点
//                    vector<D2DPair *> vecColoredD2DPair;
//                    for (auto temp : _mapID2D2DPairPtr) {
//                        if (temp.second->getColor() == colorID) {
//                            vecColoredD2DPair.push_back(temp.second);
//                        }
//                    }
//                    //统计已着该颜色的D2D节点到基站的距离小于d_k的节点个数
//                    int i = 0;//计数
//                    for (auto temp : vecColoredD2DPair) {
//                        if (temp->getTx2BSRadius() < d_k) {
//                            i++;
//                        }
//                    }
//                    if (i > 0) { //如果d_k范围内已着色节点数目大于2
//                        for (auto temp :vecSubGraphOneColor) {
//                            if (temp->getTx2BSRadius() < d_k) { //在d_k范围内的节点的候选颜色集中去掉该颜色
//                                temp->deleteCandidateColor(colorID);
//                            }
//                        }
//                    }
//                }
//            }

            ////更新子图
            map<int, D2DPair *> tempmapSubGraphOneColor = mapSubGraphOneColor;
            mapSubGraphOneColor.clear();
            vecSubGraphOneColor.clear();
            for (auto tempD2DPair : tempmapSubGraphOneColor) {
                vector<int> vecCandidateColor = tempD2DPair.second->getVecCandidateColor();
                for (int candidateColorID : vecCandidateColor) {
                    if (candidateColorID == colorID) {
                        mapSubGraphOneColor.insert(tempD2DPair);
                        vecSubGraphOneColor.push_back(tempD2DPair.second);
                    }
                }
            }
        }
    }
}

bool comD2DDegree(D2DPair *_D2DPair1, D2DPair *_D2DPair2) {
    return _D2DPair1->getDegree() > _D2DPair2->getDegree();
}

bool comD2DOrder(D2DPair *_D2DPair1, D2DPair *_D2DPair2) {
    return _D2DPair1->getOrder() > _D2DPair2->getOrder();
}

void SetD2DGraph(map<int, D2DPair *> _mapID2D2DPairPtr, vector<vector<int>> &_D2DGraph) {
    int nodeNum = (int)_mapID2D2DPairPtr.size();
    double D2DTxPower = _mapID2D2DPairPtr.at(0)->getPower();
    double threshold = 20; //dB
    int edgeID = 0;

    map<int, vector<int>> mapEdgeVecNodes;//map的键是边序号　vector储存对应的节点号

    //普通边
    double F2Ssir = 0;
    double S2Fsir = 0;
    //计算D2D pair1 First 和　D2D pair2 Second 之间的干扰
    for (auto D2DPair1 : _mapID2D2DPairPtr) {
        int D2DTx1ID = D2DPair1.second->getTxID();
        int D2DRx1ID = D2DPair1.second->getRxID();
        for (auto D2DPair2 : _mapID2D2DPairPtr) {
            if (D2DPair1.first < D2DPair2.first) { //避免重复计算
                int D2DTx2ID = D2DPair2.second->getTxID();
                int D2DRx2ID = D2DPair2.second->getRxID();
                double S2SLinkloss = GetLinkloss(D2DTx2ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2SLinkloss = GetLinkloss(D2DTx1ID, D2DRx2ID, SystemDriveBus::iSlot);
                double F2FLinkloss = GetLinkloss(D2DTx1ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2FLinkloss = GetLinkloss(D2DTx2ID, D2DRx1ID, SystemDriveBus::iSlot);
                double S2SGain = pow(10, -S2SLinkloss / 10);//线性值
                double F2SGain = pow(10, -F2SLinkloss / 10);//线性值
                double F2FGain = pow(10, -F2FLinkloss / 10);//线性值
                double S2FGain = pow(10, -S2FLinkloss / 10);//线性值
                F2Ssir = (D2DTxPower * F2FGain) / (D2DTxPower * S2FGain);
                F2Ssir = 10 * log10(F2Ssir);//dB值
                S2Fsir = (D2DTxPower * S2SGain) / (D2DTxPower * F2SGain);
                S2Fsir = 10 * log10(S2Fsir);//dB值
                if (F2Ssir > threshold && S2Fsir > threshold) {
//                    cout << "no edge" << endl;
                } else {
                    cout << "set edge: " << edgeID << endl;
                    vector<int> vecNodes;
                    vecNodes.push_back(D2DPair1.first);
                    vecNodes.push_back(D2DPair2.first);

                    //更新 D2D pair 节点对象
                    D2DPair1.second->addEdgeID(edgeID);
                    D2DPair2.second->addEdgeID(edgeID);
                    D2DPair1.second->addEdgeID2NodeID(edgeID, vecNodes);
                    D2DPair2.second->addEdgeID2NodeID(edgeID, vecNodes);

                    mapEdgeVecNodes.insert(pair<int, vector<int>>(edgeID, vecNodes));
                    edgeID++;
                }
            }
        }
    }

    //写入incidence Matrix
    for (int nodeID = 0; nodeID < nodeNum; ++nodeID) {
        vector<int> vecNode2Edge;
        for (auto temp : mapEdgeVecNodes) {
//            edgeID = temp.first;
            bool haveEdge = false;
            for (auto tempNodeID : temp.second) {
                if (nodeID == tempNodeID) haveEdge = true;
            }
            if (haveEdge) {
                vecNode2Edge.push_back(1);
            } else {
                vecNode2Edge.push_back(0);
            }
        }
        _D2DGraph.push_back(vecNode2Edge);
    }

    //输出图
    for (int i = 0; i < _D2DGraph.size(); ++i) {
        for (int j = 0; j < _D2DGraph[0].size(); ++j) {
            cout << _D2DGraph[i][j] << ",";
        }
        cout << endl;
    }
    cout << endl;
}

void D2DGraphColoring(map<int, D2DPair *> &_mapID2D2DPairPtr, vector<vector<int>> _D2DGraph, int _colorNum,
                      map<int, macroUser *> _mapID2MUEPtr) {
    for (int colorID = 0; colorID < _colorNum; ++colorID) { //遍历系统候选颜色集
        //构建关于某个颜色的子图
        map<int, D2DPair *> mapSubGraphOneColor; //存储子图中包含的 D2D pair
        vector<D2DPair *> vecSubGraphOneColor; //存储子图中包含的 D2D pair
        for (auto tempD2DPair : _mapID2D2DPairPtr) {
            vector<int> vecCandidateColor = tempD2DPair.second->getVecCandidateColor();
            for (int candidateColorID : vecCandidateColor) {
                if (candidateColorID == colorID) {
                    mapSubGraphOneColor.insert(tempD2DPair);
                    vecSubGraphOneColor.push_back(tempD2DPair.second);
                }
            }
        }

        //检查子图是否为空
        while (vecSubGraphOneColor.size()) {
            //更新 D2D pair 关于某个颜色的子图
            for (auto nodePtr : vecSubGraphOneColor) {
                //将某个节点子图中的所有边的ID存入vecEdgeID
                vector<int> vecEdgeID;
                for (auto temp : nodePtr->getMapEdgeID2NodeID()) { //遍历节点的所有边 筛选出属于子图的边
                    int edgeID = temp.first;
                    //判断边对应节点的候选颜色集是否有该颜色
                    bool pushEdgeID = true;
                    for (auto nodeID : temp.second) {
                        if (mapSubGraphOneColor.find(nodeID) == mapSubGraphOneColor.end()) { //节点不存在
                            pushEdgeID = false;
                        }
                    }
                    if (pushEdgeID) vecEdgeID.push_back(edgeID);
                }
                nodePtr->addColorID2EdgeID(colorID, vecEdgeID);
            }

            //计算所有节点关于某种颜色的节点度
            for (auto nodePtr : vecSubGraphOneColor) {
                nodePtr->updateDegree(colorID);
            }

            //////计算着色顺序
            //统计已经着色 colorID 的所有节点
            macroUser * coloredMacroUser = nullptr;
            vector<D2DPair *> vecColoredNode;
            //找出着色 colorID 的蜂窝用户
            for (auto temp : _mapID2MUEPtr) {
                if (temp.second->getColor() == colorID) {
                    coloredMacroUser = temp.second;
                }
            }
            //找出着色 colorID 的D2D pair
            for (auto temp : _mapID2D2DPairPtr) {
                if (temp.second->getColor() == colorID) vecColoredNode.push_back(temp.second);
            }
            //计算 colorID 的和速率
            double macroSinr, D2DSinr, macroUserPower, D2DTxPower, linkloss, channelGain, signalPow;
            double macroRate = 0;
            double D2DRate = 0;
            vector<double> vecD2DRate;
            double interferencePow = 0;
            double whiteNoise = -174;//-174dBm/Hz
            double noiseFig = 5;//dB
            noiseFig = pow(10, noiseFig / 10);//线性值
            double thermalNoisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

            vector<D2DPair *> vecColoredNodeAfter = vecColoredNode;
            for (auto nodePtr : vecSubGraphOneColor) {
                vecColoredNodeAfter.push_back(nodePtr); //将准备着色的节点加入已着色节点集合
                ////计算蜂窝用户速率
                if (coloredMacroUser != nullptr) {
                    //计算目标信号功率
                    macroUserPower = coloredMacroUser->getPower();
                    linkloss = GetLinkloss(coloredMacroUser->getUID(), 0, SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    signalPow = macroUserPower * channelGain;
                    for (auto temp : vecColoredNodeAfter) { //计算干扰信号功率
                        D2DTxPower = temp->getPower();
                        linkloss = GetLinkloss(temp->getTxID(), 0, SystemDriveBus::iSlot);
                        channelGain = pow(10, -linkloss / 10);//线性值
                        interferencePow = interferencePow + D2DTxPower * channelGain;
                    }
                    macroSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                    macroRate = 180000 * RBNUM * log2(1 + macroSinr);
                }
                ////计算D2D用户速率
                if (nodePtr != nullptr) {
                    //计算目标信号功率
                    D2DTxPower = nodePtr->getPower();
                    linkloss = GetLinkloss(nodePtr->getTxID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
                    channelGain = pow(10, -linkloss / 10);//线性值
                    signalPow = D2DTxPower * channelGain;
                    //计算蜂窝用户的干扰
                    if (coloredMacroUser != nullptr) {
                        macroUserPower = coloredMacroUser->getPower();
                        linkloss = GetLinkloss(coloredMacroUser->getUID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
                        channelGain = pow(10, -linkloss / 10);//线性值
                        interferencePow = macroUserPower * channelGain;
                    } else {
                        interferencePow = 0;
                    }

                    for (auto interNode : vecColoredNode) {
                        if (interNode->getID() != nodePtr->getID()) { //计算干扰信号功率
                            D2DTxPower = interNode->getPower();
                            linkloss = GetLinkloss(interNode->getTxID(), nodePtr->getRxID(), SystemDriveBus::iSlot);
                            channelGain = pow(10, -linkloss / 10);//线性值
                            interferencePow = interferencePow + D2DTxPower * channelGain;
                        }
                    }
                    D2DSinr = signalPow / (interferencePow + thermalNoisePow); //线性
                    D2DRate = 180000 * RBNUM * log2(1 + D2DSinr);
                }

                double order = (macroRate + D2DRate) / (nodePtr->getDegree() + 1);
                nodePtr->setOrder(order);
            }

//            //按节点度排序
//            sort(vecSubGraphOneColor.begin(), vecSubGraphOneColor.end(), comD2DDegree); //按度从高到低排序

            //按节点着色序号 order 排序
            sort(vecSubGraphOneColor.begin(), vecSubGraphOneColor.end(), comD2DOrder); //按order从高到低排序

            //给着色序号最先的节点着色
            vecSubGraphOneColor[0]->SetColor(colorID);
            vecSubGraphOneColor[0]->clearCandidateColor(); //清除已着色节点的候选颜色集

            //在相邻节点的候选颜色集中去掉该颜色
            map<int, vector<int>> mapEdgeID2NodeID = vecSubGraphOneColor[0]->getMapEdgeID2NodeID();
            for (auto temp : mapEdgeID2NodeID) {
                int NodeID1 = temp.second[0];
                int NodeID2 = temp.second[0];
                _mapID2D2DPairPtr.at(NodeID1)->deleteCandidateColor(colorID);
                _mapID2D2DPairPtr.at(NodeID2)->deleteCandidateColor(colorID);
            }

            ////更新子图
            map<int, D2DPair *> tempmapSubGraphOneColor = mapSubGraphOneColor;
            mapSubGraphOneColor.clear();
            vecSubGraphOneColor.clear();
            for (auto tempD2DPair : tempmapSubGraphOneColor) {
                vector<int> vecCandidateColor = tempD2DPair.second->getVecCandidateColor();
                for (int candidateColorID : vecCandidateColor) {
                    if (candidateColorID == colorID) {
                        mapSubGraphOneColor.insert(tempD2DPair);
                        vecSubGraphOneColor.push_back(tempD2DPair.second);
                    }
                }
            }
        }
    }
}

void myHypergraphColoring(map<int, D2DPair *> &_mapID2D2DPairPtr, vector<vector<int>> _D2DHypergraph, int _colorNum,
                          map<int, macroUser *> _mapID2MUEPtr) {
    vector<D2DPair *> vecD2DPairPtr;
    for (auto temp : _mapID2D2DPairPtr) {
        vecD2DPairPtr.push_back(temp.second);
    }

    while (vecD2DPairPtr.size() > 0) {
        //计算节点度
        for (auto temp : vecD2DPairPtr) {
            int degree = (int)temp->getVecEdgeID().size();
            temp->setDegree(degree);
        }

        //按节点度排序
        sort(vecD2DPairPtr.begin(), vecD2DPairPtr.end(), comD2DDegree); //按度从高到低排序

        //给着色序号最先的节点着色
        D2DPair * colorNode = vecD2DPairPtr[0];
        vector<int> vecCandidateColor = colorNode->getVecCandidateColor();
        int colorID = -1;
        if (vecCandidateColor.size() > 0) {
            colorID = vecCandidateColor[0];
            colorNode->SetColor(colorID);
        }

        //更新图 去除 colorNode 和 它的相邻边
        vector<D2DPair *> tempvecD2DPairPtr = vecD2DPairPtr;
        vecD2DPairPtr.clear();
        for (auto temp : tempvecD2DPairPtr) {
            if (temp != colorNode) {
                vecD2DPairPtr.push_back(temp);
            }
        }
        map<int, vector<int>> mapEdgeID2NodeID = colorNode->getMapEdgeID2NodeID();
        for (auto temp : mapEdgeID2NodeID) {
            int edgeID = temp.first;
            for (auto nodeID : temp.second) {
                _mapID2D2DPairPtr.at(nodeID)->deleteEdge(edgeID);
            }
            //更新候选颜色集
            if (temp.second.size() < 3) { //普通边
                _mapID2D2DPairPtr.at(temp.second[0])->deleteColor(colorID);
                _mapID2D2DPairPtr.at(temp.second[1])->deleteColor(colorID);
            } else { //超边
                if (_mapID2D2DPairPtr.at(temp.second[0])->getColor() == colorID && _mapID2D2DPairPtr.at(temp.second[1])->getColor() == colorID) {
                    _mapID2D2DPairPtr.at(temp.second[2])->deleteColor(colorID);
                }
                if (_mapID2D2DPairPtr.at(temp.second[0])->getColor() == colorID && _mapID2D2DPairPtr.at(temp.second[2])->getColor() == colorID) {
                    _mapID2D2DPairPtr.at(temp.second[1])->deleteColor(colorID);
                }
                if (_mapID2D2DPairPtr.at(temp.second[2])->getColor() == colorID && _mapID2D2DPairPtr.at(temp.second[2])->getColor() == colorID) {
                    _mapID2D2DPairPtr.at(temp.second[0])->deleteColor(colorID);
                }
            }
        }
    }
}

void SIRComputing(map<int, macroUser *> &_mapID2MUEPtr) {
    for (auto temp : _mapID2MUEPtr) {
        temp.second->SetSIRRadius(); //计算每个宏蜂窝用户的干扰区域半径
    }
}

void SetSIRD2DPair(map<int, macroUser *> _mapID2MUEPtr, map<int, D2DPair *> &_mapID2D2DPairPtr, int _colorNum) {
//根据干扰区域初始化 D2D pair 的候选颜色集
    for (auto tempD2DPair : _mapID2D2DPairPtr) {
        double dXPoint = tempD2DPair.second->getDXPoint();
        double dYPoint = tempD2DPair.second->getDYPoint();
        double D2DTx2BSRadius = getDistance(dXPoint, dYPoint, 0, 0);
        vector<int> vecNoColor;//存储不能被着色的颜色序号
        //更新 vecNoColor
        for (auto tempMUE : _mapID2MUEPtr) {
            double SIRRadius = tempMUE.second->getSIRRadius();
            if (D2DTx2BSRadius < SIRRadius) { //D2D pair 在干扰保护区域范围内 不能着色
                vecNoColor.push_back(tempMUE.second->getColor());
            }
        }
        //更新候选颜色集
        for (int colorID = 0; colorID < _colorNum; ++colorID) {
            bool colorOrNot = true;
            for (int noColorID : vecNoColor) {
                if (colorID == noColorID) colorOrNot = false;
            }
            if (colorOrNot) tempD2DPair.second->addCandidateColor(colorID);
        }
    }
}

/********************************D2DUser类*********************************/

D2DPair::D2DPair(int _ID, int _TxID, int _RxID) {
    ID = _ID;
    TxID = _TxID;
    RxID = _RxID;
}

D2DPair::~D2DPair() {

}

void D2DPair::SetColor(int _colorID) {
    color = _colorID;
    cout << "D2D Pair Tx: " << TxID << ", 着色：" << color << endl;
}

void D2DPair::initial(double _power, double _dXPoint, double _dYPoint) {
    power = _power;
    dXPoint = _dXPoint;
    dYPoint = _dYPoint;
    degree = -1;
    color = -1;
    Tx2BSRadius = getDistance(dXPoint, dYPoint, 0, 0);
}

double D2DPair::getDXPoint() const {
    return dXPoint;
}

double D2DPair::getDYPoint() const {
    return dYPoint;
}

void D2DPair::addCandidateColor(int _colorID) {
    vecCandidateColor.push_back(_colorID);
}

double D2DPair::getPower() const {
    return power;
}

int D2DPair::getTxID() const {
    return TxID;
}

int D2DPair::getRxID() const {
    return RxID;
}

int D2DPair::getID() const {
    return ID;
}

const vector<int> &D2DPair::getVecCandidateColor() const {
    return vecCandidateColor;
}

void D2DPair::setDegree(int degree) {
    D2DPair::degree = degree;
}

int D2DPair::getDegree() const {
    return degree;
}

void D2DPair::addEdgeID(int _edgeID) {
    vecEdgeID.push_back(_edgeID);
}

void D2DPair::addEdgeID2NodeID(int _edgeID, vector<int> _vecNodeID) {
    mapEdgeID2NodeID.insert(pair<int, vector<int>>(_edgeID, _vecNodeID));
}

const vector<int> &D2DPair::getVecEdgeID() const {
    return vecEdgeID;
}

const map<int, vector<int>> &D2DPair::getMapEdgeID2NodeID() const {
    return mapEdgeID2NodeID;
}

void D2DPair::addColorID2EdgeID(int _colorID, vector<int> _vecEdgeID) {
    mapColorID2EdgeID.clear();
    mapColorID2EdgeID.insert(pair<int, vector<int>>(_colorID, _vecEdgeID));
}

void D2DPair::updateDegree(int _colorID) {
    degree = (int)mapColorID2EdgeID.at(_colorID).size();
}

void D2DPair::deleteCandidateColor(int _colorID) {
    vector<int> tempvecCandidateColor = vecCandidateColor;
    vecCandidateColor.clear();
    for (int tempColorID : tempvecCandidateColor) {
        if (tempColorID != _colorID) {
            vecCandidateColor.push_back(tempColorID);
        }
    }
}

int D2DPair::getColor() const {
    return color;
}

void D2DPair::clearCandidateColor() {
    vecCandidateColor.clear();
}

void D2DPair::setOrder(double order) {
    D2DPair::order = order;
}

double D2DPair::getOrder() const {
    return order;
}

void D2DPair::deleteEdge(int edgeID) {
    vector<int> tempVec = vecEdgeID;
    vecEdgeID.clear();
    for (int temp : tempVec) {
        if (temp != edgeID) {
            vecEdgeID.push_back(temp);
        }
    }
}

void D2DPair::deleteColor(int colorID) {
    vector<int> tempVec = vecCandidateColor;
    vecEdgeID.clear();
    for (int temp : tempVec) {
        if (temp != colorID) {
            vecEdgeID.push_back(temp);
        }
    }
}

double D2DPair::getTx2BSRadius() const {
    return Tx2BSRadius;
}
