#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

fd_set readset;
int sfd[10],ind = 0;

int main(int argc, char const *argv[])
{
	FD_ZERO(&readset);
	FD_SET(0,&readset);
	char buffer[255];int sz,r,port,stoind,f;
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			stoind = ind;f = 0;
			if(FD_ISSET(0,&readset))
			{
				scanf("%[^\n]s",buffer);
				while(getchar()!='\n');
				if(strcmp(buffer,"port")==0)
				{
					scanf("%d",&port);
					while(getchar()!='\n');
					sfd[ind] = socket(AF_INET,SOCK_STREAM,0);
					struct sockaddr_in addr;
					addr.sin_family = AF_INET;
					addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
					addr.sin_port = htons(port);
					if(connect(sfd[ind],(struct sockaddr*)&addr,sizeof(addr))<0)
					{
						perror("Could not connect");
					}
					else
					{
						FD_SET(sfd[ind++],&readset);
						printf("Connected with server at port no: %d\n",port);
					}
				}
				else
				{
					scanf("%d",&port); //server number
					while(getchar()!='\n');
					send(sfd[port-1],buffer,strlen(buffer),0);
					if(strcmp(buffer,"X")==0)
						f = 1;
				}
			}
			else
				FD_SET(0,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					sz = recv(sfd[i],buffer,255,0);
					buffer[sz] = '\0';
					printf("Received: %s\n",buffer);
				}
				else
					FD_SET(sfd[i],&readset);
			}
			if(f)
				break;
		}
	}
	return 0;
}