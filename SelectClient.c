#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#define SERVERPATH "SERVER"

struct message
{
	char p[10];
	char mess[256];
};
int fd,fd1;

int max(int a,int b)
{
	return (a>b)?a:b;
}
 struct message A,A1;

int main(int argc, char const *argv[])
{
	pid_t p = getpid();
	sprintf(A.p,"%d",(int)p);
	mkfifo(A.p,O_CREAT|0666);
	
	mkfifo(SERVERPATH,O_CREAT|0666);
	fd1 = open(SERVERPATH,O_WRONLY);
	if(fd1<0)
	{
		perror("Server fifo not opened");
	}
	strcpy(A.mess,"@init");
	write(fd1,&A,sizeof(struct message));
	fd = open(A.p,O_RDONLY);
	if(fd<0)
	{
		perror("Client fifo not opened");
	}
	fd_set readset1,readset2;
	struct timeval t;int r,sz;
	FD_SET(0,&readset1);
	FD_SET(fd,&readset2);
	while(1)
	{
		t.tv_sec = 2;
		t.tv_usec = 1000;
		//printf("Write a message for broadcasting..\n");
		r = select(1,&readset1,NULL,NULL,&t);
		if(r>0)
		{
			//printf("Write a message for broadcasting..\n");
	 		scanf("%[^\n]s",A.mess);
	 		write(fd1,&A,sizeof(struct message));
	 		while(getchar()!='\n');
		}
		else
			FD_SET(0,&readset1);
		t.tv_sec = 2;
		t.tv_usec = 1000;
		r = select(fd+1,&readset2,NULL,NULL,&t);
		if(r>0)
		{
			sz = read(fd,&A1,sizeof(struct message));
			if(sz>0)
			printf("From: %s\nMessage: %s\n",A1.p,A1.mess);
		}
		else
		{
			//printf("No luck!\n");
			FD_SET(fd,&readset2);
		}
	}
	return 0;
}