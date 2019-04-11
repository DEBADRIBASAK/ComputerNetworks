#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

struct MyIPheader
{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
	#elif __BYTE_ORDER == __BIG_ENDIAN
	    unsigned int version:4;
	    unsigned int ihl:4;
	#else
	# error	"Please fix <bits/endian.h>"
	#endif
	    uint8_t tos;
	    uint16_t tot_len;
	    uint16_t id;
	    uint16_t frag_off;
	    uint8_t ttl;
	    uint8_t protocol;
	    uint16_t check;
	    uint32_t saddr;
	    uint32_t daddr;
	char name_of_sender[10];
};

int main(int argc, char const *argv[])
{
	int rsfd = socket(AF_INET,SOCK_RAW,253);
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	int val = 1;
	if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&val,sizeof(val))<0)
		perror("Could not set option");
	else
		printf("Success..\n");
	char buffer[1024];
	struct MyIPheader *ip;
	struct sockaddr_in addr,cl_addr;
	int len = sizeof(cl_addr);
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	else
		printf("Bound..\n");
	if(recvfrom(rsfd,buffer,1024,0,(struct sockaddr*)&cl_addr,&len)<0)
	{
		perror("Could not receive");exit(0);
	}
	else
	{
		printf("Success..\n");
		ip = (struct MyIPheader*)(buffer);
		char ad[INET_ADDRSTRLEN];
		printf("Sender Name: %s\nProtocol No: %d\nTTL: %d\nVersion No: %d\nPacket ID: %d\nSource Address: %s\nChecksum: %d\nLength: %d\n",ip->name_of_sender,(int)ip->protocol,(int)ip->ttl,(int)ip->version,(int)ip->id,inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN),(int)ip->check,(int)ip->ihl);
		printf("Destination: %s\n",inet_ntop(AF_INET,&(ip->daddr),ad,INET_ADDRSTRLEN));
		printf("Actual IP: %s\n",inet_ntop(AF_INET,&(cl_addr.sin_addr.s_addr),ad,INET_ADDRSTRLEN));
		printf("Data: %s\n",buffer+60);
	}
	return 0;
}