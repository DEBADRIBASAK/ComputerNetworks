#include "../cn.h"
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char block[60] = "127.0.0.3";

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
		perror("could not create socket");exit(0);
	}
	int op = 1;
	setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&op,sizeof(op));
	struct sockaddr_in addr,cl_addr;int len = sizeof(cl_addr);
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
	}
	else
		printf("Bound..\n");
	char ad[INET_ADDRSTRLEN];
	char buffer[1024],reply[1024];
	while(1)
	{
		if(recvfrom(rsfd,buffer,1024,0,(struct sockaddr*)&cl_addr,&len)<0)
			perror("Could not receive");
		else
		{
			printf("Read..\n");
			struct MyIPheader *ip,*ip1;
			ip = (struct MyIPheader*)buffer;
			if(strcmp(inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN),block)==0)
			{
				ip1 = (struct MyIPheader*)reply;
				ip1->id = 0;
				ip1->protocol = 253;
				ip1->ttl = 64;
				ip1->version  = 4;
				ip1->ihl = 5;
				ip1->check = 0;
				strcpy(reply+20,"Your IP address is blocked");
				ip1->saddr = inet_addr("127.0.0.2");
				ip1->daddr = ip->saddr;
				if(sendto(rsfd,reply,1024,0,(struct sockaddr*)&cl_addr,len)<0)
					perror("Could not send");
			}
			else
			{
				ip1 = (struct MyIPheader*)reply;
				ip1->id = 0;
				ip1->protocol = 253;
				ip1->ttl = 64;
				ip1->version  = 4;
				ip1->ihl = 5;
				ip1->check = 0;
				strcpy(reply+20,"<html>\n<body>\n<h1>\nWeb page header\n</h1>\n<p> some content </p>\n</body>\n</html>\n");
				ip1->saddr = inet_addr("127.0.0.2");
				ip1->daddr = ip->saddr;
				if(sendto(rsfd,reply,1024,0,(struct sockaddr*)&cl_addr,len)<0)
					perror("Could not send");
			}
		}
	}
	return 0;
}