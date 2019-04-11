#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netinet/ip.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_RAW,253);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr1;
	memset(&addr1,0,sizeof(addr1));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = inet_addr("127.0.0.2");
	//addr1.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		printf("Could not bind");exit(0);
	}
	char buffer[255];int sz;
	while(1)
	{
		if((sz = recvfrom(sfd,buffer,255,0,NULL,NULL))<0)
		{
			perror("Could not read");
		}
		else
		{
			if(sz==0)
				exit(0);
			struct iphdr *ip;
			ip = (struct iphdr*)buffer;
			printf("Reading: %s\n",buffer+((ip->ihl)*4));
		}
	}
	return 0;
}