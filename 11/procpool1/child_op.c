#include "child_op.h"
#include "log.h"
#include "mem.h"
#include "list.h"

extern char g_calog[1024];

/**
 * [childinfo_init description]
 * Initialize childinfo list
 * @param pchildinfo [Pointer of childinfo node]
 */
void childinfo_init(struct childinfo *pchildinfo)
{
	INIT_LIST_HEAD(&(pchildinfo->c_list));
}

/**
 * [childinfo_add description]
 * Add a childinfo node into list
 * @param  head       [Pointer of list head node]
 * @param  pchildinfo [Pointer of child node that will be added]
 * @return            [Success return 0,failed return -1]
 */
int childinfo_add(struct list_head *head,struct childinfo *pchildinfo)
{
	struct childinfo *new;
	new=Malloc(sizeof(struct childinfo));
	if(!new)
	{
		strcpy(g_calog,"Malloc error");
		SYSLOG(g_calog);
		return -1;
	}
	memcpy(new,pchildinfo,sizeof(struct childinfo));
	list_add(&(new->c_list),head);
	return 0;
}

/**
 * [childinfo_del description]
 * Delete a childinfo node from list
 * @param pchildinfo [Pointer of childinfo node that will be delete]
 */
void childinfo_del(struct childinfo *pchildinfo)
{
	list_del(&(pchildinfo->c_list));
	free(pchildinfo);
}

/**
 * [childinfo_searchpid description]
 * Search the match node by childpid
 * @param  head [Pointer of list head node]
 * @param  pid  [Childprocess Pid]
 * @return      [Success return Pointer of match child node,failed return -1]
 */
struct childinfo *childinfo_searchpid(struct list_head *head,pid_t pid)
{
	struct childinfo *pcurr;
	list_for_each_entry(pcurr, head, c_list)
	{
		if(pcurr->childpid==pid)
			return pcurr;
	}
	return NULL;
}

