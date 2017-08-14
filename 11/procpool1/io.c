#include "io.h"
#include "uerror.h"
#include "log.h"

/*int Select(int maxfd, fd_set *readfds, fd_set writefds, fd_set expectfds, size_t time)
{
	struct timeval tv;
	tv.tv_sec=time/1000;
	tv.tv_usec=time%1000;

	struct timeval* ptv;

	time ? (ptv=&tv) : (ptv=NULL);

}*/
extern char g_calog[1024];

ssize_t Send(int sockfd,void *buf,size_t size,int flags,int overtime)
{
	fd_set wfds;
	struct timeval *povertime;
    struct timeval tv;
    int ret;
    time_t first;
	time_t second;
	double xtime;
	if(overtime)
		first=time(NULL);

SENDAGAIN:
	if(overtime)
	{
		second=time(NULL);
		xtime=difftime(second,first);
	}
    FD_ZERO(&wfds);
    FD_SET(sockfd,&wfds);

    if(overtime)
    {
	    tv.tv_sec = (overtime-(int)xtime)/1000;
	    tv.tv_usec = ((overtime%1000)*1000)-((xtime-(int)xtime)*1000000);
	    povertime=&tv;
	}
	else
	{
		povertime=NULL;
	}

    ret = select(sockfd+1, NULL, &wfds, NULL, povertime);
	if(ret==-1)
	{
		if(errno==EINTR)
		{
			goto SENDAGAIN;
		}
		else
		{
			sprintf(g_calog,"select errno[%d]:%s",errno,strerror(errno));
			SYSLOG(g_calog);
			return ret;
		}
	}
    else if(ret)
    	return send(sockfd,buf,size,flags|MSG_DONTWAIT);
    else
        return TIMEOUT;
}

ssize_t Recv(int sockfd,void *buf,size_t size,int flags,int overtime)
{
	fd_set rfds;
	struct timeval *povertime;
    struct timeval tv;
    int ret;
    time_t first;
	time_t second;
	double xtime;
	if(overtime)
		first=time(NULL);

RECVAGAIN:
	if(overtime)
	{
		second=time(NULL);
		xtime=difftime(second,first);
	}
    FD_ZERO(&rfds);
    FD_SET(sockfd,&rfds);

    if(overtime)
    {
	    tv.tv_sec = (overtime-(int)xtime)/1000;
	    tv.tv_usec = ((overtime%1000)*1000)-((xtime-(int)xtime)*1000000);
	    povertime=&tv;
	}
	else
	{
		povertime=NULL;
	}

    ret = select(sockfd+1, &rfds, NULL, NULL, povertime);
	if(ret==-1)
	{
		if(errno==EINTR)
		{
			goto RECVAGAIN;
		}
		else
		{
			sprintf(g_calog,"select errno[%d]:%s",errno,strerror(errno));
			SYSLOG(g_calog);
			return ret;
		}
	}
    else if(ret)
    	return recv(sockfd,buf,size,flags|MSG_DONTWAIT);
    else
        return TIMEOUT;
}
