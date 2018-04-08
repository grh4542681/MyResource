#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <com_exception.h>

namespace COM{

class BaseCommunication{
public:
    BaseCommunication();
    ~BaseCommunication();
    virtual void open() throw(ComException);
    virtual void close() throw(ComException);
    virtual void write() throw(ComException);
    virtual void read() throw(ComException);
};

}

#endif
