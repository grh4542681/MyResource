#ifndef __CHILD_MANAGER_H__
#define __CHILD_MANAGER_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <linux/types.h>
#include "list.h"

typedef struct childinfo{
	pid_t childpid;          /*Children pid*/
	pid_t parentpid;         /*Parent's pid*/
	char status;	             /*Process status*/
	int pipefd;             /*Descriptor of communication with father for write*/
	time_t createtime;       /*Process create time*/
	struct list_head c_list;   /*List struct*/
}CHILDINFO;

typedef struct createinfo{
	char childnum[1];          /*Child process number*/
	char childpath[128];       /*Child process name*/
	int semkey;
	int memkey;
	int msgkey;
}CREATEINFO;

#define childinfo_each(pcurr,head) \
	list_for_each_entry(pcurr, head, c_list)

#define UNREGISTER '0'
#define READY '1' 
#define WORK '2'
#define DEAD '3'

#endif