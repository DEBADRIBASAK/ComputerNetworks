#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8002
#define NO_OF_REQUESTS 10

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage ./ConnectionlessClient [PORT_NO]\n");
		exit(0);
	}
	char buffer[50];
	struct sockaddr_in addr,addr1;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(atoi(argv[1]));
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Binding error");exit(0);
	}
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(PORT);
	int sz,len = sizeof(addr1);
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		if(sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr1,sizeof(addr1))<0)
		{
			perror("Sending failed");
			exit(0);
		}
		printf("Sent..\n");
		if((sz = recvfrom(sfd,buffer,50,0,(struct sockaddr*)&addr1,&len))<0)
		{
			perror("Reading error!");
			exit(0);
		}
		else
		{
			buffer[sz] = '\0';
			printf("Received: %s\n",buffer);
		}
		while(getchar()!='\n');
	}
	return 0;
}
