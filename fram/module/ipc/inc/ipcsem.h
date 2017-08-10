#ifndef __IPCSEM_H__
#define __IPCSEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#include <errno.h>

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
};

//extern int semtimedop(int semid, struct sembuf *sops, unsigned nsops,struct timespec *timeout);

int vipc_sem_create(key_t key, int semnum, mode_t mode, int num);
int vipc_sem_getid_safe(key_t key);
int vipc_sem_getid(key_t key);
int vipc_sem_destory(int semid);
int vipc_sem_grab(int semid, int semnum, int time);
int vipc_sem_grab_masksig(int semid, int semnum, int time);
int vipc_sem_grab_nowait(int semid, int semnum);
int vipc_sem_free(int semid, int semnum);
int vipc_sem_getval(int semid,int semnum);


int vipc_sem_RWlock_create(key_t key,mode_t mode);
int vipc_sem_RWlock_getid(key_t key);
int vipc_sem_RWlock_sgetid(key_t key);
int vipc_sem_Wlocked(int semid,int time);
int vipc_sem_Wunlock(int semid);
int vipc_sem_Rlocked(int semid,int time);
int vipc_sem_Runlock(int semid);
int vipc_sem_RWlock_destory(int semid);
#endif
