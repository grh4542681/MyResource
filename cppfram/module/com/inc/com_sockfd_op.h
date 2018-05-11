#ifndef __COM_SOCKFD_OP_H__
#define __COM_SOCKFD_OP_H__

#include <unistd.h>

#include <com_exception.h>
#include <com_base_socket.h>

namespace COM {

class ComSockFdOp {
public:
    int fd;
    std::string ip;
    int port;
    STATUS status;

    ComSockFdOp();
    ComSockFdOp(int);
    ~ComSockFdOp();

    void s_read() throw(ComException);
    void s_write() throw(ComException);
};

}



#endif
