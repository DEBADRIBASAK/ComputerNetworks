#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Pair
{
	int location,tourist_spot;
};


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),temp = 1,nsfd;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr,cl_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	int len = sizeof(cl_addr);
	getsockname(sfd,(struct sockaddr*)&cl_addr,&len);
	printf("Addr: %d\nPort: %d\n",(int)cl_addr.sin_addr.s_addr,(int)cl_addr.sin_port);
	struct Pair A;
	printf("Enter tourist spot and pick-up point\n");
	scanf("%d",&(A.tourist_spot));
	scanf("%d",&(A.location));
	while(getchar()!='\n');
	if(send(sfd,&A,sizeof(struct Pair),0)<0)
	{
		perror("Could not send");
	}
	else
	{
		printf("Successfully sent..\n");
		int res;
		recv(sfd,&res,sizeof(res),0);
		if(!res)
		{
			printf("Error occurred\n");exit(0);
		}
		else
		{
			printf("Successful..\n");
		}
		sfd = socket(AF_INET,SOCK_STREAM,0);
		setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
		if(bind(sfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
		{
			perror("could not bind");exit(0);
		}
		if(listen(sfd,5)<0)
		{
			perror("Could not listen");exit(0);
		}
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			exit(0);
		}
		char buffer[50];int sz;
		struct pollfd pfd[2];
		pfd[0].fd = nsfd;
		pfd[0].events = POLLIN;
		pfd[0].revents = 0;
		pfd[1].fd = 0;
		pfd[1].events = POLLIN;
		pfd[1].revents = 0;
		int r;
		while(1)
		{
			r = poll(pfd,(nfds_t)2,-1);
			if(r>0)
			{
				if(pfd[0].revents&POLLIN)
				{
					sz = recv(nsfd,buffer,50,0);
					if(sz==0)
						exit(0);
					buffer[sz] = '\0';
					
					printf("From Taxi: %s\n",buffer);
					if(strcmp(buffer,"Sorry bro :( ... Car got damaged! You go on foot!")==0)
					{
						printf("Arey! Ab chalke jaana padega! :(\n");
						printf("Walking..\nWalking..\nWalking......");
						exit(0);
					}
				}
				if(pfd[1].revents&POLLIN)
				{
					scanf("%[^\n]s",buffer);
					while(getchar()!='\n');
					send(nsfd,buffer,strlen(buffer),0);
				}
				
			}
		}
	}
	return 0;
}