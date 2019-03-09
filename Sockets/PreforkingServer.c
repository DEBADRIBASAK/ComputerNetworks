#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int sfd,nsfd;
int requests[3] = {0};
void sig_handler(int alrm)
{
	printf("Signal Received\n");
}
int main()
{
	signal(SIGUSR1,sig_handler);
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(sfd,5)<0)
	{
		perror("Listening error");
		exit(0);
	}
	pid_t p;
	for(int i=0;i<3;i++)
	{
		p = fork();
		if(p<0)
		{
			perror("Forking error!");
			exit(0);
		}
		else if(p==0)
		{
			int num = 0;
			while(num<1)
			{
				nsfd = accept(sfd,NULL,NULL);
				num++;
			}
			kill(getppid(),SIGUSR1);
			char buffer[50];
			sprintf(buffer,"Hello from child: %d",i+1);
			while(1)
			{
				send(nsfd,buffer,strlen(buffer),0);sleep(1);
			}
		}
	}
	pause();pause();pause();
	int nsfd;
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		char buffer[50];
		sprintf(buffer,"Hello from parent");
		while(1)
		{
			if(send(nsfd,buffer,strlen(buffer),0)<0)
			{
				perror("Sending failed");
				exit(0);
			}
		}
	}
	return 0;
}