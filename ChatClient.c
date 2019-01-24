#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <pthread.h>
#define SERVERPATH "SERVER"

struct message
{
	char p[10];
	char mess[256];
};
int fd,fd1;

void* read_func(void* args)
{
	struct message A1;int sz;
	while(1)
	{
		sz = read(fd,&A1,sizeof(struct message));
		while(sz<=0)
		{
			sz = read(fd,&A1,sizeof(struct message));
		}
		printf("From: %s\nMessage: %s\n",A1.p,A1.mess);
		sleep(1);
	}
}
struct message A;
void* write_func(void* arg)
{
	int sz;
	while(1)
	{
		printf("Write a message for broadcasting..\n");
		scanf("%[^\n]s",A.mess);
		write(fd1,&A,sizeof(struct message));
		while(getchar()!='\n');
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	pid_t p = getpid();
	sprintf(A.p,"%d",(int)p);
	mkfifo(A.p,O_CREAT|0666);
	fd = open(A.p,O_RDONLY|O_NONBLOCK);
	if(fd<0)
	{
		perror("Client fifo not opened");
	}
	mkfifo(SERVERPATH,O_CREAT|0666);
	fd1 = open(SERVERPATH,O_WRONLY);
	if(fd1<0)
	{
		perror("Server fifo not opened");
	}
	pthread_t p1,p2;
	pthread_create(&p1,NULL,read_func,NULL);
	pthread_create(&p2,NULL,write_func,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}