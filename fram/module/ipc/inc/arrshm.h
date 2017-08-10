#ifndef __ARRSHM_H__
#define __ARRSHM_H__

#include <sys/types.h>

typedef struct arrshmhead {
	long TotalSize;
	long SingleBlockSize;
	long TotalBlockNum;
	long UnUseBlockNum;
	int InitFlag;
}ARRSHM;

typedef struct arrshmattr {
	size_t totalsize;
	size_t blocksize;
	int stat;

	char shmftokpath[128]; 
	key_t shmkey;
	mode_t shmmode;
	int shmid;

	char semftokpath[128];
	key_t semkey;
	mode_t semmode;
	int semid;
}ARRSHMATTR;

#define TRUE 1
#define FALSE 0

#define DIRTY 1
#define CLEAN 2

#define SHMINIT 1
#define SHMREADY 2
#define SHMDESTROY 3

int ArrShmCreate(ARRSHMATTR*);
int ArrShmGetAttr(ARRSHMATTR*);
int ArrShmFormat(ARRSHMATTR*);
int ArrShmGetInfo(ARRSHMATTR*,ARRSHM*);
void ArrShmDestroy(ARRSHMATTR*);
int ArrShmOpen(ARRSHMATTR*,void**);
int ArrShmClose(void*);

int ArrShmBlockAssert(void*,int);
int ArrShmInsert(ARRSHMATTR*,void*,long);
int ArrShmWlocked(ARRSHMATTR*,int);
int ArrShmWunlock(ARRSHMATTR*);
int ArrShmRlocked(ARRSHMATTR*,int);
int ArrShmRunlock(ARRSHMATTR*);

/**
 * [ArrShmDelete Clear the shared memory unit]
 * @param  pshm [Share memory pointer]
 * @param  ptr  [Share memory unit pointer]
 * @return      [Nothing]
 */
#define ArrShmDelete(pshm,ptr)	\
			*((long*)ptr)=CLEAN;	\
			((ARRSHM*)pshm)->UnUseBlockNum++;

/**
 * [ArrShmGetData Get data pointer in memory unit]
 * @param  ptr [Share memory unit pointer]
 * @return     [Data pointer]
 */
#define ArrShmGetData(ptr)	\
			(char*)ptr+8;

/**
 * [ArrShmForeach Traverse the shared memory]
 * @param  pshm  [Share memory pointer]
 * @param  peach [Each unit pointer]
 * @return       [Nothing]
 */
#define ArrShmForeach(pshm,peach)	\
			peach=(char*)pshm+sizeof(ARRSHM);	\
			for(int loop=0;loop<((ARRSHM*)pshm)->TotalBlockNum;loop++,peach=(char*)peach+((ARRSHM*)pshm)->SingleBlockSize)	

#endif
