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
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr,addr1;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
	}

	// accept doesn't work in raw sockets

	 char buffer[BUF_LEN],buff[BUF_LEN];
	int sz;
	int len = sizeof(addr1);
	while(1)
	{
		if((sz = recvfrom(rsfd,buffer,BUF_LEN,0,(struct sockaddr*)&addr1,&len))<0)
		{
			perror("Could not read");
		}
		else
		{
			struct iphdr *ip;char ad[INET_ADDRSTRLEN];
  			ip=(struct iphdr*)buffer;
			//cout<<(buf+(ip->ihl)*4)<<endl;
			printf("TTL: %d\nVersion: %d\nProtocol: %d\nForeign IP: %s\n",(int)ip->ttl,(int)ip->version,(int)ip->protocol,inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN));
			printf("Received: %s\n",buffer+(ip->ihl)*4);
			strcpy(buff,buffer+(ip->ihl)*4);
			strcat(buff," form server");
			if(sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&addr1,sizeof(addr1))<0)
			{
				perror("Could not send");
			}
		}
		sleep(1);
	}
	return 0;
}
