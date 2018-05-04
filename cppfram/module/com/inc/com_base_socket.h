#ifndef __COM_BASE_SOCKET_H__
#define __COM_BASE_SOCKET_H__

#include <string>
#include <map>
#include <memory>

#include <communication.h>
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

typedef struct _sock_info{
    MODE mode;
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

class ComBaseSocket : public BaseCommunication{
private:
    void _open_s();
    void _open_c();
public:
    std::shared_ptr<ComBaseSockArgs> sp_sockargs;
    ComBaseSocket();
    ComBaseSocket(SockInfo*);
    ~ComBaseSocket();

    void open() throw(ComException);
    void close() throw(ComException);
    void write() throw(ComException);
    void read() throw(ComException);

    void change_opt(SockOpt*) throw(ComException);
};

}


#endif
