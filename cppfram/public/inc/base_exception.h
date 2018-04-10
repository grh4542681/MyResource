#ifndef __BASE_EXECPTION_H__
#define __BASE_EXECPTION_H__

#include <exception>
#include <string>

namespace Global{

class BaseException : public std::exception
{
public:
    BaseException(std::string);
    BaseException(std::exception&);
    ~BaseException() throw();
    const char* what();

    static const char* what(int);
private:
    std::string errcode;
    std::string message;
};

}


#endif
