#include "../cn.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "/tmp/fd_passing.socket"
int main(int argc, char const *argv[])
{
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,PATH);
	
	if(connect(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	perror("Could not connect");
	
	char buffer[50];int sz;
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		printf("Sending = %s\n",buffer);
		send(usfd,buffer,strlen(buffer),0);

		sz = recv(usfd,buffer,50,0);
		buffer[sz] = '\0';

		printf("Reading: %s\n",buffer);
	}
	

	return 0;
}
