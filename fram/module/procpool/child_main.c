#include "child_main.h"
#include "log.h"
#include "io.h"

extern char g_calog[1024];

int pipefd_f;

int main(int argc, char const *argv[])
{
	sprintf(g_calog,"child[%d] create",getpid());
	SYSLOG(g_calog);
	int a;
	pipefd_f=atoi(argv[1]);

	if(Send(pipefd_f,"1",1,0,SYSOVERTIME)<=0)
	{
		sprintf(g_calog,"child[%s] pid[%d] register error[%d]:%s\n",argv[0],getpid(),errno,strerror(errno));
		SYSLOG(g_calog);
		exit(-1);
	}

	while(1)
	{
		sleep(3);
		if(Send(pipefd_f,"2",1,0,0)<=0)
		{
			sprintf(g_calog,"child[%s] pid[%d] register error[%d]:%s\n",argv[0],getpid(),errno,strerror(errno));
			SYSLOG(g_calog);
			exit(-1);
		}
		sleep(5);
		printf("**************\n");
		if(Send(pipefd_f,"1",1,0,0)<=0)
		{
			sprintf(g_calog,"child[%s] pid[%d] register error[%d]:%s\n",argv[0],getpid(),errno,strerror(errno));
			SYSLOG(g_calog);
			exit(-1);
		}
	}
	return 0;
}