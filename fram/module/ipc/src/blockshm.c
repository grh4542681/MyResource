#include "ipcshm.h"
#include "ipcsem.h"
#include "mem.h"
#include "log.h"
#include "returnval.h"

extern char g_calog[1024];
unsigned int g_RWshmid;

/**
 * [shm_init description]
 * 初始化共享内存结构
 * @param  key  [IPC_KEY]
 * @param  size [共享内存大小]
 * @param  mode [读写权限]
 * @return      [成功返回0，失败返回-1]
 */
int shm_init(key_t key,size_t size,mode_t mode)
{
	if(!key||!size||!mode)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	void *pshm;

	if((ret=_vipc_shm_create(key,size,mode))==-1)
	{
		strcpy(g_calog,"_vipc_shm_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;

	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		_vipc_shm_destroy(shmid);
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}

	((struct shm_head*)pshm)->size=size;
	((struct shm_head*)pshm)->fsize=size-sizeof(struct shm_head);
	((struct shm_head*)pshm)->pin_head=sizeof(struct shm_head);
	((struct shm_head*)pshm)->pin_tail=size;
	((struct shm_head*)pshm)->blocknum=0;
	
	_vipc_shm_close(pshm);
	return 0;

}

/**
 * [shm_init_safe description]
 * 初始化共享内存结构 安全模式
 * @param  key  [IPC_KEY]
 * @param  size [共享内存大小]
 * @param  mode [读写权限]
 * @return      [成功返回0，失败返回-1]
 */
int shm_init_safe(key_t key,size_t size,mode_t mode)
{
	if(!key||!size||!mode)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	void *pshm;
	int shmid;
	int semid;

	if((ret=_vipc_shm_create(key,size,mode))==-1)
	{
		strcpy(g_calog,"_vipc_shm_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;

	if((ret=vipc_sem_RWlock_create(key,mode))==-1)
	{
		_vipc_shm_destroy(shmid);
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		vipc_sem_RWlock_destory(semid);
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}

	((struct shm_head*)pshm)->size=size;
	((struct shm_head*)pshm)->fsize=size-sizeof(struct shm_head);
	((struct shm_head*)pshm)->pin_head=sizeof(struct shm_head);
	((struct shm_head*)pshm)->pin_tail=size;
	((struct shm_head*)pshm)->blocknum=0;
	
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [shm_destroy description]
 * 销毁共享内存结构
 * @param  key  [IPC_KEY]
 * @return     [成功返回0，失败返回-1]
 */
int shm_destroy(key_t key)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	return _vipc_shm_destroy(shmid);	
}

/**
 * [shm_destroy_safe description]
 * 销毁共享内存结构 安全模式
 * @param  key      [IPC_KEY]
 * @param  overtime [超时时间 ms]
 * @return          [成功返回0，失败返回-1]
 */
int shm_destroy_safe(key_t key,int overtime)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=vipc_sem_Wlocked(semid,overtime))!=0)
	{
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Wlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Wlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	return (_vipc_shm_destroy(shmid)|vipc_sem_RWlock_destory(semid));
}

/**
 * [shm_getshminfo description]
 * 获取共享内存结构信息
 * @param  key     [IPC_KEY]
 * @param  shmhead [二级shm_head结构，外部需要释放]
 * @return         [成功返回0，失败返回-1]
 */
int shm_getshminfo(key_t key,struct shm_head** shmhead)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	void* pshm;
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	struct shm_head *phead=(struct shm_head*)Malloc(sizeof(struct shm_head));
	if(!phead)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"Malloc error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	memset(phead,0x00,sizeof(struct shm_head));
	if((ret=_vipc_shm_read(pshm,phead,sizeof(struct shm_head)))==-1)
	{
		free(phead);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	*shmhead=phead;
	return 0;
}

/**
 * [shm_getshminfo_safe description]
 * 获取共享内存结构信息 安全模式
 * @param  key      [IPC_KEY]
 * @param  shmhead  [二级shm_head结构，外部需要释放]
 * @param  overtime [超时时间 ms]
 * @return          [成功返回0，失败返回-1]
 */
int shm_getshminfo_safe(key_t key,struct shm_head** shmhead,int overtime)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	void* pshm;
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	struct shm_head *phead=(struct shm_head*)Malloc(sizeof(struct shm_head));
	if(!phead)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"Malloc error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	memset(phead,0x00,sizeof(struct shm_head));
	if((ret=vipc_sem_Rlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Rlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Rlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=_vipc_shm_read(pshm,phead,sizeof(struct shm_head)))==-1)
	{
		free(phead);
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	vipc_sem_Runlock(semid);
	*shmhead=phead;
	return 0;
}

/**
 * [shm_getblockinfo description]
 * 获取共享内存块信息
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @param  shmblock  [二级shm_block结构指针，外部需要释放]
 * @return           [成功返回block数目，失败返回-1]
 */
int shm_getblockinfo(key_t key,char *blockname,struct shm_block** shmblock)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int i;
	int shmid;
	void* pshm;
	struct shm_head shmhead;
	struct shm_block *ptr;
	struct shm_block *pshmblock;
	i=0;
	memset(&shmhead,0x00,sizeof(struct shm_head));

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(blockname)
	{
		ptr=pshm+shmhead.size-sizeof(struct shm_block);
		for(i=shmhead.blocknum;i>0;i--)
		{
			if(!strcmp(ptr->name,blockname))
				break;
			ptr--;
		}
		if(!i)
		{
			_vipc_shm_close(pshm);
			sprintf(g_calog,"not have [%s]block in shw",blockname);
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		pshmblock=(struct shm_block*)Malloc(sizeof(struct shm_block));
		if(!pshmblock)
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		if((ret=_vipc_shm_read(ptr,pshmblock,sizeof(struct shm_block)))==-1)
		{
			free(pshmblock);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_read error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		_vipc_shm_close(pshm);
		*shmblock=pshmblock;
		return 1;
	}
	else
	{
		if(!shmhead.blocknum)
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"no block in shw");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		pshmblock=(struct shm_block*)Malloc((sizeof(struct shm_block))*(shmhead.blocknum));
		if(!pshmblock)
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		ptr=pshm+shmhead.size;
		for(i=shmhead.blocknum;i>0;i--)
		{
			if((ret=_vipc_shm_read(ptr,pshmblock,sizeof(struct shm_block)))==-1)
			{
				free(pshmblock);
				_vipc_shm_close(pshm);
				strcpy(g_calog,"_vipc_shm_read error");
				SYSLOG(ERROR,"%s",g_calog);
				return -1;
			}
			ptr--;
			pshmblock++;
		}
		_vipc_shm_close(pshm);
		*shmblock=pshmblock;
		return shmhead.blocknum;
	}	
}

/**
 * [shm_getblockinfo_safe description]
 * 获取共享内存块信息  安全模式
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @param  shmblock  [二级shm_block结构指针，外部需要释放]
 * @param  overtime  [超时时间 ms]
 * @return           [成功返回block数目，失败返回-1]
 */
int shm_getblockinfo_safe(key_t key,char *blockname,struct shm_block** shmblock,int overtime)
{
	if(!key)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int i;
	int shmid;
	int semid;
	void* pshm;
	struct shm_head shmhead;
	struct shm_block *ptr;
	struct shm_block *pshmblock;
	i=0;
	memset(&shmhead,0x00,sizeof(struct shm_head));

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Rlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Rlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
 		strcpy(g_calog,"vipc_sem_Rlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(blockname)
	{
		ptr=pshm+shmhead.size-sizeof(struct shm_block);
		for(i=shmhead.blocknum;i>0;i--)
		{
			if(!strcmp(ptr->name,blockname))
				break;
			ptr--;
		}
		if(!i)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			sprintf(g_calog,"not have [%s]block in shw",blockname);
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		pshmblock=(struct shm_block*)Malloc(sizeof(struct shm_block));
		if(!pshmblock)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		
		if((ret=_vipc_shm_read(ptr,pshmblock,sizeof(struct shm_block)))==-1)
		{
			free(pshmblock);
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_read error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		*shmblock=pshmblock;
		return 1;
	}
	else
	{
		if(!shmhead.blocknum)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"no block in shw");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		pshmblock=(struct shm_block*)Malloc((sizeof(struct shm_block))*(shmhead.blocknum));
		if(!pshmblock)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		ptr=pshm+shmhead.size;
		for(i=shmhead.blocknum;i>0;i--)
		{
			if((ret=_vipc_shm_read(ptr,pshmblock,sizeof(struct shm_block)))==-1)
			{
				free(pshmblock);
				vipc_sem_Runlock(semid);
				_vipc_shm_close(pshm);
				strcpy(g_calog,"_vipc_shm_read error");
				SYSLOG(ERROR,"%s",g_calog);
				return -1;
			}
			ptr--;
			pshmblock++;
		}
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		*shmblock=pshmblock;
		return shmhead.blocknum;
	}	
}

/**
 * [shm_setblockinfo description]
 * 设置内存块描述信息
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @param  pshmblock [shm_block结构指针]
 * @return           [成功返回0，失败返回-1]
 */
int shm_setblockinfo(key_t key,struct shm_block* pshmblock)
{
	if(!key||!pshmblock)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	void *pshm;
	struct shm_head shmhead;
	struct shm_block *ptr;
	int i;

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	ptr=pshm+shmhead.size-sizeof(struct shm_block);
	for(i=shmhead.blocknum;i>0;i--)
	{
		if(!strcmp(ptr->name,pshmblock->name))
			break;
		ptr--;
	}
	if(!i)
	{
		_vipc_shm_close(pshm);
		sprintf(g_calog,"not have [%s]block in shw",pshmblock->name);
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	memcpy(ptr,pshmblock,sizeof(struct shm_block));
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [shm_setblockinfo_safe description]
 * 设置内存块描述信息 安全模式
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @param  pshmblock [shm_block结构指针]
 * @param  overtime  [超时时间 ms]
 * @return           [成功返回0，失败返回-1]
 */
int shm_setblockinfo_safe(key_t key,struct shm_block* pshmblock,int overtime)
{
	if(!key||pshmblock)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	void *pshm;
	struct shm_head shmhead;
	struct shm_block *ptr;
	int i;

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Wlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Wlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Wlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	ptr=pshm+shmhead.size-sizeof(struct shm_block);
	for(i=shmhead.blocknum;i>0;i--)
	{
		if(!strcmp(ptr->name,pshmblock->name))
			break;
		ptr--;
	}
	if(!i)
	{
		_vipc_shm_close(pshm);
		sprintf(g_calog,"not have [%s]block in shw",pshmblock->name);
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	memcpy(ptr,pshmblock,sizeof(struct shm_block));
	vipc_sem_Wunlock(semid);
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [_shm_upload description]
 * 上传数据到共享内存
 * @param  key        [IPC_KEY]
 * @param  data       [数据区指针]
 * @param  len        [数据长度]
 * @param  sublen     [单条记录长度]
 * @param  condoffset [索引在记录结构中的偏移量]
 * @param  condlen    [索引长度]
 * @param  blockname  [内存块名]
 * @return            [成功返回0，失败返回-1]
 */
int _shm_upload(key_t key,void* data,int len,int sublen,int condoffset,int condlen,char* blockname)
{
	if(!key||!data||!len||!sublen||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	void* pshm;
	struct shm_head shmhead;
	struct shm_block shmblock;
	struct shm_block *pshmblock=NULL;

	memset(&shmhead,0x00,sizeof(struct shm_head));
	memset(&shmblock,0x00,sizeof(struct shm_block));

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(shmhead.fsize<(len+sizeof(struct shm_block)))
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"no space in shm");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	ret=shm_getblockinfo(key,blockname,&pshmblock);
	if(ret>0)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm has this blockname");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmblock.sortflag=0;
	strcpy(shmblock.name,blockname);
	shmblock.num=len/sublen;
	shmblock.condoffset=condoffset;
	shmblock.condlen=condlen;
	shmblock.pstart=shmhead.pin_head;
	shmblock.pend=shmhead.pin_head+len;
	shmblock.recsize=sublen;
	shmblock.dirtyflg=0;
	
	if(!_vipc_shm_write(((char*)pshm)+shmhead.pin_head,data,len))
	{
		shmblock.modytime=time(0);
		if(_vipc_shm_write(((char*)pshm)+shmhead.pin_tail-sizeof(struct shm_block),&shmblock,sizeof(struct shm_block)))
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_write error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		shmhead.fsize-=(len+sizeof(struct shm_block));
		shmhead.pin_head+=len;
		shmhead.pin_tail-=sizeof(struct shm_block);
		shmhead.blocknum++;
		if(_vipc_shm_write(pshm,&shmhead,sizeof(struct shm_head)))
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_write error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
	}
	else
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_write error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [_shm_upload_safe description]
 * 上传数据到共享内存 安全模式
 * @param  key        [IPC_KEY]
 * @param  data       [数据区指针]
 * @param  len        [数据长度]
 * @param  sublen     [单条记录长度]
 * @param  condoffset [索引在记录结构中的偏移量]
 * @param  condlen    [索引长度]
 * @param  blockname  [内存块名]
 * @param  overtime   [超时时间 ms]
 * @return            [成功返回0，失败返回-1]
 */
int _shm_upload_safe(key_t key,void* data,int len,int sublen,int condoffset,int condlen,char* blockname,int overtime)
{
	if(!key||!data||!len||!sublen||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	void* pshm;
	struct shm_head shmhead;
	struct shm_block shmblock;
	struct shm_block* pshmblock=NULL;

	memset(&shmhead,0x00,sizeof(struct shm_head));
	memset(&shmblock,0x00,sizeof(struct shm_block));

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Wlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Wlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Wlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=_vipc_shm_read(pshm,&shmhead,sizeof(struct shm_head)))==-1)
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_read error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(shmhead.fsize<(len+sizeof(struct shm_block)))
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"no space in shm");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	ret=shm_getblockinfo(key,blockname,&pshmblock);
	if(ret>0)
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm has this blockname");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmblock.sortflag=0;
	strcpy(shmblock.name,blockname);
	shmblock.num=len/sublen;
	shmblock.condoffset=condoffset;
	shmblock.condlen=condlen;
	shmblock.pstart=shmhead.pin_head;
	shmblock.pend=shmhead.pin_head+len;
	shmblock.recsize=sublen;
	shmblock.dirtyflg=0;
	
	if(!_vipc_shm_write(((char*)pshm)+shmhead.pin_head,data,len))
	{
		shmblock.modytime=time(0);
		if(_vipc_shm_write(((char*)pshm)+shmhead.pin_tail-sizeof(struct shm_block),&shmblock,sizeof(struct shm_block)))
		{
			vipc_sem_Wunlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_write error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		shmhead.fsize-=(len+sizeof(struct shm_block));
		shmhead.pin_head+=len;
		shmhead.pin_tail-=sizeof(struct shm_block);
		shmhead.blocknum++;
		if(_vipc_shm_write(pshm,&shmhead,sizeof(struct shm_head)))
		{
			vipc_sem_Wunlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_write error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
	}
	else
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"_vipc_shm_write error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	vipc_sem_Wunlock(semid);
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [shm_download description]
 * 从共享内存中下载数据
 * @param  key       [IPC_KEY]
 * @param  cond      [索引]
 * @param  blockname [内存块名]
 * @param  pdata     [数据区指针，外部需要释放]
 * @return           [成功返回记录数目，失败返回-1]
 */
int shm_download(key_t key,void* cond,char* blockname,void** pdata)
{
	if(!key||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int num;
	int i;
	void* pshm;
	void* ptr;
	void* pcurr;
	struct shm_head shmhead;
	struct shm_block *shmblock;

	memset(&shmhead,0x00,sizeof(struct shm_head));
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=shm_getblockinfo(key,blockname,&shmblock))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm_getblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(cond&&(!cmpnull(cond+shmblock->condoffset,shmblock->condlen)||(cmpnull(cond,shmblock->condoffset)||cmpnull(cond+shmblock->condoffset+shmblock->condlen,shmblock->recsize-shmblock->condoffset-shmblock->condlen))))
	{
		free(shmblock);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"bad index condation");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(!cond)
	{
		ptr=Malloc((shmblock->recsize)*(shmblock->num));
		if(!ptr)
		{
			free(shmblock);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		if((ret=_vipc_shm_read(pshm+(shmblock->pstart),ptr,(shmblock->recsize)*(shmblock->num)))==-1)
		{
			free(shmblock);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"_vipc_shm_read error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		_vipc_shm_close(pshm);
		(*pdata)=ptr;
		num=shmblock->num;
		free(shmblock);
		return num;
	}
	else
	{

		if(!shmblock->sortflag)
		{
			pcurr=pshm+(shmblock->pstart);
			num=0;
			ptr=NULL;
			for(i=0;i<shmblock->num;i++)
			{
				if(!memcmp(pcurr+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					num++;
					ptr=Realloc(ptr,num*(shmblock->recsize));
					memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				}
				pcurr=((char*)pcurr)+shmblock->recsize;
			}
			_vipc_shm_close(pshm);
			(*pdata)=ptr;
			free(shmblock);
			return num;
		}
		else
		{
			pcurr=(struct shm_block*)shm_bsearch(pshm,shmblock,cond);
			char *pprev=((char*)pcurr)-shmblock->recsize;
			char *pnext=((char*)pcurr)+shmblock->recsize;
			if(!pcurr)
			{
				_vipc_shm_close(pshm);
				free(shmblock);
				return -1;
			}
			num=1;
			ptr=Malloc(num*(shmblock->recsize));
			memcpy(ptr,pcurr,shmblock->recsize);

			do{
				if(memcmp(pprev+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					break;
				}
				num++;
				ptr=Realloc(ptr,num*(shmblock->recsize));
				memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				pprev-=shmblock->recsize;
			}while(1);
			do{
				if(memcmp(pnext+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					break;
				}
				num++;
				ptr=Realloc(ptr,num*(shmblock->recsize));
				memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				pnext+=shmblock->recsize;
			}while(1);
			_vipc_shm_close(pshm);
			(*pdata)=ptr;
			free(shmblock);
			return num;
		}
	}
}

/**
 * [shm_download_safe description]
 * 从共享内存中下载数据 安全模式
 * @param  key       [IPC_KEY]
 * @param  cond      [索引]
 * @param  blockname [内存块名]
 * @param  pdata     [数据区指针，外部需要释放]
 * @param  overtime  [超时时间 ms]
 * @return           [成功返回记录数目，失败返回-1]
 */
int shm_download_safe(key_t key,void* cond,char* blockname,void** pdata,int overtime)
{
	if(!key||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	int num;
	int i;
	void* pshm;
	void* ptr;
	void* pcurr;
	struct shm_head shmhead;
	struct shm_block *shmblock;

	memset(&shmhead,0x00,sizeof(struct shm_head));

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Rlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Rlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Rlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=shm_getblockinfo(key,blockname,&shmblock))==-1)
	{
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm_getblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(cond&&(!cmpnull(cond+shmblock->condoffset,shmblock->condlen)||(cmpnull(cond,shmblock->condoffset)||cmpnull(cond+shmblock->condoffset+shmblock->condlen,shmblock->recsize-shmblock->condoffset-shmblock->condlen))))
	{
		vipc_sem_Runlock(semid);
		free(shmblock);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"bad index condation");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if(!cond)
	{
		ptr=Malloc((shmblock->recsize)*(shmblock->num));
		if(!ptr)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			free(shmblock);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		if((ret=_vipc_shm_read(pshm+(shmblock->pstart),ptr,(shmblock->recsize)*(shmblock->num)))==-1)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			free(shmblock);
			strcpy(g_calog,"_vipc_shm_read error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		(*pdata)=ptr;
		num=shmblock->num;
		free(shmblock);
		return num;
	}
	else
	{
		if(!shmblock->sortflag)
		{
			pcurr=pshm+(shmblock->pstart);
			num=0;
			ptr=NULL;
			for(i=0;i<shmblock->num;i++)
			{
				if(!memcmp(pcurr+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					num++;
					ptr=Realloc(ptr,num*(shmblock->recsize));
					memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				}
				pcurr=((char*)pcurr)+shmblock->recsize;
			}
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			(*pdata)=ptr;
			free(shmblock);
			return num;
		}
		else
		{
			pcurr=(struct shm_block*)shm_bsearch(pshm,shmblock,cond);
			char *pprev=((char*)pcurr)-shmblock->recsize;
			char *pnext=((char*)pcurr)+shmblock->recsize;
			if(!pcurr)
			{
				vipc_sem_Runlock(semid);
				_vipc_shm_close(pshm);
				free(shmblock);
				return 0;
			}
			num=1;
			ptr=Malloc(num*(shmblock->recsize));
			memcpy(ptr,pcurr,shmblock->recsize);

			do{
				if(memcmp(pprev+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					break;
				}
				num++;
				ptr=Realloc(ptr,num*(shmblock->recsize));
				memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				pprev-=shmblock->recsize;
			}while(1);
			do{
				if(memcmp(pnext+(shmblock->condoffset),cond+(shmblock->condoffset),shmblock->condlen))
				{
					break;
				}
				num++;
				ptr=Realloc(ptr,num*(shmblock->recsize));
				memcpy(ptr+(num-1)*shmblock->recsize,pcurr,shmblock->recsize);
				pnext+=shmblock->recsize;
			}while(1);
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			(*pdata)=ptr;
			free(shmblock);
			return num;
		}
	}
}

int cmpnull(void* data,int len)
{
	char *pcurr;
	int loop;

	pcurr=(char*)data;
	for(loop=len;loop>0;loop--)
	{
		if(*pcurr)
			return -1;
		pcurr++;
	}
	return 0;
}

int cmp(void* data,void* cond, unsigned int len)
{
	char *pcurr1;
	char *pcurr2;
	int loop;

	pcurr1=(char*)data;
	pcurr2=(char*)cond;
	for(loop=len;loop>0;loop--)
	{
		if(!(*pcurr2))
		{
			pcurr1++;
			pcurr2++;
			continue;
		}
		if(memcmp(pcurr1,pcurr2,sizeof(char)))
			return -1;
		pcurr1++;
		pcurr2++;
	}
	return 0;

}

int shm_select(key_t key,void *cond,char *blockname,void** data)
{
	int ret;
	int shmid;
	void *pshm;
	int i;
	int matchnum;
	char *pdata;
	char *pcurr;
	char *pnext;
	char *pprev;
	struct shm_block *pshmblock;

	/*获取内存块信息*/
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	pshmblock=(struct shm_block*)(pshm+((struct shm_head*)pshm)->pin_tail);
	for(i=((struct shm_head*)pshm)->blocknum;i>0;i--)
	{
		if(!strcmp(pshmblock->name,blockname))
		{
			break;
		}
		pshmblock++;
	}
	if(!i)
	{
		_vipc_shm_close(pshm);
		sprintf(g_calog,"blockname[%s] can't find",blockname);
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	/*非索引条件*/
	if(!cond)
	{
		pdata=(char*)Malloc((pshmblock->recsize)*(pshmblock->num));
		if(!pdata)
		{
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		memcpy(pdata,pshm+(pshmblock->pstart),(pshmblock->recsize)*(pshmblock->num));
		matchnum=pshmblock->num;
	}
	else if(cond&&(!cmpnull(cond+pshmblock->condoffset,pshmblock->condlen)||(cmpnull(cond,pshmblock->condoffset)||cmpnull(cond+pshmblock->condoffset+pshmblock->condlen,pshmblock->recsize-pshmblock->condoffset-pshmblock->condlen))))
	{
		matchnum=0;
		pdata=NULL;
		pcurr=pshm+pshmblock->pstart;
		for(i=pshmblock->num;i>0;i--)
		{
			if(!cmp(pcurr,cond,pshmblock->recsize))
			{
				matchnum++;
				pdata=Realloc(pdata,(pshmblock->recsize)*matchnum);
				memcpy(pdata+(pshmblock->recsize)*(matchnum-1),pcurr,pshmblock->recsize);
			}
			pcurr+=pshmblock->recsize;
		}
	}
	/*索引条件*/
	else
	{
		/*未排序*/
		if(!pshmblock->sortflag)
		{
			matchnum=0;
			pdata=NULL;
			pcurr=pshm+(pshmblock->pstart);
			for(i=0;i<pshmblock->num;i++)
			{
				if(!memcmp(pcurr+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					matchnum++;
					pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
					memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				}
				pcurr+=pshmblock->recsize;
			}
		}
		/*已排序*/
		else
		{
			pcurr=(char *)shm_bsearch(pshm,pshmblock,cond);
			pprev=((char*)pcurr)-pshmblock->recsize;
			pnext=((char*)pcurr)+pshmblock->recsize;
			if(!pcurr)
			{
				_vipc_shm_close(pshm);
				return 0;
			}
			matchnum=1;
			pdata=NULL;
			pdata=Malloc(matchnum*(pshmblock->recsize));
			memcpy(pdata,pcurr,pshmblock->recsize);
			do{
				if(memcmp(pprev+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					break;
				}
				matchnum++;
				pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
				memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				pprev-=pshmblock->recsize;
			}while(1);
			do{
				if(memcmp(pnext+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					break;
				}
				matchnum++;
				pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
				memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				pnext+=pshmblock->recsize;
			}while(1);
		}
	}
	_vipc_shm_close(pshm);
	(*data)=pdata;
	return matchnum;
}

int shm_select_safe(key_t key,void *cond,char *blockname,void** data,int overtime)
{
	int ret;
	int shmid;
	int semid;
	void *pshm;
	int i;
	int matchnum;
	char *pdata;
	char *pcurr;
	char *pnext;
	char *pprev;
	struct shm_block *pshmblock;

	/*获取内存块信息*/
	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Rlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Rlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Rlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	pshmblock=(struct shm_block*)(pshm+((struct shm_head*)pshm)->pin_tail);
	for(i=((struct shm_head*)pshm)->blocknum;i>0;i--)
	{
		if(!strcmp(pshmblock->name,blockname))
		{
			break;
		}
		pshmblock++;
	}
	if(!i)
	{
		vipc_sem_Runlock(semid);
		_vipc_shm_close(pshm);
		sprintf(g_calog,"blockname[%s] can't find",blockname);
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	/*非索引条件*/
	if(!cond)
	{
		pdata=(char*)Malloc((pshmblock->recsize)*(pshmblock->num));
		if(!pdata)
		{
			vipc_sem_Runlock(semid);
			_vipc_shm_close(pshm);
			strcpy(g_calog,"Malloc error");
			SYSLOG(ERROR,"%s",g_calog);
			return -1;
		}
		memcpy(pdata,pshm+(pshmblock->pstart),(pshmblock->recsize)*(pshmblock->num));
		matchnum=pshmblock->num;
	}
	else if(cond&&(!cmpnull(cond+pshmblock->condoffset,pshmblock->condlen)||(cmpnull(cond,pshmblock->condoffset)||cmpnull(cond+pshmblock->condoffset+pshmblock->condlen,pshmblock->recsize-pshmblock->condoffset-pshmblock->condlen))))
	{
		matchnum=0;
		pdata=NULL;
		pcurr=pshm+pshmblock->pstart;
		for(i=pshmblock->num;i>0;i--)
		{
			if(!cmp(pcurr,cond,pshmblock->recsize))
			{
				matchnum++;
				pdata=Realloc(pdata,(pshmblock->recsize)*matchnum);
				memcpy(pdata+(pshmblock->recsize)*(matchnum-1),pcurr,pshmblock->recsize);
			}
			pcurr+=pshmblock->recsize;
		}
	}
	/*索引条件*/
	else
	{
		/*未排序*/
		if(!pshmblock->sortflag)
		{
			matchnum=0;
			pdata=NULL;
			pcurr=pshm+(pshmblock->pstart);
			for(i=0;i<pshmblock->num;i++)
			{
				if(!memcmp(pcurr+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					matchnum++;
					pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
					memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				}
				pcurr+=pshmblock->recsize;
			}
		}
		/*已排序*/
		else
		{
			pcurr=(char *)shm_bsearch(pshm,pshmblock,cond);
			pprev=((char*)pcurr)-pshmblock->recsize;
			pnext=((char*)pcurr)+pshmblock->recsize;
			if(!pcurr)
			{
				vipc_sem_Runlock(semid);
				_vipc_shm_close(pshm);
				return 0;
			}
			matchnum=1;
			pdata=NULL;
			pdata=Malloc(matchnum*(pshmblock->recsize));
			memcpy(pdata,pcurr,pshmblock->recsize);
			do{
				if(memcmp(pprev+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					break;
				}
				matchnum++;
				pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
				memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				pprev-=pshmblock->recsize;
			}while(1);
			do{
				if(memcmp(pnext+(pshmblock->condoffset),cond+(pshmblock->condoffset),pshmblock->condlen))
				{
					break;
				}
				matchnum++;
				pdata=Realloc(pdata,matchnum*(pshmblock->recsize));
				memcpy(pdata+(matchnum-1)*pshmblock->recsize,pcurr,pshmblock->recsize);
				pnext+=pshmblock->recsize;
			}while(1);
		}
	}
	vipc_sem_Runlock(semid);
	_vipc_shm_close(pshm);
	(*data)=pdata;
	return matchnum;
}

/**
 * [shm_bsearch description]
 * 二分法检索共享内存
 * @param  pshm     [共享内存首地址]
 * @param  shmblock [内存块信息结构指针]
 * @param  cond     [索引]
 * @return          [成功返回匹配地址，失败返回-1]
 */
void *shm_bsearch(void *pshm,struct shm_block *shmblock,const void *cond)
{
	void *base=pshm+(shmblock->pstart);
	int num=shmblock->num;
	int size=shmblock->recsize;

	size_t start = 0, end = num;
	int result;

	while (start < end) {
		size_t mid = start + (end - start) / 2;

		result = memcmp(cond+(shmblock->condoffset), (base + mid * size)+(shmblock->condoffset),shmblock->condlen);
		if (result < 0)
			end = mid;
		else if (result > 0)
			start = mid + 1;
		else
			return (void *)base + mid * size;
	}
	return NULL;
}  
 
static void swap(char *a,char *b,size_t width)  
{  
    char tmp;  
    if ( a != b )
    { 
        while ( width-- )
        {  
            tmp = *a;  
            *a++ = *b;  
            *b++ = tmp;  
        }  
    }
}   

/**
 * [_shm_ssort description]
 * 共享内存插入排序
 * @param pshm      [共享内存首地址]
 * @param pshmblock [内存块信息结构指针]
 */
void _shm_ssort(void *pshm,struct shm_block *pshmblock)  
{
	printf("ssort\n");
	char *lo=((char *)pshm)+pshmblock->pstart;
	char *hi=((char *)pshm)+pshmblock->pend-pshmblock->recsize;
	size_t width=pshmblock->recsize;
    char *p, *max;
    int ret;
    while (hi > lo) {  
        max = lo;  
        for (p = lo+width; p <= hi; p += width) { 
        	ret=memcmp(p+(pshmblock->condoffset),max+(pshmblock->condoffset),pshmblock->condlen);
            if (ret > 0) {
                max = p;
            }
        }
        swap(max, hi, width);   
        hi -= width;  
    }
} 

/**
 * [_shm_qsort description]
 * 共享内存快速排序
 * @param pshm      [共享内存首地址]
 * @param pshmblock [内存块信息结构指针]
 */
void _shm_qsort(void *pshm,struct shm_block *pshmblock)  
{
    char *lo, *hi;
    char *mid; 
    char *loguy, *higuy;
    size_t size;
    char *lostk[STKSIZ], *histk[STKSIZ];  
    int stkptr; 
    void *base=((char*)pshm)+pshmblock->pstart;
    size_t num=pshmblock->num;
    size_t width=pshmblock->recsize;
  
    if (num < 2 || width == 0)  
        return;
    stkptr = 0;
    lo = base;  
    hi = (char *)base + width * (num-1);
 
recurse:  
  
    size = (hi - lo) / width + 1; 
    if(size <= CUTOFF)
    {  
        _shm_ssort(pshm,pshmblock);  
    }  
    else
    {
        mid = lo + (size / 2) * width;
        if(memcmp(lo+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) > 0)
        {  
            swap(lo, mid, width);  
        }  
        if(memcmp(lo+(pshmblock->condoffset),hi+(pshmblock->condoffset),pshmblock->condlen) > 0)
        {  
            swap(lo, hi, width);  
        }  
        if(memcmp(mid+(pshmblock->condoffset),hi+(pshmblock->condoffset),pshmblock->condlen) > 0)
        {  
            swap(mid, hi, width);  
        }  
        loguy = lo; 
        higuy = hi; 
        for (;;)
        { 
            if(mid > loguy)
            {  
                do
                {  
                    loguy += width;  
                } while (loguy < mid && memcmp(loguy+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) <= 0);  
            }  
            if(mid <= loguy)
            {  
                do
                {  
                    loguy += width;  
                } while (loguy <= hi && memcmp(loguy+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) <= 0);  
            }
            do
            {  
                higuy -= width;  
            }while (higuy > mid && memcmp(higuy+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) > 0);  
            if(higuy < loguy)  
                break;
            swap(loguy, higuy, width);  
            if(mid == higuy)  
                mid = loguy;
        }
        higuy += width;  
        if(mid < higuy)
        {  
            do
            {  
                higuy -= width;  
            } while (higuy > mid && memcmp(higuy+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) == 0);  
        }  
        if(mid >= higuy)
        {  
            do
            {  
                higuy -= width;  
            } while (higuy > lo && memcmp(higuy+(pshmblock->condoffset),mid+(pshmblock->condoffset),pshmblock->condlen) == 0);  
        }  
        if( higuy - lo >= hi - loguy )
        {  
            if(lo < higuy)
            {  
                lostk[stkptr] = lo;  
                histk[stkptr] = higuy;  
                ++stkptr;  
            }
            if(loguy < hi)
            {  
                lo = loguy;  
                goto recurse;
            }  
        }  
        else
        {  
            if(loguy < hi)
            {  
                lostk[stkptr] = loguy;  
                histk[stkptr] = hi;  
                ++stkptr;
            }  
            if(lo < higuy)
            {  
                hi = higuy;  
                goto recurse;
            }  
        }  
    }  
    --stkptr;  
    if(stkptr >= 0)
    {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse; 
    }
    else  
        return; 
}

/**
 * [shm_qsort description]
 * 共享内存块排序
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @return           [成功返回0，失败返回-1]
 */
int shm_qsort(key_t key,char *blockname)
{
	if(!key||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	void *pshm;
	struct shm_block *shmblock;

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=shm_getblockinfo(key,blockname,&shmblock))==-1)
	{
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm_getblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	_shm_qsort(pshm,shmblock);
	shmblock->sortflag=1;
	if((ret=shm_setblockinfo(key,shmblock))==-1)
	{
		_vipc_shm_close(pshm);
		free(shmblock);
		strcpy(g_calog,"shm_setblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	free(shmblock);
	_vipc_shm_close(pshm);
	return 0;
}

/**
 * [shm_qsort_safe description]
 * 共享内存块排序 安全模式
 * @param  key       [IPC_KEY]
 * @param  blockname [内存块名]
 * @param  overtime  [超时时间 ms]
 * @return           [成功返回0，失败返回-1]
 */
int shm_qsort_safe(key_t key,char *blockname,int overtime)
{
	if(!key||!blockname)
	{
		strcpy(g_calog,"bad arguments");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	int ret;
	int shmid;
	int semid;
	void *pshm;
	struct shm_block *shmblock;

	if((ret=_vipc_shm_getid(key))==-1)
	{
		strcpy(g_calog,"_vipc_shm_getid error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	shmid=ret;
	if((ret=vipc_sem_RWlock_getid(key))==-1)
	{
		strcpy(g_calog,"vipc_sem_RWlock_create error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	semid=ret;
	if((ret=_vipc_shm_open(shmid,&pshm))==-1)
	{
		strcpy(g_calog,"_vipc_shm_open error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=vipc_sem_Wlocked(semid,overtime))!=0)
	{
		_vipc_shm_close(pshm);
		if(ret==TIMEOUT)
		{
			strcpy(g_calog,"vipc_sem_Wlocked timeout");
			SYSLOG(ERROR,"%s",g_calog);
			return ret;
		}
		strcpy(g_calog,"vipc_sem_Wlocked error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	if((ret=shm_getblockinfo(key,blockname,&shmblock))==-1)
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		strcpy(g_calog,"shm_getblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	_shm_qsort(pshm,shmblock);
	shmblock->sortflag=1;
	if((ret=shm_setblockinfo(key,shmblock))==-1)
	{
		vipc_sem_Wunlock(semid);
		_vipc_shm_close(pshm);
		free(shmblock);
		strcpy(g_calog,"shm_setblockinfo error");
		SYSLOG(ERROR,"%s",g_calog);
		return -1;
	}
	vipc_sem_Wunlock(semid);
	free(shmblock);
	_vipc_shm_close(pshm);
	return 0;
}
