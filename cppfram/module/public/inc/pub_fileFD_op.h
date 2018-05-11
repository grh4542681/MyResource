#ifndef __BASE_FILEFD_H__
#define __BASE_FILEFD_H__

#include <pub_exception.h>

namespace Pub {

class PubFileFDOp{
private:
    int fd;
public:
    int status;

    void open() throw(PubException);
    void close() throw(PubException);
    void read() throw(PubException);
    void write() throw(PubException);
}

}

#endif
