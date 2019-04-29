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

struct arphdr
{
	uint16_t htype;
	uint16_t ptype;
	uint8_t hlen;
	uint8_t plen;
	uint16_t opcode;
	uint8_t source_mac[6];
	uint8_t source_ip[4];
	uint8_t dest_mac[6];
	uint8_t dest_ip[4];
};

int main(int argc, char const *argv[])
{ 
	int rsfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	char buffer[1500];
	struct ether_header* eth;
	struct arphdr* arp;
	eth = (struct ether_header*)buffer;
	char sender_mac[6],receiver_mac[6];
	// sscanf(sender_mac,"%c:%c:%c:%c:%c:%c","e4:e7:49:0f:cd:31");
	// sscanf(receiver_mac,"%c:%c:%c:%c:%c:%c","ff:ff:ff:ff:ff:ff");
	sender_mac[0] = 0xE4;
	sender_mac[1] = 0xE7;
	sender_mac[2] = 0x49;
	sender_mac[3] = 0x0F;
	sender_mac[4] = 0xCD;
	sender_mac[5] = 0x31;

	receiver_mac[0] = 0xFF;
	receiver_mac[1] = 0xFF;
	receiver_mac[2] = 0xFF;
	receiver_mac[3] = 0xFF;
	receiver_mac[4] = 0xFF;
	receiver_mac[5] = 0xFF;

	memcpy(eth->ether_shost,sender_mac,6);
	memcpy(eth->ether_dhost,receiver_mac,6);

	eth->ether_type = htons(ETHERTYPE_ARP);

	arp = (struct arphdr*)(buffer+14);

	arp->htype = htons(1);
	arp->ptype = 8;
	arp->hlen = 6;
	arp->plen = 4;

	arp->opcode = htons(2);

	memcpy(arp->source_mac,sender_mac,6);
	memcpy(arp->dest_mac,receiver_mac,6);

	arp->source_ip[0] = 172;
	arp->source_ip[1] = 30;
	arp->source_ip[2] = 104;
	arp->source_ip[3] = 95;

	arp->dest_ip[0] = 172;
	arp->dest_ip[1] = 30;
	arp->dest_ip[2] = 104;
	arp->dest_mac[3] = 1;


	struct sockaddr_ll addr;

	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = if_nametoindex("eno1");
	memcpy(addr.sll_addr,receiver_mac,6);
	addr.sll_halen = 6;

	while(1)
	sendto(rsfd,buffer,1500,(struct sockaddr*)&addr,sizeof(addr));

	return 0;
}