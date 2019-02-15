#include "../cn.h"
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <syslog.h>
#include <netinet/in.h>
#define NO_OF_REQUEST 10
#define HOST_NAME_MAX 256
#define SERVER_PORT 8000

struct Operation
{
	int a,b,c;
};

int main(int argc, char const *argv[])
{
	int sfd,nsfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(SERVER_PORT);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connection failed");
		exit(0);
	}
	else
	{
		printf("Connection established\n");
	}
	char buffer[50];
	while(1)
	{
		printf("Enter a message to send to server\n");
		scanf("%[^\n]s",buffer);
		if(send(sfd,buffer,strlen(buffer),0)<0)
		{
			printf("Sending failed\n");
			exit(0);
		}
		if(strcmp(buffer,"bye")==0)
			break;
		while(getchar()!='\n');
		if(recv(sfd,buffer,50,0)<0)
		{
			perror("Could not read from server");
		}
		else
		{
			printf("Received: %s\n",buffer);
		}
	}
	close(sfd);
	return 0;
}