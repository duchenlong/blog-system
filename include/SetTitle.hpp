#pragma once

#include "head.hpp"
#include "config.hpp"

#include <fcntl.h>          // open
#include <sys/types.h>      // umask
#include <sys/stat.h>       // umask

extern char** environ;

class ProTitle {
public:
    ProTitle(const int argc,char** argv);   // 初始化
    bool MoveOsEnv();                       // 环境变量搬移
    bool SetProcTitle(const char* name);    // 设置进程标题
    char* GetArgv(const int idx);           // 获取指定位置的命令行参数
    bool SetDaemon();                       // 设置主进程以守护进程运行
private:
    char**  _osArgv;        // 系统命令行参数的开始位置
    int     _osArgc;        // 系统命令行参数的数量
    char*   _envMove;       // 搬移后环境变量的位置
    int     _envMoveLen;    // 搬移后环境变量的大小
};