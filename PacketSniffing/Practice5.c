#include "../cn.h"
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

int n = 0;
void print_packet_info(const u_char *packet,struct pcap_pkthdr packet_header) 
{
    printf("Packet capture length: %d :: ", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

void packet_handler(u_char* args,const struct pcap_pkthdr* header,const u_char* packet)
{
	struct ether_header* eth_hdr;
	eth_hdr = (struct ether_header*)packet;
	if(ntohs(eth_hdr->ether_type)==ETHERTYPE_IP)
	{
		
		 //length
		const u_char* ip_header,*tcp_header,*payload,*udp_header;
		const struct iphdr* iph;
		int ethernet_length = 14,ip_length,tcp_length,payload_length,udp_length;

		// ethernet header is 14 bytes( how??!?!? )

		ip_header = packet+ethernet_length;

		ip_length = ((*ip_header)&0x0F);
		ip_length*=4;
		//printf("IP Header length: %d bytes\n",ip_length);
		u_char protocol = *(ip_header+9);

		iph = (const struct iphdr*)ip_header;

		char ad[INET_ADDRSTRLEN],ad1[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,&(iph->saddr),ad,INET_ADDRSTRLEN);
		inet_ntop(AF_INET,&(iph->daddr),ad1,INET_ADDRSTRLEN);
		//printf("Sender IP: %s\n",inet_ntop(AF_INET,&(iph->saddr),ad,INET_ADDRSTRLEN));
		//printf("Receiver IP: %s\n",inet_ntop(AF_INET,&(iph->daddr),ad1,INET_ADDRSTRLEN));

		if(strcmp(ad1,"127.0.0.1")!=0)
		{		
			n++;
			return;		
		}
		printf("%d. (IP) : ",n);
		print_packet_info(packet,*header);
		printf("Sender IP: %s\n",ad);
			printf("Receiver IP: %s\n",ad1);
		
		if(protocol!=IPPROTO_TCP&&protocol!=IPPROTO_UDP)
		{
			printf("Not a tcp/udp packet..ignoring..\n");
		}
		else
		{
			if(protocol==IPPROTO_TCP)
			{
				printf("tcp Packet\n");
				tcp_header = packet+ethernet_length+ip_length;
				tcp_length = ((*(tcp_header+12))&0xF0)>>4;
				tcp_length*=4;
			//	printf("TCP Header length: %d bytes\n",tcp_length);
				payload_length = header->caplen-(ethernet_length+ip_length+tcp_length);
			//	printf("payload length: %d\n",payload_length);
				payload = packet+(ethernet_length+ip_length+tcp_length);
				printf("Trying to print the data: \n");
				const u_char* pnt = payload;
				int cnt = 0;
				while(cnt<payload_length)
				{
					printf("%c",*pnt);
					pnt++;cnt++;
				}
				printf("\n");
			}
			else if(protocol==IPPROTO_UDP)
			{
				const struct udphdr* udph;

				printf("udp packet\n");
				udp_header = packet+ethernet_length+ip_length;
				udph = (const struct udphdr*)udp_header;

				printf("Sender Port: %d\n",(int)udph->source);
				printf("Receiver Port: %d\n",(int)udph->dest);

				udp_length = 8;
			//	printf("UDP Header length: %d bytes\n",udp_length);
				payload_length = header->caplen-(ethernet_length+ip_length+udp_length);
			//	printf("payload length: %d\n",payload_length);
				payload = packet+(ethernet_length+ip_length+udp_length);
				printf("Trying to print the data: \n");
				const u_char* pnt = payload;
				int cnt = 0;
				while(cnt<payload_length)
				{
					printf("%c",*pnt);
					pnt++;cnt++;
				}
				printf("\n\n\n");
			}

		}
	}
	// else if(ntohs(eth_hdr->ether_type)==ETHERTYPE_ARP)
	// {
	// 	printf("ARP Packet\n");
	// }
	//print_packet_info(packet,*header);
	n++;
	if(n==1000)
		exit(0);
}

int main(int argc, char const *argv[])
{
	char* device;
	char error[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	int timeout = 1000;
	device = pcap_lookupdev(error);
	if(device==NULL)
	{
		printf("Error: %s\n",error);exit(0);
	}
	handle = pcap_open_live(device,BUFSIZ,0,timeout,error);
	if(handle==NULL)
	{
		perror("Could not get handle");exit(0);
	}
	pcap_loop(handle,0,packet_handler,NULL);
	return 0;
}