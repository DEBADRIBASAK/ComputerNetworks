#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <sys/un.h>
#include <pcap.h>


void packet_handler(u_char* args,const struct pcap_pkthdr* pkt,const u_char* packet)
{
	struct ether_header* ether;
	ether = (struct ether_header*)packet;
	if(ntohs(ether->ether_type) == ETHERTYPE_IP)
	{
		
		struct iphdr* ip;
		ip = (struct iphdr*)(packet+14);
		char ad[INET_ADDRSTRLEN],ad1[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,&ip->saddr,ad,INET_ADDRSTRLEN);
		inet_ntop(AF_INET,&ip->daddr,ad1,INET_ADDRSTRLEN);
		if(strcmp(ad,"127.0.0.1")!=0&&strcmp(ad1,"127.0.0.1")!=0)
			return;
		printf("IP PACKET:  \n\n");
		printf("Source Mac Address: %s\n",ether_ntoa((const struct ether_addr*)(ether->ether_shost)));
		printf("Destination Mac Address: %s\n",ether_ntoa((const struct ether_addr*)(ether->ether_dhost)));
		printf("Source IP: %s\n",ad);
		printf("Destination IP: %s\n",ad1);
		printf("TTL: %d\n",ip->ttl);
		printf("Protocol: %d\n",ip->protocol);
		printf("Version: %d\n",ip->version);

		if(ip->protocol==6)
		{
			struct tcphdr* tcp;
			tcp = (struct tcphdr*)(packet+14+ip->ihl*4);
			printf("Source Port: %d\n",ntohs(tcp->source));
			printf("Destination Port: %d\n",ntohs(tcp->dest));
			printf("Data: %s\n",packet+14+ip->ihl*4+tcp->doff*4);
		}
	}
}

int main(int argc, char const *argv[])
{
	char device[10];
	strcpy(device,"lo");
	char error[PCAP_ERRBUF_SIZE];
	pcap_t* handle;
	int timeout = 1000;
	//device = pcap_lookupdev(error);
	// if(device==NULL)
	// {
	// 	printf("Could not find device: %s\n",error);exit(0);
	// }

	handle = pcap_open_live(device,BUFSIZ,0,timeout,error);
	if(handle==NULL)
	{
		printf("Could not find handle: %s\n",error);exit(0);
	}
	pcap_loop(handle,0,packet_handler,NULL);
	return 0;
}