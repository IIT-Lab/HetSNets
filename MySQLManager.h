//
// Created by lee on 17-7-27.
//

#ifndef HETSNETS_MYSQLMANAGER_H
#define HETSNETS_MYSQLMANAGER_H

#include "/opt/lampp/include/mysql.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
//#include <strstream>
#include <sstream>

using namespace std;

class MySQLManager
{
public:
    /**
     * Init MySQL
     * @param hosts:         Host IP address
     * @param userName:        Login UserName
     * @param password:        Login Password
     * @param dbName:        Database Name
     * @param port:                Host listen port number
     */
    MySQLManager(std::string hosts, std::string userName, std::string password, std::string dbName, unsigned int port);
    ~MySQLManager();
    void initConnection();
    /**
     * Making query from database
     * @param mysql:        MySQL Object
     * @param sql:                Running SQL command
     */
    bool runSQLCommand(std::string sql);
    /**
     * 执行插入语句
     * @param sql: 执行的SQL语句
     * @return: 受影响的行数
     */
    unsigned int insert(std::string sql);
    /**
     * Destroy MySQL object
     * @param mysql                MySQL object
     */
    void destroyConnection();
    bool getConnectionStatus();
    vector< vector<string> > getResult();
    void clearResultList();
protected:
    void setUserName(std::string userName);
    void setHosts(std::string hosts);
    void setPassword(std::string password);
    void setDBName(std::string dbName);
    void setPort(unsigned int port);
private:
    bool IsConnected;
    vector< vector<string> > resultList;
    MYSQL mySQLClient;
    unsigned int DEFAULTPORT;
    char * HOSTS;
    char * USERNAME;
    char * PASSWORD;
    char * DBNAME;
};

string intToString(int _number);
string doubleToString(double _number);
int string2Int(string _string);
double string2Double(string _string);
double GetLinkloss(int _TxID, int _RxID, int _slotID);
void SetSINR(int _TxID, int _RxID, int _RBID, int _slotID, double _sinr, double _rate);
double GetSumRate(int _slotID);
double GetSinr(int _slotID, int _TxID);
void pushSumRate(int _cueNum, int _D2DNum, int _RBNUM, double _SumRate, int _algorithm); //_algorithm 算法序号 1:普通图 2:超图 3：干扰区域图 4:干扰区域超图
double GetSystemCapacity(int _cueNum, int _D2DNum, int _RBNUM, int _algorithm); //取平均系统容量
/*************统计输出*************/

/*************统计输出*************/

#endif //HETSNETS_MYSQLMANAGER_H
