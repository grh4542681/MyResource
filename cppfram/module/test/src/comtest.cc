#include <com_base_socket.h>
#include <iostream>
#include <exception>
#include <com_exception.h>
#include <string>
#include <base_exception.h>

#include <com_log.h>
using namespace std;
int main(){

    cout<<"------------------------------"<<__cplusplus<<endl;
    LOGINIT((char*)(std::string("/home/ezgaoro/grh/MyResource/cppfram/etc/log.conf").c_str()));
    COM::SockInfo sockinfo;
    sockinfo.mode = COM::MODE::CLIENT;
    sockinfo.protocol = COM::PROTOCOL::TCP;
    sockinfo.addr = "127.0.0.1";
    sockinfo.port = 8888;

    COM::ComBaseSocket sockargs(&sockinfo);

    int a = 3;
    sockargs.sp_sockargs->add_opt(1,1,&a,sizeof(a));
    sockargs.sp_sockargs->print_opt();
    a=5;
    sockargs.sp_sockargs->add_opt(1,10,&a,sizeof(a));
    sockargs.sp_sockargs->print_opt();
    try{
        sockargs.sp_sockargs->del_opt(2);
    } catch(Global::BaseException& e) {
        cout<<e.what()<<endl;
        cout<<e.trace()<<endl;
    }

    LOGFREE();

    return (0);
}
