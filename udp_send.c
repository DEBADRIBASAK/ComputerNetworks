#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr,addr1;int len = sizeof(addr1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8001);

	memset(&addr1,0,sizeof(addr));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8000);

	
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[100];int sz;
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr1,sizeof(addr1))<0)
			perror("Could not send!");
	}
	return 0;
}