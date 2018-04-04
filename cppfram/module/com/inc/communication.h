#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

class BaseCommunication{
public:
    BaseCommunication();
    ~BaseCommunication();
    virtual void open() throw(...);
    virtual void close() throw(...);
    virtual void write() throw(...);
    virtual void read() throw(...);
}

#endif
