#ifndef __IPCSHM_H__
#define __IPCSHM_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int _vipc_shm_create(key_t key,size_t size,mode_t mode);
int _vipc_shm_getid(key_t key);
int _vipc_shm_open(int id,void** mptr);
int _vipc_shm_close(void* mptr);
int _vipc_shm_destroy(int id);
int _vipc_shm_stat(int id,struct shmid_ds* info);
int _vipc_shm_write(void* pshmin,void* data,unsigned int len);
int _vipc_shm_swrite(void* pshmin,void* data,unsigned int len,int overtime);
int _vipc_shm_read(void* pshmout,void* data,unsigned int len);
int _vipc_shm_sread(void* pshmout,void* data,unsigned int len,int overtime);

#endif
