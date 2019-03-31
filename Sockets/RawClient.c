#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#define BUF_LEN 160

int main(int argc, char const *argv[])
{
	if(setuid(0)<0)
		perror("Could not set uid");
	int rsfd = socket(AF_INET,SOCK_RAW,253);
	int optval = 1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if(connect(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");exit(0);
	}
	char buffer[BUF_LEN];int sz;
	while(1)
	{
		scanf("%[^\n]s",buffer);
		printf("Input: %s\n",buffer);
		while(getchar()!='\n');
		if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			perror("Could not write");
		}
		else if((sz = recvfrom(rsfd,buffer,BUF_LEN,0,NULL,NULL))<0)
		{
				perror("Could not read");
		}
		else
		{
			//buffer[sz] = '\0';
			struct iphdr *ip;char ad[INET_ADDRSTRLEN];
  			ip=(struct iphdr*)buffer;
			//cout<<(buf+(ip->ihl)*4)<<endl;
			
			printf("TTL: %d\nVersion: %d\nProtocol: %d\nForeign IP: %s\n",(int)ip->ttl,(int)ip->version,(int)ip->protocol,inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN));
			
			printf("Reading: %s\n",buffer+(ip->ihl)*4);
		}

	}
	return 0;
}
