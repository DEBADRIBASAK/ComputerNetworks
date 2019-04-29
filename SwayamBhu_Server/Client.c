#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#define KEYPATH "."
#define CONTROLLEN CMSG_LEN(sizeof(int))
#define PATH "/tmp/file.socket"


int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [PORT_NO]\n",argv[0]);exit(0);
	}

	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in t_addr;
	t_addr.sin_family = AF_INET;
	t_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	t_addr.sin_port = htons(8003);
	char c = '?';int rpl;
	sendto(sfd,&c,sizeof(c),0,(struct sockaddr*)&t_addr,sizeof(t_addr));
	printf("Available ports are:\n");
	while(1)
	{
		recvfrom(sfd,&rpl,sizeof(rpl),0,NULL,NULL);
		if(rpl==-1)
			break;
		printf("%d ,",rpl);
	}
	
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(atoi(argv[1]));
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");exit(0);
	}
	else
		printf("Connected\n");
	char buffer[50];
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		send(sfd,buffer,strlen(buffer),0);
		int sz = recv(sfd,buffer,50,0);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
	}
	return 0;
}