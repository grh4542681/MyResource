#include "parserINI.h"
#include "log.h"

int main(int args,char** argv)
{
	LOGINIT("/home/ezgaoro/grh/MyResource/fram/etc/log.conf");

	INISEC* ini= LoadINI("/home/ezgaoro/grh/MyResource/fram/etc/1.ini");
	INIITEM* item=GetSectionItem(ini,"STEP1","DESCRIPT");
	if(!item)
	{
		printf("error\n");
	}
	printf("item [%s][%s]\n",item->key,item->value);

	INISEC* section=GetSection(ini,"STEP1");
	if(!section)
	{
		printf("error\n");
	}
	printf("section [%s]\n",section->section);
	

	item=GetSectionItem(ini,"STEP1","VERSION");
	//item=GetSectionItem(ini,"STEP1","CHANEL");
	if(!item)
	{
		printf("error\n");
	}
	printf("item [%s][%s]\n",item->key,item->value);

	FreeINI(ini);

	LOGFREE();	
}
