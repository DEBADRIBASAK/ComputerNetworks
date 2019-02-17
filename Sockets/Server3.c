#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define NO_OF_REQUESTS 10
int nsfd,sfd2;
void server_operation()
{
	char buffer[50];int sz;
	while(1)
	{
		if((sz = recv(sfd2,buffer,50,0))<=0)
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
		strcat(buffer," from s2");
		if(send(sfd2,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}

	}
	close(sfd2);
}

int main(int argc, char const *argv[])
{
	int sfd;
	struct sockaddr_in addr,addr_check;
	char address[INET_ADDRSTRLEN];
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8003);

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Binding error");
		exit(0);
	}
	if(listen(sfd,NO_OF_REQUESTS)<0)
	{
		perror("Listening error");
		exit(0);
	}
	
	while(1)
	{
		printf("Waiting..\n");
		struct sockaddr_in cl_addr;
		nsfd = accept(sfd,NULL,NULL);
		recv(nsfd,(struct sockaddr_in*)&cl_addr,sizeof(cl_addr),0);
		sfd2 = socket(AF_INET,SOCK_STREAM,0);
		//sleep(5);
		if(connect(sfd2,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
		{
			perror("Could not connect");
			exit(0);
		}
		else
		{
			close(nsfd);
		}
		server_operation();
	}
	return 0;
}