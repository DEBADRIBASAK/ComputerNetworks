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

int sent[65536] = {0};
int fd;


struct shmstr
{
	int ports[255];
	int ind;
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
		if(ip->protocol==6) //tcp
		{
			tcp = (struct tcphdr*)(packet+14+(ip->ihl*4));
			int port = ntohs(tcp->source); // caught during 3 way handshaking
			if(!sent[port])
			{
				printf("Sending Port Number: %d\n",port);
				write(fd,&port,sizeof(port));sent[port] = 1;
			}
		}
	}
}

void* func1(void* arg)
{
	pcap_loop(handle,0,packet_handler,NULL);
}

int sfd;

struct shmstr* ptr;
int shmid;
int semid;

void* func2(void* arg)
{
	char c;
	struct sockaddr_in cl_addr;
	int len = sizeof(cl_addr);
	while(1)
	{
		recvfrom(sfd,&c,sizeof(c),0,(struct sockaddr*)&cl_addr,&len);
		printf("Got it!\n");
		for (int i = 0; i < ptr->ind; ++i)
		{
			sendto(sfd,&ptr->ports[i],sizeof(int),0,(struct sockaddr*)&cl_addr,len);
		}
		int k = -1;
		sendto(sfd,&k,sizeof(int),0,(struct sockaddr*)&cl_addr,len);
	}
}




int main(int argc, char const *argv[])
{
	printf("Bufsize: %d\n",BUFSIZ);
	mkfifo("FIFO",O_CREAT|0666);
	fd = open("FIFO",O_WRONLY);
	if(fd<0)
	{
		perror("Could not open");
	}
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8003);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}

	key_t k = ftok(KEYPATH,37);
	shmid = shmget(k,sizeof(struct shmstr),IPC_CREAT|0666);
	semid = semget(k,1,IPC_CREAT|0666);
	ptr = (struct shmstr*)shmat(shmid,NULL,0);


	char error[PCAP_ERRBUF_SIZE];
	handle = pcap_open_live(device,BUFSIZ,0,1000,error);
	if(handle==NULL)
	{
		printf("Could not find handle: %s\n",error);exit(0);
	}
	bpf_u_int32 subnet_mask,ip;
	pcap_lookupnet(device,&ip,&subnet_mask,error);
	struct bpf_program filter;
	char filet_exp[50] = "src port 8000 || src port 8001 || src port 8002";
	pcap_compile(handle,&filter,filet_exp,0,ip);
	pcap_setfilter(handle,&filter);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,func1,NULL);
	pthread_create(&p2,NULL,func2,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}