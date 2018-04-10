#ifndef __COM_LOG_H__
#define __COM_LOG_H__

extern "C" {
#include <log.h>
}
#define COMLOG(level,format,msg...)    \
            SYSLOG(level,format,msg)

#endif
