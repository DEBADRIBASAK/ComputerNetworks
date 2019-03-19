#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int ports[2] = {8001,8002};

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd,sfd1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp, sizeof(temp));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		printf("Could not listen");exit(0);
	}
	int serv_num;struct sockaddr_in cl_addr,serv_addr;
	int sz1 = sizeof(cl_addr);
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			continue;
		}
		getpeername(nsfd,(struct sockaddr*)&cl_addr,&sz1);
		cl_addr.sin_family = AF_INET;
		cl_addr.sin_addr.s_addr = htonl(cl_addr.sin_addr.s_addr);
		cl_addr.sin_port = htons(cl_addr.sin_port);
		if(recv(nsfd,&serv_num,sizeof(int),0)<0)
		{
			perror("could not read");continue;
		}

		printf("Address: %d\nPort: %d\n",(int)cl_addr.sin_addr.s_addr,(int)cl_addr.sin_port);
		sfd1 = socket(AF_INET,SOCK_STREAM,0);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		serv_addr.sin_port = htons(ports[serv_num-1]);
		if(connect(sfd1,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		{
			perror("Could not connect");
		}
		else
		{
			printf("Connected to remote server\n");
			if(send(sfd1,&cl_addr,sizeof(cl_addr),0)<0)
				perror("Could not send address");
		}
	}
	return 0;
}