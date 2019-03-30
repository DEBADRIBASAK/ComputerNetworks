#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr,cl_addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	char buffer[50];int len = sizeof(cl_addr);
	for(int j=0;j<3;j++)
	{
		int sz = recvfrom(sfd,buffer,50,0,(struct sockaddr*)&cl_addr,&len);
		buffer[sz] = '\0';
		for(int i=0;i<sz;i++)
			if(buffer[i]>='a'&&buffer[i]<='z')
			buffer[i] = buffer[i]-'a'+'A';
		sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&cl_addr,sizeof(cl_addr));
		printf("Received: %s\n",buffer);
	}
	sleep(3);
	if(sendto(sfd,buffer,strlen(buffer),0,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
	{
		perror("Sending error");
	}
	while(1);
	return 0;
}