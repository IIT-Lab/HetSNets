//
// Created by lee on 17-9-26.
//

#ifndef HETSNETS_INTERFERENCEINDEX_H
#define HETSNETS_INTERFERENCEINDEX_H

#include <set>
#include <map>
#include <vector>

using namespace std;

class InterferenceIndex
{
protected:
    InterferenceIndex(int iSlotN);
private:
    static InterferenceIndex* _Instance;//单件模式的对象
public:
    static InterferenceIndex& GetInstance();//获取单件模式的对象
    //~InterferenceIndex();

    //void RFRegister(int const iEquipmentID, int  iSlotID, set<int> &sRFOccupation);
    //分RB填充所有有影响的用户ID，输入中有map<int（时隙，set<int>(用户ID)>
    void RFRegisterOneRB(int const iEquipmentID, int  iSlotID, int RB);
    //分单个RB填充所有有影响的用户ID

    //获取对某时隙某RB上所有用户的ID
    void GetRelevantUserTxRB(int const iRBIndex, int  iSlotID, set<int> &sRelevantUserList);
    void GetRelevantUserRxRB(int const iRBIndex, int  iSlotID, set<int> &sRelevantUserList);

    //存储时隙占用表的大容器，最外层vector的下标是时隙编号，第二维map的int index是子载波编号，set内部是所有发射机的编号
    vector<map<int, set<int>>> vTxOccupationList;
    vector<map<int, set<int>>> vRxOccupationList;
    int iSlotNum;
    map<int, map<int, set<int>>>randomRBList;                               //时隙号下，某发射机号下，对应的RB列表
    set<int>RBList;

    //
    void RFRxRegisterOneRB(int const iEquipmentID, int  iSlotID, int RB);
};


#endif //HETSNETS_INTERFERENCEINDEX_H
