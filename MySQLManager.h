//
// Created by lee on 17-7-27.
//

#ifndef HETSNETS_MYSQLMANAGER_H
#define HETSNETS_MYSQLMANAGER_H

#include "/opt/lampp/include/mysql.h"

#include <string>
#include <iostream>
#include <vector>
#include <string.h>

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

#endif //HETSNETS_MYSQLMANAGER_H
