#ifndef __COM_EXCEPTION_H__
#define __COM_EXCEPTION_H__

#include <exception>
#include <string>
#include <map>

#include <base_exception.h>

namespace Pub{

enum PubErrMark{
    BAD_ARGS = 1,
    UNKNOW_OPTNAME,
    UNKNOW_RUNMODE,
    UNKNOW_PROTOCOL,
    SOCK_INI_ERR,
    BIND_INI_ERR,
    LISTEN_INI_ERR
};

class PubException : public Global::BaseException{
public:
    PubException(std::string);
    PubException(std::exception&);
    PubException(std::string, int);

    ~PubException() throw();
};

extern std::map<std::string, std::string> PubErrCode;

}

#endif
