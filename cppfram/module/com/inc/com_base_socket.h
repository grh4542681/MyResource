#ifndef __COM_BASE_SOCKET_H__
#define __COM_BASE_SOCKET_H__

#include <string>
#include <map>
#include <memory>

#include <com_exception.h>

namespace COM{

enum class MODE:unsigned int{
    SERVER = 1,
    CLIENT
};

enum class PROTOCOL:unsigned int{
    TCP = 1,
    UDP
};

enum class STATUS:unsigned int{
    S_UNUSE = 1,
    S_INIT,
    S_ACCEPT,
    S_READY,
    S_CLOSE,

    C_READY,

    F_READY,
    F_CLOSE,
    F_READBALE,
    F_WRITEBALE,
};

typedef struct _sock_info{
    PROTOCOL protocol;
    std::string addr;
    short port;
}SockInfo;

typedef struct _sock_opt{
    int level;
    int optname;
    const void *optval;
}SockOpt;

class ComBaseSockArgs{
public:
    SockInfo info;
    std::map<int, SockOpt> opt;

    ComBaseSockArgs(SockInfo*);
    ~ComBaseSockArgs();

    void add_opt(int, int, const void*, unsigned int) throw(ComException);
    void del_opt(int) throw(ComException);
    void print_opt(void) noexcept;
};

class ComBaseSocket{
public:
    int sockfd;
    std::shared_ptr<ComBaseSockArgs> sp_sockargs;
    ComBaseSocket();
    ComBaseSocket(SockInfo*);
    ~ComBaseSocket();

    virtual void s_open() throw(ComException) {};
    virtual void s_close() throw(ComException) {};

    void change_opt(SockOpt*) throw(ComException);
};

}


#endif
