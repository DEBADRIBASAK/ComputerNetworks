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
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Binding failed");
		exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Listening failed");
		exit(0);
	}
	int nsfd;
	nsfd = accept(sfd,NULL,NULL);
	if(nsfd>0)
	{
		send(nsfd,"Hello",5,0);
	}
	nsfd = accept(sfd,NULL,NULL);
	if(nsfd>0)
	{
		if(send(nsfd,"Hello",5,0)<0)
		{
			perror("!");
		}
	}
	close(sfd);
	return 0;
}