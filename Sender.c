#include "cn.h"

void sig_handler(int alrm)
{
	printf("Signal Received\n");
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	printf("%d\n",(int)getpgid(0));
	getchar();
	if(setpgid(0,100)<0)
	{
		perror("Could not change gid");
	}
	kill(0,SIGUSR1);
	return 0;
}