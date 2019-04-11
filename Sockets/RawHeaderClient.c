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
	//setsockopt(rsfd,IPPROTO_IP,SO_BROADCAST|IP_HDRINCL,&val,sizeof(val));
		if(setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&val,sizeof(val))<0)
		perror("Could not set option");
	else
		printf("Success..\n");
	char buffer[1024];
	struct MyIPheader* ip;
	ip = (struct MyIPheader*)buffer;
	ip->protocol = 253;
	ip->version = 4;
	ip->id = 0;
	ip->ttl = 64;
	ip->ihl = 5;
	ip->saddr = inet_addr("1.2.3.4");
	ip->daddr = inet_addr("127.0.0.2");
	strcpy(ip->name_of_sender,"debadri");
	printf("Enter a string..\n");
	scanf("%s",buffer+60);
	printf("Entered: %s\n",buffer+60);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if(sendto(rsfd,buffer,1024,0,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not send");
	else
		printf("Sent successfully..\n");
	return 0;
}