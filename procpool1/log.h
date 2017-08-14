#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

#define LEVEL_SYS 0
#define LEVEL_APP 1

#define SYSLOGDIR "./"
#define APPLOGDIR "./"

#define SYSLOG(str) \
		Log(__FILE__,__LINE__,str,LEVEL_SYS);

	
char g_calog[1024];

#endif
