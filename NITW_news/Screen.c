// tomorrow
#include "../cn.h"
#define SCREEN "SCREEN"
int main(int argc, char const *argv[])
{
	char buffer[256];
	mkfifo(SCREEN,O_CREAT|0666);
	int fd = open(SCREEN,O_RDONLY);
	if(fd<0)
	{
		perror("Screen not opened");
		exit(0);
	}
	int sz;
	while(1)
	{
		sz = read(fd,buffer,256);
		buffer[sz] = '\0';
		printf("%s\n",buffer);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}