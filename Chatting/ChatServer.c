#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#define SERVERPATH "SERVER"
#define MAX_CLIENT 256
int cnt = 0;
char *clients[MAX_CLIENT];

struct message
{
	char p[10];
	char mess[256];
};

int is_in(struct message* m)
{
	int f = 0;
	for(int i=0;i<cnt;i++)
	{
		if(strcmp(clients[i],m->p)==0)
			f = 1;
		else
		{
			int fd = open(clients[i],O_WRONLY);
			if(fd<0)
			{
				perror("Client fifo not opened");
			}
			else
			{
				write(fd,m,sizeof(struct message));
				close(fd);
			}
		}
	}
	return f;
}

int main(int argc, char const *argv[])
{
	mkfifo(SERVERPATH,O_CREAT|0666);
	int fd = open(SERVERPATH,O_RDONLY);
	if(fd<0)
	{
		perror("Server fifo not opened");
	}
	struct message A;
	while(1)
	{
		read(fd,&A,sizeof(struct message));
		mkfifo(A.p,O_CREAT);
		if(strcmp(A.mess,"@init")==0||!is_in(&A))
		{
			if(cnt==MAX_CLIENT)
			{
				printf("Sorry! These highest no of clients are %d\n",MAX_CLIENT);
			}
			clients[cnt] = NULL;
			clients[cnt] = (char*)malloc(sizeof(char));
			strcpy(clients[cnt],A.p);
			cnt++;
		}
		sleep(1);
	}
	return 0;
}