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

#ifndef __PROTOCOL_H

struct MyTCP
{
	uint16_t source;
	uint16_t dest;
	uint8_t con_req;
	uint8_t con_acc;
	uint8_t fin_ack;
	uint8_t data_flg;
	uint16_t seq;
	uint16_t rec_seq;
};

int my_connect(int sfd,int sport,int dport,struct sockaddr* addr,ssize_t sz)
{
	struct MyTCP *buff;int sz1;
	while(1)
	{
		buff = (struct MyTCP*)malloc(sizeof(struct MyTCP));
		memset(buff,0,sizeof(buff));
		buff->source = sport;
		buff->dest = dport;
		buff->con_req = 1;
		buff->seq = 0;
		buff->rec_seq = 0;
		buff->fin_ack = 0;
		sendto(sfd,buff,sizeof(buff),0,addr,sz);
		char buffer[1000];
		if(recvfrom(sfd,buffer,1000,0,addr,&sz1)<0)
			perror("Could not my_receive");
		else
			printf("Success..\n");
		struct iphdr* ip;
		ip = (struct iphdr*)buffer;
		buff = (struct MyTCP*)(buffer+(ip->ihl*4));
		if(buff->con_acc)
			break;
	}
	printf("Herer\n");
	buff = (struct MyTCP*)malloc(sizeof(struct MyTCP));
	memset(buff,0,sizeof(buff));
	buff->source = sport;
	buff->dest = dport;
	buff->con_req = 0;
	buff->seq = 0;
	buff->rec_seq = 0;
	buff->fin_ack = 1;
	if(sendto(sfd,buff,sizeof(buff),0,addr,sz1)<0)
		perror("Could not send it");
	else
		printf("SEnt..\n");
	return 1;
}

int my_accept(int sfd,int sport,int dport,struct sockaddr* addr,int *sz)
{
	struct MyTCP *buff;
	char buffer[1000];
	recvfrom(sfd,buffer,1000,0,addr,sz);
	struct iphdr* ip;
	ip = (struct iphdr*)buffer;
	buff = (struct MyTCP*)(buffer+(ip->ihl*4));
	printf("Connection Req: %d\n",buff->con_req);
	buff = (struct MyTCP*)malloc(sizeof(struct MyTCP));
	memset(buff,0,sizeof(buff));
	buff->source = sport;
	buff->dest = dport;
	buff->con_req = 0;
	buff->con_acc = 1;
	buff->seq = 0;
	buff->rec_seq = 0;
	buff->fin_ack = 0;

	printf("IP: %s\n",inet_ntoa(((struct sockaddr_in*)addr)->sin_addr));

	if(sendto(sfd,buff,sizeof(buff),0,addr,*sz)<0)
		perror("Could not send");
	else
		printf("Got\n");
	memset(buffer,0,1000);
	recvfrom(sfd,buffer,1000,0,addr,sz);
	return 1;
}

int seq = 0;


int my_send(int sfd,int sport,int dport,char *buffer,int bufsiz,struct sockaddr* addr,ssize_t sz)
{
	char buffer1[1000];
	struct MyTCP *buff;
	buff = (struct MyTCP*)(buffer1);
	memset(buff,0,sizeof(buff));
	buff->source = sport;
	buff->dest = dport;
	buff->con_req = 0;
	buff->con_acc = 0;
	buff->seq = seq;
	buff->rec_seq = 0;
	buff->fin_ack = 0;
	buff->data_flg = 1;
	memcpy(buffer1+sizeof(*buff),buffer,bufsiz);
	sendto(sfd,buffer1,1000,0,addr,sz);
}


int my_receive(int sfd,int sport,int dport,char *buffer,int bufsiz,struct sockaddr* addr,int *sz)
{
	char buffer1[1000];
	
	recvfrom(sfd,buffer1,1000,0,addr,sz);
	struct MyTCP *buff;
	struct iphdr* ip;
	ip = (struct iphdr*)(buffer1);
	buff = (struct MyTCP*)(buffer1+(ip->ihl*4));
	memset(buff,0,sizeof(buff));
	seq +=buff->seq;
	memcpy(buffer,buffer1+(ip->ihl*4)+sizeof(*buff),bufsiz);
}

#endif // __PROTOCOL_H


