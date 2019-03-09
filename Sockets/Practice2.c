#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connection failed");
		exit(0);
	}
	char buffer[50];int sz;
	sz = recv(sfd,buffer,50,0);
	buffer[sz] = '\0';
	printf("Received: %s\n",buffer);
	close(sfd);
	//shutdown(sfd,SHUT_RDWR);
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Connection failed");
		exit(0);
	}
	//char buffer[50];int sz;
	sz = recv(sfd,buffer,50,0);
	buffer[sz] = '\0';
	printf("Received: %s\n",buffer);
	return 0;
}