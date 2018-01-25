#include "mem.h"
#include "log.h"

extern char g_calog[1024];

void *Malloc(size_t size)
{
	void *data;
	data=malloc(size);
	if(!data)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(g_calog);
		return 0;
	}
	return data;
}

void *Realloc(void *p,size_t size)
{
	void *data;
	data=realloc(p,size);
	if(!data)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(g_calog);
		return 0;
	}
	return data;
}
