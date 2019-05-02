#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <pcap.h>
#include <pthread.h>
#include "Protocol.h"
#define PORT1 8000
#define PORT2 8002


int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_RAW,253);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.2");
	struct sockaddr_in addr1;int sz = sizeof(addr1);
	bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(my_accept(sfd,8000,8002,(struct sockaddr*)&addr1,&sz)<0)
	{
		perror("Could not get");exit(0);
	}
	char buffer[100];
	int sz1;
	my_receive(sfd,PORT1,PORT2,buffer,100,(struct sockaddr*)&addr1,&sz1);
	printf("Received: %s\n",buffer);
	return 0;
}