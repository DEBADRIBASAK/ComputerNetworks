#include "../cn.h"

char* paths[5] = {"SERVER1","SERVER2","SERVER3","SERVER4","SERVER5"};
int client_list[5][50];
int client_pid_list[5][50];
int ind[5],fd[5];
struct Message
{
	char p[10];
	char message[256];
};
fd_set fifo_readset;
int maxfd = -1;
void init()
{
	for(int i=0;i<5;i++)
	{
		ind[i] = 0;
		mkfifo(paths[i],O_CREAT|0666);
		fd[i] = open(paths[i],O_RDONLY|O_NONBLOCK);
		if(fd[i]<0)
		{
			perror("FIFO not opened");
			exit(0);
		}
		else
		{
			maxfd = (maxfd<fd[i])?fd[i]:maxfd;
			printf("Successful..\n");
			int flags = fcntl(fd[i],F_GETFL,0);
			flags&=~O_NONBLOCK;
			flags|=O_RDONLY;
			fcntl(fd[i],F_SETFL,flags);
			FD_SET(fd[i],&fifo_readset);
		}
	}
}

void sig_handler(int alrm)
{
	printf("A fifo was closed\n");
}

int main(int argc, char const *argv[])
{
	signal(SIGPIPE,sig_handler);
	init();
	//printf("Done!\n");
	struct timeval t;int r,sz;
	struct Message A;
	while(1)
	{
		t.tv_sec = 2;
		t.tv_usec = 1000;
		r = select(maxfd+1,&fifo_readset,NULL,NULL,&t);
		if(r>0)
		{
			//printf("Time left: %ld : %ld\n",t.tv_sec,t.tv_usec);
			for(int i=0;i<5;i++)
			{
				if(FD_ISSET(fd[i],&fifo_readset))
				{
					sz = read(fd[i],&A,sizeof(struct Message));
					if(sz<=0)
					{
						//printf("Problem: %d\n",sz);
						FD_CLR(fd[i],&fifo_readset);
						FD_SET(fd[i],&fifo_readset);
					}
					else if(strcmp(A.message,"@init")==0)
					{
						//printf("%d : PID: %s\n",i+1,A.p);
						mkfifo(A.p,O_CREAT|0666);
						client_list[i][ind[i]] = open(A.p,O_WRONLY);
						if(client_list[i][ind[i]]<0)
						{
							perror("Client FIFO not opened");
							exit(0);
						}
						else
						{
							client_pid_list[i][ind[i]] = atoi(A.p);
							ind[i]++;
						//printf("ind[%d] = %d\n",i,ind[i]);
						}
					}
					else
					{
						int k = atoi(A.p),sz;
						for(int j=0;j<ind[i];j++)
						{
							if(client_pid_list[i][j]!=-1&&k!=client_pid_list[i][j])
							{
							//printf("pid: %d\n",client_pid_list[i][j]);
								sz = write(client_list[i][j],&A,sizeof(struct Message));
								if(sz<0)
								{
									close(client_list[i][j]);
									client_list[i][j] = -1;
									client_pid_list[i][j] = -1;
								}
							}
						}
					}
				}
				else
					FD_SET(fd[i],&fifo_readset);
			}
		}
		else
		{
			for(int i=0;i<5;i++)
			{
				FD_SET(fd[i],&fifo_readset);
			}
		}
		sleep(1);
	}
	return 0;
}