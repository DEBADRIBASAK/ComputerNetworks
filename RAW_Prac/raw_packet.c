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

int main(int argc, char const *argv[])
{
	int rsfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	char buffer[65536];
	if(setsockopt(rsfd,SOL_SOCKET,SO_BINDTODEVICE,"eno1",strlen("eno1"))<0)
	{
		perror("Could not set option");
	}
	int sz;
	while(1)
	{
		sz = recvfrom(rsfd,buffer,65536,0,NULL,NULL);
		if(sz<0)
		{
			perror("Reading error");
		}
		else
		{
			printf("Reading %d bytes\n",sz);
			struct ether_header* eth;
			struct iphdr* ip;
			eth = (struct ether_header*)buffer;
			printf("******************************************\n\n");
			printf("Sender MAC Address: %s\n",ether_ntoa((const struct ether_addr*)eth->ether_shost));
			printf("Receiver MAC Address: %s\n",ether_ntoa((const struct ether_addr*)eth->ether_dhost));
			if(ntohs(eth->ether_type)==ETHERTYPE_IP)
			{
				ip = (struct iphdr*)(buffer+14);
				printf("Version: %d\n",ip->version);
				printf("Protocol: %d\n",ip->protocol);
				printf("Type of Service: %d\n",ip->tos);
				char ad[INET_ADDRSTRLEN],ad1[INET_ADDRSTRLEN];
				int k = (ip->saddr);
				printf("Sender IP: %s\n",inet_ntop(AF_INET,&k,ad,INET_ADDRSTRLEN));
				k = (ip->daddr);
				printf("Receiver IP: %s\n",inet_ntop(AF_INET,&k,ad1,INET_ADDRSTRLEN));
			}

			printf("******************************************\n\n");
		}
	}
	return 0;
}