#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int sfd;//sfd1;
struct sockaddr_in cl_addr;

void* func(void* arg)
{
	struct sockaddr_in addr1 = *(struct sockaddr_in*)arg;
	int sfd1 = socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd1,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not connect");
	}
	else
	{
		char buffer[50];int sz;
		while(1)
		{
			sz = recv(sfd1,buffer,50,0);
			buffer[sz] = '\0';
			if(strcmp(buffer,"end")==0)
				break;
			for(int i=0;i<sz;i++)
				buffer[i] = buffer[i]-'A'+'a';
			send(sfd1,buffer,sz,0);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	sfd = socket(AF_INET,SOCK_STREAM,0);//,sfd1;
	int nsfd;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8002);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		printf("Could not listen");exit(0);
	}
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");continue;
		}
		else
		{
			printf("Accepted..\n");
		}
		if(recv(nsfd,&cl_addr,sizeof(cl_addr),0)<0)
			perror("Could not get address");
		else
		{
			pthread_t p;
			pthread_create(&p,NULL,func,(void*)&cl_addr);
		}
	}
	return 0;
}