#include "pub.h"
#include "xml.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

extern char g_caTrcMsg[1024];
extern XmlList* g_pstNodeList[LISTLEN];

int main(int argc, char const *argv[])
{
	char caxml[2048];
	memset(caxml,0,sizeof(caxml));
	strcpy(caxml,"<?xml version=\"1.0\" encoding=\"GB2312\"?><root><Shool1><class1 id=\"5\"><name>1</name><age>2</age><sex>m</sex></class1><class id=\"1\"><name>2</name><age>2</age><sex>m</sex></class><class id=\"2\"><name>b</name><age>21</age><sex>w</sex></class><class id=\"3\"><name>c</name><age>22</age><sex>w</sex></class></Shool1><Shool2><class id=\"1\"><name>d</name><age>20</age><sex>m</sex></class><class id=\"2\"><name>e</name><age>21</age><sex>w</sex></class><class id=\"3\"><name>f</name><age>22</age><sex>w</sex></class></Shool2><Shool3><class id=\"1\"><name>g</name><age>20</age><sex>m</sex></class><class id=\"2\"><name>h</name><age>21</age><sex>w</sex></class><class id=\"3\"><name>i</name><age>22</age><sex>w</sex></class></Shool3><Shool4><class id=\"1\"><name></name><age>2</age><sex>m</sex></class><class id=\"2\"><name>b</name><age>21</age><sex>w</sex></class><class id=\"3\"><name>c</name><age>22</age><sex>w</sex></class></Shool4></root>");

	//makexmlstr(&caxml);
	printf("buff[%s]\n",caxml);
	printf("-------------------------------------------------------------\n");

	XmlList *pstXmlListHead;

	if(xmlUnPack(caxml,&pstXmlListHead))
	{
		sprintf(g_caTrcMsg,"xmlUnPack error FILE[%s] LINE[%d]\n",g_caTrcMsg);
		printf("%s\n",g_caTrcMsg);
		xmlFreeList(pstXmlListHead);
		return -1;
	}
	printf("-------------------------------------------------------------\n");
	xmlPrintList4F(pstXmlListHead);
	printf("-------------------------------------------------------------\n");

    XmlList* pstNodeList[LISTLEN];
    int Listlen=LISTLEN;
    XmlList** p=pstNodeList;
    xmlFindNode(pstXmlListHead,"age",NULL,&p,&Listlen,KEY);
    printf("surplus len:[%d]\n",Listlen);
    int i=0;
    while(pstNodeList[i])
    {
        printf("[%s]\n",pstNodeList[i]->caKey);
        i++;
    }
    printf("-------------------------------------------------------------\n");
    char age[1024];
    memset(age,0,sizeof(age));

    xmlGetValueByPath(pstXmlListHead,"/Shool1/class1/age",age);
    printf("age[%s]\n",age);

    printf("-------------------------------------------------------------\n");
    int num;
    num=xmlGetListByKey(pstXmlListHead,"name");
    printf("num of [%s] is [%d]\n","name",num);
    num=xmlGetListByAttr(pstXmlListHead,"id","1");
    printf("num of attr[%s:%s] is [%d]\n","id","1",num);
    num=xmlGetListByAttr(pstXmlListHead,"id",NULL);
    printf("num of attr[%s] is [%d]\n","id",num);
    printf("-------------------------------------------------------------\n");
    memset(age,0,sizeof(age));
    xmlGetValueByKey(pstXmlListHead,"name",age,3);
    printf("name[%s]\n",age);
    xmlGetValueByKey(pstXmlListHead,"name",age,4);
    printf("name[%s]\n",age);
    xmlGetValueByKey(pstXmlListHead,"name",age,5);
    printf("name[%s]\n",age);
    xmlGetValueByKey(pstXmlListHead,"sex",age,5);
    printf("sex[%s]\n",age);
    printf("-------------------------------------------------------------\n");

	xmlFreeList(pstXmlListHead);
	return 0;
}

int makexmlstr(char** pBuff)
{
    char szout[1024];
    char s[1024];
    char i[1024];
    char o[1024];
    char* x;
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);

    g2u("¸ßÈÙ»ª",strlen("¸ßÈÙ»ª"),szout,1024);
    //u2g(szout,strlen("¸ßÈÙ»ª"),s,1024);
    printf("[%s]\n",szout);
    xmlNewChild(root_node, NULL, BAD_CAST "name",BAD_CAST szout);
    //xmlNewChild(root_node, NULL, BAD_CAST "name",BAD_CAST "¸ßÈÙ»ª");

    node=xmlNewChild(root_node, NULL, BAD_CAST "age", BAD_CAST "22");

    node = xmlNewNode(NULL, BAD_CAST "sex");
    node1 = xmlNewText(BAD_CAST "w");
    xmlAddChild(node, node1);
    xmlAddChild(root_node, node);

    //char *pBuff;
    int iBuffLength;
    //xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, "GB2312", 1);
    xmlDocDumpMemoryEnc(doc,(xmlChar**)(pBuff),&iBuffLength,"gb2312");
    printf("buff[%s]\n",*pBuff);
    xmlFreeDoc(doc);
    
    strcpy(i,*pBuff);
    code2u(i,iBuffLength,&x,"gb2312");
    printf("o[%s]\n",x);
    //xmlCleanupParser();
    //xmlMemoryDump();

    return 0;
}


