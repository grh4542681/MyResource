/**
 * Array form of shared memory
 *
 * 				Author by RonghuaGao
 * 						  2017-05-24
 */

#include "arrshm.h"
#include "log.h"
#include "returnval.h"
#include "ipcshm.h"
#include "ipcsem.h"

/**
 * [ArrShmCreate Create shared memory in array format]
 * @param  key  [IPC key]
 * @param  size [Memory size]
 * @param  mode [Authority like 0666]
 * @return      [Shared memory descriptor or ERROR]
 */
int ArrShmCreate(ARRSHMATTR* arrshmattr)
{
	int Ret = 0;

	if(arrshmattr->stat == SHMREADY)
	{
		SYSLOG(ERROR,"%s","Shm is already created");
		return SUCCESS;
	}

	arrshmattr->stat = SHMINIT;
	
	if(!strlen(arrshmattr->shmftokpath))
	{
		arrshmattr->shmkey = ftok(arrshmattr->shmftokpath,100);
	}
	if(!strlen(arrshmattr->semftokpath))
	{
		arrshmattr->semkey = ftok(arrshmattr->semftokpath,100);
	}
	if(!(arrshmattr->shmmode))
	{
		arrshmattr->shmmode = 0666;
	}
	if(!(arrshmattr->semmode))	
	{
		arrshmattr->semmode = 0666;
	}

	// Init sem
	Ret = vipc_sem_RWlock_create(arrshmattr->semkey,arrshmattr->semmode);
	if(Ret == SYSERROR)
	{
		SYSLOG(ERROR,"Init sem error [%d]:%s",errno,strerror(errno));
		return SYSERROR;
	}
	arrshmattr->semid = Ret;
	
	// Init shm
	Ret = _vipc_shm_create(arrshmattr->shmkey,arrshmattr->totalsize,arrshmattr->shmmode);
	if(Ret == SYSERROR)
	{
		SYSLOG(ERROR,"Init shm error [%d]:%s",errno,strerror(errno));
		return SYSERROR;
	}
	arrshmattr->shmid = Ret;

	void *Ptr = NULL;
	Ret = _vipc_shm_open(arrshmattr->shmid,&Ptr);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Open shm_key[%d] error [%d]",arrshmattr->shmid,errno);
		return SYSERROR;
	}

	/* Init ArrShmHead */
	ARRSHM ArrShmHead;
	memset(&ArrShmHead,0x00,sizeof(ARRSHM));
	
	ArrShmHead.TotalSize = arrshmattr->totalsize - sizeof(ARRSHM);
	ArrShmHead.InitFlag = 0;

	memcpy(Ptr,&ArrShmHead,sizeof(ARRSHM));

	Ret = _vipc_shm_close(Ptr);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Close shmid[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}

	arrshmattr->stat = SHMREADY;

	return SUCCESS;
}

int ArrShmGetAttr(ARRSHMATTR* arrshmattr)
{
	int Ret = 0;

	if(arrshmattr->stat == SHMREADY)
	{
		SYSLOG(ERROR,"%s","Shm is already created");
		return SUCCESS;
	}

	arrshmattr->stat = SHMINIT;
	
	if(!strlen(arrshmattr->shmftokpath))
	{
		arrshmattr->shmkey = ftok(arrshmattr->shmftokpath,100);
	}
	if(!strlen(arrshmattr->semftokpath))
	{
		arrshmattr->semkey = ftok(arrshmattr->semftokpath,100);
	}
	if(!(arrshmattr->shmmode))
	{
		arrshmattr->shmmode = 0666;
	}
	if(!(arrshmattr->semmode))	
	{
		arrshmattr->semmode = 0666;
	}

	// Init sem
	Ret = vipc_sem_RWlock_sgetid(arrshmattr->semkey);
	if(Ret == SYSERROR)
	{
		SYSLOG(ERROR,"Init sem error [%d]:%s",errno,strerror(errno));
		return SYSERROR;
	}
	arrshmattr->semid = Ret;
	
	// Init shm
	Ret = _vipc_shm_getid(arrshmattr->shmkey);
	if(Ret == SYSERROR)
	{
		SYSLOG(ERROR,"Init shm error [%d]:%s",errno,strerror(errno));
		return SYSERROR;
	}
	return SUCCESS;
}
/**
 * [ArrShmFormat Format shared memory]
 * @param  shmid     [Shared memory descriptor]
 * @param  blocksize [Block size]
 * @return           [SUCCESS or else]
 */
int ArrShmFormat(ARRSHMATTR* arrshmattr)
{
	int Ret = 0;
	ARRSHM *ArrShmHead = NULL;
	
	Ret = _vipc_shm_open(arrshmattr->shmid,(void**)&ArrShmHead);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Open shm[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}
	
	ArrShmHead->SingleBlockSize = (arrshmattr->blocksize/8) ? (arrshmattr->blocksize+8) : (((arrshmattr->blocksize/8)+1)*8+8);
	ArrShmHead->TotalBlockNum = (ArrShmHead->TotalSize)/(ArrShmHead->SingleBlockSize);
	ArrShmHead->UnUseBlockNum = ArrShmHead->TotalBlockNum;
	ArrShmHead->InitFlag = 1;

	memset((char*)ArrShmHead+sizeof(ARRSHM),0x00,ArrShmHead->TotalSize);

	void* peach=NULL;
	ArrShmForeach(ArrShmHead,peach)
	{
		*((long*)peach)=CLEAN;
	}
	
	Ret = _vipc_shm_close((void*)ArrShmHead);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Close shm[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}

	return SUCCESS;
}

/**
 * [ArrShmGetInfo Get shared memory information]
 * @param  shmid      [Shared memory descriptor]
 * @param  ArrShmHead [Memory information structure pointer (output parameter)]
 * @return            [SUCCESS or ERROR]
 */
int ArrShmGetInfo(ARRSHMATTR* arrshmattr,ARRSHM* ArrShmHead)
{
	int Ret = 0;
	void *Ptr = NULL;

	Ret = _vipc_shm_open(arrshmattr->shmid,&Ptr);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Open shm[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}

	memcpy(ArrShmHead,Ptr,sizeof(ARRSHM));

	Ret = _vipc_shm_close((void*)ArrShmHead);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Close shm[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}
	return SUCCESS;
}

/**
 * [ArrShmDestroy Destroy shared memory]
 * @param  shmid [Shared memory descriptor]
 * @return       [SUCCESS or ERROR]
 */
void ArrShmDestroy(ARRSHMATTR* arrshmattr)
{
	arrshmattr->stat=SHMDESTROY;
	_vipc_shm_destroy(arrshmattr->shmid);
	vipc_sem_RWlock_destory(arrshmattr->semid);
}

/**
 * [ArrShmOpen Mount shared memory]
 * @param  shmid    [Shared memory descriptor]
 * @param  pshmhead [Memory pointer (output paramater)]
 * @return          [SUCCESS or ERROR]
 */
int ArrShmOpen(ARRSHMATTR* arrshmattr,void** pshmhead)
{
	return _vipc_shm_open(arrshmattr->shmid,pshmhead);
}

/**
 * [ArrShmClose Unmount shared memory]
 * @param  pshmhead [Shared memory descriptor]
 * @return          [SUCCESS or ERROR]
 */
int ArrShmClose(void* pshmhead)
{
	return _vipc_shm_close(pshmhead);
}

/**
 * [ArrShmInsert Add data to shared memory]
 * @param  shmid  [Shared memory descriptor]
 * @param  data   [Data]
 * @param  length [Data length]
 * @return        [SUCCESS or ERROR]
 */
int ArrShmInsert(ARRSHMATTR* arrshmattr,void *data,long length)
{
	int Ret = 0;
	void *Ptr = NULL;

	Ret = _vipc_shm_open(arrshmattr->shmid,&Ptr);
	if ( Ret != SUCCESS )
	{
		SYSLOG(ERROR,"Open shm[%d] error [%d]",arrshmattr->shmid,errno);
		return Ret;
	}

	if(((ARRSHM*)Ptr)->InitFlag != 1)
	{
		SYSLOG(ERROR,"Shm[%d] not initialized",arrshmattr->shmid);
		return SYSERROR;	
	}

	if(((ARRSHM*)Ptr)->UnUseBlockNum <= 0)
	{
		SYSLOG(ERROR,"Shm[%d] is all filled",arrshmattr->shmid);
		return SYSERROR;
	}
	
	if(((ARRSHM*)Ptr)->SingleBlockSize < length)
	{
		SYSLOG(ERROR,"Shm[%d] The data length is greater than the block length",arrshmattr->shmid);
		return SYSERROR;
	}

	char* p = ((char*)Ptr)+sizeof(ARRSHM);
	for(int loop=0;loop<((ARRSHM*)Ptr)->TotalBlockNum;loop++)
	{
		if((*(long*)p)!=DIRTY)
		{
			break;
		}
		p = p+((ARRSHM*)Ptr)->SingleBlockSize;
	}
	
	memcpy(p+8,data,length);
	*(long*)p=DIRTY;
	(((ARRSHM*)Ptr)->UnUseBlockNum)--;

	return SUCCESS;
}

/**
 * [ArrShmBlockAssert Determine the shared memory unit status]
 * @param  pblock [Block pointer]
 * @param  Flag   [Flag]
 * @return        [TRUE or FALSE]
 */
int ArrShmBlockAssert(void* pblock,int Flag)
{
	if(*((long*)pblock) == Flag)
		return TRUE;
	else
		return FALSE;
}

int ArrShmWlocked(ARRSHMATTR* arrshmattr,int overtime)
{
	return vipc_sem_Wlocked(arrshmattr->semid, overtime);	
}

int ArrShmWunlock(ARRSHMATTR* arrshmattr)
{
	return vipc_sem_Wunlock(arrshmattr->semid);	
}

int ArrShmRlocked(ARRSHMATTR* arrshmattr,int overtime)
{
	return vipc_sem_Rlocked(arrshmattr->semid, overtime);	
}

int ArrShmRunlock(ARRSHMATTR* arrshmattr)
{
	return vipc_sem_Runlock(arrshmattr->semid);	
}

