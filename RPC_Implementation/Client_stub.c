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
#define PATH "."
#define PROJ_ID 37


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
pcap_t *handle;
char error[PCAP_ERRBUF_SIZE];

bpf_u_int32 subnet_mask,ip;
int msqid;

int rsfd;

struct message
{
	long type;
	struct RPC_hdr buff[1];
};

void* func1(void* args)
{
	struct message A;
	while(1)
	{
		if(msgrcv(msqid,&A,sizeof(A.buff),1,0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			struct RPC_hdr *rpc;
			char buffer[1500];
			memset(buffer,0,1500);
			rpc = (struct RPC_hdr*)buffer;
			*rpc = A.buff[0];
			struct sockaddr_in addr1;
			memset(&addr1,0,sizeof(addr1));
			addr1.sin_family = AF_INET;
			addr1.sin_addr.s_addr = inet_addr("127.0.0.8");
			if(sendto(rsfd,buffer,1500,0,(struct sockaddr*)&addr1,sizeof(addr1))<0)
			{
				perror("Could not send");
			}
			else
			{
				printf("Successfully sent\n");
			}
		}
	}
}

void* func2(void* arg)
{
	char buffer[1500];
	while(1)
	{
		if(recvfrom(rsfd,buffer,1500,0,NULL,NULL)<0)
		{
			perror("Could not get");
		}
		else
		{
			struct message A;
			A.type = 3;
			struct RPC_hdr* rpc;
			struct iphdr* ip;
			ip = (struct iphdr*)buffer;
			rpc = (struct RPC_hdr*)(buffer+(ip->ihl*4));
			A.buff[0] = *rpc;
			if(msgsnd(msqid,&A,sizeof(A.buff),0)<0)
			{
				perror("Could not send");
			}
		}
	}
}



int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJ_ID);
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Could not send");exit(0);
	}
	pthread_t p1,p2;
	pthread_create(&p1,NULL,func1,NULL);
	pthread_create(&p2,NULL,func2,NULL);

	// pcap_lookupnet(device,&ip,&subnet_mask,error);
	// handle = pcap_open_live(device,BUFSIZ,0,1000,error);
	
	// if(handle==NULL)
	// {
	// 	printf("Could not find handle: %s\n",error);exit(0);
	// }

	// struct bpf_program filter;
	// char filet_exp[50] = "dest host 127.0.0.2";
	// pcap_compile(handle,&filter,filet_exp,0,ip);
	// pcap_setfilter(handle,&filter);


	rsfd = socket(AF_INET,SOCK_RAW,253);
	if(rsfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	int op = 1;
	//setsockopt(rsfd,IPPROTO_IP,SO_BROADCAST,&op,sizeof(op));


	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}