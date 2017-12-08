//
// Created by lee on 17-7-27.
//
#include "MySQLManager.h"

MySQLManager::MySQLManager(string hosts, string userName, string password, string dbName, unsigned int port)
{
    IsConnected = false;
    this ->setHosts(hosts);            //    设置主机IP地址
    this ->setUserName(userName);            //    设置登录用户名
    this ->setPassword(password);            //    设置登录密码
    this ->setDBName(dbName);            //    设置数据库名
    this ->setPort(port);            //    设置端口号
}

MySQLManager::~MySQLManager()
{
    this ->destroyConnection();
}

void MySQLManager::setDBName(string dbName)
{
    if ( dbName.empty() )
    {//        用户没有指定数据库名
        std::cout << "DBName is null! Used default value: mysql" << std::endl;
        this ->DBNAME = new char[5];
        strcpy(this ->DBNAME, "mysql");
    }
    else
    {
        this ->DBNAME = new char[dbName.length()];
        strcpy(this ->DBNAME, dbName.c_str());
    }
}

void MySQLManager::setHosts(string hosts)
{
    if ( hosts.empty() )
    {//    用户没有指定数据库IP地址
        std::cout << "Hosts is null! Used default value: localhost" << std::endl;
        this ->HOSTS = new char[9];
        strcpy(this ->HOSTS, "localhost");
    }
    else
    {
        this ->HOSTS = new char[hosts.length()];
        strcpy(this ->HOSTS, hosts.c_str());
    }
}

void MySQLManager::setPassword(string password)
{//    用户没有指定密码
    if ( password.empty() )
    {
//        std::cout << "Password is null! Used default value: " << std::endl;
        this ->PASSWORD = new char[1];
        strcpy(this ->PASSWORD, "");
    }
    else
    {
        this ->PASSWORD = new char[password.length()];
        strcpy(this ->PASSWORD, password.c_str());
    }
}

void MySQLManager::setPort(unsigned int port)
{//    用户没有指定端口号，使用默认端口号
    if ( port <= 0 )
    {
        std::cout << "Port number is null! Used default value: 0" << std::endl;
        this->DEFAULTPORT = 0;
    }
    else
    {
        this ->DEFAULTPORT = port;
    }
}

void MySQLManager::setUserName(string userName)
{//    用户没有指定登录用户名
    if ( userName.empty() )
    {
        std::cout << "UserName is null! Used default value: root" << std::endl;
        this ->USERNAME = new char[4];
        strcpy(this ->USERNAME, "root");
    }
    else
    {
        this ->USERNAME = new char[userName.length()];
        strcpy(this ->USERNAME, userName.c_str());
    }
}

void MySQLManager::initConnection()
{
    if ( IsConnected )
    {//    已经连接到服务器
        std::cout << "Is connected to server!" <<std::endl;
        return;
    }
    mysql_init(&mySQLClient);//    初始化相关对象
    if ( !mysql_real_connect( &mySQLClient, HOSTS, USERNAME, PASSWORD, DBNAME, DEFAULTPORT, NULL, 0) )
    {//    连接到服务器
        cout << "HHHHHHHHHHHHH"<<endl;
        std::cout << "Error connection to database: \n" << mysql_error(&mySQLClient) << std::endl;
    }
    IsConnected = true;//    修改连接标识
}

bool MySQLManager::runSQLCommand(string sql)
{
    if ( !IsConnected )
    {//    没有连接到服务器
        std::cout << "Not connect to database!" << std::endl;
        return false;
    }
    if ( sql.empty() )
    {//    SQL语句为空
        std::cout << "SQL is null!" << std::endl;
        return false;
    }

    MYSQL_RES *res;
    MYSQL_ROW row;

    unsigned int i,j = 0;

    i = mysql_real_query(&mySQLClient,sql.c_str(),(unsigned int)strlen(sql.c_str()));//    执行查询
    if ( i < 0 )
    {
        std::cout << "Error query from database: \n" << mysql_error(&mySQLClient) << std::endl;
        return false;
    }
    res = mysql_store_result(&mySQLClient);
    vector<string> objectValue;
    while( (row = mysql_fetch_row(res)) )
    {//    遍历结果集
        objectValue.clear();
        for ( j = 0 ; j < mysql_num_fields(res) ; j++ )
        {
            char temp_char[10];
            strcpy(temp_char, row[j]);
            string temp_string(&temp_char[0],&temp_char[strlen(temp_char)]);
            objectValue.push_back(temp_string);
        }
        this ->resultList.push_back(objectValue);
    }
    mysql_free_result(res);         //free result after you get the result

    return true;
}

unsigned int MySQLManager::insert(std::string sql)
{
    if(!IsConnected) {
        cout << "" << endl;
        return -1;
    }
    if(sql.empty()){
        cout << "sql is null " << endl;
        return -1;
    }
    int rows = -1;
    int res = mysql_query(&mySQLClient, sql.c_str());
    if(res >= 0){
        // 返回受影响的行数
        rows = mysql_affected_rows(&mySQLClient);
//        cout << "Inserted "<< rows << " rows\n";
        return rows;
    } else {
        cout << "Insert error " << mysql_errno(&mySQLClient) << "," << mysql_error(&mySQLClient) << endl;
        return -1;
    }
}

vector< vector<string> > MySQLManager::getResult()
{
    return resultList;
}

void MySQLManager::destroyConnection()
{
    mysql_close(&mySQLClient);
    this ->IsConnected = false;
}

bool MySQLManager::getConnectionStatus()
{
    return IsConnected;
}

void MySQLManager::clearResultList()
{
    this ->resultList.clear();
}

string intToString(int _number)
{
    stringstream ss;
    string s;
    ss << _number;
    ss >> s;
    return s;
}

string doubleToString(double _number)
{
    stringstream ss;
    string s;
    ss << _number;
    ss >> s;
    return s;
}

int string2Int(string _string) {
    stringstream ss;
    int number;
    ss << _string;
    ss >> number;
    return number;
}

double string2Double(string _string) {
    stringstream ss;
    double number;
    ss << _string;
    ss >> number;
    return number;
}

double GetLinkloss(int _TxID, int _RxID, int _slotID) {
    string TxID = intToString(_TxID); //发射机的ID
    string RxID = intToString(_RxID); //接收机的ID
    string slotID = intToString(_slotID);
    double linkLoss = 0;

    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        mysql->clearResultList();
        string SQLString = "SELECT linkLoss FROM channelGain WHERE RxID = " + RxID + " AND TxID = " + TxID + " AND slotID = " + slotID;
        if(mysql->runSQLCommand(SQLString)) {
            vector<vector<string> > result = mysql->getResult();
            for(auto & vec : result) {
                for(auto &str : vec) {
                    string strlinkLoss = str.c_str();
//                    cout << "linkLoss：" << strlinkLoss << endl;;
                    linkLoss = string2Double(strlinkLoss);
                }
            }
        }
        else
            cout << "执行失败" << endl;
    }
    mysql->destroyConnection();
    return linkLoss;
}

void SetSINR(int _RxID, int _RBID, int _slotID, double _sinr, double _rate) {
    string RxID = intToString(_RxID); //接收机的ID
    string RBID = intToString(_RBID); //RBID
    string slotID = intToString(_slotID);
    string sinr = doubleToString(_sinr);
    string rate = doubleToString(_rate);

    MySQLManager *mysql = new MySQLManager("127.0.0.1", "root", "", "platform", (unsigned int)3306);
    mysql->initConnection();
    if(mysql->getConnectionStatus()) {
        string SQLString = "INSERT INTO SINR(RxID, RBID, slotID, sinr, rate) "
                                   "VALUES(" + RxID + "," + RBID + "," + slotID + "," + sinr + "," + rate + ")";
        if(mysql->insert(SQLString)) {
//            cout << "插入成功" << endl;
        }
        else
            cout << "执行失败" << endl;
    }
    mysql->destroyConnection();
}


