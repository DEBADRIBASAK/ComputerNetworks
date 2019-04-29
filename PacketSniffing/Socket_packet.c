#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

int main(int argc, char const *argv[])
{
	int rsfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	char buffer[42];
	char device[10];
	strcpy(device,"eno1");
	struct sockaddr_ll addr,addr1;
	memset(&addr,0,sizeof(addr));
	if((addr.sll_ifindex = if_nametoindex(device))==0)
	{
		perror("Could not find device index number");exit(0);
	}
	addr.sll_family = AF_PACKET;
	char dst[6];
	dst[0] = 00;
	dst[1] = 37;
	dst[2] = 131;
	dst[3] = 112;
	dst[4] = 16;
	dst[5] = 00;
	memcpy(addr.sll_addr,dst,sizeof(uint8_t)*6);
	addr.sll_halen = 6;
	int len = sizeof(addr1);
	while(1)
	{
		if(recvfrom(rsfd,buffer,42,0,(struct sockaddr*)&addr1,&len)<0)
			perror("Could not read");
		else
		{
			struct ether_header* eth;
			eth = (struct ether_header*)buffer;
			if(htons(eth->ether_type)==ETHERTYPE_IP)
			{
				printf("IP Packet\n");
				struct iphdr* ip;
				ip = (struct iphdr*)(buffer+14);
				char ad[INET_ADDRSTRLEN],ad1[INET_ADDRSTRLEN];
				inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN);
				inet_ntop(AF_INET,&(ip->daddr),ad1,INET_ADDRSTRLEN);
				printf("Sender IP: %s\n",ad);
				printf("Receiver IP: %s\n",ad1);
				printf("IP header length: %d\n",ip->ihl);
				printf("IP Version: %d\n",ip->version);
				printf("Protocol: ");
				if(ip->protocol==IPPROTO_TCP)
				{
					printf("TCP\n");
					struct tcphdr* tcp;
					tcp = (struct tcphdr*)(buffer+14+ip->ihl*4);
					printf("Sender port: %d\n",tcp->source);
					printf("Destination port: %d\n",tcp->dest);
				}
				else if(ip->protocol==IPPROTO_UDP)
				{
					printf("UDP\n");
					struct udphdr* udp;
					udp = (struct udphdr*)(buffer+14+ip->ihl*4);
					printf("Sender port: %d\n",udp->source);
					printf("Destination port: %d\n",udp->dest);
				}
				printf("*************************************\n\n");
			}
			else if(htons(eth->ether_type)==ETHERTYPE_ARP)
			{
				printf("Just an ARP Packet\n\n");
			}

		}
	}
	return 0;
}
