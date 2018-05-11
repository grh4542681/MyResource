#ifndef __COM_SOCKET_CLIENT_H__
#define __COM_SOCKET_CLIENT_H__

#include <com_base_socket.h>
#include <com_exception.h>
#include <com_sockfd_op.h>

namespace COM {

class ComSocketClient : public ComBaseSocket{
public:
    ComSockFdOp client;

    ComSocketClient();
    ComSocketClient(SockInfo*);
    ~ComSocketClient();

    void s_open() throw(ComException);
    void s_close() throw(ComException);
    void s_read() throw(ComException);
    void s_write() throw(ComException);
};

}

#endif
