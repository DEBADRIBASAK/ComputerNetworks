// 5th question
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: ./Client [PORTNO]\n");
		exit(0);
	}
	int portno = atoi(argv[1]);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(portno);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	char buffer[100];int sz;
	while(1)
	{
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(sfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}
		if((sz = recv(sfd,buffer,100,0))<0)
		{
			perror("Could not read");
		}
		buffer[sz] = '\0';
		printf("%s\n",buffer);
		sleep(1);
	}
	return 0;
}