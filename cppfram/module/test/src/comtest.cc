#include <com_base_socket.h>
#include <iostream>
#include <exception>
#include <com_exception.h>
#include <string>
#include <base_exception.h>
#include <com_socket_server.h>

#include <com_log.h>
using namespace std;
int main(){

    cout<<"------------------------------"<<__cplusplus<<endl;
    LOGINIT((char*)(std::string("/home/ezgaoro/grh/MyResource/cppfram/etc/log.conf").c_str()));
    COM::SockInfo sockinfo;
    sockinfo.protocol = COM::PROTOCOL::TCP;
    sockinfo.addr = "127.0.0.1";
    sockinfo.port = 8888;
#if 0
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
#endif

    COM::ComSocketServer s(&sockinfo);
    s.s_open();
    //cout<<"status "<<s.status<<endl;
    s.s_accept();

    LOGFREE();

    return (0);
}
