#ifndef __IPCMSG_H__
#define __IPCMSH_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <mqueue.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <bits/local_lim.h>

#define MAXMSGLEN 1024

typedef struct msg_notify_para
{
	void* pmqd;
	void (*pfunc)(void*);
	struct sigevent* psev;
}msg_notify_para;


int vipc_msg_create(key_t key,mode_t mode);
int vipc_msg_getid(key_t key);
int vipc_msg_destory(int id);
int vipc_msg_send_nowait(int id,void* data,unsigned int len,long msgtype);
int vipc_msg_send(int id,void* data,unsigned int len,long msgtype);
int vipc_msg_send_masksig(int id,void* data,unsigned int len,long msgtype);
int vipc_msg_recv_nowait(int id,void *data,unsigned int dlen,long msgtype);
int vipc_msg_recv(int id,void *data,unsigned int dlen,long msgtype);
int vipc_msg_recv_masksig(int id,void *data,unsigned int dlen,long msgtype);
int vipc_msg_clean(int id,long msgtype);
int vipc_msg_send_recv(int id,char* in,size_t inlen,char* out,size_t outlen,long smsgtype,long rmsgtype,long utime);
void vipc_msg_info(int id,struct msqid_ds* info);

mqd_t pipc_msg_create(const char* name,mode_t mode,struct mq_attr *attr);
mqd_t pipc_msg_getid(const char* name);
int pipc_msg_close(mqd_t mqd);
int __pipc_msg_destory(const char* name);
int pipc_msg_destory(const char* name, size_t num);
int pipc_msg_getattr(mqd_t mqd,struct mq_attr *attr);
int pipc_msg_setattr(mqd_t mqd,struct mq_attr *newattr,struct mq_attr *oldattr);
int pipc_msg_send(mqd_t mqd,const char* ptr,size_t len,unsigned int prio);
int pipc_msg_send_nowait(mqd_t mqd,const char* ptr,size_t len,unsigned int prio);
ssize_t pipc_msg_recv(mqd_t mqd,char* ptr,size_t len,unsigned int* prio);
ssize_t pipc_msg_recv_nowait(mqd_t mqd, char* ptr, size_t len, unsigned int* prio);
int pipc_msg_clean(mqd_t mqd,unsigned int* prio);
void pipc_msg_thread_func(union sigval sv);
int pipc_msg_notify_thread(mqd_t mqd, void (*func)(void*), pthread_attr_t *pthreadattr);

#endif
