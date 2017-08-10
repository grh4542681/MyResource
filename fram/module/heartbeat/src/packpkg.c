#include <string.h>

#include "returnval.h"
#include "log.h"
#include "heartbeat.h"
#include "packpkg.h"
#include "cJSON.h"

char* PackHBpkg4J(MCHINF* hostinfo)
{
	cJSON *JsonRoot = NULL;
	cJSON *JsonMchInf = NULL;
	char* JsonString = NULL;

	JsonRoot = cJSON_CreateObject();
	if(JsonRoot == NULL)
	{
		SYSLOG(ERROR,"%s","Create Json root error");
		return (char*)SYSERROR;
	}

	JsonMchInf = cJSON_CreateObject();
	if(JsonRoot == NULL)
	{
		cJSON_Delete(JsonRoot);
		SYSLOG(ERROR,"%s","Create Json item error");
		return (char*)SYSERROR;
	}

	cJSON_AddStringToObject(JsonMchInf,"name",hostinfo->name);
	cJSON_AddItemToObject(JsonRoot,"machine",JsonMchInf);

	JsonString = cJSON_PrintUnformatted(JsonRoot);
	if(JsonString == NULL)
	{
		cJSON_Delete(JsonRoot);
		SYSLOG(ERROR,"%s","Convert Json to string error");
		return (char*)SYSERROR;
	}
	
	cJSON_Delete(JsonRoot);
	return JsonString;
}

int UnPackHBpkg4J(MCHINF* hostinfo,char* pkg)
{
	cJSON *JsonRoot = NULL;
	cJSON *JsonMchInf = NULL;
	cJSON *JsonMchInf_name = NULL;

	JsonRoot = cJSON_Parse(pkg);
	if(JsonRoot == NULL)
	{
		SYSLOG(ERROR,"Paser string to json error [%s]",pkg);
		return SYSERROR;
	}

	JsonMchInf = cJSON_GetObjectItem(JsonRoot,"machine");
	if(JsonMchInf == NULL)
	{
		SYSLOG(ERROR,"Get json item[machine] error[%s]",pkg);
		return SYSERROR;
	}
	JsonMchInf_name = cJSON_GetObjectItem(JsonMchInf,"name");
	if(JsonMchInf_name == NULL)
	{
		SYSLOG(ERROR,"Get json item[name] error[%s]",pkg);
		return SYSERROR;
	}
	memcpy(hostinfo->name,JsonMchInf_name->valuestring,strlen(JsonMchInf_name->valuestring));

	cJSON_Delete(JsonRoot);

	return SUCCESS;
}










