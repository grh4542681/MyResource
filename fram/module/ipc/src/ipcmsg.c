#include "ipcmsg.h"
#include "log.h"
#include "mem.h"

extern char g_calog[1024];
static int i_vmsg_overtime_flag;

/*System V mq*/
/**
 * [vipc_msg_create description]
 * 创建消息队列
 * @param  key  [IPC键值]
 * @param  mode [读写执行权限位]
 * @return      [成功返回描述符，失败返回-1]
 */
int vipc_msg_create(key_t key,mode_t mode)
{
	int ret;
	ret=msgget(key,mode|IPC_CREAT|IPC_EXCL);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [vipc_msg_getid description]
 * 获取消息队列描述符
 * @param  key [IPC键值]
 * @return     [成功返回描述符，失败返回-1]
 */
int vipc_msg_getid(key_t key)
{
	int ret;
	ret=msgget(key,0);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [vipc_msg_destory description]
 * 销毁消息队列
 * @param  id [消息队列描述符]
 * @return    [成功返回0，失败返回！0]
 */
int vipc_msg_destory(int id)
{
	int ret;
	ret=msgctl(id,IPC_RMID,0);
	if(ret)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [vipc_msg_send_nowait description]
 * 写消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  len     [数据长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回0，失败返回-1]
 */
int vipc_msg_send(int id, void* data, unsigned int len, long msgtype)
{
	int ret;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);
	memcpy(ptr,&msgtype,sizeof(long));
	memcpy(ptr+sizeof(long),data,len);

	ret=msgsnd(id,ptr,sizeof(long)+len,0);
	if(ret)
	{
		free(ptr);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	free(ptr);
	return ret;
}

/**
 * [vipc_msg_send_nowait description]
 * 非阻塞模式写消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  len     [数据长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回0，失败返回-1]
 */
int vipc_msg_send_nowait(int id,void* data,unsigned int len,long msgtype)
{
	int ret;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);
	memcpy(ptr,&msgtype,sizeof(long));
	memcpy(ptr+sizeof(long),data,len);

	ret=msgsnd(id,ptr,sizeof(long)+len,IPC_NOWAIT);
	if(ret)
	{
		free(ptr);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	free(ptr);
	return ret;
}

/**
 * [vipc_msg_send description]
 * 阻塞模式写消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  len     [数据长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回0，失败返回-1]
 */
int vipc_msg_send_masksig(int id,void* data,unsigned int len,long msgtype)
{
	int ret;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);
	memcpy(ptr,&msgtype,sizeof(long));
	memcpy(ptr+sizeof(long),data,len);

VMSGSENDAGAIN:
	ret=msgsnd(id,ptr,sizeof(long)+len,0);
	if(ret)
	{
		if(errno==EINTR)
		{
			goto VMSGSENDAGAIN;
		}
		else
		{
			free(ptr);
			sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
	}
	free(ptr);
	return ret;
}

/**
 * [vipc_msg_recv_nowait description]
 * 读消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  dlen    [数据区长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回接收长度，失败返回-1]
 */
int vipc_msg_recv(int id,void *data,unsigned int dlen,long msgtype)
{
	int ret;
	int len=dlen?dlen:MAXMSGLEN;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);

	ret=msgrcv(id,ptr,sizeof(long)+len,msgtype,0);
	if(ret==-1)
	{
		free(ptr);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	memcpy(data,ptr+sizeof(long),len);
	free(ptr);
	return ret;
}
/**
 * [vipc_msg_recv_nowait description]
 * 非阻塞模式读消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  dlen    [数据区长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回接收长度，失败返回-1]
 */
int vipc_msg_recv_nowait(int id,void *data,unsigned int dlen,long msgtype)
{
	int ret;
	int len=dlen?dlen:MAXMSGLEN;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);

	ret=msgrcv(id,ptr,sizeof(long)+len,msgtype,IPC_NOWAIT);
	if(ret==-1)
	{
		free(ptr);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	memcpy(data,ptr+sizeof(long),len);
	free(ptr);
	return ret;
}

/**
 * [vipc_msg_recv_nowait description]
 * 阻塞模式读消息队列
 * @param  id      [消息队列描述符]
 * @param  data    [数据区地址]
 * @param  dlen    [数据区长度]
 * @param  msgtype [消息类型]
 * @return         [成功返回接收长度，失败返回-1]
 */
int vipc_msg_recv_masksig(int id,void *data,unsigned int dlen,long msgtype)
{
	int ret;
	int len=dlen?dlen:MAXMSGLEN;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);

VMSGRECVAGAIN:
	ret=msgrcv(id,ptr,sizeof(long)+len,msgtype,0);
	if(ret==-1)
	{
		if(errno==EINTR)
		{
			goto VMSGRECVAGAIN;
		}
		else
		{
			free(ptr);
			sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
	}
	memcpy(data,ptr+sizeof(long),len);
	free(ptr);
	return ret;
}

/**
 * [vipc_msg_clean description]
 * 清空队列
 * @param  id [消息队列描述符]
 * @return    [成功返回0，失败返回-1]
 */
int vipc_msg_clean(int id,long msgtype)
{
	int ret;
	int len=MAXMSGLEN;
	char* ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);

	while((ret=msgrcv(id,ptr,sizeof(long)+len,msgtype,IPC_NOWAIT))!=-1)
	{
		sprintf(g_calog,"clean msg szie[%d]",ret);
		SYSLOG(ERROR,"%s",g_calog);
	}
	free(ptr);
	if(errno==EAGAIN||errno==ENOMSG)
	{
		return 0;
	}
	sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
	SYSLOG(ERROR,"%s",g_calog);
	return ret;
}

void vmsg_alram()
{
	i_vmsg_overtime_flag=1;
}

/**
 * [vipc_msg_send_recv description]
 * 发送消息并在超时时间到达之前接受回应
 * @param  id       [消息队列描述符]
 * @param  in       [发送数据缓冲区]
 * @param  inlen    [发送数据缓冲区长度]
 * @param  out      [接收数据缓冲区]
 * @param  outlen   [接收数据缓冲区长度]
 * @param  smsgtype [发送消息类型]
 * @param  rmsgtype [接收消息类型]
 * @param  utime    [超时时间]
 * @return          [成功返回0，失败返回-1]
 */
int vipc_msg_send_recv(int id,char* in,size_t inlen,char* out,size_t outlen,long smsgtype,long rmsgtype,long time)
{
	int ret;
	int len;
	char *ptr;

	i_vmsg_overtime_flag=0;

	ret=vipc_msg_send(id, in, inlen, smsgtype);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","vipc_msg_send_recv send error");
		return -1;
	}
	
	signal(SIGALRM,vmsg_alram);
	alarm(time);

	len=outlen?outlen:MAXMSGLEN;
	ptr=Malloc(sizeof(long)+len);
	if(!ptr)
		return -1;
	memset(ptr,0,sizeof(long)+len);
 
VMSGRECVAGAINALARM:
	ret=msgrcv(id,ptr,sizeof(long)+len,rmsgtype,0);
	if(ret==-1)
	{
		if(errno==EINTR)
		{
			if(!i_vmsg_overtime_flag)
				goto VMSGRECVAGAINALARM;
		}
		else
		{
			free(ptr);
			sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
	}
	if(!i_vmsg_overtime_flag)
	{
		alarm(0);
		memcpy(out,ptr+sizeof(long),len);
		free(ptr);
		return ret;
	}
	else
	{
		SYSLOG(ERROR,"%s","vipc_msg_send_recv timeout");
		return -2;
	}

}

/**
 * [vipc_msg_info description]
 * 获取消息队列信息
 * @param id   [消息队列描述符]
 * @param info [系统定义的消息队列结构体指针]
 */
void vipc_msg_info(int id,struct msqid_ds* info)
{
	int ret;
	ret=msgctl(id,IPC_STAT,info);
	if(ret)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return;
	}
	return;
}




/*POSIX mq*/
/**
 * [pipc_msg_create description]
 * 创建消息队列
 * @param  name [消息队列名]
 * @param  mode [队列权限]
 * @param  attr [队列属性]
 * @return      [成空返回描述符，失败返回-1]
 */
mqd_t pipc_msg_create(const char* name,mode_t mode,struct mq_attr *attr)
{
	mqd_t mqd;
	mqd=mq_open(name,O_RDWR|O_CREAT|O_EXCL,mode,attr);
	if(mqd==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return mqd;
}

/**
 * [pipc_msg_getid description]
 * 获取消息队列描述符
 * @param  name [消息队列名]
 * @return      [成功返回描述符，失败返回-1]
 */
mqd_t pipc_msg_getid(const char* name)
{
	mqd_t mqd;
	mqd=mq_open(name,O_RDWR);
	if(mqd==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return mqd;
}

/**
 * [pipc_msg_close description]
 * 关闭队列描述符
 * @param  mqd [描述符]
 * @return     [成功返回0，失败返回-1]
 */
int pipc_msg_close(mqd_t mqd)
{
	int ret=mq_close(mqd);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return ret;
}

/**
 * [__pipc_msg_destory description]
 * 销毁消息队列
 * @param  name [消息队列名字]
 * @return      [成功返回0，失败返回-1]
 */
int __pipc_msg_destory(const char* name)
{
	int ret=mq_unlink(name);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [pipc_msg_destory description]
 * 销毁消息队列
 * @param  name [消息队列名]
 * @param  num  [打开的描述符个数]
 * @return      [成功返回0，失败返回-1]
 */
int pipc_msg_destory(const char* name, size_t num)
{
	int ret;
	int loop=0;

	mqd_t mqd=pipc_msg_getid(name);
	if(pipc_msg_clean(mqd,0))
	{
		pipc_msg_close(mqd);
		SYSLOG(ERROR,"%s","pipc_msg_clean error");
		return -1;
	}
	//data=Malloc()
	for(loop=0;loop<num;loop++)
	{
		if((ret=pipc_msg_send(mqd,NULL,0,31))==-1)
		{
			SYSLOG(ERROR,"%s","pipc_msg_send error");
			return -1;
		}
	}
	pipc_msg_close(mqd);
	if((ret=__pipc_msg_destory(name))==-1)
	{
		SYSLOG(ERROR,"%s","__pipc_msg_destory error");
		return -1;
	}
	return 0;
}

/**
 * [pipc_msg_getattr description]
 * 获取消息队列属性
 * @param  mqd  [描述符]
 * @param  attr [传出参数，属性结构体指针]
 * @return      [成功返回0，失败返回-1]
 */
int pipc_msg_getattr(mqd_t mqd,struct mq_attr *attr)
{
	int ret=mq_getattr(mqd,attr);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [pipc_msg_setattr description]
 * 获取消息队列属性
 * @param  mqd     [描述符]
 * @param  newattr [设置的属性结构体指针]
 * @param  oldattr [原来的属性结构体指针]
 * @return         [成功返回0，失败返回-1]
 */
int pipc_msg_setattr(mqd_t mqd,struct mq_attr *newattr,struct mq_attr *oldattr)
{
	int ret=mq_setattr(mqd,newattr,oldattr);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return ret;
}

/**
 * [pipc_msg_send description]
 * 写消息队列
 * @param  mqd  [描述符]
 * @param  ptr  [数据区指针]
 * @param  len  [数据区长度]
 * @param  prio [消息级别]
 * @return      [成功返回0，失败返回-1]
 */
int pipc_msg_send(mqd_t mqd,const char* ptr,size_t len,unsigned int prio)
{
	if(prio>=MQ_PRIO_MAX)
	{
		SYSLOG(ERROR,"%s","bad prio");
		return -1;
	}
	int ret;
	struct mq_attr newattr;
	struct mq_attr oldattr;

	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));

	newattr.mq_flags=O_NONBLOCK;
	ret=pipc_msg_setattr(mqd,&newattr,&oldattr);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_setattr error");
		return -1;
	}

PMSGSENDAGAIN:
	ret=mq_send(mqd,ptr,len,prio);
	if(ret==-1)
	{
		if(errno==EINTR)
			goto PMSGSENDAGAIN;
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	ret=pipc_msg_setattr(mqd,&oldattr,&newattr);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_setattr error");
		return -1;
	}
	return ret;
}

/**
 * [pipc_msg_send_nowait description]
 * 非阻塞式写消息队列
 * @param  mqd  [描述符]
 * @param  ptr  [数据区指针]
 * @param  len  [数据区长度]
 * @param  prio [消息级别]
 * @return      [成功返回0，失败返回-1]
 */
int pipc_msg_send_nowait(mqd_t mqd,const char* ptr,size_t len,unsigned int prio)
{
	if(prio>=MQ_PRIO_MAX)
	{
		SYSLOG(ERROR,"%s","bad prio");
		return -1;
	}
	int ret;
	struct mq_attr newattr;
	struct mq_attr oldattr;

	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));

	newattr.mq_flags=O_NONBLOCK;
	ret=pipc_msg_setattr(mqd,&newattr,&oldattr);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_setattr error");
		return -1;
	}
	ret=mq_send(mqd,ptr,len,prio);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	ret=pipc_msg_setattr(mqd,&oldattr,&newattr);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_setattr error");
		return -1;
	}
	return 0;
}

/**
 * [pipc_msg_recv description]
 * 读消息队列
 * @param  mqd  [描述符]
 * @param  ptr  [数据区指针]
 * @param  len  [数据区长度]
 * @param  prio [消息级别]
 * @return      [成功返回接受长度，失败返回-1]
 */
ssize_t pipc_msg_recv(mqd_t mqd,char* ptr,size_t len,unsigned int* prio)
{
	struct mq_attr newattr;
	struct mq_attr oldattr;
	int ret;
	ssize_t rlen;
	char* data;

	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));

	newattr.mq_flags=0;
	ret=pipc_msg_setattr(mqd,&newattr,&oldattr);
	if(ret==-1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_setattr error");
		return -1;
	}
	data=(char*)Malloc(oldattr.mq_msgsize);
	if(!data)
		return -1;
	memset(data,0,oldattr.mq_msgsize);

PMSGRECVAGAIN:
	rlen=mq_receive(mqd,data,oldattr.mq_msgsize,prio);
	if(rlen==-1)
	{
		if(errno==EINTR)
			goto PMSGRECVAGAIN;
		free(data);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	else if(rlen > len)
	{
		free(data);
		ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
		if(ret == -1)
		{
			SYSLOG(ERROR,"%s","pipc_msg_getattr error");
			return -1;
		}
		SYSLOG(ERROR,"%s","Out of buffers");
		return -1;
	}
	ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		return -1;
	}
	if(*prio==31)
	{
		return pipc_msg_close(mqd);
	}
	memcpy(ptr,data,rlen);
	free(data);
	return rlen;
}

/**
 * [pipc_msg_recv_nowait description]
 * 非阻塞式读队列
 * @param  mqd  [描述符]
 * @param  ptr  [数据区指针]
 * @param  len  [数据区长度]
 * @param  prio [消息级别]
 * @return      [成功返回接收长度，失败返回-1]
 */
ssize_t pipc_msg_recv_nowait(mqd_t mqd, char* ptr, size_t len, unsigned int* prio)
{
	int ret;
	ssize_t rlen;
	char* data;
	struct mq_attr newattr;
	struct mq_attr oldattr;

	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));
	newattr.mq_flags=O_NONBLOCK;
	ret=pipc_msg_setattr(mqd, &newattr, &oldattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		return -1;
	}
	data = (char*)Malloc(oldattr.mq_msgsize);
	if(!data)
		return -1;
	memset(data, 0, oldattr.mq_msgsize);

	rlen=mq_receive(mqd, data, oldattr.mq_msgsize, prio);
	if(rlen == -1)
	{
		free(data);
		if(errno==EAGAIN)
		{
			return 0;
		}
		else{
			sprintf(g_calog, "errno[%d]:%s", errno, strerror(errno));
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
	}
	else if(rlen > len)
	{
		free(data);
		ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
		if(ret == -1)
		{
			SYSLOG(ERROR,"%s","pipc_msg_getattr error");
			return -1;
		}
		SYSLOG(ERROR,"%s","Out of buffers");
		return -1;
	}

	ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		return -1;
	}
	if(*prio==31)
	{
		return pipc_msg_close(mqd);
	}
	memcpy(ptr,data,rlen);
	free(data);
	return rlen;
}

/**
 * [pipc_msg_clean description]
 * 清空消息队列
 * @param  mqd [描述符]
 * @return     [成功返回0，失败返回-1]
 */
int pipc_msg_clean(mqd_t mqd,unsigned int* prio)
{
	int ret;
	int rlen;
	char* data;
	struct mq_attr newattr;
	struct mq_attr oldattr;

	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));
	newattr.mq_flags=O_NONBLOCK;
	ret=pipc_msg_setattr(mqd, &newattr, &oldattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		return -1;
	}
	data = (char*)Malloc(oldattr.mq_msgsize);
	if(!data)
		return -1;
	memset(data, 0, oldattr.mq_msgsize);
	while((rlen=mq_receive(mqd, data, oldattr.mq_msgsize, prio))!=-1)
	{
		sprintf(g_calog,"msg_clean size[%d]",rlen);
		SYSLOG(ERROR,"%s",g_calog);
	}

	free(data);

	ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		return -1;
	}
	if(errno==EAGAIN)
		return 0;
	else
		return -1;
}

/**
 * [pipc_msg_thread_func description]
 * @param sv [description]
 */
void pipc_msg_thread_func(union sigval sv)
{
	int ret;
	ssize_t rlen;
	mqd_t mqd=*((mqd_t*)(((struct msg_notify_para*)sv.sival_ptr)->pmqd));
	struct sigevent sev;
	char* data;
	struct mq_attr newattr;
	struct mq_attr oldattr;

	memset(&sev, 0, sizeof(struct sigevent));
	memset(&newattr, 0, sizeof(struct mq_attr));
	memset(&oldattr, 0, sizeof(struct mq_attr));

	memcpy(&sev,((struct msg_notify_para*)sv.sival_ptr)->psev, sizeof(struct sigevent));
	if((ret = mq_notify(mqd, &sev))==-1)
	{
		sprintf(g_calog, "errno[%d]:%s", errno, strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		pthread_exit((void*)-1);
	}

	newattr.mq_flags=O_NONBLOCK;
	ret=pipc_msg_setattr(mqd, &newattr, &oldattr);
	if(ret == -1)
	{
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		pthread_exit((void*)-1);;
	}

	data = (char*)Malloc(oldattr.mq_msgsize);
	if(!data)
		pthread_exit((void*)-1);
	memset(data, 0, oldattr.mq_msgsize);

	while((rlen=mq_receive(mqd, data, oldattr.mq_msgsize, NULL)) >=0 )
	{
		(*(((struct msg_notify_para*)sv.sival_ptr)->pfunc))((void*)data);
	}
	if(errno!=EAGAIN)
	{
		free(data);
		SYSLOG(ERROR,"%s","pipc_msg_thread_func error");
		pthread_exit((void*)-1);;
	}

	ret=pipc_msg_setattr(mqd, &oldattr, &newattr);
	if(ret == -1)
	{
		free(data);
		SYSLOG(ERROR,"%s","pipc_msg_getattr error");
		pthread_exit((void*)-1);
	}
	free(data);
	pthread_exit(NULL);
}

/**
 * [pipc_msg_notify_thread description]
 * @param  mqd         [description]
 * @param  func        [description]
 * @param  pthreadattr [description]
 * @return             [description]
 */
int pipc_msg_notify_thread(mqd_t mqd, void (*func)(void*), pthread_attr_t *pthreadattr)
{
	int ret;
	struct sigevent sev;
	struct msg_notify_para para;

	memset(&sev,0,sizeof(sev));
	memset(&para, 0, sizeof(para));

	para.pmqd = &mqd;
	para.psev = &sev;
	para.pfunc = (void (*)(void*))func;

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = pipc_msg_thread_func;
	sev.sigev_notify_attributes = pthreadattr;
	sev.sigev_value.sival_ptr = &para;

	if((ret = mq_notify(mqd, &sev))==-1)
	{
		sprintf(g_calog, "errno[%d]:%s", errno, strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return 0;
}
