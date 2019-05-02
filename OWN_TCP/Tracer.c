#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <pcap.h>
#include <pthread.h>
#include "Protocol.h"
#define KEYPATH "."
struct RPC_args
{
	int op1,op2;//,op;
};

struct RPC_hdr
{
	char func_name[10];
	int req;
	int result;
	struct RPC_args arg;
};

char device[10] = "lo";

pcap_t * handle;

void packet_handler(u_char *args,const struct pcap_pkthdr* header,const u_char* packet)
{
	struct ether_header* ether;
	struct iphdr* ip;
	struct tcphdr* tcp;
	ether = (struct ether_header*)packet;
	if(ntohs(ether->ether_type)==ETHERTYPE_IP)
	{
		printf("***************************\n\n");
		ip = (struct iphdr*)(packet+14);
		if(ip->protocol!=253)
			return;
		printf("IP Version: %d\n",ip->version);
		printf("IP Length: %d\n",ip->ihl);
		printf("IP Protocol: %d\n",ip->protocol);
		char ad[INET_ADDRSTRLEN];
		printf("Sender IP: %s\n",inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN));
		printf("Receiver IP: %s\n",inet_ntop(AF_INET,&(ip->daddr),ad,INET_ADDRSTRLEN));
		struct MyTCP *tcp;
		tcp = (struct MyTCP*)(packet+14+(ip->ihl*4));
		printf("Source PORT: %d\n",tcp->source);
		printf("Destination PORT: %d\n",tcp->dest);
		printf("Connection Request Flag: %d\n",tcp->con_req);
		printf("Connection Accepted Flag: %d\n",tcp->con_acc);
		printf("Final One: %d\n",tcp->fin_ack);
		printf("Sequence NO: %d\n",tcp->rec_seq);
		printf("Data Flag: %d\n",tcp->data_flg);
		printf("***************************\n\n");
	}
}








int main(int argc, char const *argv[])
{
	char error[PCAP_ERRBUF_SIZE];
	handle = pcap_open_live(device,BUFSIZ,0,1000,error);
	if(handle==NULL)
	{
		printf("Could not find handle: %s\n",error);exit(0);
	}
	//bpf_u_int32 subnet_mask,ip;
	//pcap_lookupnet(device,&ip,&subnet_mask,error);
	//struct bpf_program filter;
	//char filet_exp[50] = "src port 8000 || src port 8001 || src port 8002";
	//pcap_compile(handle,&filter,filet_exp,0,ip);
	//pcap_setfilter(handle,&filter);
	pcap_loop(handle,0,packet_handler,NULL);
	return 0;
}