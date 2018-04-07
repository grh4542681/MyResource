#ifndef __COM_EXCEPTION_H__
#define __COM_EXCEPTION_H__

class ComException : public Global::CommException{
public:
    ComException(int);
    ComException(std::exception&);
    ~ComException() throw();
}

#endif
