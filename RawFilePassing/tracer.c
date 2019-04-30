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


void packet_handler(u_char* arg,const struct pcap_pkthdr* header,const u_char* packet)
{
	struct ether_header* eth;
	struct iphdr* ip;
	eth = (struct ether_header*)packet;
	ip = (struct iphdr*)(packet+14);
	if(ntohs(eth->ether_type)==ETHERTYPE_IP)
	{
		printf("Koi Mil gaya!!\n");
		char ad[INET_ADDRSTRLEN],ad1[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,&ip->saddr,ad,INET_ADDRSTRLEN);
		inet_ntop(AF_INET,&ip->daddr,ad1,INET_ADDRSTRLEN);
		printf("Source IP: %s\n",ad);
		printf("Destination IP: %s\n",ad1);
	}
}

int main(int argc, char const *argv[])
{
	char device[10] = "eno1";
	pcap_t *handle;
	char error[PCAP_ERRBUF_SIZE];
	handle = pcap_open_live(device,BUFSIZ,0,1000,error);
	if(handle==NULL)
	{
		printf("ERROR: %s\n",error);exit(0);
	}
	struct bpf_program filter;
	char filter_exp[] = "src host 127.0.0.1 || src host 127.0.0.2";
	bpf_u_int32 subnet_mask,ip;
	pcap_lookupnet(device,&ip,&subnet_mask,error);
	if(pcap_compile(handle,&filter,filter_exp,0,ip)<0)
	{
		perror("Could not compile");exit(0);
	}
	pcap_setfilter(handle,&filter);
	pcap_loop(handle,0,packet_handler,NULL);
	return 0;
}