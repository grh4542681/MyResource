#include "pub.h"
#include "xml.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

extern char g_caTrcMsg[1024];

int xmlUnPack(char* pcaXmlData,XmlList** ppstXmlList)
{

	xmlDocPtr pDoc=NULL;
	xmlNodePtr pRoot=NULL;
	XmlList* pList;

	char *pcaData=NULL;

	char caEncode[10];
	memset(caEncode,0,sizeof(caEncode));


	xmlGetCode(pcaXmlData,caEncode);
	printf("%s\n",caEncode);
	
/*
	if(strcmp(caEncode,"UTF-8"))
	{
		code2u(pcaXmlData,strlen(pcaXmlData),&pcaData,caEncode);
		if(!pcaData)
		{
			sprintf(g_caTrcMsg,"Encode change error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg);
			return -1;
		}
		printf("xml[%s\n]",pcaData);
	}
	else
	{
		pcaData=pcaXmlData;
	}
*/
	
	pcaData=pcaXmlData;

	pDoc=xmlReadMemory(pcaData,strlen(pcaData)+1,NULL,caEncode,0);
	if(!pDoc)
	{
		sprintf(g_caTrcMsg,"Parse Xml error FILE[%s] LINE[%s]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg);
		xmlFreeDoc(pDoc);
		return ;
	}

	pRoot=xmlDocGetRootElement(pDoc);
	if(!pRoot)
	{
		sprintf(g_caTrcMsg,"Parse Xml error FILE[%s] LINE[%s]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg);
		xmlFreeDoc(pDoc);
		return ;
	}
	printf("RootNode=[%s] \n",BAD_CAST(pRoot->name));
	FillNode(pRoot,&pList,ROOT);

	if(xmlParseChild(pRoot->xmlChildrenNode,&pList->pchild))
	{
		sprintf(g_caTrcMsg,"xmlParseChild error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg);
		xmlFreeDoc(pDoc);
		return -1;
	}
/*
	if(strcmp(caEncode,"UTF-8"))
	{
		free(pcaData);
	}
	*/
	xmlFreeDoc(pDoc);
	*ppstXmlList=pList;

	return 0;	
}


int xmlParseChild(xmlNodePtr pnode,XmlList** ppstXmlList)
{
	xmlChar* value;
	xmlAttrPtr attrPtr;
	XmlList** pList=ppstXmlList;
	char caAttr[1024];
	if(!pnode)
		return 0;	
	memset(caAttr,0,sizeof(caAttr));

	if(pnode->xmlChildrenNode==NULL)
	{
		/*
		xmlParseAttr(pnode,ATTRNAME,caAttr);
		value=xmlNodeGetContent(pnode);
		if(!value)
		{
			sprintf(g_caTrcMsg,"xmlNodeGetContent error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg);
			return -1;
		}
		printf("key=[%s] value[%s] Attr[%s] Type[%d]\n",BAD_CAST(pnode->name),BAD_CAST(value),caAttr,pnode->type);
		xmlFree(value);
	*/
		FillNode(pnode,pList,CHILD);
		xmlParseChild(pnode->next,&((*pList)->pnext));
	}
	else if(pnode->xmlChildrenNode->next==NULL)
	{
/*
		xmlParseAttr(pnode,ATTRNAME,caAttr);
		value=xmlNodeGetContent(pnode);
		if(!value)
		{
			sprintf(g_caTrcMsg,"xmlNodeGetContent error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg);
			return -1;
		}
		printf("key=[%s] value[%s] Attr[%s] Type[%d]\n",BAD_CAST(pnode->name),BAD_CAST(value),caAttr,pnode->type);
		xmlFree(value);
	*/
		FillNode(pnode,pList,CHILD);
		xmlParseChild(pnode->next,&((*pList)->pnext));
	}
	else
	{
		/*
		xmlParseAttr(pnode,ATTRNAME,caAttr);
		printf("key=[%s] Attr[%s] Type[%d] has children\n",BAD_CAST(pnode->name),caAttr,pnode->type);
		*/
		FillNode(pnode,pList,PARENT);
		if(xmlParseChild(pnode->xmlChildrenNode,&((*pList)->pchild)))
		{
			sprintf(g_caTrcMsg,"xmlParseChild error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg);
			return -1;
		}
		if(xmlParseChild(pnode->next,&((*pList)->pnext)))
		{
			sprintf(g_caTrcMsg,"xmlParseChild error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg);
			return -1;
		}
	}
	return 0;
}

int xmlParseAttr(xmlNodePtr pnode,char* caAttrname,char* caAttrvalue)
{
	xmlAttrPtr attrPtr;
	xmlChar* szAttr;
	attrPtr = pnode->properties;
	if (xmlHasProp(pnode,BAD_CAST(caAttrname)))
	{	
		while (attrPtr != NULL)
		{
			if (!xmlStrcmp(attrPtr->name, BAD_CAST(caAttrname)))
			{
				szAttr = xmlGetProp(pnode,BAD_CAST(caAttrname));
				strcpy(caAttrvalue,BAD_CAST(szAttr));
				xmlFree(szAttr);
				break;
			}
			attrPtr = attrPtr->next;
		}
		return 0;
	}
	return -1;
}

int FillNode(xmlNodePtr pnode,XmlList** pstXmlList,int iNodeType)
{


	char SysEnv[20];
	int CodeFlag;
	xmlAttrPtr attrPtr;
	XmlList* plist;
	char caAttr[1024];
	xmlChar* value;
	//char* p2code;
	char* p2code=(char*)malloc(1024);
	int outlen=1024;
	int inlen;

	plist=(XmlList*)malloc(sizeof(struct XmlList));
	
	memset(plist,0x00,sizeof(struct XmlList));
	memset(caAttr,0x00,sizeof(caAttr));

	ENV(SysEnv);
	
	if(strstr(SysEnv,"2312"))
		CodeFlag=1;
	else
		CodeFlag=0;

	switch (iNodeType)
	{
		case ROOT:
			plist->iNodeType=iNodeType;
			strcpy(plist->caKey,BAD_CAST(pnode->name));
			break;
		case PARENT:
			plist->iNodeType=iNodeType;
			strcpy(plist->caKey,BAD_CAST(pnode->name));
			if(!xmlParseAttr(pnode,ATTRNAME,caAttr))
			{
				strcpy(plist->caAttrname,ATTRNAME);
				/*if(CodeFlag)
				{
					inlen=sizeof(plist->caAttrvalue);
					printf("sizeof[%d]\n",inlen);
					u2g(BAD_CAST(caAttr),inlen,p2code,outlen);
					strcpy(plist->caAttrvalue,p2code);
				}
				else*/
					strcpy(plist->caAttrvalue,BAD_CAST(caAttr));
			}	
			break;
		case CHILD:
			plist->iNodeType=iNodeType;
			strcpy(plist->caKey,BAD_CAST(pnode->name));
			value=xmlNodeGetContent(pnode);
			if(value)
			{
				/*if(CodeFlag)
				{

					inlen=sizeof(plist->caValue);
					u2g(BAD_CAST(value),inlen,p2code,outlen);
					strcpy(plist->caValue,p2code);
				}
				else*/
					strcpy(plist->caValue,BAD_CAST(value));
				xmlFree(value);
			}
			
			if(!xmlParseAttr(pnode,ATTRNAME,caAttr))
			{
				strcpy(plist->caAttrname,ATTRNAME);
				
				/*if(CodeFlag)
				{
					inlen=sizeof(plist->caAttrvalue);
					u2g(BAD_CAST(caAttr),inlen,p2code,outlen);
					strcpy(plist->caAttrvalue,p2code);
				}
				else*/
					strcpy(plist->caAttrvalue,BAD_CAST(caAttr));
			}
			break;
		default:
			sprintf(g_caTrcMsg,"Unknow Node type FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg );
			free(plist);
			return -1;
	}
	free(p2code);
	*pstXmlList=plist;
	return 0;
}




int xmlGetCode(char* pcaData,char* caencode)
{
	char* catemp1;
	char* catemp2;
	
	catemp1=strstr(pcaData,"encoding");
	if(!catemp1)
	{
		printf("can\'t find encoding\n");
		return -1;
	}
	catemp2=strstr(catemp1+10,"\"");
	strncpy(caencode,catemp1+10,catemp2-(catemp1+10));
	return 0;
}


