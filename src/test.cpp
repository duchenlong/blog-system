#include "test.hpp"

// 配置文件测试
void test_conf() {
    bool ret = read(FILE_CONFIG);
    if(ret == false) return ;

    cout << " [MySQL] " << endl;
    cout << GetString("host") << endl;
    cout << GetInt("port",3306) << endl;
    cout << GetString("user") << endl;
    cout << GetString("password") << endl;
    cout << GetString("name") << endl;

    cout << " [Network]" << endl;
    cout << GetString("listenIp") << endl;
    cout << GetInt("listenPort",19999) << endl;

    cout << " [WWW] " << endl;
    cout << GetString("wwwroot") << endl;
}

// 进程模块测试
void test_proc(int argc, char* argv[]) {
    ProTitle pt(argc,argv);
    bool ret = pt.MoveOsEnv();               // 环境变量搬家
    if(ret == false) {
        cout<< " move os environ error " << endl;
    }

    ret = pt.SetProcTitle(GetCString("proc_name"));    // 设置主进程标题为 master
    if(ret == false) {
        cout << "Cet title error" <<endl;
    }

    for(int i = 0; i < argc; i++)       // 得到指定环境变量
        cout<<pt.GetArgv(i)<<endl;
    
    // 是否以守护进程方式运行
    if( GetInt("isDaemon",0) == 1) {
        pt.SetDaemon();
    }
}