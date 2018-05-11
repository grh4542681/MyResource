#ifndef __COM_SOCKET_SERVER_H__
#define __COM_SOCKET_SERVER_H__

#include <list>

#include <com_base_socket.h>
#include <com_sockfd_op.h>

namespace COM {

class ComSocketServer : public ComBaseSocket{
private:
    void monitor_handler();
public:
    STATUS status;
    int max_clients;
    int cur_clients;
    std::list<ComSockFdOp> clients;

    ComSocketServer(SockInfo* info, int maxclients = 100);
    ~ComSocketServer();
    //heartbeat handler

    void s_open() throw(ComException);
    ComSockFdOp* s_accept() throw(ComException);
    void c_close(ComSockFdOp*) throw(ComException);
    void s_close() throw(ComException);
};

}


#endif
