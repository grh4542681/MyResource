#include <base_exception.h>

namespace Global {

BaseException::BaseException(std::string message):std::exception()
{
    this->message = message;
}

BaseException::BaseException(std::exception& e):std::exception(e)
{
    this->message = e.what();
}

BaseException::~BaseException() throw()
{

}

const char* BaseException::what() const throw()
{
    return ((this->message).c_str());
}

}
