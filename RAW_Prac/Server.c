#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(int argc, char const *argv[])
{
	if(argc<3)
	{
		printf("Usage : %s [IP] [PROTOCOL]\n",argv[0]);exit(0);
	}
	int protocol = atoi(argv[1]);
	int sfd = socket(AF_INET,SOCK_RAW,protocol);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	char buffer[100];
	while(1)
	{
		memset(buffer,0,100);
		recvfrom(sfd,buffer,100,0,NULL,NULL);
		struct iphdr* ip;
		ip = (struct iphdr*)buffer;
		printf("Received: %s\n",buffer+(ip->ihl*4));
	}
	return 0;
}