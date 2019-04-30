#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>
#include <net/if.h>
#include <pcap.h>

int main(int argc, char const *argv[])
{
	char buffer[1500] = {0};
	struct ether_header* eth;
	struct iphdr* ip;
	eth = (struct ether_header*)buffer;
	ip = (struct iphdr*)(buffer+14);
	// e4:e7:49:0f:cd:31
	char src[6];
	src[0] = 0xE4;
	src[1] = 0xE7;
	src[2] = 0x49;
	src[3] = 0x0F;
	src[4] = 0xCD;
	src[5] = 0x31;
	memcpy(eth->ether_shost,src,6);
	char dest[6];
	dest[0] = 0xFF;
	dest[1] = 0xFF;
	dest[2] = 0xFF;
	dest[3] = 0xFF;
	dest[4] = 0xFF;
	dest[5] = 0xFF;
	memcpy(eth->ether_dhost,dest,6);
	eth->ether_type = htons(ETHERTYPE_IP);
	ip->version = 4;
	ip->ttl = 64;
	ip->ihl = 5;
	ip->tot_len = 1486;
	ip->saddr = inet_addr("127.0.0.1");
	ip->daddr = inet_addr("127.0.0.2");
	ip->check = 0;
	ip->protocol = 253;
	char device[10] = "eno1";
	pcap_t* handle;
	char error[PCAP_ERRBUF_SIZE];
	handle = pcap_open_live(device,BUFSIZ,0,1000,error);
	if(handle==NULL)
	{
		perror("Could not get handle");exit(0);
	}

	while(1)
	{
		if(pcap_inject(handle,buffer,1500)<0)
		{
			perror("Could not inject");
		}
		else
		{
			printf("Success\n");
		}
		sleep(1);
	}
	return 0;
}