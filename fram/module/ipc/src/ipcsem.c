#include "ipcsem.h"
#include "log.h"
#include "mem.h"
#include "returnval.h"

extern char g_calog[1024];
static int iRnum;
/**
 * [vipc_sem_create description]
 * 创建信号量集
 * @param  key    [IPC_KEY值]
 * @param  semnum [信号量数量]
 * @param  mode   [读写权限]
 * @param  num    [初始值]
 * @return        [成功返回描述符，失败返回-1]
 */
int vipc_sem_create(key_t key, int semnum, mode_t mode, int num)
{
	int ret;
	int i;
	int semid;
	union semun arg;
	unsigned short* ptr;

	if((ret=semget(key, semnum, mode|IPC_CREAT|IPC_EXCL))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;		
	}
	semid=ret;

	ptr=Malloc(semnum*sizeof(unsigned short));
	arg.array=ptr;

	for(i=0; i<semnum; i++)
	{
		ptr[i]=num;
	}
	if((ret=semctl(semid, 0, SETALL, arg))==-1)
	{
		free(ptr);
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;		
	}
	free(ptr);
	return semid;
}

/**
 * [vipc_sem_getid_safe description]
 * 获取信号量集描述符(安全模式)
 * @param  key [IPC_KEY]
 * @return     [成功返回描述符，失败返回-1]
 */
int vipc_sem_getid_safe(key_t key)
{
	int ret;
	int semid;
	union semun arg;
	struct semid_ds seminfo;
	
	if((ret=semget(key, 0, 0))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;		
	}
	semid=ret;
	arg.buf=&seminfo;

	if((ret=semctl(semid,0,IPC_STAT,arg))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;	
	}
	if(seminfo.sem_otime)
		return semid;
	else
		sleep(1);
	if(!seminfo.sem_otime)
		return SYSERROR;

	return semid;
}

/**
 * [vipc_sem_getid_safe description]
 * 获取信号量集描述符
 * @param  key [IPC_KEY]
 * @return     [成功返回描述符，失败返回-1]
 */
int vipc_sem_getid(key_t key)
{
	int ret;
	if((ret=semget(key, 0, 0))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;		
	}
	return ret;
}

int vipc_sem_destory(int semid)
{
	int ret;
	if((ret=semctl(semid,0,IPC_RMID))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

int vipc_sem_getval(int semid,int semnum)
{
	int ret;
	/*union semun arg;
	struct semid_ds seminfo;
	
	arg.buf=&seminfo;*/

	if((ret=semctl(semid,semnum,GETVAL,NULL))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;	
	}
	return ret;
}

/**
 * [vipc_sem_grab description]
 * 抢占信号量(获取资源)
 * @param  semid  [描述符]
 * @param  semnum [信号量标号]
 * @return        [成功返回0，失败返回-1]
 */
int vipc_sem_grab(int semid, int semnum, int maxtime)
{
	int ret;
	struct sembuf ops;
	struct timespec timeout;
	struct timespec* otime;

	ops.sem_num=semnum;
	ops.sem_op=-1;
	ops.sem_flg=0;     /*进程结束后恢复所有信号量*/

	timeout.tv_sec=maxtime/1000;
	timeout.tv_nsec=maxtime%1000*1000000;

	maxtime?(otime=&timeout):(otime=NULL);

	if((ret=semtimedop(semid, &ops, 1, otime))==-1)
	{
		if(errno==EAGAIN)
		{
			return TIMEOUT;
		}
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [vipc_sem_grab_masksig description]
 * 抢占信号量，防信号打断(获取资源)
 * @param  semid  [描述符]
 * @param  semnum [信号量标号]
 * @return        [成功返回0，失败返回-1]
 */
int vipc_sem_grab_masksig(int semid, int semnum, int maxtime)
{
	int ret;
	struct sembuf ops;
	struct timespec timeout;
	struct timespec* otime;
	time_t first;
	time_t second;
	double xtime;

	ops.sem_num=semnum;
	ops.sem_op=-1;
	ops.sem_flg=0;

	timeout.tv_sec=maxtime/1000;
	timeout.tv_nsec=maxtime%1000*1000000;

	maxtime?(otime=&timeout):(otime=NULL);

	first=0;
	if(otime)
		first=time(NULL);

VSEMGRABAGAIN:
	if((ret=semtimedop(semid, &ops, 1, otime))==-1)
	{
		if(errno==EINTR)
		{
			if(otime)
			{
				second=time(NULL);
				xtime=difftime(second,first);
				otime->tv_sec -= (int)(xtime);
				otime->tv_nsec -= (int)((xtime-(int)xtime)*1000000000);
				first=second;
			}
			goto VSEMGRABAGAIN;
		}
		if(errno==EAGAIN)
		{
			return TIMEOUT;
		}
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [vipc_sem_grab_masksig description]
 * 抢占信号量，非阻塞(获取资源)
 * @param  semid  [描述符]
 * @param  semnum [信号量标号]
 * @return        [成功返回0，失败返回-1]
 */
int vipc_sem_grab_nowait(int semid, int semnum)
{
	int ret;
	struct sembuf ops;
	ops.sem_num=semnum;
	ops.sem_op=-1;
	ops.sem_flg=IPC_NOWAIT;

	if((ret=semtimedop(semid, &ops, 1, NULL))==-1)
	{
		if(errno==EAGAIN)
		{
			return NORESOURCE;
		}
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}
/**
 * [vipc_sem_free description]
 * 释放信号量(释放资源)
 * @param  semid  [描述符]
 * @param  semnum [信号量标号]
 * @return        [成功返回0，失败返回-1]
 */
int vipc_sem_free(int semid, int semnum)
{
	int ret;
	struct sembuf ops;
	ops.sem_num=semnum;
	ops.sem_op=1;
	ops.sem_flg=0;
	if((ret=semop(semid, &ops, 1))==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/*int vipc_sem_free_masksig(int semid, int semnum)
{
	int ret;
	struct sembuf ops;
	ops.sem_num=semnum;
	ops.sem_op=1;
	ops.sem_flg=0;

VSEMFREEAGAIN:
	if((ret=semop(semid, &ops, 1))==-1)
	{
		if(errno==EINTR)
			goto VSEMFREEAGAIN;
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}*/

/**
 * [vipc_sem_RWlock_create description]
 * 创建读写锁，0信号量为读锁，1信号量为写锁
 * @param  key  [IPC_KEY]
 * @param  mode [读写权限]
 * @return      [成功返回描述符，失败返回-1]
 */
int vipc_sem_RWlock_create(key_t key,mode_t mode)
{
	int ret;
	if((ret=vipc_sem_create(key, 2, mode,1))==-1)
	{
		strcpy(g_calog, "vipc_sem_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return ret;
}

int vipc_sem_RWlock_getid(key_t key)
{
	int ret;
	if((ret=vipc_sem_getid(key))==-1)
	{
		strcpy(g_calog, "vipc_sem_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return ret;
}

int vipc_sem_RWlock_sgetid(key_t key)
{
	int ret;
	if((ret=vipc_sem_getid_safe(key))==-1)
	{
		strcpy(g_calog, "vipc_sem_getid_safe error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return ret;
}

/**
 * [vipc_sem_Wlocked description]
 * 加写锁
 * @param  semid [描述符]
 * @param  time  [超时时间]
 * @return       [成功返回0，失败返回-1]
 */
int vipc_sem_Wlocked(int semid,int maxtime)
{
	int ret;
	time_t first;
	time_t second;
	double xtime;

	memset(&first,0x00,sizeof(time_t));
	
	if(maxtime)
	{
		first=time(NULL);
	}
	ret=vipc_sem_grab_masksig(semid, 1, maxtime);
	if(ret)
	{
		if(ret==-1)
		{
			strcpy(g_calog,"vipc_sem_grab_masksig W error");
			SYSLOG(ERROR,"%s",g_calog);
			return SYSERROR;
		}
		return ret;
	}
	if(maxtime)
	{
		second=time(NULL);
		xtime=difftime(second,first);
		maxtime -= (int)(xtime*1000);
	}
	ret=vipc_sem_grab_masksig(semid, 0, maxtime);
	if(ret)
	{
		if(ret==-1)
		{
			strcpy(g_calog,"vipc_sem_grab_masksig R error");
			SYSLOG(ERROR,"%s",g_calog);
			return SYSERROR;
		}
		return ret;
	}
	return ret;
}

/**
 * [vipc_sem_Wunlock description]
 * 解写锁
 * @param  semid [描述符]
 * @return       [成功返回0，失败返回-1]
 */
int vipc_sem_Wunlock(int semid)
{
	int ret;
	ret=vipc_sem_free(semid,0);
	if(ret==-1)
	{
		strcpy(g_calog,"vipc_sem_free R error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	ret=vipc_sem_free(semid,1);
	if(ret==-1)
	{
		strcpy(g_calog,"vipc_sem_free W error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [vipc_sem_Rlocked description]
 * 加读锁
 * @param  semid [描述符]
 * @param  time  [超时时间]
 * @return       [成功返回0，失败返回-1]
 */
int vipc_sem_Rlocked(int semid,int maxtime)
{
	int ret;
	time_t first;
	time_t second;
	double xtime;

	memset(&first,0x00,sizeof(time_t));

	if(maxtime)
	{
		first=time(NULL);
	}
	ret=vipc_sem_grab_nowait(semid, 1);
	if(ret && (ret!=NORESOURCE))
	{
		strcpy(g_calog,"vipc_sem_grab_masksig W error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	if(maxtime)
	{
		second=time(NULL);
		xtime=difftime(second,first);
		maxtime -= (int)(xtime*1000);
	}
	ret=vipc_sem_grab_masksig(semid, 0, maxtime );
	if(ret)
	{
		if(ret==-1)
		{
			strcpy(g_calog,"vipc_sem_grab_masksig R error");
			SYSLOG(ERROR,"%s",g_calog);
			return SYSERROR;
		}
		return ret;
	}
	ret=vipc_sem_free(semid, 0);
	if(ret==-1)
	{
		strcpy(g_calog,"vipc_sem_free R error");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	iRnum++;
	return SUCCESS;
}

/**
 * [vipc_sem_Runlock description]
 * 解读锁
 * @param  semid [描述符]
 * @return       [成功返回0，失败返回-1]
 */
int vipc_sem_Runlock(int semid)
{
	int ret;
	iRnum--;
	if(!iRnum)
	{
		ret=vipc_sem_free(semid,1);
		if(ret==-1)
		{
			strcpy(g_calog,"vipc_sem_free W error");
			SYSLOG(ERROR,"%s",g_calog);
			return SYSERROR;
		}
	}
	return SUCCESS;
}
/**
 * [vipc_sem_RWlock_destory description]
 * 销毁读写锁
 * @param  semid [描述符]
 * @return       [成功返回0，失败返回-1]
 */
int vipc_sem_RWlock_destory(int semid)
{
	return vipc_sem_destory(semid);
}
