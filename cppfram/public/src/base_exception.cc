#include <base_exception.h>
#include <error_code.h>

namespace Global {

BaseException::BaseException(std::string errcode):std::exception()
{
    this->errcode = errcode;
    this->message = exception_message(errcode);   
}

BaseException::BaseException(std::exception& e):std::exception(e)
{
    this->errcode = DEFAULT_ERROR;
    this->message = e.what();
}

BaseException::~BaseException() throw()
{

}

const char* BaseException::what()
{
    return ((this->message).c_str());
}

}
