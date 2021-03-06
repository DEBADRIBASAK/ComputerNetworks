#include "../cn.h"
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int n = 0;
void print_packet_info(const u_char *packet,struct pcap_pkthdr packet_header) 
{
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

void packet_handler(u_char* args,const struct pcap_pkthdr* header,const u_char* packet)
{
	print_packet_info(packet,*header);
	n++;
	if(n==20)
		exit(0);
}

int main(int argc, char const *argv[])
{
	char* device;
	char error[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	int timeout = 10000;
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