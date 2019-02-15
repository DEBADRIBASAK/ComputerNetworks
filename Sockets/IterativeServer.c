/* This is my first practice program on Sockets */
#include "../cn.h"
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define NO_OF_REQUEST 10
#define HOST_NAME_MAX 256
#define SERVER_PORT 8000

int nsfd;

void server_operation()
{
	char buffer[50];int sz;
	while(1)
	{
		if((sz = recv(nsfd,buffer,50,0))<0)
		{
			perror("Reading error!");
			close(nsfd);
			return;
		}
		buffer[sz] = '\0';
		if(strcmp(buffer,"bye")==0)
			break;
		printf("Received: %s\n",buffer);
		int n = strlen(buffer);
		for(int i=0;i<n;i++)
		{
			if(buffer[i]>='a'&&buffer[i]<='z')
			{
				buffer[i] = buffer[i]-'a'+'A';
			}
		}
		if(send(nsfd,buffer,n,0)<0)
		{
			perror("Could not send");
		}

	}
	close(nsfd);
}

int main(int argc,char* argv[])
{
	char address[20];
	const char *ip;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr_check;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(SERVER_PORT);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr)))
	{
		perror("Could not bind address");
		exit(0);
	}
	else
	{
		getsockname(sfd,(struct sockaddr*)&addr_check,NULL);
		ip = inet_ntop(AF_INET,&(addr_check.sin_addr),address,INET_ADDRSTRLEN);
		printf("Server started with IP: %s\n",ip);
	}
	if(listen(sfd,NO_OF_REQUEST)<0)
	{
		perror("Error in listen");
		exit(0);
	}
	while(1)
	{
		printf("Waiting..\n");
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not establish connection!");
			exit(0);
		}
		else
		{
			getpeername(nsfd,(struct sockaddr*)&addr_check,NULL);
			ip = inet_ntop(AF_INET,&(addr_check.sin_addr),address,INET_ADDRSTRLEN);
			printf("Accepted IP: %s\n",ip);	
		}
		server_operation();
	}
	return 0;
}
