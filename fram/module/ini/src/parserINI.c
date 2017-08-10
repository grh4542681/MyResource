#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "itemlist.h"
#include "sectionlist.h"
#include "parserINI.h"

INISEC* LoadINI(char* filepath)
{
	if(access(filepath,F_OK|R_OK))
	{
		SYSLOG(ERROR,"File[%s] test error [%s]",filepath,strerror(errno));
		return NULL;
	}

	FILE* pfile = fopen(filepath,"r");
	if(!pfile)
	{
		SYSLOG(ERROR,"File[%s] test error [%s]",filepath,strerror(errno));
		return NULL;
	}
	
	INISEC* INIhead = NULL;
	INISEC* INIcurr = NULL;
	char* ptmp1 = NULL;

	char line[INI_MAXLINE];
	char cursection[INI_MAXLINE];
	char curitemkey[INI_MAXLINE];
	char curitemvalue[INI_MAXLINE];
	memset(line,0x00,sizeof(line));
	memset(cursection,0x00,sizeof(cursection));
	memset(curitemkey,0x00,sizeof(curitemkey));
	memset(curitemvalue,0x00,sizeof(curitemvalue));

	while(fgets(line,INI_MAXLINE,pfile))
	{
		if(strlen(line)==0 || line[0]=='\n' || line[0]=='#')
			continue;
		if(line[0]=='[')
		{
			ptmp1 = strchr(line,']');
			if(!ptmp1)
			{
				SYSLOG(ERROR,"Match'[]' error on line:[%s]",line);
				SectionDestroy(&INIhead);
				return NULL;
			}
			else
			{
				// get section
				memset(cursection,0x00,sizeof(cursection));
				memcpy(cursection,line+1,ptmp1-line-1);
				INIcurr = SectionADD(&INIhead,cursection);
				if(!INIhead)
				{
					INIhead = INIcurr;
				}
			}
		}
		else
		{
			if(!INIcurr)
				continue;
			else
			{
				ptmp1 = strchr(line,'=');
				if(!ptmp1)
				{
					SYSLOG(ERROR,"Search '=' error on line:[%s]",line);
					SectionDestroy(&INIhead);
					return NULL;
				}
				else
				{
					// get item
					memset(curitemkey,0x00,sizeof(curitemkey));
					memset(curitemvalue,0x00,sizeof(curitemvalue));
					memcpy(curitemkey,line,ptmp1-line);
					memcpy(curitemvalue,ptmp1+1,strlen(line)-strlen(curitemkey)-1-1);
					SectionItemADD(INIcurr,curitemkey,curitemvalue);	
				}
			}
		}
		
	}
	return INIhead;
}


INISEC* GetSection(INISEC* Sectionhead,char* Section)
{
	return GetSectionByName(Sectionhead,Section);
}

INIITEM* GetSectionItem(INISEC* Sectionhead,char* Section,char* Item)
{
	INISEC* section = GetSectionByName(Sectionhead,Section);
	if(!section)
	{
		SYSLOG(ERROR,"Con't find section [%s]",Section);
		return NULL;
	}
	else
	{
		return GetItemByKey(section->child,Item);
	}
}

void FreeINI(INISEC* Sectionhead)
{
	SectionDestroy(&Sectionhead);
}
