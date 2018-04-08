#include <base_exception.h>
#include <error_code.h>

namespace Global {

BaseException::BaseException(int code):std::exception()
{
    this->code = code;
    this->message = exception_message(code);   
}

BaseException::BaseException(std::exception& e):std::exception(e)
{
    this->code = DEFAULT_ERROR;
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
