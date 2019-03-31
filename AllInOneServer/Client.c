#include "../cn.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd;
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");exit(0);
	}
	int ch;
	printf("Enter one service No.\n");
	scanf("%d",&ch);
	while(getchar()!='\n');
	send(sfd,&ch,sizeof(ch),0);
	recv(sfd,&ch,sizeof(ch),0);
	if(ch==0)
	{
		printf("Server denied\n");
		exit(0);
	}
	char buffer[50];
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		send(sfd,buffer,strlen(buffer),0);
		if(strcmp(buffer,"end")==0)
			break;
		if((ch = recv(sfd,buffer,50,0))<0)
		{
			perror("Could not read");
		}
		else
		{
			buffer[ch] = '\0';
			printf("Reading: %s\n",buffer);
		}
	}

	return 0;
}