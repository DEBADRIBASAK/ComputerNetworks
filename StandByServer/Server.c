#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

pthread_cond_t pcond;
pthread_mutex_t pmut;

int nsfd[255],ind = 0,flag = 1,sfd1,index1[255];

struct Message
{
	struct sockaddr_in cl_addr;
	int f;
	int ind;
};



void* func(void* arg)
{
	int fd = nsfd[*(int*)arg],sz;char buffer[100];
	int fl;
	//fl = fcntl(fd,F_GETFL);
	//fl|=O_NONBLOCK;
	//fcntl(fd,F_SETFL,fl);
	memset(buffer,0,sizeof(buffer));
	while(1)
	{
		if(!flag)
		{
			
			memset(buffer,0,sizeof(buffer));
			strcpy(buffer,"Server is down");
			send(fd,buffer,100,0);
			sleep(1);
			struct Message A;
			int len = sizeof(A.cl_addr);
			getpeername(fd,(struct sockaddr*)&A.cl_addr,&len);
			A.ind = *(int*)arg;
			A.f = 1;
			if(send(sfd1,&A,sizeof(A),0)<0)
				perror("Could not send");
			else
				printf("Client info sent successfully\n");
			printf("Thread suspended..\n");
			pthread_mutex_lock(&pmut);
			pthread_cond_wait(&pcond,&pmut);
			pthread_mutex_unlock(&pmut);
			printf("Thread resumed..\n");
			A.f = 0;
			A.ind = *(int*)arg;
			send(sfd1,&A,sizeof(A),0);
			sleep(1);
			memset(buffer,0,sizeof(buffer));
			strcpy(buffer,"Server is Up");
			send(sfd1,buffer,100,0);
		}
		memset(buffer,0,sizeof(buffer));
		if((sz = recv(fd,buffer,100,0))<0)
			perror("Could not receive");
		else
		{
			if(sz==0)
				pthread_exit(NULL);
			//buffer[sz] = '\0';
			for(int i=0;i<strlen(buffer);i++)
				if(buffer[i]>='a'&&buffer[i]<='z')
				buffer[i] = buffer[i]-'a'+'A';
			send(fd,buffer,100,0);
		}
	}
}


int main(int argc, char const *argv[])
{
	pthread_mutex_init(&pmut,NULL);
	pthread_cond_init(&pcond,NULL);
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(0,&readset);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	else if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	else
	{
		FD_SET(sfd,&readset);
	}
	sfd1 = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8001);
	if(connect(sfd1,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not connect");exit(0);
	}
	int r,sz;char buffer[10];
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			if(FD_ISSET(sfd,&readset))
			{
				nsfd[ind] = accept(sfd,NULL,NULL);
				pthread_t p;
				index1[ind] = ind;
				pthread_create(&p,NULL,func,&index1[ind]);
				ind++;
			}
			else if(flag)
				FD_SET(sfd,&readset);
			if(FD_ISSET(0,&readset))
			{
				scanf("%s",buffer);
				if(strcmp(buffer,"down")==0)
				{
					printf("Setting flag\n");
					flag = 0;
					FD_CLR(sfd,&readset);
				}
				else //up
				{
					printf("Unset flag\n");
					pthread_cond_broadcast(&pcond);
					FD_SET(sfd,&readset);
					flag = 1;
				}
			}
			else
				FD_SET(0,&readset);
		}
	}
	return 0;
}