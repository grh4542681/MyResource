#include <comm_exception.h>
#include <error_code.h>

namespace Global {

CommException::CommException(int code):std::exception()
{
    this->code = code;
    this->message = exception_message(code);   
}

CommException::CommException(std::exception& e):std::exception(e)
{
    this->code = DEFAULT_ERROR;
    this->message = e.what();
}

CommException::~CommException() throw()
{

}

const char* CommException::what()
{
    return (this->message).c_str();
}

}
