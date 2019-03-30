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
#include <string.h>
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

void sig_handler(int alrm)
{
	printf("A trainis arriving...\n");
}

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [INDEX_POSITION]\n",argv[0]);exit(0);
	}
	signal(SIGUSR1,sig_handler);
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
	p->p[atoi(argv[1])] = getpid();
	p->b[atoi(argv[1])] = 1;
	sfd = socket(AF_INET,SOCK_STREAM,0);int temp = 1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr;//,plat_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(ports[atoi(argv[1])]);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	char buffer[20];int sz;
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			exit(0);
		}
		p->b[atoi(argv[1])] = 0;
		while(1)
		{
			sz = recv(nsfd,buffer,20,0);
			buffer[sz] = '\0';
			printf("Passing: %s\n",buffer);
			if(strcmp(buffer,"end")==0)
			{
				printf("Train passed totally\n");break;
			}
		}
		p->b[atoi(argv[1])] = 1;
	}
	return 0;
}