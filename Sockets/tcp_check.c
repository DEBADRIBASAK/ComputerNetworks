#include "../cn.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>


int main(int argc, char const *argv[])
{
	int rsfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	if(rsfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(rsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	char buffer[1500];int sz;struct sockaddr_in addr1;int len = sizeof(addr1);
	while(1)
	{
		if((sz = recvfrom(rsfd,buffer,1500,0,(struct sockaddr*)&addr1,&len))<0)
		{
			perror("Could not read");
		}
		else
		{
			// uint16_t doff:4;
			// uint16_t res1:4;
			// uint16_t res2:2;
			// uint16_t urg:1;
			// uint16_t ack:1;
			// uint16_t psh:1;
			// uint16_t rst:1;
			// uint16_t syn:1;
			// uint16_t fin:1;
			struct iphdr* ip;
			ip = (struct iphdr*)buffer;
			struct tcphdr* tcp;
			tcp = (struct tcphdr*)(buffer+(ip->ihl)*4);
			printf("*********************************\n");
			printf("Destination port: %d\n",ntohs(tcp->dest));
			printf("Source Port: %d\n",ntohs(tcp->source));
			printf("Sequence Number: %d\n",ntohs(tcp->seq));
			printf("Acknowledgement No: %d\n",ntohs(tcp->ack_seq));
			printf("FIN: %d\n",(int)tcp->fin);
			printf("ACK: %d\n",(int)tcp->ack);
			printf("URG: %d\n",(int)tcp->urg);
			printf("SYN: %d\n",(int)tcp->syn);
			printf("RESET: %d\n",(int)tcp->rst);
			printf("PUSH: %d\n",(int)tcp->psh);

			printf("Window Size: %d\n",(int)tcp->window);

			printf("The data: %s\n",buffer+(ip->ihl)*4+(tcp->doff)*4);

			printf("*********************************\n\n");
		}
	}
	return 0;
}


/*

( the first three messages are arts of the three way handshaking !!)

*********************************
Destination port: 8000
Source Port: 47478
Sequence Number: 35666
Acknowledgement No: 0
FIN: 0
ACK: 0
URG: 0
SYN: 1
RESET: 0
PUSH: 0
Window Size: 43690
The data: R
*********************************

*********************************
Destination port: 47478
Source Port: 8000
Sequence Number: 23546
Acknowledgement No: 35666
FIN: 0
ACK: 1
URG: 0
SYN: 1
RESET: 0
PUSH: 0
Window Size: 43690
The data: R
*********************************

*********************************
Destination port: 8000
Source Port: 47478
Sequence Number: 35666
Acknowledgement No: 23546
FIN: 0
ACK: 1
URG: 0
SYN: 0
RESET: 0
PUSH: 0
Window Size: 22017
The data: 4���R
*********************************

*********************************
Destination port: 8000
Source Port: 47478
Sequence Number: 35666
Acknowledgement No: 23546
FIN: 0
ACK: 1
URG: 0
SYN: 0
RESET: 0
PUSH: 1
Window Size: 22017
The data: helloR
*********************************

*********************************
Destination port: 47478
Source Port: 8000
Sequence Number: 23546
Acknowledgement No: 35666
FIN: 0
ACK: 1
URG: 0
SYN: 0
RESET: 0
PUSH: 0
Window Size: 22017
The data: helloR
*********************************

*********************************
Destination port: 47478
Source Port: 8000
Sequence Number: 23546
Acknowledgement No: 35666
FIN: 0
ACK: 1
URG: 0
SYN: 0
RESET: 0
PUSH: 1
Window Size: 22017
The data: HELLOR
*********************************

*********************************
Destination port: 8000
Source Port: 47478
Sequence Number: 35666
Acknowledgement No: 23546
FIN: 0
ACK: 1
URG: 0
SYN: 0
RESET: 0
PUSH: 0
Window Size: 22017
The data: HELLOR
*********************************


*/