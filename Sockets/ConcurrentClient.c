#include "../cn.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <syslog.h>
#include <sys/select.h>

int ports[4] = {8000,8001,8002,8003};
struct triplet
{
	int a,b,c;
};

union ops
{
	struct triplet t;
	char buffer[50];
};

struct Packet
{
	int ind;
	union ops Opt;
};

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: ./ConcurrentClient [OPTION]\n");
		exit(0);
	}
	int t = atoi(argv[1]);
	struct Packet p;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not open");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(ports[t-1]);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connection failed");
		exit(0);
	}
	while(1)
	{
		switch(t)
		{
			case 1:
			case 3:
				printf("Enter a string\n");
				scanf("%[^\n]s",p.Opt.buffer);
				while(getchar()!='\n');
				break;
			case 2:
			case 4:
				printf("Enter two numbers and one operation\n");
				scanf("%d%d%d",&(p.Opt.t.a),&(p.Opt.t.b),&(p.Opt.t.c));
				break;
		}
		if(send(sfd,&p,sizeof(struct Packet),0)<0)
		{
			perror("sending failed");
			exit(0);
		}
		if(t!=2)
		{
			if(strcmp(p.Opt.buffer,"bye")==0)
				break;
		}
		else if(p.Opt.t.c==-1)
		{
			break;
		}
		if(recv(sfd,&p,sizeof(struct Packet),0)<0)
		{
			perror("Reading failed");
			exit(0);
		}
		else
		{
			if(t!=2)
			printf("String received: %s\n",p.Opt.buffer);
			else
			printf("Result: %d\n",p.ind);
		}
	}
	return 0;
}