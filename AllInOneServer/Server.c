#include "../cn.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
//#define PATH "/tmp/nsfd.socket"
#define CONTROLLEN CMSG_LEN(sizeof(int))

char* path[3] = {"/tmp/usfd1.socket"};//,"/tmp/usfd2.socket","/tmp/usfd3.socket"};
char* proc[3] = {"./S1"};
pid_t p[1];

void sig_handler(int alrm)
{
	for(int i=0;i<1;i++)
	{
		kill(p[i],9);
		unlink(path[i]);
	}
	exit(0);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT,sig_handler);
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd;
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);

	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	int usfd[1];// = socket(AF_UNIX,SOCK_STREAM,0);
	
	for(int i=0;i<1;i++)
	{
		p[i] = fork();
		if(p[i]<0)
		{
			perror("Forking error");
		}
		else if(p[i]==0)
		{
			execlp(proc[i],proc[i],NULL);
		}
		else
		{
			sleep(5);
			usfd[i] = socket(AF_UNIX,SOCK_STREAM,0);
			if(usfd[i]<0)
			{
				perror("Could not create Unix socket");
			}
			struct sockaddr_un addr1;
			addr1.sun_family = AF_UNIX;
			strcpy(addr1.sun_path,path[i]);
			if(connect(usfd[i],(struct sockaddr*)&addr1,sizeof(addr1))<0)
				perror("Could not connect");
		}
	}
	int ch,sz;
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
			perror("Could not accept");
		else
		{
			if((sz = recv(nsfd,&ch,sizeof(ch),0))<0)
			{
				perror("Could not receive");
				ch = 0;
				send(nsfd,&ch,sizeof(ch),0);
			}
			else if(sz==0)
			{
				continue;
			}
			else
			{
				struct msghdr msg;
				char buffer[2];
				struct iovec iov[1];
				iov[0].iov_base = buffer;
				iov[0].iov_len = 2;
				msg.msg_iov = iov;
				msg.msg_iovlen = 1;
				msg.msg_name = NULL;
				msg.msg_namelen = 0;
				struct cmsghdr *cmptr;
				cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
				cmptr->cmsg_level = SOL_SOCKET;
				cmptr->cmsg_type = SCM_RIGHTS;
				cmptr->cmsg_len = CONTROLLEN;
				msg.msg_control = cmptr;
				msg.msg_controllen = CONTROLLEN;
				*(int*)CMSG_DATA(cmptr) = nsfd;
				if(sendmsg(usfd[ch-1],&msg,0)<0)
					perror("Could not send");
				else
					printf("File descriptor sent\n");
				ch = 1;
				send(nsfd,&ch,sizeof(ch),0);
			}
		}
	}
	return 0;
}