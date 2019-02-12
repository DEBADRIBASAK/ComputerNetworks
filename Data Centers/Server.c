// the server program
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
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

int main(int argc, char *argv[])
{
	signal(SIGPIPE,sig_handler);
	struct pollfd pfd[4];
	int present[3] = {0,0,0},ind[3];
	//char* l1[10],*l2[10],*l3[10];
	int cfd[3][10];//,cfd2[10],cfd3[10];
	ind[0] = ind[1] = ind[2] = 0;
	mkfifo(SERVERPATH,O_CREAT|0666);
	pfd[0].fd = open(SERVERPATH,O_RDONLY);
	if(pfd[0].fd<0)
	{
		perror("Server fifo not opened");
	}
	pfd[0].events = POLLIN;
	pfd[0].revents = pfd[1].revents =pfd[2].revents = pfd[3].revents = 0;
	char buffer[50];
	struct Pair A;
	while(1)
	{
		int r = poll(pfd,(nfds_t)4,2000);
		if(r>0)
		{
			if(pfd[0].revents&POLLIN)
			{
				read(pfd[0].fd,&A,sizeof(struct Pair));
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
					pfd[A.req].fd = fileno(fp);
					pfd[A.req].events = POLLIN;
				}
				mkfifo(A.p,O_CREAT|0666);
				cfd[A.req-1][ind[A.req-1]++] = open(A.p,O_WRONLY);
			}
			for(int i=1;i<4;i++)
			{
				if(pfd[i].revents&POLLIN)
				{
					int sz = read(pfd[i].fd,buffer,100);
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
			}
		}
		sleep(1);
	}
	return 0;
}