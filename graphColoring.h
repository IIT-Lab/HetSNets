//
// Created by lee on 17-12-8.
//

#ifndef HETSNETS_GRAPHCOLORING_H
#define HETSNETS_GRAPHCOLORING_H

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

/********************************node类*********************************/

class node {
public:
    node(int _ID);
    ~node();
    void initial(vector<vector<int>> _incidenceMatrix);
    void update(int _edgeID);

    int getID() const;

    int getDegree() const;

    int getOrder() const;

    int getColor() const;

    const map<int, vector<int>> &getMapEdgeID2AdjacencyNodeID() const;

    void setOrder(int order);

    void setColor();

    void setVecCandidateColor(int _colorNum, map<int, node *> _mapNodeID2NodePtr);

private:
    int ID; //编号 从0开始
    int degree; //度
    int color; //节点的颜色
    int order; //着色顺序 从0开始
    vector<int> vecEdgeID; //节点相邻边的ID 从0开始
    vector<int> vecAdjacencyNodeID; //普通边相邻节点ID 从0开始
    map<int, vector<int>> mapEdgeID2AdjacencyNodeID; //相邻边的ID对相邻节点ID的映射表
    vector<int> vecCandidateColor; //节点候选颜色集
};

/********************************hyperNode类*********************************/

class hyperNode {
public:
    hyperNode(int _ID);
    ~hyperNode();
    void initial(vector<vector<int>> _incidenceMatrix);
    void update(int _edgeID);

    int getID() const;

    int getDegree() const;

    int getOrder() const;

    int getColor() const;

    const map<int, vector<int>> &getMapEdgeID2AdjacencyNodeID() const;

    void setOrder(int order);

    void setColor();

    void setVecCandidateColor(int _colorNum, map<int, hyperNode *> _mapNodeID2HyperNodePtr);

private:
    int ID; //编号 从0开始
    int degree; //度
    int color; //节点的颜色
    int order; //着色顺序 从-开始
    vector<int> vecEdgeID; //节点相邻边的ID 从0开始
    vector<int> vecNormalEdgeID; //节点相邻普通边的ID
    vector<int> vecHyperEdgeID; //节点相邻超边的ID
    vector<int> vecNorAdjNodeID; //普通边相邻节点ID 从0开始
    vector<vector<int>> vecHyperAdjNodeID; //超边相邻节点ID 从0开始
    map<int, vector<int>> mapEdgeID2AdjacencyNodeID; //相邻边的ID对相邻节点ID的映射表
    map<int, vector<int>> mapHyperEdgeID2AdjNodeID; //相邻超边的ID对相邻节点ID集合的映射表
    vector<int> vecCandidateColor; //节点候选颜色集
};

/********************************功能函数*********************************/

bool comDegree(node* _node1, node* _node2);
bool comOrder(node* _node1, node* _node2);

bool comhyperDegree(hyperNode* _node1, hyperNode* _node2);
bool comhyperOrder(hyperNode* _node1, hyperNode* _node2);

void graghColoring(map<int, node*> _mapNodeID2NodePtr, int _colorNum);
void hypergraghColoring(map<int, hyperNode*> _mapNodeID2HyperNodePtr, int _colorNum);

/********************************功能函数*********************************/

/********************************macroUser类*********************************/
class macroUser {
public:
    macroUser(int _uID, double _power, double _D2DTxPower, double _channelGain, double _cellRadius);
    ~macroUser();
    void SetSLARadius(); //计算干扰区域半径
    void SetColor(int _colorID);

    const vector<double> &getSLARadius() const;

    int getColor() const;

private:
    int uID;
    double power; //宏蜂窝用户发射功率
    double D2DTxPower; //D2D用户发射功率
    double channelGain; //宏蜂窝用户到宏基站的信道增益
    double cellRadius; //宏蜂窝半径
    int color; //节点的颜色
    vector<double> SLARadius; //离散干扰区域半径
};

/********************************macroUser类*********************************/

/********************************D2DPair*********************************/
class D2DPair {
public:
    D2DPair(int _ID, int _TxID, int _RxID);
    ~D2DPair();
    void initial(double _power, double _dXPoint, double _dYPoint);
    void SetColor(int _colorID); //给 D2D pair 分配颜色
    void addCandidateColor(int _colorID); //将候选颜色加入候选颜色集
    void setDegree(int degree);

    double getDXPoint() const;

    double getDYPoint() const;

    double getPower() const;

    int getID() const;

    int getTxID() const;

    int getRxID() const;

    const vector<int> &getVecCandidateColor() const;

    int getDegree() const;

    void addEdgeID(int _edgeID);

    void addEdgeID2NodeID(int _edgeID, vector<int> _vecNodeID);

    void addColorID2EdgeID(int _colorID, vector<int> _vecEdgeID);

    const vector<int> &getVecEdgeID() const;

    const map<int, vector<int>> &getMapEdgeID2NodeID() const;

private:
    int ID; //编号 从0开始
    int TxID;
    int RxID;
    double power; //D2D用户发射功率
    double dXPoint; //D2DTx的坐标
    double dYPoint; //D2DTx的坐标

    int degree; //度
    int color; //节点的颜色
    vector<int> vecCandidateColor; //节点候选颜色集
    vector<int> vecEdgeID; //相邻边集
    map<int, vector<int>> mapColorID2EdgeID; //颜色ID——相邻边集
    map<int, vector<int>> mapEdgeID2NodeID; //边ID——相邻节点集
};

/********************************macroUser类*********************************/

/********************************干扰区域超图着色*********************************/

void SLAComputing(map<int, macroUser*> &_mapID2MUEPtr); //干扰区域计算

void macroUserColoring(map<int, macroUser*> &_mapID2MUEPtr, int _colorNum); //给蜂窝用户着色

void SetD2DPair(map<int, macroUser*> _mapID2MUEPtr, map<int, D2DPair*> &_mapID2D2DPairPtr, int _colorNum);

void SetD2DHypergraph(map<int, D2DPair*> _mapID2D2DPairPtr, vector<vector<int>> &_D2DHypergraph);

void D2DHypergraphColoring(map<int, D2DPair*> &_mapID2D2DPairPtr, vector<vector<int>> _D2DHypergraph, int _colorNum);

bool comD2DDegree(D2DPair* _D2DPair1, D2DPair* _D2DPair2);

#endif //HETSNETS_GRAPHCOLORING_H
