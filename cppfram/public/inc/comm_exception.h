#ifndef __COMM_EXECPTION_H__
#define __COMM_EXECPTION_H__

#include <exception>
#include <string>

namespace Global{

class CommException : public std::exception
{
public:
    CommException(int);
    CommException(std::exception&);
    ~CommException() throw();
    const char* what();

    static const char* what(int);
private:
    int code;
    std::string message;
};

}


#endif
