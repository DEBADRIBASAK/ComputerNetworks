#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	char addr1[INET_ADDRSTRLEN];
	const char *adr;
	struct sockaddr_in addr,broad_addr;
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
	addr.sin_addr.s_addr = inet_addr("127.0.255.255");
	addr.sin_port = htons(8001);
	adr = inet_ntop(AF_INET,&addr.sin_addr.s_addr,addr1,INET_ADDRSTRLEN);
	printf("The address is = %s\n",adr);
	char buffer[20];
	while(1)
	{
		scanf("%s",buffer);
		if(sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			perror("Could not send");
		}
		else
			printf("Success..\n");
	}
	return 0;
}