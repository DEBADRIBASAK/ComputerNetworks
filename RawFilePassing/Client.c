#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

struct Message
{
	char addr[INET_ADDRSTRLEN];
	char filename[255];
};

int main(int argc, char const *argv[])
{
	struct Message A;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not connect");exit(0);
	}
	printf("Enter IP address: \n");
	scanf("%s",A.addr);
	printf("Enter filename: \n");
	scanf("%s",A.filename);
	if(send(sfd,&A,sizeof(A),0)<0)
	{
		perror("Could not send");
	}
	else
	{
		printf("SEnding successful\n");
	}
	return 0;
}