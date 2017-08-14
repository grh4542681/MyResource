#ifndef __CHILD_MONITOR_MAIN_H__
#define __CHILD_MONITOR_MAIN_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "child_op.h"
#include "list.h"

#define SYSOVERTIME 60
#define ALARMTIME 10
#define MINCHILDNUM 5
#define MAXCHILDNUM 20

typedef struct poolusage{
	float usagepercent;
	struct list_head p_list;
}POOLUSAGE;

#define poolusage_each(pcurr,head) \
	list_for_each_entry(pcurr, head, p_list)

void sigchld_func(int signo);
void sigalrm_func(int signo);
void siguser1_func(int signo);
void kill_all_children();
void deal_parent_msg(char *command);
void deal_child_msg(struct childinfo* pchildinfo, char *command);
void do_monitor();
void child_init(int childnum);

#endif