#include "cn.h"
#include <sys/select.h>


int f = 0;

void sig_handler(int alrm)
{
	f = 1;
	signal(SIGINT,sig_handler);
}

int main(int argc, char const *argv[])
{
	//signal(SIGINT,sig_handler);
	int fd[5];
	FILE* fp[5];
	char proc[] = {'.','/','o','u','t',' ','1','\0'};
	struct timeval t;
	fd_set readset,writeset;
	char buffer[10];
	int m = -1,r;
	for(int i=0;i<5;i++)
	{
		if(i<4)
		{
			proc[6] = (char)('0'+i+1);
			fp[i] = popen(proc,"r");
			if(fp[i]!=NULL)
			{
				fd[i] = fileno(fp[i]);
				FD_SET(fd[i],&readset);
				m = (m>fd[i])?m:fd[i];
			}
			else
			{
				perror("popen failed");
			}
		}
		else
		{
			fp[i] = popen("./in","w");
			if(fp[i]!=NULL)
			{
				fd[i] = fileno(fp[i]);
			}
			else
			{
				perror("popen failed");
			} 
		}
	}
	struct pollfd pfd[1];
	pfd[0].fd = 0;
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	m++;
	while(1)
	{
		t.tv_sec = 2;
		t.tv_usec = 2000;
		r = select(m,&readset,NULL,NULL,&t);
		if(r>0)
		{
			printf("Time left: %ld sec %ld usec\n",t.tv_sec,t.tv_usec);
			for(int i=0;i<4;i++)
			{
				//printf("*");
				if(FD_ISSET(fd[i],&readset))
				{
					fscanf(fp[i],"%[^\n]s",buffer);
					while(fgetc(fp[i])!='\n');
					fprintf(fp[4],"%s\n",buffer);
					fflush(fp[4]);
				}
				else
				{
					//printf("i = %d\n",i+1);
					FD_SET(fd[i],&readset);
				}
			}
		}
		else //if(f)
		{
			r = poll(pfd,(nfds_t)1,0);
			if(!r)
			{
				printf("Console not available!\n");
			}
			else
			{
				f = 0;
				scanf("%[^\n]s",buffer);
				while(getchar()!='\n');
				fprintf(fp[4],"%s\n",buffer);
				fflush(fp[4]);
			}
			for(int i=0;i<4;i++)
			{
				FD_SET(fd[i],&readset);
			}
		}
		sleep(1);
	}
	return 0;
}