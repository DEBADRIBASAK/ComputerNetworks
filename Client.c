#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define SERVERPATH "SERVER"


struct Pair
{
	char p[10];
	int req;
};

int main(int argc, char *argv[])
{
	struct Pair A;
	char p[10],buffer[15];
	pid_t pd = getpid();
	sprintf(A.p,"%d",(int)pd);
	int f = mkfifo(SERVERPATH,O_CREAT|0666);
	if(f<0)
	{
		perror("Server fifo not created");
	}
	int fd1 = open(SERVERPATH,O_WRONLY);
	if(fd1<0)
	{
		perror("Server fifo not opened");
	}
	f = mkfifo(A.p,O_CREAT|0666);
	if(f<0)
	{
		perror("Client fifo not created");
	}
	int fd = open(A.p,O_RDWR|O_NONBLOCK);
	if(fd<0)
	{
		perror("Client fifo not opened");
	}
	else
	{
		printf("Success!\n");
	}
	A.req = atoi(argv[1]);
	write(fd1,&A,sizeof(struct Pair));
	while(1)
	{
		int sz = read(fd,buffer,15);
		while(sz<=0)
		{
			sz = read(fd,buffer,15);
		}
		buffer[sz] = '\0';
		printf("Received message = %s\n",buffer);
		sleep(1);
	}
	return 0;
}