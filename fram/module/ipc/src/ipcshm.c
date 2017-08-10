#include "ipcshm.h"
#include "ipcsem.h"
#include "mem.h"
#include "log.h"
#include "returnval.h"

char g_calog[1024];
unsigned int g_RWshmid;

/**
 * [_vipc_shm_create description]
 * 创建共享内存
 * @param  key  [IPC_KEY]
 * @param  size [共享内存大小]
 * @param  mode [读写权限]
 * @return      [成功返回描述符，失败返回-1]
 */
int _vipc_shm_create(key_t key,size_t size,mode_t mode)
{
	memset(g_calog,0,sizeof(g_calog));

	if(size<=0)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	int ret;
	ret=shmget(key,size,mode|IPC_CREAT|IPC_EXCL);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return ret;
}

/**
 * [_vipc_shm_getid description]
 * 获取描述符
 * @param  key  [IPC_KEY]
 * @return     [成功返回描述符，失败返回-1]
 */
int _vipc_shm_getid(key_t key)
{
	memset(g_calog,0,sizeof(g_calog));

	int ret;
	ret=shmget(key,0,0);
	if(ret==-1)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return ret;
}

/**
 * [_vipc_shm_open description]
 * 打开共享内存
 * @param  id   [描述符]
 * @param  mptr [共享内存首地址 传出参数]
 * @return      [成功返回0，失败返回-1]
 */
int _vipc_shm_open(int id,void** mptr)
{
	*mptr=shmat(id,NULL,0);
	if((long)(*mptr)<0)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [_vipc_shm_close description]
 * 关闭共享内存
 * @param  mptr [共享内存首地址 传出参数]
 * @return      [成功返回0，失败返回-1]
 */
int _vipc_shm_close(void* mptr)
{
	int ret;
	ret=shmdt(mptr);
	if(ret)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [_vipc_shm_destroy description]
 * 销毁共享内存
 * @param  id [描述符]
 * @return    [成功返回0，失败返回-1]
 */
int _vipc_shm_destroy(int id)
{
	int ret;
	ret=shmctl(id,IPC_RMID,0);
	if(ret)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return SYSERROR;
	}
	return SUCCESS;
}

/**
 * [_vipc_shm_stat description]
 获取共享内存状态
 * @param  id   [描述符]
 * @param  info [shmid_ds结构指针]
 * @return      [成功返回0，失败返回-1]
 */
int _vipc_shm_stat(int id,struct shmid_ds* info)
{
	int ret;
	ret=shmctl(id,IPC_STAT,info);
	if(ret)
	{
		sprintf(g_calog,"errno[%d]:%s",errno,strerror(errno));
		SYSLOG(ERROR,"%s",g_calog);
		return ret;
	}
	return ret;
}

/**
 * [_vipc_shm_write description]
 * 写共享内存
 * @param  pshmin [写地址]
 * @param  data   [数据区地址]
 * @param  len    [数据长度]
 * @return        [成功返回0，失败返回-1]
 */
int _vipc_shm_write(void* pshmin,void* data,unsigned int len)
{
	if(len<=0||!pshmin||!data)
	{
		return SYSERROR;
	}
	memcpy(pshmin,data,len);
	return SUCCESS;
}

/**
 * [_vipc_shm_read description]
 * 读共享内存
 * @param  pshmout [读地址]
 * @param  data    [数据区地址 传出参数]
 * @param  len     [数据长度]
 * @return         [成功返回0，失败返回-1]
 */
int _vipc_shm_read(void* pshmout,void* data,unsigned int len)
{
	if(len<=0||!pshmout||!data)
	{
		return SYSERROR;
	}
	memcpy(data,pshmout,len);
	return SUCCESS;
}

