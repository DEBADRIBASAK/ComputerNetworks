#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),ASsfd,nASsfd,flg = 1;
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");exit(0);
	}
	char buffer[100];
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(sfd,&readset);
	FD_SET(0,&readset);
	struct sockaddr_in cl_addr;int len = sizeof(cl_addr),r;
	getsockname(sfd,(struct sockaddr*)&cl_addr,&len);
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			if(!flg)
			{
				if(FD_ISSET(nASsfd,&readset))
				{
					recv(nASsfd,buffer,100,0);
					if(strcmp(buffer,"Move to actual server")==0)
					{
						printf("Moving to actual server..\n");
						flg = 1;
						FD_CLR(nASsfd,&readset);
					}
					else
					{
						printf("Received: %s\n",buffer);
					}
				}
				else
					FD_SET(nASsfd,&readset);
			}
			if(FD_ISSET(sfd,&readset))
			{
				printf("sfd selected\n");
				recv(sfd,buffer,100,0);
				if(strcmp(buffer,"Server is down")==0)
				{
					printf("Server is down...\n");
					ASsfd = socket(AF_INET,SOCK_STREAM,0);
					setsockopt(ASsfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
					if(bind(ASsfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
						perror("Could not bind");
					else if(listen(ASsfd,10)<0)
						perror("Could not listen");
					else
					{
						printf("Listening..\n");
						nASsfd = accept(ASsfd,NULL,NULL);
						if(nASsfd>0)
						{
							printf("Success...\n");
						}
						flg = 0;
						FD_SET(nASsfd,&readset);
						printf("set the nASsfd\n");
					}
				}
				else if(strcmp(buffer,"Server is Up")==0)
				{
					printf("Message from actual server...It is working now\n");
					FD_CLR(nASsfd,&readset);
					printf("unset the nASsfd\n");
				}
				else
				{
					printf("Received: %s\n",buffer);
				}
			}
			else
				FD_SET(sfd,&readset);
			if(FD_ISSET(0,&readset))
			{
				scanf("%[^\n]s",buffer);
				while(getchar()!='\n');
				if(flg)
				{
					printf("Sending to sfd\n");
					send(sfd,buffer,100,0);
					//recv(sfd,buffer,100,0);
					//printf("Received: %s\n",buffer);
				}
				else
				{
					printf("Sending to nASsfd\n");
					send(nASsfd,buffer,100,0);
					//recv(nASsfd,buffer,100,0);
					//printf("Received: %s\n",buffer);
				}
			}
			else
				FD_SET(0,&readset);
		}
	}
	return 0;
}