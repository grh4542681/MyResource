#ifndef __COM_LOG_H__
#define __COM_LOG_H__

#include <log.h>

#define COMLOG(level,format,...)    \
            SYSLOG(level,format,__VA_ARGS__)

#endif
