// the server program
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/select.h>
#include <string.h>
#define SERVERPATH "SERVER"

struct Pair
{
	char p[10];
	int req;
};

void sig_handler(int sigpipe)
{
	printf("A client fifo was closed\n");
}

int max(int a,int b)
{
	return (a>b)?a:b;
}

int main(int argc, char *argv[])
{
	signal(SIGPIPE,sig_handler);
	fd_set readset;
	
	int maxfdp1 = 0,fd;
	int dfd[3];
	int present[3] = {0,0,0},ind[3];
	int cfd[3][10];
	ind[0] = ind[1] = ind[2] = 0;
	mkfifo(SERVERPATH,O_CREAT|0666);
	fd = open(SERVERPATH,O_RDONLY);
	if(fd<0)
	{
		perror("Server fifo not opened");
	}
	FD_SET(fd,&readset);
	maxfdp1 = fd;
	char buffer[50];
	struct Pair A;
	struct timeval t;
	t.tv_sec = 5;
	t.tv_usec = 1000;
	while(1)
	{
		int r = select(maxfdp1+1,&readset,NULL,NULL,&t);
		if(r>0)
		{
			if(FD_ISSET(fd,&readset))
			{
				read(fd,&A,sizeof(struct Pair));
				printf("Reading: %s",A.p);
				if(!present[A.req-1])
				{
					char pro[] = {'.','/','D','1','\0'};
					pro[3] = (char)(A.req+'0');
					FILE* fp = popen(pro,"r");
					if(fp==NULL)
					{
						perror("Data centre not opened");
						exit(0);
					}
					dfd[A.req-1] = fileno(fp);
					maxfdp1 = max(maxfdp1,dfd[A.req-1]);
					//FD_SET(dfd[A.req-1],&readset);
				}
				mkfifo(A.p,O_CREAT|0666);
				cfd[A.req-1][ind[A.req-1]++] = open(A.p,O_WRONLY);
			}
			else
				FD_SET(fd,&readset);
			for(int i=0;i<3;i++)
			{
				if(FD_ISSET(dfd[i],&readset))
				{
					int sz = read(dfd[i],buffer,100);
					buffer[sz] = '\0';
					for(int j=0;j<ind[i-1];j++)
					{
						if(cfd[i-1][j]>0)
						{
							int r = write(cfd[i-1][j],buffer,sz);
							if(r<0)
							{
								close(cfd[i-1][j]);
								cfd[i-1][j] = -1;
							}
						}
					}
				}
				else
					FD_SET(dfd[i],&readset);
			}
		}
		sleep(1);
	}
	return 0;
}