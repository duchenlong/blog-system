#pragma once

#include <string.h>         // memset
#include <stdio.h>          // printf
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <fstream>
#include <iostream>

// 默认配置文件名称
#define FILE_CONFIG "my.conf"
// 默认监听端口号
#define LISTEN_PORT 19999
// 默认数据库连接端口号
#define MYSQL_PORT 3306

// #define EXIT_SUCCESS 0  正常退出
// #define EXIT_FAILURE 1  异常退出
#define ERR_EXIT(err) \
do { \
    perror(err); \
    exit(EXIT_FAILURE); \
} while ( 0 );

// 获取时间戳
class TimeUtil {
    public:
        static int64_t GetTimeStampMs() {
            struct timeval tv;
            gettimeofday(&tv,NULL);
            return tv.tv_sec + tv.tv_usec / 1000;
        }

        static std::string GetTimeStamp() {
            time_t st;
            time(&st);
            struct tm* t = localtime(&st);

            char buf[30] = {'\0'};
            snprintf(buf,sizeof buf - 1, "%04d-%02d-%02d %02d:%02d:%02d",\
                    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, \
                    t->tm_hour, t->tm_min, t->tm_sec);
            return buf;
        }
};

inline std::ostream& Log(const char* file,int line) {
    std::cout << "[ " << TimeUtil::GetTimeStamp() \
            << " " << file << " " << line << " ";
    return std::cout;
}

#define LOG() Log(__FILE__,__LINE__)
