#ifndef __ITEMLIST_H__
#define __ITEMLIST_H__

typedef struct IniItem{
    char* key;
    char* value;
    struct IniItem* next;
}INIITEM;

int ItemADD(INIITEM**,char*,char*);
int ItemDEL(INIITEM**,char*);
INIITEM* GetItemByKey(INIITEM*,char*);
int ItemDestroy(INIITEM**);

#endif
