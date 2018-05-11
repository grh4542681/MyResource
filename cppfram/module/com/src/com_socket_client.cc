#include <com_socket_client.h>
#include <com_exception.h>


namespace COM {

ComSocketClient::ComSocketClient()
{

}

ComSocketClient::ComSocketClient(SockInfo* info):ComBaseSocket(info)
{

}

ComSocketClient::~ComSocketClient()
{

}

void ComSocketClient::s_open() throw(ComException)
{

}

void ComSocketClient::s_close() throw(ComException)
{

}

void ComSocketClient::s_read() throw(ComException)
{

}

void ComSocketClient::s_write() throw(ComException)
{

}

}
