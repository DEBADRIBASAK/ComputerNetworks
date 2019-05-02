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

struct sockaddr_in cl_addr;


int main(int argc, char const *argv[])
{
	key_t k1 = ftok(PATH,PROJ_ID);
	msqid = msgget(k1,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Could not send");exit(0);
	}
	struct message A,B;
	while(1)
	{
		if(msgrcv(msqid,&A,sizeof(A.buff),2,0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			if(strcmp(A.buff[0].func_name,"add")==0)
			{
				B.buff[0].result = A.buff[0].arg.op1+A.buff[0].arg.op2;
			}
			else
			{
				B.buff[0].result = A.buff[0].arg.op1*A.buff[0].arg.op2;
			}
			B.type = 4;
			if(msgsnd(msqid,&B,sizeof(B.buff),0)<0)
			{
				perror("Could not send");
			}
		}
	}
	return 0;
}