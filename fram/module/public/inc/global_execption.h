#ifndef __GLOBAL_EXECPTION_H__
#define __GLOBAL_EXECPTION_H__

#include <execption>

class GlobalExecption : public std::execption
{
    GlobalExecption();
    GlobalExecption(char* format, ...);
    ~GlobalExecption();
}


#endif
