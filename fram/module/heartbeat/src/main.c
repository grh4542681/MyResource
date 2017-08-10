#include <sys/types.h>
#include <sys/ipc.h>

#include "log.h"
#include "arrshm.h"

int main()
{
	int ret;
	LOGINIT("/home/helsinki/fram/etc/log.conf");

	SYSLOG(INFO,"%s%s%s","hello"," world","aaaaa");

	int shmid=ArrShmCreate(ftok(".",100),1024,0666);
	printf("%d\n",shmid);
	shmid=ArrShmGetid(ftok(".",100));
	printf("%d\n",shmid);
	ArrShmFormat(shmid,sizeof(long));

	ARRSHM head;
	ArrShmGetInfo(shmid,&head);
	printf("%ld\n",head.TotalSize);
	printf("%ld\n",head.SingleBlockSize);
	printf("%ld\n",head.TotalBlockNum);
	printf("%ld\n",head.UnUseBlockNum);
	printf("%d\n",head.InitFlag);

	long data=5;
	ArrShmInsert(shmid,&data,sizeof(long));
	data=100;
	ArrShmInsert(shmid,&data,sizeof(long));
	ArrShmGetInfo(shmid,&head);
	printf("%ld\n",head.TotalSize);
	printf("%ld\n",head.SingleBlockSize);
	printf("%ld\n",head.TotalBlockNum);
	printf("%ld\n",head.UnUseBlockNum);
	printf("%d\n",head.InitFlag);
	
	void *phead=NULL;
	void *peach=NULL;
	ret=ArrShmOpen(shmid,&phead);

	printf("------------------------------------\n");
	printf("ret=%d\n",ret);
	printf("head--%p\n",phead);
	
	ArrShmForeach(phead,peach)
	{
			printf("flag----%ld\n",*((long*)peach));
		if(ArrShmBlockAssert(peach,DIRTY))
		{
			void *pdata=ArrShmGetData(peach);
			printf("data----%ld\n",*((long*)pdata));
			if(*((long*)pdata) == 5)
			{
				ArrShmDelete(phead,peach);
			}
		}
	}

	ArrShmClose(phead);

	ArrShmGetInfo(shmid,&head);
	printf("%ld\n",head.TotalSize);
	printf("%ld\n",head.SingleBlockSize);
	printf("%ld\n",head.TotalBlockNum);
	printf("%ld\n",head.UnUseBlockNum);
	printf("%d\n",head.InitFlag);

	ret=ArrShmOpen(shmid,&phead);
	ArrShmForeach(phead,peach)
	{
			printf("flag----%ld\n",*((long*)peach));
		if(ArrShmBlockAssert(peach,DIRTY))
		{
			void *pdata=ArrShmGetData(peach);
			printf("data----%ld\n",*((long*)pdata));
		}
	}
	ArrShmClose(phead);

	ArrShmDestroy(shmid);

	LOGFREE();
	return 0;
}
