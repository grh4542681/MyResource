#include "pub.h"
#include "xml.h"

extern XmlList* g_pstNodeList[LISTLEN];
extern char g_caTrcMsg[1024];
static int g_z=LISTLEN;

/********************************************************************
    函数名：xmlGetValueByPath
	功能：根据路径，查找节点，获得符合要求的
		  节点指针数组
	参数：1、xml数据链表
		  2、路径(.../.../...)
		  3、值
	返回值： -1 失败

*********************************************************************/
int xmlGetValueByPath(XmlList* pstXmlList,char* caPath,char* caValue)
{
	char* caSPath;
	char caTPath[1024];
	int ret;
	memset(caTPath,0,sizeof(caTPath));

 	XmlList* NodeList[2];
    XmlList** p=NodeList;
	int ListLen=2;

	int temp=ListLen;
	if(caPath[0]=='/')
		caPath++;
	caSPath=strstr(caPath,"/");
	//printf("剩余spath[%s]\n",caSPath);
	if(caSPath)
	{
		strncpy(caTPath,caPath,caSPath-caPath);
		//printf("path[%s][%d]\n",caTPath,caSPath-caPath);

		ret=xmlFindPathNode(pstXmlList,caTPath,NULL,&p,&ListLen);
		//printf("tp[%s]\n",caTPath);
		//xmlPrintList4F(NodeList[0]);
		if(temp==ListLen)
		{
			printf("标签[%s]不存在\n",caTPath);
			return -1;
		}
		if(ret==-1)
		{
			printf("路径包含循环标签[%s]\n",caTPath);
			return -1;
		}

		if(strlen(caSPath+1)&& strcmp(caSPath,"/"))
		{

			ret=xmlGetValueByPath(NodeList[0],caSPath,caValue);
			if(ret==-1)
				return -1;
		}
		else
		{
			if(NodeList[0]->iNodeType==PARENT)
			{
				printf("该节点是父节点\n");
				return -2;
			}
			strcpy(caValue,NodeList[0]->caValue);
			return 0;
		}
	}
	else
	{
		ret=xmlFindPathNode(pstXmlList,caPath,NULL,&p,&ListLen);
		if(temp==ListLen)
		{
			printf("标签[%s]不存在\n",caPath);
			return -1;
		}
		if(NodeList[0]->iNodeType==PARENT)
		{
			printf("该节点是父节点\n");
			return -2;
		}
		strcpy(caValue,NodeList[0]->caValue);
		return 0;
	}
}

/********************************************************************
    函数名：xmlGetValueByKey
	功能：根据标签名，找到所有标签名相符的
		  节点指针数组，并找到第num个节点，取值
	参数：1、xml数据链表
		 2、标签名
		 3、下标
	返回值： -1 失败 

*********************************************************************/

int xmlGetValueByKey(XmlList* pstXmlList,char* caKey,char* caValue,int num)
{
	int ListLen=g_z;
	int temp=ListLen;
	XmlList** p=g_pstNodeList;

	if(g_pstNodeList[0]==NULL||strcmp(g_pstNodeList[0]->caKey,caKey))
	{
		//printf("++++++\n");
		memset(g_pstNodeList,0x00,4*g_z);
		if(xmlFindNode(pstXmlList,caKey,NULL,&p,&ListLen,KEY))
		{
			sprintf(g_caTrcMsg,"FindNode error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg );
			return -1;
		}
		if(temp==ListLen)
		{
			printf("标签[%s]不存在\n",caKey);
			return -1;
		}
	}
	if(!g_pstNodeList[num-1])
	{
		sprintf(g_caTrcMsg,"index error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg );
		return -1;
	}
	if(g_pstNodeList[num-1]->iNodeType!=CHILD)
	{
		sprintf(g_caTrcMsg,"Nodetype is not child FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg );
		return -1;
	}
	sprintf(caValue,"%s",g_pstNodeList[num-1]->caValue);
	return 0;
}

/********************************************************************
    函数名：xmlGetListByKey
	功能：根据标签名，找到所有标签名相符的
		  节点指针数组
	参数：1、xml数据链表
		 2、标签名
	返回值： -1 失败 >=0 节点数量

*********************************************************************/

int xmlGetListByKey(XmlList* pstXmlList,char* caKey)
{
	memset(g_pstNodeList,0x00,4*g_z);
	int ListLen=g_z;
	int temp=ListLen;
	XmlList** p=g_pstNodeList;
	if(xmlFindNode(pstXmlList,caKey,NULL,&p,&ListLen,KEY))
	{
		sprintf(g_caTrcMsg,"FindNode error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
		printf("%s\n",g_caTrcMsg );
		return -1;
	}
	return temp-ListLen;
}

/********************************************************************
    函数名：xmlGetListByAttr
	功能：根据属性名和属性值，找到所有相符的
		  节点指针数组
	参数：1、xml数据链表
		 2、属性名
		 3、属性值（为NULL，只匹配属性名）
	返回值： -1 失败 >=0 节点数量

*********************************************************************/

int xmlGetListByAttr(XmlList* pstXmlList,char* caAttrname,char* caAttrvalue)
{
	memset(g_pstNodeList,0x00,4*g_z);
	int ListLen=g_z;
	int temp=ListLen;
	XmlList** p=g_pstNodeList;
	if(caAttrvalue){
		if(xmlFindNode(pstXmlList,caAttrname,caAttrvalue,&p,&ListLen,ATTR))
		{
			sprintf(g_caTrcMsg,"FindNode error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg );
			return -1;
		}
	}
	else
	{
		if(xmlFindNode(pstXmlList,caAttrname,NULL,&p,&ListLen,ATTR))
		{
			sprintf(g_caTrcMsg,"FindNode error FILE[%s] LINE[%d]\n",__FILE__,__LINE__);
			printf("%s\n",g_caTrcMsg );
			return -1;
		}
	}
	return temp-ListLen;
}

/*int xmlGetChildNum(XmlList* pstXmlList,char* caParentKey)
{

}
int xmlGetValueByParentKey(XmlList* pstXmlList,char* caParentKey)
{

}

int xmlGetValueByAttr(XmlList* pstXmlList,char* caAttrname,char* caAttrvalue)
{

}*/


/********************************************************************
    
	功能：根据Key，或属性名和属性值查找节点，获得符合要求的
		  节点指针数组
	参数：1、xml数据链表
		  2、key名或属性名
		  3、属性值(非属性模式查找，为NULL)
		  4、节点指针数组
		  5、传入为节点指针数组长度，传出为数组剩余可用长度
		  6、查找模式(KEY or ATTR)
	返回值： -1 失败

*********************************************************************/
int xmlFindNode(XmlList* pstXmlList,char* caName,char* cavalue,XmlList*** NodeList,int* ListLen,int mode)
{
	if((*ListLen)==0)
	{
		return -1;
	}
	int ret;
	if(mode==KEY)
	{	
		if(!strcmp(caName,pstXmlList->caKey))
		{
			*(*NodeList)=pstXmlList;
			(*NodeList)++;
			(*(ListLen))--;
			if(pstXmlList->pnext)
			{
				ret=xmlFindNode(pstXmlList->pnext,caName,cavalue,NodeList,ListLen,mode);
				if(ret==-1)
					return -1;
			}
		}
		else
		{
			if(pstXmlList->pchild)
			{
				//printf("ccc|%s|%d\n",caName,*ListLen);
				ret=xmlFindNode(pstXmlList->pchild,caName,cavalue,NodeList,ListLen,mode);
				if(ret==-1)
					return -1;
			}
			if(pstXmlList->pnext)
			{
				//printf("nnn|%s|%d\n",caName,*ListLen);
				ret=xmlFindNode(pstXmlList->pnext,caName,cavalue,NodeList,ListLen,mode);
				if(ret==-1)
					return -1;
			}
		}
	}
	else if(mode==ATTR)
	{
		if(cavalue){
			if(!strcmp(caName,pstXmlList->caAttrname)&&!strcmp(cavalue,pstXmlList->caAttrvalue))
			{
				*(*NodeList)=pstXmlList;
				(*NodeList)++;
				(*(ListLen))--;
				if(!(*ListLen))
					return -1;
			}
		}
		else
		{
			if(!strcmp(caName,pstXmlList->caAttrname))
			{
				*(*NodeList)=pstXmlList;
				(*NodeList)++;
				(*(ListLen))--;
				if(!(*ListLen))
					return -1;
			}
		}
		if(pstXmlList->pchild)
		{
			xmlFindNode(pstXmlList->pchild,caName,cavalue,NodeList,ListLen,mode);
		}
		if(pstXmlList->pnext)
		{
			xmlFindNode(pstXmlList->pnext,caName,cavalue,NodeList,ListLen,mode);
		}
	}
	else
		return -1;
	return 0;	
}


int xmlFindPathNode(XmlList* pstXmlList,char* caName,char* cavalue,XmlList*** NodeList,int* ListLen)
{
	if((*ListLen)==0)
	{
		return -1;
	}
	int ret;

	if(!strcmp(caName,pstXmlList->caKey))
	{
		*(*NodeList)=pstXmlList;
		(*NodeList)++;
		(*(ListLen))--;
		if(pstXmlList->pnext)
		{
			ret=xmlFindPathNode(pstXmlList->pnext,caName,cavalue,NodeList,ListLen);
			if(ret==-1)
				return -1;
		}
	}
	else
	{
		if(pstXmlList->pchild)
		{

			ret=xmlFindPathNode(pstXmlList->pchild,caName,cavalue,NodeList,ListLen);
			if(ret==-1)
				return -1;
		}
		else{
			if(pstXmlList->pnext)
			{
				ret=xmlFindPathNode(pstXmlList->pnext,caName,cavalue,NodeList,ListLen);
				if(ret==-1)
					return -1;
			}
		}
	}
	return 0;	
}


/********************************************************************

	功能：链表释放
	参数：链表头指针

*********************************************************************/
void xmlFreeList(XmlList* pstXmlList)
{
	if(pstXmlList->pnext)
	{
		xmlFreeList(pstXmlList->pnext);
	}
	if(pstXmlList->pchild)
	{
		xmlFreeList(pstXmlList->pchild);
	}
	free(pstXmlList);
}


void xmlPrintList(XmlList* pstXmlList)
{
	if(pstXmlList)
	{
		switch(pstXmlList->iNodeType)
		{
			case HEAD:
				xmlPrintList(pstXmlList->pnext);
				break;
			case ROOT:
				printf("root node -> key[%s] attr[%s]\n",pstXmlList->caKey,pstXmlList->caAttrvalue);
				xmlPrintList(pstXmlList->pchild);
				break;
			case PARENT:
				printf("parent node -> key[%s] attr[%s]\n",pstXmlList->caKey,pstXmlList->caAttrvalue);
				xmlPrintList(pstXmlList->pchild);
				xmlPrintList(pstXmlList->pnext);
				break;
			case CHILD:
				printf("child node -> key[%s] value[%s] attr[%s]\n",pstXmlList->caKey,pstXmlList->caValue,pstXmlList->caAttrvalue);
				xmlPrintList(pstXmlList->pnext);
				break;
			default:
				exit(-1);
		}
	}
}


void xmlPrintList4F(XmlList* pstXmlList)
{
	XmlList* p1;
	XmlList* p2;
	XmlList* p3;
	XmlList* p4;
	p1=pstXmlList;
	if(pstXmlList==NULL)
		return;
	while(p1)
	{
		printf("<%s",p1->caKey);

		if(p1->iNodeType==CHILD)
		{
			if(p1->caAttrvalue[0])
			{
				printf(" %s=%s>%s",p1->caAttrname,p1->caAttrvalue,p1->caValue);
			}
			else
				printf(">%s",p1->caValue);
		}
		else
		{
			if(p1->caAttrvalue[0])
			{
				printf(" %s=%s>\n",p1->caAttrname,p1->caAttrvalue);
			}
			else
				printf(">\n");
		}
		p2=p1->pchild;
		while(p2)
		{
			printf("    ");
			printf("<%s",p2->caKey);

			if(p2->iNodeType==CHILD)
			{
				if(p2->caAttrvalue[0])
				{
					printf(" %s=%s>%s",p2->caAttrname,p2->caAttrvalue,p2->caValue);
				}
				else
					printf(">%s",p2->caValue);
				}
			else
			{
				if(p2->caAttrvalue[0])
				{
					printf(" %s=%s>\n",p2->caAttrname,p2->caAttrvalue);
				}
				else
					printf(">\n");
			}
			p3=p2->pchild;
			while(p3)
			{
				printf("    ");
				printf("    ");
				printf("<%s",p3->caKey);
				if(p3->iNodeType==CHILD)
				{
					if(p3->caAttrvalue[0])
					{
						printf(" %s=%s>%s",p3->caAttrname,p3->caAttrvalue,p3->caValue);
					}
					else
						printf(">%s",p3->caValue);
					}
				else
				{
					if(p3->caAttrvalue[0])
					{
						printf(" %s=%s>\n",p3->caAttrname,p3->caAttrvalue);
					}
					else
						printf(">\n");
				}
				p4=p3->pchild;
				while(p4)
				{
					printf("    ");
					printf("    ");
					printf("    ");
					printf("<%s",p4->caKey);	
					if(p4->iNodeType==CHILD)
					{
						if(p4->caAttrvalue[0])
						{
							printf(" %s=%s>%s",p4->caAttrname,p4->caAttrvalue,p4->caValue);
						}
						else
							printf(">%s",p4->caValue);
						}
					else
					{
						if(p4->caAttrvalue[0])
						{
							printf(" %s=%s>\n",p4->caAttrname,p4->caAttrvalue);
						}
						else
							printf(">\n");
					}
					if(p4->iNodeType!=CHILD)
					{
						printf("    ");
						printf("    ");
						printf("    ");	
					}
					
					printf("</%s>\n",p4->caKey);
					p4=p4->pnext;
				}
				if(p3->iNodeType!=CHILD)
				{
					printf("    ");
					printf("    ");
				}
				printf("</%s>\n",p3->caKey);
				p3=p3->pnext;
			}
			if(p2->iNodeType!=CHILD)
			{
				printf("    ");
			}
			printf("</%s>\n",p2->caKey);
			p2=p2->pnext;
		}
		printf("</%s>\n",p1->caKey);
		p1=p1->pnext;
	} 
}


