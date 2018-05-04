#include <cstring>
#include <stdlib.h>
#include <execinfo.h>

#include <sstream>
#include <iostream>

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
