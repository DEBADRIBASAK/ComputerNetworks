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
	int rsfd = socket(AF_INET,SOCK_RAW,17);
	int val = 1;
	setsockopt(rsfd,IPPROTO_IP,IP_HDRINCL,&val,sizeof(val));
	struct sockaddr_in addr,addr1;//int len = sizeof(addr1);
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);



	memset(&addr1,0,sizeof(addr1));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(8000);



	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	char buffer[1500] = {0};int sz;




	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer+28);
		struct iphdr* ip;
		struct udphdr* udp;
		ip = (struct iphdr*)buffer;
		udp = (struct udphdr*)(buffer+20);
		ip->ihl = 5;
		ip->protocol = 17;
		ip->version = 4;
		ip->check = 0;
		ip->ttl = 64;
		ip->id = 0;
		ip->saddr = htonl(INADDR_LOOPBACK);
		ip->daddr = htonl(INADDR_LOOPBACK);
		
		ip->check = csum((unsigned short*)ip,sizeof(struct iphdr));

		udp->dest = htons(8000);
		udp->len = 1480;
		udp->source = 0;
		udp->check = csum((unsigned short*)(buffer+20),1480);


		while(getchar()!='\n');
		if(sendto(rsfd,buffer,1500,0,(struct sockaddr*)&addr1,sizeof(addr1))<0)
			perror("Could not send!");
	}
	return 0;
}
