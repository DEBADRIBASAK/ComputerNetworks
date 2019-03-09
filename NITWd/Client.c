#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: ./Client [PORT_NO]\n");
		exit(0);
	}
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(atoi(argv[1]));
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connection error");
		exit(0);
	}
	else
	{
		printf("Connection established\n");
	}
	char buffer[20];int sz;
	while(1)
	{
		scanf("%[^\n]s",buffer);
		if(send(sfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}
		else if((sz = recv(sfd,buffer,20,0))<0)
		{
			buffer[sz] = '\0';
			perror("Could not receive");
		}
		else
		{
			printf("Received: %s\n",buffer);
		}
		while(getchar()!='\n');
	}
	return 0;
}