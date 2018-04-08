#ifndef __COM_EXCEPTION_H__
#define __COM_EXCEPTION_H__

#include <exception>
#include <base_exception.h>

namespace COM{

class ComException : public Global::BaseException{
public:
    ComException(int);
    ComException(std::exception&);
    ~ComException() throw();
};

}

#endif
