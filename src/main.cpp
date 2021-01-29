#include "test.hpp"
#include "server.h"

int main(int argc, char* argv[])
{
    test_conf();
    test_proc(argc,argv);

    //初始化是三个表格的类
    if( !TableInit()) {
        LOG() <<  " mysql init error" << std::endl;
        return 0;
    }

    if( HttpLink() == false) {
        LOG() << " http server set error" << std::endl;
        return 0;
    }
    return 0;
}
