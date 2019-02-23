#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define NO_OF_REQUESTS 10

struct Pair
{
	int req,new_port_no;
};

int main(int argc, char const *argv[])
{
	int sfd,sfd2,nsfd[255],ind = 0;
	struct sockaddr_in addr,addr1[3];//,addr2,addr3;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);

	addr1[0].sin_family = AF_INET;
	addr1[0].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1[0].sin_port = htons(8001);

	addr1[1].sin_family = AF_INET;
	addr1[1].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1[1].sin_port = htons(8002);

	addr1[2].sin_family = AF_INET;
	addr1[2].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1[2].sin_port = htons(8003);

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Binding error");
		exit(0);
	}
	if(listen(sfd,NO_OF_REQUESTS)<0)
	{
		perror("Listening error");
		exit(0);
	}
	while(1)
	{
		nsfd[ind] = accept(sfd,NULL,NULL);
		if(nsfd[ind]<0)
		{
			perror("Could not accept request");
			exit(0);
		}
		else
		{
			sfd2 = socket(AF_INET,SOCK_STREAM,0);
			if(sfd2<0)
			{
				perror("Socket 2 not created");
				exit(0);
			}
			struct Pair req;
			if(recv(nsfd[ind],&req,sizeof(struct Pair),0)<0)
			{
				perror("Reading error");
			}
			else
			{
				printf("Request: %d\n",req.req);
				struct sockaddr_in cl_addr;
				int sz = sizeof(cl_addr);
				getpeername(nsfd[ind],(struct sockaddr*)&cl_addr,&sz);
				cl_addr.sin_port = htons(req.new_port_no);
				if(connect(sfd2,(struct sockaddr*)&addr1[req.req-1],sizeof(addr1[req.req-1]))<0)
				{
					perror("Connection failed");
					exit(0);
				}
				send(sfd2,(struct sockaddr_in*)&cl_addr,sizeof(cl_addr),0);
				close(sfd2);
				close(nsfd[ind]);
				//dup2(nsfd[ind],sfd2);
				//ind++;
				printf("Success..\n");
			}
		}
	}
	return 0;
}