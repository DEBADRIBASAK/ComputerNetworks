#include "../cn.h"

char* paths[5] = {"SERVER1","SERVER2","SERVER3","SERVER4","SERVER5"};
struct Message
{
	char p[10];
	char message[256];
};
fd_set readset;
int self_fd ;
void sig(int alrm)
{
	close(self_fd);
	printf("Closed FIFO\n");
	exit(0);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT,sig);
	struct Message A,B;
	pid_t p = getpid();
	sprintf(A.p,"%d",(int)p);
	int gid = atoi(argv[1]);
	strcpy(A.message,"@init");
	mkfifo(paths[gid-1],O_CREAT|0666);
	int fd = open(paths[gid-1],O_WRONLY);
	if(fd<0)
	{
		perror("Server FIFO not opened");
		exit(0);
	}
	else
	{
		write(fd,&A,sizeof(struct Message));
		printf("Sent successfully...\n");
	}
	mkfifo(A.p,O_CREAT|0666);
	self_fd = open(A.p,O_RDONLY);
	if(self_fd<0)
	{
		perror("Own FIFO not opened");
		exit(0);
	}
	FD_SET(0,&readset);
	FD_SET(self_fd,&readset);
	struct timeval t;int r;
	while(1)
	{
		t.tv_sec = 5;
		t.tv_usec = 1000;
		r = select(self_fd+1,&readset,NULL,NULL,&t);
		if(r>0)
		{
			if(FD_ISSET(0,&readset))
			{
				scanf("%[^\n]s",A.message);
				write(fd,&A,sizeof(struct Message));
				while(getchar()!='\n');
			}
			else
				FD_SET(0,&readset);
			if(FD_ISSET(self_fd,&readset))
			{
				read(self_fd,&B,sizeof(struct Message));
				printf("From: %s\nMessage: %s\n",B.p,B.message);
			}
			else
				FD_SET(self_fd,&readset);
		}
		else
		{
			FD_SET(0,&readset);
			FD_SET(self_fd,&readset);
		}
		sleep(1);
	}
	return 0;
}