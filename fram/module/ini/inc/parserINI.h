#ifndef __PARSERINI_H__
#define __PARSERINI_H__

#include "sectionlist.h"
#include "itemlist.h"

#define INI_MAXLINE 1024

INISEC* LoadINI(char*);
INISEC* GetSection(INISEC*,char*);
INIITEM* GetSectionItem(INISEC*,char*,char*);
void FreeINI(INISEC*);

#endif
