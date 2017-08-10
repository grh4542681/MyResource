#ifndef __HEARTBEAT_H__
#define __HEARTBEAT_H__

typedef struct MachineInfo{
	char name[32];
	char ip[16];
	int stat;
	int factor;
}MCHINF;

#define ONLINE 1
#define OFFLINE	2

#define CACHESIZE 1024*1024
#define LISTENMAXNUM 1024


#endif
