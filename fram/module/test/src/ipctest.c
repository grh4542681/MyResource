#include <string.h>

#include "arrshm.h"
#include "log.h"

int main(int argv,char** args)
{
	LOGINIT("/home/helsinki/fram/etc/log.conf");

	ARRSHMATTR shmattr;
	memset(&shmattr,0x00,sizeof(ARRSHMATTR));

	shmattr.totalsize = 1024*1024;
	shmattr.blocksize = 32;
	shmattr.shmkey=12345;
	shmattr.semkey=12346;

	ArrShmCreate(&shmattr);


	ArrShmDestroy(&shmattr);

	LOGFREE();
	return 0;
}
