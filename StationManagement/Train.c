#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),temp = 1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr1,addr2;
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not connect");exit(0);
	}
	if(recv(sfd,&addr2,sizeof(addr2),0)<0)
	{
		perror("Could not get the platform address");
		exit(0);
	}
	int sz;sfd = socket(AF_INET,SOCK_STREAM,0);//,temp = 1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	if(connect(sfd,(struct sockaddr*)&addr2,sizeof(addr2))<0)
	{
		perror("Could not connect to platform");exit(0);
	}
	char buffer[20];
	while(1)
	{
		printf("Enter a boggy num\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(sfd,buffer,strlen(buffer),0)<0)
		{
			perror("Sending failed");
		}
		if(strcmp(buffer,"end")==0)
			break;
	}
	close(sfd);
	return 0;
}