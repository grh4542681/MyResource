#include <com_base_socket.h>
#include <iostream>
#include <string>

#include <com_log.h>
using namespace std;
int main(){

    cout<<"------------------------------"<<endl;
    LOGINIT((char*)(std::string("/home/ezgaoro/grh/MyResource/cppfram/etc/log.conf").c_str()));
    COM::SockInfo sockinfo;
    sockinfo.mode = CLIENT;
    sockinfo.protocol = TCP;
    sockinfo.addr = "127.0.0.1";
    sockinfo.port = 8888;

    COM::ComBaseSockArgs sockargs(&sockinfo);

    int a = 3;
    sockargs.add_opt(1,1,&a,sizeof(a));
    sockargs.del_opt(NULL);
    a=5;
    sockargs.add_opt(1,10,&a,sizeof(a));
    sockargs.del_opt(NULL);


    LOGFREE();

    return (0);
}
