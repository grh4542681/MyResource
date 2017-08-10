#include <string.h>

#include "log.h"
#include "returnval.h"
#include "mem.h"
#include "itemlist.h"

int ItemADD(INIITEM** head, char* key, char* value)
{
	INIITEM* node = (INIITEM*)Malloc(sizeof(INIITEM));
	if(!node)
	{
		SYSLOG(ERROR,"%s","Malloc Item node error");
		return SYSERROR;
	}
	memset(node,0x00,sizeof(INIITEM));

	node->key = (char*)Malloc(strlen(key)+1);
	if(!(node->key))
	{
		SYSLOG(ERROR,"%s","Malloc Item node->key error");
		Free(node);
		return SYSERROR;
	}
	if(strlen(value)>0)
	{
		node->value = (char*)Malloc(strlen(value)+1);
		if(!(node->value))
		{
			SYSLOG(ERROR,"%s","Malloc Item node->value error");
			Free(node);
			Free(node->key);
			return SYSERROR;
		}
		memset(node->value,0x00,strlen(value));
		strcpy(node->value,value);
	}
	else
	{
		node->value=NULL;
	}
	memset(node->key,0x00,strlen(key));
	strcpy(node->key,key);

	if(!(*head))
	{
		*head = node;
	}
	else
	{
		INIITEM* tmp = *head;
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = node;
	}
	return SUCCESS;
}

int ItemDEL(INIITEM** head, char* key)
{
	INIITEM* tmp = *head;
	INIITEM* tmp_father = NULL;
	int DelNum = 0;
	while(tmp)
	{
		if(!(strcmp(tmp->key,key)))
		{
			if(!tmp_father)
			{
				*head = tmp->next;
				Free(tmp->key);
				if(tmp->value)
					Free(tmp->value);
				Free(tmp);
				tmp = *head;
				DelNum++;
				continue;
			}
			else
			{
				tmp_father->next = tmp->next;
				Free(tmp->key);
				if(tmp->value)
					Free(tmp->value);
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

INIITEM* GetItemByKey(INIITEM* head,char* key)
{
	INIITEM* tmp = head;
	while(tmp)
	{
		if(!strcmp(tmp->key,key))
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

int ItemDestroy(INIITEM** head)
{
	INIITEM* tmp = *head;
	INIITEM* tmp_next = NULL;
	int DelNum = 0;
	
	while(tmp)
	{
		tmp_next = tmp->next;
		//printf("Free item[%s]\n",tmp->key);
		Free(tmp->key);
		if(tmp->value)
			Free(tmp->value);
		Free(tmp);
		tmp = tmp_next;
		DelNum++;
	}
	*head = NULL;
	return DelNum;
}





