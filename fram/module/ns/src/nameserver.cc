#include <stdio.h>
#include <unistd.h>

int main()
{
		sleep(5);
		printf("nameserver main program\n");
	printf("%s%d%s\n",__FILE__,__LINE__,__func__);
	return 0;
}
