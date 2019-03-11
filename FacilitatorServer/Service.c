#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int sfd,nsfd;
struct sockaddr_in addr;


void* func(void* args)
{
	printf("Thread started..\n");
	int nsfd1 = accept(sfd,NULL,NULL);
	if(nsfd1<0)
	{
		perror("Could not accept");
	}
	printf("Thread accepted..\n");

	char buffer[100];int sz;
	while(1)
	{
		if((sz = recv(nsfd1,buffer,100,0))<0)
		{
			perror("Could not read");
		}
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
		if((sz = recv(nsfd,buffer,100,0))<0)
		{
			perror("Could not read");
		}
		buffer[sz] = '\0';
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
	sfd = 10+atoi(argv[1]);
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
		}
		func1();
	}
	return 0;
}