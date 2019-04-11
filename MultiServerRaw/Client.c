#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [NO_OF_SERVERS_AVAILABLE] [RAW_IP_ADDRESS]\n",argv[0]);exit(0);
	}
	int rsfd = socket(AF_INET,SOCK_RAW,253),sz,optval = 1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	struct sockaddr_in rawaddr,cl_addr;memset(&rawaddr,0,sizeof(rawaddr));
	rawaddr.sin_family = AF_INET;
	rawaddr.sin_addr.s_addr = inet_addr("127.0.0.2");
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_addr.s_addr = inet_addr("127.0.0.3");
	if(bind(rsfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
	perror("Could not bind");
	else
	printf("Success..\n");
	/*if(connect(rsfd,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("Could not connect");exit(0);
	}*/
	int n = atoi(argv[1]);
	char buffer[100];
	strcpy(buffer,"?");
	if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("Could not send");
	}
	else
	{
		for(int i=0;i<n;i++)
		{
			if(recvfrom(rsfd,buffer,100,0,NULL,NULL)<0){
				perror("Could not read");
			}
			else
			{
				struct iphdr *ip;
				ip = (struct iphdr*)buffer;
				printf("Reading: %s\n",buffer+(ip->ihl*4));
			}
		}
		printf("Enter the port no you want to connect to\n");
		int portno;
		scanf("%d",&portno);while(getchar()!='\n');
		int sfd = socket(AF_INET,SOCK_STREAM,0);
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr.sin_port = htons(portno);
		if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			perror("Could not connect");
		}
		else
		{
			while(1)
			{
				printf("Enter a string\n");
				scanf("%[^\n]s",buffer);
				while(getchar()!='\n');
				send(sfd,buffer,strlen(buffer),0);
				sz = recv(sfd,buffer,100,0);
				buffer[sz] = '\0';
				printf("Reading: %s\n",buffer);
			}
		}
	}
	return 0;
}
