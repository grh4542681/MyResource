#include "child_monitor_main.h"
#include "child_op.h"
#include "procpool.h"
#include "mem.h"
#include "log.h"
#include "io.h"
#include "list.h"

static int pipefd_f;
static char g_childpath[128];
static char g_childname[32];
static int g_childnum;
static struct ppoolinfo stppoolinfo;
static int checknum;

/*Create child_info list head*/
LIST_HEAD(child_head);
/*Create pool usage percent list head*/
LIST_HEAD(poolusage_head);

void poolusage_init(struct poolusage *ppoolusage)
{
	INIT_LIST_HEAD(&(ppoolusage->p_list));
}

int poolusage_add(struct list_head *head,struct poolusage *ppoolusage)
{
	struct poolusage *new;
	new=Malloc(sizeof(struct poolusage));
	if(!new)
	{
		strcpy(g_calog,"Malloc error");
		SYSLOG(g_calog);
		return -1;
	}
	memcpy(new,ppoolusage,sizeof(struct poolusage));
	list_add(&(new->p_list),head);
	return 0;
}

void poolusage_del(struct poolusage *ppoolusage)
{
	list_del(&(ppoolusage->p_list));
	free(ppoolusage);
}



void sigchld_func(int signo)
{
	struct childinfo *pchildinfo;
	pid_t pid;
	while(pid=waitpid(-1,NULL,WNOHANG))
	{
		childinfo_each(pchildinfo,&child_head)
		{
			if(pchildinfo->childpid==pid)
			{
				sprintf(g_calog,"In procpool[%d] child[%d] dead",pchildinfo->parentpid,pchildinfo->childpid);
				SYSLOG(g_calog);
				break;
			}
		}
	}
}

void sigalrm_func(int signo)
{
	union sigval sigarg;
	struct poolusage new;
	struct poolusage *pcurr;

	memset(&sigarg,0x00,sizeof(union sigval));
	memset(&new,0x00,sizeof(struct poolusage));

	stppoolinfo.idle=((float)(stppoolinfo.freechildnum)/(float)(stppoolinfo.childnum))*100;
	new.usagepercent=stppoolinfo.idle;
	poolusage_add(&poolusage_head,&new);

	checknum++;
	if(stppoolinfo.childnum<MINCHILDNUM)
	{
		sigarg.sival_int=MINCHILDNUM-stppoolinfo.childnum;
		sigqueue(getpid(),SIGUSR1,sigarg);
	}
	if(stppoolinfo.childnum>MAXCHILDNUM)
	{
		sigarg.sival_int=MAXCHILDNUM-stppoolinfo.childnum;
		sigqueue(getpid(),SIGUSR1,sigarg);
	}
	if(checknum>=5)
	{
		checknum=0;
		float sum=0.0;
		float equ=0.0;
		poolusage_each(pcurr,&poolusage_head)
		{
			sum+=pcurr->usagepercent;
			checknum++;
			poolusage_del(pcurr);
		}
		equ=sum/(float)checknum;
		if(equ<30.0)
		{
			sigarg.sival_int=((((stppoolinfo.freechildnum)*10/4))-stppoolinfo.freechildnum+stppoolinfo.childnum) > MAXCHILDNUM ? MAXCHILDNUM-stppoolinfo.childnum : (((stppoolinfo.freechildnum)*10/4)+1)-stppoolinfo.freechildnum;
			sprintf(g_calog,"sival_int [%d][%f]",sigarg.sival_int,equ);
			SYSLOG(g_calog);
			sigqueue(getpid(),SIGUSR1,sigarg);
		}
		if(equ>80.0)
		{
			sigarg.sival_int=((((stppoolinfo.freechildnum)*10/6))-stppoolinfo.freechildnum+stppoolinfo.childnum) < MINCHILDNUM ? MINCHILDNUM-stppoolinfo.childnum : (((stppoolinfo.freechildnum)*10/4)+1)-stppoolinfo.freechildnum;
			sprintf(g_calog,"sival_int [%d][%f]",sigarg.sival_int,equ);
			SYSLOG(g_calog);
			sigqueue(getpid(),SIGUSR1,sigarg);
		}
		checknum=0;
	}
	alarm(ALARMTIME);
}

void sigusr1_func(int signo, siginfo_t *siginfo, void *context)
{
	if(siginfo->si_int>0)
	{
		child_init(siginfo->si_int);
	}
	else if(siginfo->si_int<0)
	{
		kill_free_children(0-siginfo->si_int);
	}
	else
		return ;
}

void kill_all_children()
{
	struct childinfo *pchildinfo;
	childinfo_each(pchildinfo,&child_head)
	{
		sprintf(g_calog,"kill child [%d]",pchildinfo->childpid);
		SYSLOG(g_calog);
		kill(pchildinfo->childpid,SIGKILL);
		pchildinfo->status=DEAD;
	}
}

void kill_free_children(int num)
{
	struct childinfo *pchildinfo;
	childinfo_each(pchildinfo,&child_head)
	{
		if((pchildinfo->status==READY)&&(num--))
		{
			sprintf(g_calog,"kill child [%d]",pchildinfo->childpid);
			SYSLOG(g_calog);
			kill(pchildinfo->childpid,SIGKILL);
			//pchildinfo->status=DEAD;
			
			if(!num)
				break;
		}
	}
}

void deal_parent_msg(char *command)
{
	switch(*command)
	{
		/** Report procpool info */
		case REPORT :
			if(send(pipefd_f,&stppoolinfo,sizeof(struct ppoolinfo),MSG_DONTWAIT)<=0)
			{
				sprintf(g_calog,"Creater [%d] who create this pool [%d] dead!\nDestory pool[%d]",stppoolinfo.createrid,stppoolinfo.managerid,stppoolinfo.managerid);
				SYSLOG(g_calog);
				kill_all_children();
				exit(-1);
			}
			break;
		/** Kill self */
		case DESTROY :
			kill_all_children();
			sleep(6);
			exit(0);
		/** Restart */
		case RESTART :
			kill_all_children();
			child_init(g_childnum);
			break;
		/** default */
		default:
			sprintf(g_calog,"Unknow command[%c] that recv from parent[%d]",*command,stppoolinfo.createrid);
			SYSLOG(g_calog);
			break;
	}
}

void deal_child_msg(struct childinfo* pchildinfo, char *command)
{
	switch(*command)
	{
		/** Begin working */
		case WORK:
			pchildinfo->status=WORK;
			stppoolinfo.freechildnum-=1;
			break;
		/** Waiting for work */
		case READY:
			pchildinfo->status=READY;
			stppoolinfo.freechildnum+=1;
			break;
		/** Default */
		default:
			sprintf(g_calog,"Unknow message[%c] that recv from child[%d]",*command,pchildinfo->childpid);
			SYSLOG(g_calog);
			break;
	}
}

/*child manager main loop*/
void do_monitor()
{
	fd_set rfds;
	int maxfd=0;
	int ret;
	char command;
	struct childinfo* pchildinfo;
	/*Main loop*/
	while(1)
	{
		/*Add descriptor into fd_set*/
		FD_ZERO(&rfds);
		FD_SET(pipefd_f,&rfds);
		(maxfd > pipefd_f) ? maxfd : (maxfd=pipefd_f);
		childinfo_each(pchildinfo,&child_head)
		{
			FD_SET(pchildinfo->pipefd,&rfds);
			(maxfd > pchildinfo->pipefd) ? maxfd : (maxfd=pchildinfo->pipefd);
		}
		/*Listen*/
		ret=select(maxfd+1,&rfds,NULL,NULL,NULL);
		if(ret<0)
		{
			if(errno==EINTR)
			{
				continue;
			}
			else
			{
				sprintf(g_calog,"Poolmanager pid[%d] select error",getpid());
				SYSLOG(g_calog);
				exit(-1);
			}
		}
		while(ret--)
		{
			/*Parent message*/
			if(FD_ISSET(pipefd_f,&rfds))
			{
				FD_CLR(pipefd_f,&rfds);
				if(recv(pipefd_f,&command,1,MSG_DONTWAIT)<0)
				{
					sprintf(g_calog,"Parent of poolmanager pid[%d] dead\nPoolmanager exit",getpid());
					SYSLOG(g_calog);
					exit(-1);
				}
				deal_parent_msg(&command);
			}
			/*Child message*/
			childinfo_each(pchildinfo,&child_head)
			{
				if(FD_ISSET(pchildinfo->pipefd,&rfds))
				{
					FD_CLR(pchildinfo->pipefd,&rfds);
					if(recv(pchildinfo->pipefd,&command,1,MSG_DONTWAIT)<=0)
					{
						sprintf(g_calog,"Poolmanager pid[%d] : child[%d] dead",getpid(),pchildinfo->childpid);
						SYSLOG(g_calog);
						kill(pchildinfo->childpid,SIGKILL);
						childinfo_del(pchildinfo);
						stppoolinfo.childnum--;
						if(pchildinfo->status==READY)
						{
							stppoolinfo.freechildnum--;
						}
						continue;
					}
					deal_child_msg(pchildinfo,&command);
				}
				
			}
		}

	}
}


void child_init(int childnum)
{
	int i;
	int ret;
	int childpid;
	int sockfd[2];
	char pipefd_c[3];
	char status;
	struct childinfo *pchildinfo;

		/** Create children */
	for(i=childnum;i>0;i--)
	{
		memset(pipefd_c,0x00,sizeof(pipefd_c));
		memset(sockfd,0x00,sizeof(int)*2);

		/** Create pipe between parents and children */
		if(socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd))
		{
			sprintf(g_calog,"socketpair errno[%d]:%s",errno,strerror(errno));
	        SYSLOG(g_calog);
	        exit(-1);
		}

		/** Create children */
		childpid=fork();
		if(childpid<0)
		{
			sprintf(g_calog,"fork errno[%d]:%s",errno,strerror(errno));
	        SYSLOG(g_calog);
	        exit(-1);
		}
		else if(!childpid) /** children process */
		{
			sprintf(g_calog,"child [%d] create father [%d]",getpid(),getppid());
			SYSLOG(g_calog);
			close(sockfd[1]);
			sprintf(pipefd_c,"%d",sockfd[0]);
			
			execl(g_childpath,g_childname,pipefd_c,NULL);
		}
		else    /** parent process */
		{
			sprintf(g_calog,"Poolmanager pid[%d] create child[%d]\n",getpid(),childpid);
			SYSLOG(g_calog);

			/** Waiting for child register */
			if((ret=Recv(sockfd[1],&status,1,0,1000))<=0)
			{
				sprintf(g_calog,"Poolmanager pid[%d] create child[%d] error\n",getpid(),childpid);
				SYSLOG(g_calog);
				kill_all_children();
				kill(SIGKILL,childpid);
				exit(-1);
			}
			sprintf(g_calog,"child [%d] register",childpid);
			SYSLOG(g_calog);
			sprintf(g_calog,"Poolmanager pid[%d] create child[%d] success\n",getpid(),childpid);
			SYSLOG(g_calog);
			pchildinfo=(struct childinfo*)Malloc(sizeof(struct childinfo));
			if(!pchildinfo)
			{
				sprintf(g_calog,"Malloc error pid[%d]",getpid());
	        	SYSLOG(g_calog);
	        	exit(-1);
			}
			pchildinfo->pipefd=sockfd[1];
			close(sockfd[0]);
			pchildinfo->childpid=childpid;
			pchildinfo->parentpid=getpid();
			pchildinfo->status=status;

			/** Save child information */
			stppoolinfo.childnum++;
			stppoolinfo.freechildnum++;
			childinfo_add(&child_head,pchildinfo);
		}
	}
}

/** Child manager process */
int main(int argc, char const *argv[])
{
	if(argc!=4)
	{
		sprintf(g_calog,"Bad arguments for Child manager pid[%d]",getpid());
		SYSLOG(g_calog);
		exit(-1);
	}

	char *p;
	char *p1;
	char commond;
	struct sigaction sig_usr1;

	memset(&stppoolinfo,0x00,sizeof(struct ppoolinfo));
	memset(g_childpath, 0x00, sizeof(g_childpath));
	memset(g_childname, 0x00, sizeof(g_childname));

	pipefd_f=atoi(argv[1]); 	/**< Descriper comm with parent */
	
	g_childnum=atoi(argv[2]);	/**< Child num */
	strcpy(g_childpath,argv[3]);


	p=g_childpath;
	while(p1=strstr(p,"/"))
	{
		SYSLOG(g_calog);
		p=p1+1;
	}
	strcpy(g_childname,p);
	strcpy(stppoolinfo.childpath,g_childpath);
	strcpy(stppoolinfo.childname,g_childname);
	stppoolinfo.managerid=getpid();
	stppoolinfo.createrid=getppid();
	stppoolinfo.createtime=time(NULL);
	stppoolinfo.status=PUNREGISTER;

	if(access(g_childpath,X_OK|F_OK))
	{
		sprintf(g_calog,"Can't find childpath[%s] pid[%d]",g_childpath,getpid());
		SYSLOG(g_calog);
		exit(-1);
	}

	signal(SIGCHLD,sigchld_func);
	signal(SIGALRM,sigalrm_func);

	sig_usr1.sa_sigaction=sigusr1_func;
	sig_usr1.sa_flags=SA_SIGINFO|SA_RESTART;
	sigaction(SIGUSR1,&sig_usr1,NULL);

	/** Create child */
	child_init(g_childnum);
	/** Manager register to parent */
	if(Send(pipefd_f,"2",1,0,SYSOVERTIME)<=0)
	{
		sprintf(g_calog,"Poolmanager pid[%d] register error[%d]:%s\n",getpid(),errno,strerror(errno));
		SYSLOG(g_calog);
		exit(-1);
	}
	stppoolinfo.status=PREADY;
	alarm(ALARMTIME);
	stppoolinfo.status=PWORK;

	/** into while(1) */
	do_monitor();

	return 0;
}

