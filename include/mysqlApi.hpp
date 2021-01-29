#pragma once

#include "config.hpp"
#include <mysql/mysql.h>

class myMysql {
    public:
        myMysql() = default;  
        ~myMysql() = default;
        bool MysqlQuery(const char* sql);   // 执行一条sql语句
        void MysqlClose();                  // 是否数据库操作句柄
        bool MysqlInit();                   // 初始化操作句柄
        MYSQL_RES* MysqlRes();              // 返回数据库查询的结果集
    private:
        static MYSQL* _mysqlFd;
};

