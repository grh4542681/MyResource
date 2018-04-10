#ifndef __COM_BASE_SOCKET_H__
#define __COM_BASE_SOCKET_H__

#include <string>
#include <map>
#include <memory>

#include <communication.h>
#include <com_exception.h>

namespace COM{

#define SERVER 1
#define CLIENT 2

#define TCP 1
#define UDP 2

typedef struct _sock_info{
    int mode;
    int protocol;
    std::string addr;
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
    std::map<int, SockOpt> opt;

public:
    ComBaseSockArgs(SockInfo*);
    ~ComBaseSockArgs();

    void add_opt(int, int, const void*, unsigned int) throw(ComException);
    void del_opt(SockOpt*) throw(ComException);
};

class ComBaseSocket : public BaseCommunication{
private:
    std::shared_ptr<ComBaseSockArgs> sp_sockargs;

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
