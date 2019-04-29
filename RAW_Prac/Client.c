#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr(argv[1]);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	char buffer[100],ip[INET_ADDRSTRLEN];
	while(1)
	{
		memset(buffer,0,100);
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);while(getchar()!='\n');
		printf("Enter the IP\n");
		scanf("%[^\n]s",ip);
		while(getchar()!='\n');
		addr.sin_addr.s_addr = inet_addr(ip);
		sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr,sizeof(addr));
	}
	return 0;
}