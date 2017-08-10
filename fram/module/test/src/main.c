#include "log.h"
#include "Tpool.h"

int main(int argv,char** argc)
{
//	int ret;
	LOGINIT("/home/helsinki/fram/etc/log.conf");

	APPLOG(THREAD,INFO,"%s","Begin thread pool test!!!!!---------------------");

	LOGFREE();
	return 0;
}
