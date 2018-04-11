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
    const char* what() const throw();
    ~BaseException() throw();

private:
    std::string message;
};

}


#endif
