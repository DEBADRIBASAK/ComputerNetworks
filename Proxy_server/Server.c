#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd;
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	int op = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&op,sizeof(op));
	struct sockaddr_in addr,cl_addr;int len = sizeof(cl_addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	char buffer[1024];int sz;
	while(1)
	{
		nsfd = accept(sfd,(struct sockaddr*)&cl_addr,&len);
		if(nsfd<0)
		{
			perror("Could not accept");
		}
		else
		{
			if((sz = recv(nsfd,buffer,1024,0))<0)
			{
				perror("Could not read");
			}
			else
			{
				buffer[sz] = '\0';
				int i = 0,j;
				while(buffer[i]!=':')
					i++;
				i+=2;
				j = i;
				while(j<sz&&buffer[j]!='/')
					j++;
				int sfd1 = socket(AF_INET,SOCK_STREAM,0);
				struct sockaddr_in addr1;
				buffer[j] = '\0';
				printf("Address: %s\n",buffer+i+1);
				addr1.sin_family = AF_INET;
				addr1.sin_addr.s_addr = inet_addr(buffer+i+1);
				addr1.sin_port = htons(80);
				if(connect(sfd1,(struct sockaddr*)&addr1,sizeof(addr1))<0)
				{
					perror("Could not connect");
				}
				else
				{
					char buf[65536];
					sz = recv(sfd1,buf,65536,0);
					if(send(nsfd,buf,sz,0)>0)
						printf("Success...\n");
				}

			}
		}
	}
	return 0;
}