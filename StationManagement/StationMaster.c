#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#define PATH "."
#define PROJID 37
int ports[3] = {8001,8002,8003};
int shmid,sfd,nsfd;

struct Shmstr
{
	int b[3];
	int p[3];
};
struct Shmstr* p;

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not create key");exit(0);
	}
	shmid = shmget(k,sizeof(struct Shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");
		exit(0);
	}
	p = (struct Shmstr*)shmat(shmid,NULL,0);
	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	sfd = socket(AF_INET,SOCK_STREAM,0);int temp = 1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr,plat_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	plat_addr.sin_family = AF_INET;
	plat_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	while(1)
	{
		if(p->b[0]||p->b[1]||p->b[2])
		{
			nsfd = accept(sfd,NULL,NULL);
			if(nsfd<0)
			{
				perror("Could not accept");exit(0);
			}
			else
			{
				if(p->b[0])
				{
					printf("1\n");
					plat_addr.sin_port = htons(ports[0]);
				}
				else if(p->b[1])
				{
					printf("2\n");
					plat_addr.sin_port = htons(ports[1]);
				}
				else if(p->b[2])
				{
					printf("3\n");
					plat_addr.sin_port = htons(ports[2]);
				}
				if(send(nsfd,&plat_addr,sizeof(plat_addr),0)<0)
				{
					perror("Could not send");
				}
				else
				{
					kill(p->p[0],SIGUSR1);kill(p->p[1],SIGUSR1);kill(p->p[2],SIGUSR1);
				}
			}
		}
		sleep(1);
	}
	return 0;
}