#ifndef __COM_BASE_SOCKET__
#define __COM_BASE_SOCKET__

#include <string>
#include <map>

#include <communication.h>
#include <com_exception.h>

namespace COM{

#define SERVER 1
#define CLIENT 2

#define TCP 1
#define UDP 2

using namespace std;

typedef struct _sock_info{
    int mode;
    int protocol;
    string addr;
    short port;
}SockInfo;

typedef struct _sock_opt{
    int level;
    int optname;
    const void *optval;
}SockOpt;

class ComBaseSockArgs{
private:
    SockInfo info;
    map<int, SockOpt> opt;

public:
    ComBaseSockArgs();
    ComBaseSockArgs(SockInfo*, map<int, SockOpt>*);
    ~ComBaseSockArgs();

    void add_opt(SockOpt*);
    void del_opt(SockOpt*);
};

class ComBaseSocket : public BaseCommunication{
private:
    ComBaseSockArgs sockargs;

public:
    ComBaseSocket();
    ComBaseSocket(ComBaseSockArgs*);
    ~ComBaseSocket();

    void open() throw(ComException);
    void close() throw(ComException);
    void write() throw(ComException);
    void read() throw(ComException);

    void change_opt(SockOpt*) throw(ComException);
};

}


#endif
