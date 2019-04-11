#include "../cn.h"
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

int main(int argc, char const *argv[])
{
	char *device;
	char error[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	const u_char* packet;
	struct pcap_pkthdr packet_header;
	int packet_count_limit = 1;
	int timeout = 10000;
	device = pcap_lookupdev(error);
	if(device==NULL)
	{
		printf("Error: %s\n",error);exit(0);
	}
	else
	{
		printf("Success: %s\n",device);
	}
	handle = pcap_open_live(device,BUFSIZ,packet_count_limit,timeout,error);
	if(handle==NULL)
	{
		perror("Could not get handle");exit(0);
	}
	else
	{
		printf("Handle obtained\n");
	}
	packet = pcap_next(handle,&packet_header);
	if(packet==NULL)
	{
		printf("No packet found\n");exit(0);
	}
	else
	{
		print_packet_info(packet,packet_header);
	}
	return 0;
}