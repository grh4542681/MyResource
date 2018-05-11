#include <com_sockfd_op.h>

namespace COM {

ComSockFdOp::ComSockFdOp()
{

}

ComSockFdOp::ComSockFdOp(int fd)
{
    this->fd = fd;
}

ComSockFdOp::~ComSockFdOp()
{

}

void ComSockFdOp::s_read() throw(ComException)
{

}

void ComSockFdOp::s_write() throw(ComException)
{

}

}
