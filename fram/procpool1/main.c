#include "procpool.h"
#include "child_op.h"
#include "log.h"

extern char g_calog[1024];

int main(int argc, char const *argv[])
{
	int ret;
	struct childinfo manager;
	struct ppoolinfo poolinfo;
	ret=procpool_create(&manager,"./child",3,10000);
	printf("ret=[%d]\n",ret);
	sleep(10);
	ret=procpool_stat(&manager,&poolinfo);
		sprintf(g_calog,"ret=[%d]\n",ret);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.idel[%f]\n",poolinfo.idle);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.childnum[%d]\n",poolinfo.childnum);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.freechildnum[%d]\n",poolinfo.freechildnum);
		SYSLOG(g_calog);
	//Send(manager.pipefd,"p",1,0,0);
	/*ret=procpool_sizectrl(&manager,-3);
	printf("ret=[%d]\n",ret);
	ret=procpool_stat(&manager,&poolinfo);
		sprintf(g_calog,"ret=[%d]\n",ret);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.idel[%f]\n",poolinfo.idle);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.childnum[%d]\n",poolinfo.childnum);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.freechildnum[%d]\n",poolinfo.freechildnum);
		SYSLOG(g_calog);*/
	//sleep(2);
	//ret=procpool_destroy(&manager);
	//printf("ret=[%d]\n",ret);
	while(1)
	{
		printf("----------------\n");
		sleep(2);
		ret=procpool_stat(&manager,&poolinfo);
		sprintf(g_calog,"ret=[%d]\n",ret);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.idel[%f]\n",poolinfo.idle);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.childnum[%d]\n",poolinfo.childnum);
		SYSLOG(g_calog);
		sprintf(g_calog,"poolinfo.freechildnum[%d]\n",poolinfo.freechildnum);
		SYSLOG(g_calog);
	}
	return 0;
}