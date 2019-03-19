#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <pthread.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd;//,sfd1;
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp, sizeof(temp));
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr,self_addr,rem_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	else
	{
		int len = sizeof(self_addr);
		getsockname(sfd,(struct sockaddr*)&self_addr,&len);
		self_addr.sin_family = AF_INET;
		printf("Address: %d\nPort: %d\n",(int)self_addr.sin_addr.s_addr,(int)self_addr.sin_port);
		// self_addr.sin_addr.s_addr = ntohl((self_addr.sin_addr.s_addr));
		// self_addr.sin_port = ntohs((self_addr.sin_port));
		//self_addr.sin_addr.s_addr = htonl((self_addr.sin_addr.s_addr));
		//self_addr.sin_port = htons((self_addr.sin_port));
		///printf("Port No: %d\n",(int)self_addr.sin_port);

		printf("Address: %d\nPort: %d\n",(int)htonl((self_addr.sin_addr.s_addr)),(int)htons((self_addr.sin_port)));
		
	}
	int d;
	printf("Which service?\n1. For Upper-Case\n2. For Lower-Case\n");
	scanf("%d",&d);
	while(getchar()!='\n');
	send(sfd,&d,sizeof(int),0);
	//close(sfd);
	sfd = socket(AF_INET,SOCK_STREAM,0);//,nsfd,sfd1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp, sizeof(temp));
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	if(bind(sfd,(struct sockaddr*)&self_addr,sizeof(self_addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		printf("Could not listen");exit(0);
	}
	nsfd = accept(sfd,NULL,NULL);
	char buffer[50];int sz;
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(nsfd,buffer,strlen(buffer),0)<0)
			perror("Could not send");
		else if((sz = recv(nsfd,buffer,50,0))<0)
			perror("Could not receive");
		else
		{
			buffer[sz] = '\0';
			printf("Received: %s\n%d bytes\n",buffer,sz);
		}
	}
	return 0;
}