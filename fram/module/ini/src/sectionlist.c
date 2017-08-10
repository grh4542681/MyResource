#include <string.h>

#include "mem.h"
#include "returnval.h"
#include "log.h"
#include "itemlist.h"
#include "sectionlist.h"

INISEC* SectionADD(INISEC** head, char* section)
{
	INISEC* node = (INISEC*)Malloc(sizeof(INISEC));
	if(!node)
	{
		SYSLOG(ERROR,"%s","Malloc Section node error");
		return (INISEC*)SYSERROR;
	}
	memset(node,0x00,sizeof(INISEC));

	node->section = (char*)Malloc(strlen(section)+1);
	if(!(node->section))
	{
		SYSLOG(ERROR,"%s","Malloc Section node->section error");
		return (INISEC*)SYSERROR;
	}
	memset(node->section,0x00,strlen(section));

	strcpy(node->section,section);

	if(!(*head))
	{
		*head = node;
	}
	else
	{
		INISEC* tmp = *head;
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = node;
	}
	return node;
}

int SectionItemADD(INISEC* section,char* key, char* value)
{
	return ItemADD(&(section->child),key,value);
}

int SectionDEL(INISEC** head,char* section)
{
	INISEC* tmp = *head;
	INISEC* tmp_father = NULL;
	int DelNum = 0;

	while(tmp)
	{
		if(!strcmp(tmp->section,section))
		{
			if(!tmp_father)
			{
				*head = tmp->next;
				ItemDestroy(&(tmp->child));
				Free(tmp);
				DelNum++;
				tmp = *head;
				continue;
			}
			else
			{
				tmp_father->next = tmp->next;
				ItemDestroy(&(tmp->child));
				Free(tmp);
				DelNum++;
				tmp = tmp_father->next;
				continue;
			}
		}
		else
		{
			tmp_father = tmp;
			tmp = tmp->next;
		}
	}
	return DelNum;
}

INISEC* GetSectionByName(INISEC* head,char* section)
{
	INISEC* tmp = head;
	while(tmp)
	{
		if(!strcmp(tmp->section,section))
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

int SectionDestroy(INISEC** head)
{
	INISEC* tmp = *head;
	INISEC* tmp_next = NULL;
	int DelNum = 0;

	while(tmp)
	{
		tmp_next = tmp->next;
		ItemDestroy(&(tmp->child));
		//printf("Free section[%s]\n",tmp->section);
		Free(tmp);
		tmp = tmp_next;
		DelNum++;
	}
	return DelNum;
}

