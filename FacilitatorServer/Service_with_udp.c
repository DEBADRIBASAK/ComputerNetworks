#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define PATH "."
#define PROJID 37
int sfd,nsfd;
struct sockaddr_in addr;

struct ClientList
{
	struct sockaddr_in addr[255];
	int i;
};

struct ClientList* pt;

void* func(void* args)
{
	printf("Thread started..\n");
	int nsfd1 = accept(sfd,NULL,NULL);
	if(nsfd1<0)
	{
		perror("Could not accept");
	}
	int len = sizeof(pt->addr[pt->i]);
	getsockname(nsfd1,(struct sockaddr*)&(pt->addr[pt->i]),&len);
	pt->i++;
	printf("So far no of clients...: %d\n",pt->i+1);
	printf("Thread accepted..\n");

	char buffer[100];int sz;
	while(1)
	{
		if((sz = recv(nsfd1,buffer,100,0))<0)
		{
			perror("Could not read 1");
		}
		printf("Read(thread): %s\n",buffer);
		buffer[sz] = '\0';
		buffer[sz] = '\0';
		if(strcmp(buffer,"end")==0)
			pthread_exit(NULL);
		for(int i=0;i<sz;i++)
		{
			if(buffer[i]>='a'&&buffer[i]<='z')
			buffer[i] = buffer[i]-'a'+'A';
		}
		if(send(nsfd1,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}
		sleep(1);
	}
}

void func1()
{
	//int nsfd1 = accept(sfd,NULL,NULL);
	if(nsfd<0)
	{
		perror("Could not accept");
	}
	char buffer[100];int sz;
	while(1)
	{
		///printf("here\n");
		if((sz = read(nsfd,buffer,100))<0)
		{
			perror("Could not read");
		}
		buffer[sz] = '\0';
		printf("Read: %s\n",buffer);
		if(strcmp(buffer,"end")==0)
			return;
		for(int i=0;i<sz;i++)
		{
			if(buffer[i]>='a'&&buffer[i]<='z')
			buffer[i] = buffer[i]-'a'+'A';
		}
		if(send(nsfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}
		else
		{
			printf("Sent sucessfully...\n");
		}
		sleep(1);
	}
}

void sig_handler(int alrm)
{
	printf("Signal received...\n");
	//int nsfd1 = accept(sfd,NULL,NULL);
	
	pthread_t p;
	pthread_create(&p,NULL,func,&nsfd);
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Key could not be created");
		exit(0);
	}
	int shmid = shmget(k,sizeof(struct ClientList),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");
		exit(0);
	}
	pt = (struct ClientList*)shmat(shmid,NULL,0);
	if(pt==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	sfd = 10+atoi(argv[1]);
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		int len = sizeof(pt->addr[pt->i]);
		printf("Connected\n");
		printf("So far no of clients: %d\n",pt->i+1);
		if(nsfd<0)
		{
			perror("Could not accept");
		}
		func1();
	}
	return 0;
}
