#include<stdio.h>	//for printf
#include<string.h> //memset
#include<sys/socket.h>	//for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header
#include<arpa/inet.h>
#include<pthread.h>
#include<pcap.h>
#include<unistd.h>

int sd;

struct psd_tcp {
	struct in_addr src;
	struct in_addr dst;
	unsigned char pad;
	unsigned char proto;
	unsigned short tcp_len;
	struct tcphdr tcp;
};

unsigned short in_cksum(unsigned short *addr, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&answer) = *(unsigned char *) w;
		sum += answer;
	}
	
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

unsigned short in_cksum_tcp(int src, int dst, unsigned short *addr, int len)
{
	struct psd_tcp buf;
	u_short ans;

	memset(&buf, 0, sizeof(buf));
	buf.src.s_addr = src;
	buf.dst.s_addr = dst;
	buf.pad = 0;
	buf.proto = IPPROTO_TCP;
	buf.tcp_len = htons(len);
	memcpy(&(buf.tcp), addr, len);
	ans = in_cksum((unsigned short *)&buf, 12 + len);
	return (ans);
}

void send_syn()
{
	struct ip ip;
	struct tcphdr tcp;
	const int on = 1;
	struct sockaddr_in sin;

	u_char *packet;
	packet = (u_char *)malloc(60);
	

	ip.ip_hl = 0x5;
	ip.ip_v = 0x4;
	ip.ip_tos = 0x0;
	ip.ip_len = sizeof(struct ip) + sizeof(struct tcphdr); 
	ip.ip_id = htons(12830);
	ip.ip_off = 0x0;
	ip.ip_ttl = 64;
	ip.ip_p = IPPROTO_TCP;
	ip.ip_sum = 0x0;
	ip.ip_src.s_addr = inet_addr("192.168.43.16");
	ip.ip_dst.s_addr = inet_addr("192.168.43.40");
	ip.ip_sum = in_cksum((unsigned short *)&ip, sizeof(ip));
	memcpy(packet, &ip, sizeof(ip));

	tcp.th_sport = htons(8081);
	tcp.th_dport = htons(8080);
	tcp.th_seq = htonl(12345);
	tcp.th_off = sizeof(struct tcphdr) / 4;
	tcp.th_flags = TH_SYN;
	tcp.th_win = htons(32768);
	tcp.th_sum = 0;
	tcp.th_sum = in_cksum_tcp(ip.ip_src.s_addr, ip.ip_dst.s_addr, (unsigned short *)&tcp, sizeof(tcp));
	memcpy((packet + sizeof(ip)), &tcp, sizeof(tcp));
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip.ip_dst.s_addr;

	if (sendto(sd, packet, 60, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
}
void send_syn_ack(int s_seq)
{
	struct ip ip;
	struct tcphdr tcp;
	const int on = 1;
	struct sockaddr_in sin;

	u_char *packet;
	packet = (u_char *)malloc(60);
	

	ip.ip_hl = 0x5;
	ip.ip_v = 0x4;
	ip.ip_tos = 0x0;
	ip.ip_len = sizeof(struct ip) + sizeof(struct tcphdr); 
	ip.ip_id = htons(12831);
	ip.ip_off = 0x0;
	ip.ip_ttl = 64;
	ip.ip_p = IPPROTO_TCP;
	ip.ip_sum = 0x0;
	ip.ip_src.s_addr = inet_addr("192.168.43.16");
	ip.ip_dst.s_addr = inet_addr("192.168.43.40");
	ip.ip_sum = in_cksum((unsigned short *)&ip, sizeof(ip));
	memcpy(packet, &ip, sizeof(ip));

	tcp.th_sport = htons(8081);
	tcp.th_dport = htons(8080);
	tcp.th_seq = htonl(0x131123 + 1);

	tcp.th_ack = htonl(s_seq + 1);
	tcp.th_off = sizeof(struct tcphdr) / 4;
	tcp.th_flags = TH_ACK;
	tcp.th_win = htons(32768);
	tcp.th_sum = 0;
	tcp.th_sum = in_cksum_tcp(ip.ip_src.s_addr, ip.ip_dst.s_addr, (unsigned short *)&tcp, sizeof(tcp));
	memcpy((packet + sizeof(ip)), &tcp, sizeof(tcp));
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip.ip_dst.s_addr;

	if (sendto(sd, packet, 60, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
}

void raw_packet_receiver(u_char *udata, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	struct ip *ip;
	struct tcphdr *tcp;
	u_char *ptr;
	int l1_len = (int)udata;
	int s_seq;

	ip = (struct ip *)(packet + l1_len);
	tcp = (struct tcphdr *)(packet + l1_len + sizeof(struct ip));

	printf("%d\n", l1_len);

	printf("a packet came, ack is: %d\n", ntohl(tcp->th_ack));
	printf("a packet came, seq is: %u\n", ntohl(tcp->th_seq));
	s_seq = ntohl(tcp->th_seq);

	send_syn_ack(s_seq);

	sleep(100);
}

void *pth_capture_run(void *arg)
{
	pcap_t *pd;
	char *filter = "dst host 192.168.43.40 and ip";
	char *dev = "wlp1s0";
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32	netp;
	bpf_u_int32	maskp;
	struct bpf_program	fprog;					/* Filter Program	*/
	int dl = 0, dl_len = 0;

	if ((pd = pcap_open_live(dev, 1514, 1, 500, errbuf)) == NULL) {
		fprintf(stderr, "cannot open device %s: %s\n", dev, errbuf);
		exit(1);
	}

	pcap_lookupnet(dev, &netp, &maskp, errbuf);
	pcap_compile(pd, &fprog, filter, 0, netp);
	if (pcap_setfilter(pd, &fprog) == -1) {
		fprintf(stderr, "cannot set pcap filter %s: %s\n", filter, errbuf);
		exit(1);
	}
	pcap_freecode(&fprog);
	dl = pcap_datalink(pd);
	
	switch(dl){
		case 1:
			dl_len = 14;
			break;
		default:
			dl_len = 14;
			break;
	}

	if (pcap_loop(pd, -1, raw_packet_receiver, (u_char *)dl_len) < 0) {
		fprintf(stderr, "cannot get raw packet: %s\n", pcap_geterr(pd));
		exit(1);
	}
}

void *run(void *arg)
{
	struct ip ip;
	struct tcphdr tcp;
	const int on = 1;
	struct sockaddr_in sin;


	if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("raw socket");
		exit(1);
	}

	if (setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	send_syn();
	
}

int main(int argc, char **argv)
{
	pthread_t tid_pr;

	if (pthread_create(&tid_pr, NULL, pth_capture_run, NULL) != 0) {
		fprintf(stderr, "cannot create raw packet reader: %s\n", strerror(errno));
		exit(1);
	}
	printf("raw packet reader created, waiting 1 seconds for packet reader thread to settle down...\n");
	sleep(1);

	run(NULL);	

	pthread_join(tid_pr, NULL);
	return 0;
}
