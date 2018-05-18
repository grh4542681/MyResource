#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <execinfo.h>
#include <stdarg.h>

#include <sstream>
#include <iostream>
#include <vector>
#include <boost/format.hpp>

#include <base_exception.h>

namespace Global {

BaseException::BaseException(std::string message):std::exception()
{
    this->message = message;

    memset(this->trace_buffer, 0x00, TRACE_LEN);
    this->trace_string = NULL;
    this->trace_size = backtrace(trace_buffer, TRACE_LEN);
    this->trace_string = backtrace_symbols(this->trace_buffer, this->trace_size);
}

BaseException::BaseException(std::exception& e):std::exception(e)
{
    this->message = e.what();

    memset(this->trace_buffer, 0x00, TRACE_LEN);
    this->trace_string = NULL;
    this->trace_size = backtrace(trace_buffer, TRACE_LEN);
    this->trace_string = backtrace_symbols(this->trace_buffer, this->trace_size);
}

BaseException::BaseException(const char* format, ...)
{
    va_list args;
    va_start(args ,format);
    int len = vsnprintf(NULL, 0, format, args);
    if (len > 0) {
        std::vector<char> buf(len+1);
        vsprintf(&buf.front(), format, args);
        this->message.assign(buf.begin(), buf.end() - 1);
    } else {
        this->message = "";
    }
    va_end(args);

    memset(this->trace_buffer, 0x00, TRACE_LEN);
    this->trace_string = NULL;
    this->trace_size = backtrace(trace_buffer, TRACE_LEN);
    this->trace_string = backtrace_symbols(this->trace_buffer, this->trace_size);
}

BaseException::BaseException(std::string message, int mark):std::exception()
{
    this->mark = mark;
    this->message = message;

    memset(this->trace_buffer, 0x00, TRACE_LEN);
    this->trace_string = NULL;
    this->trace_size = backtrace(trace_buffer, TRACE_LEN);
    this->trace_string = backtrace_symbols(this->trace_buffer, this->trace_size);
}


BaseException::~BaseException() throw()
{
    if (this->trace_string != NULL) {
        free(this->trace_string);
    }
}

const char* BaseException::what() const throw() 
{
    return ((this->message).c_str());
}

const char* BaseException::trace(int depth) noexcept
{
    if (this->trace_string == NULL) {
        return (NULL);
    }
    std::string trace_stream;
    int size = depth < this->trace_size ? depth : this->trace_size;
    for (int i = 0; i < size; ++i) {
        trace_stream += trace_string[i];
        trace_stream += "\n";
    }
    return (trace_stream.c_str());
}

}
