#include <pub_exception.h>
#include <stdarg.h>

namespace Pub{
std::map<std::string, std::string> PubErrCode = { 
    { "BAD_ARGS", "Bad arguments" },
    { "UNKNOW_OPTNAME", "Unknow optname" },
    { "UNKNOW_RUNMODE", "Unknow run mode" },
    { "UNKNOW_PROTOCOL", "Unknow procotol" },
    { "SOCK_INI_ERR", "Socket get fd error" },
    { "BIND_INI_ERR", "Bind socket error" },
    { "LISTEN_INI_ERR", "Listen socket error" }
};

PubException::PubException(std::string errmark):Global::BaseException(errmark)
{

}

PubException::PubException(std::exception& e):Global::BaseException(e)
{

}

PubException::PubException(std::string message, int mark):Global::BaseException(message, mark)
{

}

PubException::~PubException() throw()
{

}

}
