#include "cn.h"

void sig_handler(int alrm)
{
	printf("Signal Received\n");
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	int d;
	scanf("%d",&d);
	if(setpgid(0,d)<0)
	{
		perror("Could not change gid");
	}
	while(1);
	return 0;
}