#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

int main(int argc, char const *argv[])
{
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr,addr1;int len = sizeof(addr1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[100];int sz;
	while(1)
	{
		sz = recvfrom(sfd,buffer,100,0,(struct sockaddr*)&addr1,&len);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
		char ad[INET_ADDRSTRLEN];
		printf("Sender IP: %s\n",inet_ntop(AF_INET,&addr1.sin_addr.s_addr,ad,INET_ADDRSTRLEN));
		printf("Sender Port: %d\n",(int)ntohs(addr1.sin_port));
	}
	return 0;
}
