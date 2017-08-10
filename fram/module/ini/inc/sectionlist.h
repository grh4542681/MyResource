#ifndef __SECTIONLIST_H__
#define __SECTIONLIST_H__

#include "itemlist.h"

typedef struct IniSection{
    char* section;
    struct IniItem* child;
    struct IniSection* next;
}INISEC;

INISEC* SectionADD(INISEC**,char*);
int SectionItemADD(INISEC*,char*,char*);
int SectionDEL(INISEC**,char*);
INISEC* GetSectionByName(INISEC*,char*);
int SectionDestroy(INISEC**);

#endif
