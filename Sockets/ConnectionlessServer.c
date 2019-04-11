#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8002
#define NO_OF_REQUESTS 10


void to_upper(char* s)
{
	int len = strlen(s);
	for(int i=0;i<len;i++)
	{
		if(s[i]>='a'&&s[i]<='z')
		s[i] = s[i]-'a'+'A';
	}
}

int main()
{
	char buffer[50];
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = PORT;
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Binding error");exit(0);
	}
	else
	printf("Successful..\n");
	int nsfd;
	struct sockaddr_in cl_addr;
	int sz,len = sizeof(cl_addr);
	while(1)
	{
		printf("waiting...\n");
		if((sz = recvfrom(sfd,buffer,50,0,(struct sockaddr*)&cl_addr,&len))<0)
		{
			perror("Receiving error");
			exit(0);
		}
		else
		{
			buffer[sz] = '\0';
			printf("Receiving: %s\n",buffer);
			to_upper(buffer);
			if(sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&cl_addr,len)<0)
			{
				perror("Sending failed");
				exit(0);
			}
		}
	}
	return 0;
}
