#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/epoll.h>

#include "heartbeat.h"
#include "log.h"
#include "returnval.h"
#include "Socket.h"
#include "arrshm.h"
#include "cJSON.h"
#include "packpkg.h"
#include "Tpool.h"

int main(int argc,char** argv)
{

	LOGINIT("/home/ezgaoro/grh/MyResource/fram/etc/log.conf");

	int ret;
	int shmid;
	int sockfd;
	int aptfd;
	int epfd;
	int nfds;
	int loop;
	struct sockaddr_in addr;
	socklen_t addrlen;
	struct epoll_event iev;
	struct epoll_event oev[LISTENMAXNUM];
	MCHINF mchinf;
	char cache[CACHESIZE+1];

	memset(&addr,0x00,sizeof(struct sockaddr_in));
	memset(&iev,0x00,sizeof(struct epoll_event));
	memset(&mchinf,0x00,sizeof(MCHINF));
	addrlen=sizeof(struct sockaddr_in);

	ARRSHMATTR shmattr;
   memset(&shmattr,0x00,sizeof(ARRSHMATTR));
   shmattr.totalsize = 1024*1024;
   shmattr.blocksize = sizeof(MCHINF); 
   shmattr.shmkey=12345;
   shmattr.semkey=12346;
   ArrShmCreate(&shmattr);

	epfd=epoll_create1(0);

	sockfd = InitTCPSocket("132.196.28.87","9494");
	
	iev.data.fd = sockfd;
	iev.events = EPOLLIN|EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&iev);

	while(1)
	{
		nfds = epoll_wait(epfd,oev,LISTENMAXNUM,-1);
		if(nfds == -1)
		{
			SYSLOG(ERROR,"epoll_wait errno[%d]:%s",errno,strerror(errno));
			return SYSERROR;
		}
		for(loop=0;loop<nfds;loop++)
		{
			if ((oev[loop].events & EPOLLERR)||(oev[loop].events & EPOLLHUP)||(!(oev[loop].events & EPOLLIN)))
			{
				SYSLOG(ERROR,"epoll fd[%d] events error",oev[loop].data.fd);
				epoll_ctl(epfd,EPOLL_CTL_DEL,oev[loop].data.fd,NULL);
				close(oev[loop].data.fd);
				continue;
			}
			else
			{
				if(oev[loop].data.fd == sockfd)
				{
					//new clinet
					memset(&addr,0x00,sizeof(struct sockaddr_in));
					aptfd=accept(sockfd,&addr,&addrlen);
					printf("%s\n",inet_ntoa(addr.sin_addr));
					
					//Register to shared memory

					//Join the epoll queue
					memset(&iev,0x00,sizeof(struct epoll_event));
					iev.data.fd = aptfd;
					iev.events = EPOLLIN|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_ADD,aptfd,&iev);
				}
				else
				{
					ret=PkgRecv(oev[loop].data.fd,cache,sizeof(cache),BLOCK);
					if(ret == 0)
					{
						epoll_ctl(epfd,EPOLL_CTL_DEL,oev[loop].data.fd,NULL);
						close(oev[loop].data.fd);
						SYSLOG(INFO,"Client fd[%d] down",oev[loop].data.fd);
						continue;
					}
					else
					{
						memset(&mchinf,0x00,sizeof(MCHINF));
						UnPackHBpkg4J(&mchinf,cache);
						printf("%s\n",mchinf.name);
					}
				}
			}
		}
	}

	CloseTCPSocket(sockfd);
	return 0;

}
