#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

int sfd1[255],ind = 0,map[255];
struct Message
{
	struct sockaddr_in cl_addr;
	int f;
	int ind;
};

int find(int i)
{
	for(int j=0;j<ind;j++)
		if(map[j]==i)
			return j;
}

int main(int argc, char const *argv[])
{
	fd_set readset;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8001);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	else if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	int nsfd = accept(sfd,NULL,NULL);

	if(nsfd<0)
	{
		perror("Could not accept");exit(0);
	}
	else{
		printf("Accepted.. nsfd = %d\n",nsfd);
	}
	FD_ZERO(&readset);
	FD_SET(nsfd,&readset);
	int r,sz,stoind;
	struct Message A;
	char buffer[100];
	while(1)
	{

		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			//printf("Selected..\n");
			stoind = ind;
			//printf("stoind = %d\n",stoind);
			if(FD_ISSET(nsfd,&readset))
			{
				if((sz = recv(nsfd,&A,sizeof(A),0))<0)
					perror("Could not receive");
				else if(sz==0)
					continue;
				else
				{
					//printf("Received..\n");
					if(A.f==0)
					{
						int j = find(A.ind);
						strcpy(buffer,"Move to actual server");
						FD_CLR(sfd1[j],&readset);
						send(sfd1[j],buffer,100,0);
						close(sfd1[j]);
						sfd1[j] = -1;
						continue;
					}
					printf("Reading client ID\n");
					sfd1[ind] = socket(AF_INET,SOCK_STREAM,0);
					if(connect(sfd1[ind],(struct sockaddr*)&A.cl_addr,sizeof(A.cl_addr))<0)
						perror("Could not connect");
					else
					{
						printf("Successfully connected...\n");
						FD_SET(sfd1[ind],&readset);
						map[ind] = A.ind;
						ind++;
					}
				}
			}	
			else
				FD_SET(nsfd,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(sfd1[i]==-1)
					continue;
				if(FD_ISSET(sfd1[i],&readset))
				{
					
					if(recv(sfd1[i],buffer,100,0)<=0)
						continue;

					printf("Reading...: %s\n", buffer);
					for(int j=0;j<strlen(buffer);j++)
						if(buffer[j]>='a'&&buffer[j]<='z')
							buffer[j] = buffer[j]-'a'+'A';
					send(sfd1[i],buffer,100,0);
				}
				else
				{
					//printf("Not selected\n");
					FD_SET(sfd1[i],&readset);
				}
			}
		}
	}

	return 0;
}