// 5th question
#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Services
{
	char name[20];
	int portno;
	char description[100];
};

struct Services arr[255];int ind;

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: ./Client [PORTNO]\n");
		exit(0);
	}

	int fsfd = socket(AF_INET,SOCK_DGRAM,0);
	if(fsfd<0)
	{
		perror("Could not create");
	}
	struct sockaddr_in cl_addr,serv_addr;
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	cl_addr.sin_port = htons(atoi(argv[1]));
	printf("Port: %d\n",cl_addr.sin_port);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(8000);
	int len = sizeof(serv_addr);
	if(bind(fsfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
	{
		perror("Could not bind");
	}
// sending enquiry
	if(sendto(fsfd,"services",8,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		perror("Could not send");
	}

	if(recvfrom(fsfd,arr,sizeof(arr),0,NULL,NULL)<0)
	{
		perror("Could not read");
	}
	int cnt;
	if(recvfrom(fsfd,&cnt,sizeof(int),0,NULL,NULL)<0)
	{
		perror("Could not read");
	}
	for(int i=0;i<cnt;i++)
	{
		printf("Name: %s Descr: %s portno: %d\n",arr[i].name,arr[i].description,arr[i].portno);
	}
	printf("Which service do u want?\n");
	int i1;
	scanf("%d",&i1);
	getchar();
	int portno = arr[i1-1].portno;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(portno);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	else
	{
		printf("Successfully connected..\n");
	}

	char buffer[100];int sz;
	while(1)
	{
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		if(send(sfd,buffer,strlen(buffer),0)<0)
		{
			perror("Could not send");
		}
		else
		{
			printf("Successfully sent..\n");
		}
		if((sz = recv(sfd,buffer,100,0))<0)
		{
			perror("Could not read");
		}
		buffer[sz] = '\0';
		printf("%s\n",buffer);
		sleep(1);
	}
	return 0;
}
