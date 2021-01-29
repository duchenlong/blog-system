#include "mysqlApi.hpp"

MYSQL* myMysql::_mysqlFd = nullptr;

/*
    MYSQL * mysql_real_connect(MYSQL *mysql, \
            const char *host, const char *user, \
            const char *passwd, const char *db, \
            unsigned int port, const char *unix_socket, unsigned long clientflag)
*/
// 初始化操作句柄
bool myMysql::MysqlInit() {
    if(_mysqlFd) return true;

    // 为数据库操作句柄开辟空间
    _mysqlFd = mysql_init(nullptr);
    if(_mysqlFd == nullptr) {
        LOG() << "create mysql error" << endl;
        return false;
    }


    // 连接MYSQL服务器
    if(mysql_real_connect(_mysqlFd,GetCString("host"),GetCString("user"),\
       GetCString("password"),GetCString("name"),\
       GetInt("port",MYSQL_PORT),nullptr,0) == nullptr) {
           LOG() << "mysql connect error, error is " << mysql_error(_mysqlFd) << endl;
           MysqlClose();
           return false;
       }
    
    // 设置字符集
    if(mysql_set_character_set(_mysqlFd,GetCString("character")) != 0) {
        LOG() << "set mysql character error, error is " << mysql_error(_mysqlFd) << endl;
        MysqlClose();
        return false;
    }
    return true;
}

MYSQL_RES* myMysql::MysqlRes() {
    //查找结果集
    //得到查询的结果集合，是一个二维数组
    return mysql_store_result(_mysqlFd);
}

// 执行sql语句
bool myMysql::MysqlQuery(const char* sql) {
    if(_mysqlFd == nullptr) return false;
    int ret = mysql_query(_mysqlFd,sql);
    if(ret != 0) {
        LOG() << "sql query error, error is " << mysql_error(_mysqlFd) << endl;
        return false;
    }
    return true;
}

// 释放操作句柄
void myMysql::MysqlClose() {
    if(_mysqlFd)
        mysql_close(_mysqlFd);
    _mysqlFd = nullptr;
}
