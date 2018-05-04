#ifndef __BASE_EXECPTION_H__
#define __BASE_EXECPTION_H__

#include <exception>
#include <string>

#define TRACE_LEN (100)

namespace Global{


class BaseException : public std::exception
{
public:
    BaseException(std::string);
    BaseException(std::exception&);
    const char* what() const throw();
    const char* trace(int depth = 5) noexcept;
    ~BaseException() throw();

private:
    std::string message;
    void *trace_buffer[TRACE_LEN];
    char **trace_string;
    int trace_size;
};

}


#endif
