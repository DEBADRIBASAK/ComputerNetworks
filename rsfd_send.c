#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_RAW,17);
	struct sockaddr_in addr,addr1;int len = sizeof(addr1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[100];int sz;
	while(1)
	{
		sz = recvfrom(sfd,buffer,100,0,(struct sockaddr*)&addr1,&len);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer+28);
	}
	return 0;
}