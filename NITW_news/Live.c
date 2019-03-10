// tomorrow
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SCREEN "SCREEN"
#define PATH "."
#define PROJID 23
int nsfd;

void perform_live_telecast()
{
	char buffer[256];
	while(1)
	{
		printf("Enter some news\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(nsfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send to newsreader");
		}
		else
		{
			printf("Successfully sent..\n");
		}
		if(strcmp(buffer,"end")==0)
			break;
	}
}


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind with given address");
		exit(0);
	}
	if(listen(sfd,5)<0)
	{
		perror("Could not listen");
		exit(0);
	}

	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			exit(0);
		}
		perform_live_telecast();
	}
	return 0;
}