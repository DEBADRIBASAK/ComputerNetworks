// 5th question
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8000
fd_set readset;
int sfd[255],clone[255],ind = 0,fsfd;
pid_t p[255];

struct Services
{
	char name[20];
	char description[100];
};



int main(int argc, char const *argv[])
{
	fsfd = socket(AF_INET,SOCK_DGRAM,0);
	FD_SET(0,&readset);
	int maxfp1 = 0;
	char buffer[255];
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(PORT);
	if(bind(fsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
	}
	FD_SET(fsfd,&readset);
	maxfp1 = (maxfp1<fsfd)?fsfd:maxfp1;
	int r,sz,portno,stoind;
	while(1)
	{
		r = select(maxfp1+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			stoind = ind;
			if(FD_ISSET(0,&readset))
			{
				scanf("%d",&portno);
				scanf("%s",buffer);
				printf("portno: %d path: %s\n",portno,buffer);
				sfd[ind] = socket(AF_INET,SOCK_STREAM,0);
				if(sfd[ind]<0)
				{
					perror("Could not create socket");
				}
				int temp;
				if(setsockopt(sfd[ind],SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(int))<0)
				{
					perror("Could not reuse address");
				}
				addr.sin_port = htons(portno);
				if(bind(sfd[ind],(struct sockaddr*)&addr,sizeof(addr))<0)
				{
					perror("Could not bind");
				}
				else if(listen(sfd[ind],10)<0)
				{
					perror("Listening error");
				}
				else
				{
					printf("Successful\n");
					//int temp = 1;
					clone[ind] = ind+10;
					dup2(sfd[ind],clone[ind]);
					char num[2];
					sprintf(num,"%d",ind);
					p[ind] = fork();
					if(p[ind]<0)
					{
						perror("Forking error");
					}
					else if(p[ind]==0)
					{
						//char prt[10];
						//sprintf(prt,"%d",portno);
						printf("%s\n",buffer);
						execlp(buffer,buffer,num,NULL);
					}
					else
					{
						printf("portno: %d path: %s\n",portno,buffer);
						FD_SET(sfd[ind],&readset);
						maxfp1 = (maxfp1<sfd[ind])?sfd[ind]:maxfp1;
						ind++;
					}
				}
			}
			else
				FD_SET(0,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					printf("Request arrived for Service-%d\n",i+1);
					kill(p[i],SIGUSR1);
				}
				else
					FD_SET(sfd[i],&readset);
			}
			if(FD_ISSET(fsfd,&readset))
			{
				// do something
			}
			else
				FD_SET(fsfd,&readset);
		}
		sleep(1);
	}
	return 0;
}