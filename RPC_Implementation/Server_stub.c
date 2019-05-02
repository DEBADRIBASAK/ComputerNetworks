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


int msqid;

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
struct message
{
	long type;
	struct RPC_hdr buff[1];
};

struct sockaddr_in cl_addr,sto;

int rsfd;
char ad[INET_ADDRSTRLEN];
void* func1(void* arg)
{
	char buffer[1500];
	int len = sizeof(cl_addr);
	if(recvfrom(rsfd,buffer,1500,0,(struct sockaddr*)&cl_addr,&len)<0)
	{
		perror("Could not receive");
	}
	else
	{
		printf("IP: %s\n",inet_ntoa(cl_addr.sin_addr));

		struct RPC_hdr* rpc;
		struct iphdr* ip;
		ip = (struct iphdr*)buffer;
		
		printf("IP2: %s\n",inet_ntop(AF_INET,&ip->saddr,ad,INET_ADDRSTRLEN));
		rpc = (struct RPC_hdr*)(buffer+(ip->ihl*4));
		struct message A;
		A.type = 2;
		A.buff[0] = *rpc;
		if(msgsnd(msqid,&A,sizeof(A.buff),0)<0)
		{
			perror("Could not send");
		}
		else
		{
			printf("Sent successfully\n");
		}
	}
}

void* func2(void* arg)
{
	struct message A,B;
	char buffer[1500];
	while(1)
	{
		if(msgrcv(msqid,&A,sizeof(A.buff),4,0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			memset(buffer,0,1500);
			struct RPC_hdr* rpc;
			rpc = (struct RPC_hdr*)(buffer);
			rpc->req = 2;
			rpc->result = A.buff[0].result;
			struct sockaddr_in addr;
			memset(&addr,0,sizeof(addr));
			printf("IP3: %s\n",ad);
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr(ad);
			if(sendto(rsfd,buffer,1500,0,(struct sockaddr*)&addr,sizeof(addr))<0)
			{
				perror("Could not send here");
				printf("IP: %s\n",inet_ntoa(addr.sin_addr));
			}
			else
			{
				printf("Sent to client");
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.8");
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