#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int sfd;

int main(int argc, char const *argv[])
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connect failed");
		exit(0);
	}
	char buffer[50];int sz;
	while(1)
	{
		sz = recv(sfd,buffer,50,0);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
		sleep(1);
	}
	return 0;
}