#ifndef __COM_EXCEPTION_H__
#define __COM_EXCEPTION_H__

#include <exception>
#include <string>
#include <map>

#include <base_exception.h>

namespace COM{

enum ComErrMark{
    BAD_ARGS = 1,
    UNKNOW_OPTNAME,
    UNKNOW_RUNMODE,
    UNKNOW_PROTOCOL,
    SOCK_INI_ERR,
    BIND_INI_ERR,
    LISTEN_INI_ERR
};

class ComException : public Global::BaseException{
public:
    ComException(std::string);
    ComException(std::exception&);
    ComException(std::string, int);

    ~ComException() throw();
};

extern std::map<std::string, std::string> ComErrCode;

}

#endif
