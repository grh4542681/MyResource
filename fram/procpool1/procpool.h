#ifndef __PROCPOOL_H__
#define __PROCPOOL_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <linux/types.h>
#include "list.h"

typedef struct ppoolinfo{
	pid_t createrid;
	pid_t managerid;
	time_t createtime;
	float idle;
	char childpath[128];
	char childname[32];
	int childnum;
	int freechildnum;
	char status;
}PPOOLINFO;

/** command from parent */
#define REPORT 'p'
#define DESTROY 'k'
#define RESTART 'r'

/** Status */
#define PUNREGISTER '0'
#define PREADY '1'
#define PWORK '2'

#define MANAGERDIR "./cmonitor"
#define MANAGERNAME "cmonitor"

//int procpool_create(struct childinfo *pmanagerinfo,char *childpath,int procnum,int overtime);

#endif