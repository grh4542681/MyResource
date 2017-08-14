#include "procpool.h"
#include "child_op.h"
#include "log.h"
#include "mem.h"
#include "list.h"
#include "uerror.h"

extern char g_calog[1024];
static int managerpid;

void clean_dchild(int signo)
{
	if(waitpid(managerpid,NULL, WNOHANG)<0)
	{
		sprintf(g_calog,"waitpid[%d] error errno[%d]:%s",managerpid,errno,strerror(errno));
        SYSLOG(g_calog);
        return ;
	}
}

int procpool_create(struct childinfo *pmanagerinfo,char *childpath,int procnum,int overtime)
{
	int ret;
	int sockfd[2];
	int childpid;
	char status;
	struct createinfo creinfo;
	struct sigaction signew;
	struct sigaction sigold;

	memset(&signew,0x00,sizeof(sigaction));
	memset(&sigold,0x00,sizeof(sigaction));
	memset(&creinfo,0x00,sizeof(struct createinfo));
	childinfo_init(pmanagerinfo);

	
	if(socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd))
	{
		sprintf(g_calog,"socketpair errno[%d]:%s",errno,strerror(errno));
        SYSLOG(g_calog);
        return -1;
	}
	

	signew.sa_handler=clean_dchild;

	sigaction(SIGCHLD,&signew,&sigold);
	//Create manager
	
	childpid=fork();
	if(!childpid)
	{
		char capipefd[3];
		char cachildnum[3];

		memset(capipefd,0x00,sizeof(capipefd));
		memset(cachildnum,0x00,sizeof(cachildnum));

		sprintf(cachildnum,"%d",procnum);
		//close(sockfd[1]);
		sprintf(capipefd,"%d",sockfd[0]);

		execl(MANAGERDIR,MANAGERNAME,capipefd,cachildnum,childpath,(char*)0);
	}
	else if(childpid<0)
	{
		sprintf(g_calog,"fork errno[%d]:%s",errno,strerror(errno));
        SYSLOG(g_calog);
        return -1;
	}
	else
	{
		//close(sockfd[1]);
		pmanagerinfo->pipefd=sockfd[1];
		pmanagerinfo->childpid=childpid;
		managerpid=childpid;
		if((ret=Recv(pmanagerinfo->pipefd,&status,1,0,overtime))<=0)
		{
			if(ret==TIMEOUT)
			{
				sprintf(g_calog,"Procpool Create timeout poolmanager pid[%d]\n",pmanagerinfo->childpid);
				SYSLOG(g_calog);
			}
			sprintf(g_calog,"Procpool manager register error poolmanager pid[%d]\n",pmanagerinfo->childpid);
			SYSLOG(g_calog);
			if(kill(pmanagerinfo->childpid,SIGKILL))
			{
				sprintf(g_calog,"kill errno[%d]:%s",errno,strerror(errno));
		        SYSLOG(g_calog);
		        return -1;
			}
			sigaction(SIGCHLD,&sigold,NULL);
			return ret;
		}
		sprintf(g_calog,"manager send is [%c]",status);
		SYSLOG(g_calog);
		sigaction(SIGCHLD,&sigold,NULL);
		pmanagerinfo->parentpid=getpid();
		pmanagerinfo->createtime=time(NULL);
		pmanagerinfo->status=status;
		sprintf(g_calog,"Procpool Create Success poolmanager pid[%d]\n",pmanagerinfo->childpid);
		SYSLOG(g_calog);
		return 0;
	}
}

int procpool_destroy(struct childinfo *pmanagerinfo)
{
	//Kill manager
	int ret;
	if(ret=Send(pmanagerinfo->pipefd,"k",1,0,0)<=0)
	{
		sprintf(g_calog,"Procpool [%d] already dead",pmanagerinfo->childpid);
		SYSLOG(g_calog);
		return -1;
	}
	return ret;
}

int procpool_stat(struct childinfo *pmanagerinfo,struct ppoolinfo *pppoolinfo)
{
	//Pipe with manager
	if(Send(pmanagerinfo->pipefd,"p",1,0,2000)<=0)
	{
		sprintf(g_calog,"Procpool [%d] already dead",pmanagerinfo->childpid);
		SYSLOG(g_calog);
		return -1;
	}
	return Recv(pmanagerinfo->pipefd,pppoolinfo,sizeof(struct ppoolinfo),0,0);
}

int procpool_sizectrl(struct childinfo *pmanagerinfo,int num)
{
	union sigval sigarg;
	memset(&sigarg,0x00,sizeof(union sigval));
	sigarg.sival_int=num;
	if(sigqueue(pmanagerinfo->childpid,SIGUSR1,sigarg)<0)
	{
		sprintf(g_calog,"Procpool [%d] already dead",pmanagerinfo->childpid);
		SYSLOG(g_calog);
		return -1;
	}
	return 0;
}