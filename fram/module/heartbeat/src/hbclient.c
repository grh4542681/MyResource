#include <sys/types.h>

#include "heartbeat.h"
#include "log.h"
#include "returnval.h"
#include "Socket.h"
#include "packpkg.h"
#include "cJSON.h"

int main(int argc,char** argv)
{
	LOGINIT("/home/ezgaoro/grh/MyResource/fram/etc/log.conf");
	int ret;
	int sockfd;
	MCHINF hostinfo;

	memset(&hostinfo,0x00,sizeof(MCHINF));
	gethostname(hostinfo.name,sizeof(hostinfo.name));
	
	char* pkg=PackHBpkg4J(&hostinfo);

	sockfd = ConnTCPSocket("132.196.28.87","9494");
	while(1)
	{
		//ret=SendSocketWait(sockfd,"hello world",11);
		ret=PkgSend(sockfd,pkg,strlen(pkg),BLOCK);
//		ret=write(sockfd,"hello",5);
		printf("%d\n",ret);
		sleep(5);
	}
		CloseTCPSocket(sockfd);
	
	printf("hello\n");
	return 0;

}
