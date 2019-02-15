#include "../cn.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <syslog.h>
#include <sys/select.h>
#define NO_OF_REQUEST 10

int ports[3] = {8000,8001,8002};
char* operations[3] = {"change_to_upper_case","mathematics","encryption"};

int main(int argc, char const *argv[])
{
	struct sockaddr_in addr[3];
	int sfd[3],nsfd;
	for(int i=0;i<3;i++){
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);
		if(sfd[i]<0)
		{
			perror("socket not created");
			exit(0);
		}
		addr[i].sin_family = AF_INET;
		addr[i].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr[i].sin_port = htons(ports[i]);
		if(bind(sfd[i],(struct sockaddr*)&addr[i],sizeof(addr[i]))<0)
		{
			perror("Could not bind!");
			exit(0);
		}
		listen(sfd[i],NO_OF_REQUEST);
	}
	fd_set readset;int m = -1;
	for(int i=0;i<3;i++)
	{
		m = (m<sfd[i])?sfd[i]:m;
		FD_SET(sfd[i],&readset);
	}
	int r;pid_t p;
	while(1)
	{
		printf("Waiting...\n");
		r = select(m+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			for(int i=0;i<3;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					nsfd = accept(sfd[i],NULL,NULL);
					if(nsfd<0)
					{
						perror("Accept failed!");
					}
					else
					{
						p = fork();
						if(p<0)
						{
							perror("Fork failed!");
						}
						else if(p>0)
						{
							close(nsfd);
							printf("Operation: %s\n",operations[i]);
						}
						else
						{
							close(sfd[i]);
							dup2(nsfd,0);
							dup2(nsfd,1);
							char* arg[1]= {NULL};
							execv(operations[i],arg);
						}
					}
				}
				else
				{
					FD_SET(sfd[i],&readset);
				}
			}
		}
	}
	return 0;
}