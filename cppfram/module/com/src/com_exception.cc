#include <com_exception.h>
#include <stdarg.h>

namespace COM{
std::map<std::string, std::string> ComErrCode = { 
    { "BAD_ARGS", "Bad arguments" },
    { "UNKNOW_OPTNAME", "Unknow optname" },
    { "UNKNOW_RUNMODE", "Unknow run mode" },
    { "UNKNOW_PROTOCOL", "Unknow procotol" },
    { "SOCK_INI_ERR", "Socket get fd error" },
    { "BIND_INI_ERR", "Bind socket error" },
    { "LISTEN_INI_ERR", "Listen socket error" },
    { "BAD_STATUS", "Bad status" },
    { "SOCK_ACCEPT_ERR", "Socket fd accept error"}
};

ComException::ComException(std::string errmark):Global::BaseException(errmark)
{

}

ComException::ComException(std::exception& e):Global::BaseException(e)
{

}

ComException::ComException(std::string message, int mark):Global::BaseException(message, mark)
{

}

ComException::~ComException() throw()
{

}

}
