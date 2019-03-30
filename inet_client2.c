#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	char addr1[INET_ADDRSTRLEN];
	const char *adr;
	struct sockaddr_in addr,cl_addr;
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	int broadcast = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0)
    {
        perror("Error in setting broadcast");
        return 0;
    }
    else
    	printf("Done!..\n");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	addr.sin_port = htons(atoi(argv[1]));
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[20];int len = sizeof(cl_addr);
	while(1)
	{
		int sz = recvfrom(sfd,buffer,20,0,(struct sockaddr*)&cl_addr,&len);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
	}
	return 0;
}