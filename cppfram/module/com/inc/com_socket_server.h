#ifndef __COM_SOCKET_SERVER_H__
#define __COM_SOCKET_SERVER_H__

#include <list>

#include <thread_safe_queue.h>
#include <com_base_socket.h>
#include <com_sockfd_op.h>
#include <com_message.h>

namespace COM {

class ComSocketServer : public ComBaseSocket{
private:
    Pub::ThreadSafeQueue<ComMessage> recv_q;
    Pub::ThreadSafeQueue<ComMessage> send_q;

    void monitor_thread_handler();
    void recv_thread_handler();
    void send_thread_handler();
    void listen_thread_handler();
public:
    mutable STATUS status;
    mutable int max_clients;
    mutable int cur_clients;
    std::list<ComSockFdOp> clients;

    ComSocketServer(SockInfo* info, int maxclients = 100);
    ~ComSocketServer();

    void s_open() throw(ComException);
    ComSockFdOp* s_accept() throw(ComException);
    void c_close(ComSockFdOp*) throw(ComException);
    void s_close() throw(ComException);

    void start() throw(ComException);
    void pop() throw(ComException);
    void push() throw(ComException);

};

}


#endif
