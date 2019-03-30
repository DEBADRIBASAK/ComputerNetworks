#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr,self_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	self_addr.sin_family = AF_INET;
	self_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	self_addr.sin_port = htons(8001);
	bind(sfd,(struct sockaddr*)&self_addr,sizeof(self_addr));
	char buffer[50];
	for(int j=0;j<3;j++)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr,sizeof(addr));
		int sz = recvfrom(sfd,buffer,50,0,NULL,NULL);
		buffer[sz] = '\0';
		printf("Received: %s\n",buffer);
	}
	close(sfd);
	return 0;
}