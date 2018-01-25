#ifndef __XML_H__
#define __XML_H__

typedef struct XmlList
{
	int iNodeType;
	char caAttrname[20];
	char caAttrvalue[1024];
	char caKey[20];
	char caValue[1024];
	struct XmlList *pnext;
	struct XmlList *pchild;
}XmlList;

void PrintXmlList(XmlList*);


#define LISTLEN 1024

#define HEAD 0
#define ROOT 1
#define PARENT 2
#define CHILD 3

#define KEY 1
#define ATTR 2

#define ATTRNAME "id"

#define ENV(a) 	\
{ \
	memset(a,0x00,sizeof(a)); \
	sprintf(a,"%s",getenv("LANG")); \
}

XmlList* g_pstNodeList[LISTLEN];

int xmlGetValueByPath(XmlList*,char*,char*);
int xmlGetListByKey(XmlList* ,char* );
int xmlGetValueByKey(XmlList* ,char* ,char* ,int );
int xmlGetListByAttr(XmlList* ,char* ,char* );
int xmlFindNode(XmlList* ,char* ,char* ,XmlList*** ,int* ,int );
int xmlFindPathNode(XmlList* ,char* ,char* ,XmlList*** ,int* );
void xmlFreeList(XmlList* );
void xmlPrintList(XmlList* );
void xmlPrintList4F(XmlList* );


#endif


