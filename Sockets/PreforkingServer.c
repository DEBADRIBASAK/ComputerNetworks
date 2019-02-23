#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int sfd,nsfd;
int requests[3] = {0};
void sig_handler()
int main()
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htos(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Listening error");
		exit(0);
	}

	return 0;
}