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
	if(bind(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	else
		printf("Bind successful\n");
	if(listen(usfd,10)<0)
		perror("Could not listen");
	else
		printf("Listen successful\n");
	int nsfd = accept(usfd,NULL,NULL);
	if(nsfd<0)
		perror("Could not accept");
	char buffer[50];int sz;
	while(1)
	{
		if((sz = recv(nsfd,buffer,50,0))<0)
		{
			perror("Could not read");
		}
		else
		{
			buffer[sz] = '\0';
			printf("Read: %s\n",buffer);
			strcat(buffer," reply from server");
			printf("Sending: %s\n",buffer);
			if(send(nsfd,buffer,strlen(buffer),0)<0)
				perror("Could not send");
			else
				printf("Successful..\n");
			//sleep(5);
		}
	}
	
	if(unlink(PATH)<0)
		perror("Could not delete");
	else
		printf("Successfully deleted\n");
	return 0;
}
