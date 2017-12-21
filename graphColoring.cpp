//
// Created by lee on 17-12-8.
//


#include "graphColoring.h"

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
    double targetSinr = 10; //dB
    targetSinr = pow(10, targetSinr / 10); //线性值
    double targetOutageProbability = 0.01; //目标中断概率
    double PLExponent = 7;

    //1个RB,12个连续的载波,12*15000=180000Hz
    double whiteNoise = -174;//-174dBm/Hz
    double noiseFig = 5;//dB
    noiseFig = pow(10, noiseFig / 10);//线性值
    double noisePow = pow(10, (whiteNoise - 30) / 10) * 180000 * noiseFig;//线性值

    double pathLoss_0 = -10 * log10(targetOutageProbability / D2DTxPower * (power * channelGain / targetSinr - noisePow));//dB
    pathLoss_0 = pow(10, -pathLoss_0 / 10);//线性值
    double d_0 = pow(1 / pathLoss_0, 1.0 / PLExponent);

    if (d_0 > cellRadius) { //d_0 大于小区半径
        d_0 = 500;
        SLARadius.push_back(d_0);
    } else {
        SLARadius.push_back(d_0);
    }
}

void macroUser::SetColor(int _colorID) {
    color = _colorID;
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

void SetD2DPair(map<int, macroUser *> _mapID2MUEPtr, map<int, D2DPair *> &_mapID2D2DPairPtr) {

}

void SetD2DHypergraph(map<int, D2DPair*> mapID2D2DPairPtr, vector<vector<int>> &D2DHypergraph) {

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
}

void D2DPair::initial(double _power, double _dXPoint, double _dYPoint) {
    power = _power;
    dXPoint = _dXPoint;
    dYPoint = _dYPoint;
}
