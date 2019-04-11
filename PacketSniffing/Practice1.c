#include "../cn.h"
#include <pcap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main(int argc, char const *argv[])
{
	char *device;
	char ip[INET_ADDRSTRLEN];
	char subnet_mask[INET_ADDRSTRLEN];
	bpf_u_int32 ip_raw;
	bpf_u_int32 subnet_raw;
	char error[PCAP_ERRBUF_SIZE];
	int lookup_return_code;
	struct in_addr addr;
	device = pcap_lookupdev(error);
	if(device==NULL)
	{
		printf("error: %s\n",error);exit(0);
	}
	lookup_return_code = pcap_lookupnet(device,&ip_raw,&subnet_raw,error);
	if(lookup_return_code<0)
	{
		printf("Could not lookup: %s\n",error);exit(0);
	}
	else
	{
		addr.s_addr = ip_raw;
		strcpy(ip,inet_ntoa(addr));
		addr.s_addr = subnet_raw;
		strcpy(subnet_mask,inet_ntoa(addr));
		printf("Device: %s\nIP: %s\nSubnet Mask: %s\n",device,ip,subnet_mask);
	}
	return 0;
}