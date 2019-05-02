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


int add(int a,int b)
{
	struct RPC_args arg;
	arg.op1 = a;
	arg.op2 = b;
	struct message A,B;
	A.type = 1;
	strcpy(A.buff[0].func_name,"add");
	A.buff[0].req = 1;
	A.buff[0].result = 0; //
	A.buff[0].arg = arg;
	if(msgsnd(msqid,&A,sizeof(A.buff),0)<0)
	{
		perror("Could not send");
	}
	else if(msgrcv(msqid,&B,sizeof(B.buff),3,0)<0)
	{
		perror("Could not receive");
	}
	else
	{
		return B.buff[0].result;
	}
	return -1;
}
int mul(int a,int b)
{
	struct RPC_args arg;
	arg.op1 = a;
	arg.op2 = b;
	struct message A,B;
	A.type = 1;
	strcpy(A.buff[0].func_name,"mul");
	A.buff[0].req = 1;
	A.buff[0].result = 0; //
	A.buff[0].arg = arg;
	if(msgsnd(msqid,&A,sizeof(A.buff),0)<0)
	{
		perror("Could not send");
	}
	else if(msgrcv(msqid,&B,sizeof(B.buff),3,0)<0)
	{
		perror("Could not receive");
	}
	else
	{
		return B.buff[0].result;
	}
	return -1;
}

int main(int argc, char const *argv[])
{
	key_t k1 = ftok(PATH,PROJ_ID);
	msqid = msgget(k1,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Could not send");exit(0);
	}

	int k;
	while(1)
	{
		printf("Choice: 1. for add 2. for mul\n");
		scanf("%d",&k);
		while(getchar()!='\n');
		int op1,op2;
		printf("Enter two operands\n");
		scanf("%d%d",&op1,&op2);
		while(getchar()!='\n');
		if(k==1)
		printf("Result: %d\n",add(op1,op2));
		else
		printf("Result: %d\n",mul(op1,op2));
	}

	return 0;
}