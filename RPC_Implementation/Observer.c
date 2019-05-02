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
		ip = (struct iphdr*)(packet+14);
		if(ip->protocol!=253)
			return;
		printf("IP Version: %d\n",ip->version);
		printf("IP Length: %d\n",ip->ihl);
		printf("IP Protocol: %d\n",ip->protocol);
		char ad[INET_ADDRSTRLEN];
		printf("Sender IP: %s\n",inet_ntop(AF_INET,&(ip->saddr),ad,INET_ADDRSTRLEN));
		printf("Receiver IP: %s\n",inet_ntop(AF_INET,&(ip->daddr),ad,INET_ADDRSTRLEN));
		if(ip->protocol==253)
		{
			struct RPC_hdr* rpc;
			rpc = (struct RPC_hdr*)(packet+14+(ip->ihl*4));
			if(rpc->req==1)
			{
				printf("%s :\nOp1: %d\nOp2: %d\n",rpc->func_name,rpc->arg.op1,rpc->arg.op2);
			}
			else
			{
				printf("Result: %d\n",rpc->result);
			}
		}
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