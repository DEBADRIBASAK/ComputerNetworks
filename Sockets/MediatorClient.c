#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Pair
{
	int req,new_port_no;
};

int main(int argc, char const *argv[])
{
	if(argc<3)
	{
		printf("Usage: ./MediatorClient [REQUEST] [NEW_PORT_NO]\n");
		exit(0);
	}
	struct Pair p;
	p.req = atoi(argv[1]);;
	p.new_port_no =atoi(argv[2]);
	int sfd,sfd2;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	sfd2 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr,self_addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	self_addr.sin_family = AF_INET;
	self_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	self_addr.sin_port = htons(p.new_port_no);
	int sz = sizeof(self_addr);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	else
	{
		send(sfd,&p,sizeof(struct Pair),0);
		int t = 1;
	}
	if(bind(sfd2,(struct sockaddr*)&self_addr,sizeof(self_addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(sfd2,2)<0)
	{
		perror("Could not listen");
		exit(0);
	}
	int nsfd = accept(sfd2,NULL,NULL);
	char buffer[50];int sz1;
	while(1)
	{
		printf("Enter one string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(nsfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
			exit(0);
		}
		if(strcmp(buffer,"bye")==0)
			exit(0);
		if((sz1 = recv(nsfd,buffer,50,0))<0)
		{
			perror("Could not receive");
			exit(0);
		}
		else
		{
			buffer[sz1] = '\0';
			printf("Received: %s\n",buffer);
		}
	}
	return 0;
}