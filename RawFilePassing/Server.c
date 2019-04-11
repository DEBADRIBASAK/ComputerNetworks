#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
struct Message
{
	char addr[INET_ADDRSTRLEN];
	char filename[255];
};
int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	struct Message A;int nsfd;
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
		}
		else
		{
			if(recv(nsfd,&A,sizeof(A),0)<0)
			{
				perror("Could not receive");
			}
			else
			{
				pid_t c;
				int rsfd = socket(AF_INET,SOCK_RAW,253);
				if(rsfd<0)
				{
					perror("Raw socket not created");
				}
				else
				{
					struct sockaddr_in addr2;
					memset(&addr2,0,sizeof(addr2));
					addr2.sin_family = AF_INET;
					addr2.sin_addr.s_addr = inet_addr(A.addr);
					if(connect(rsfd,(struct sockaddr*)&addr2,sizeof(addr2))<0)
					{
						perror("Could not connect");continue;
					}
					int fd = open(A.filename,O_RDONLY);
					if(fd<0)
					{
						perror("Could not open");
					}
					else
					{
						c = fork();
						if(c<0)
						{
							perror("Forking error");
						}
						else if(c>0)
						{
							close(rsfd);close(fd);
						}
						else
						{
							dup2(fd,0);dup2(rsfd,1);
							execl("./S2","./S2",NULL);
						}
					}
				}
			}
		}
	}
	return 0;
}