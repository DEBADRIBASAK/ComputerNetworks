#include "../cn.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define PATH "/tmp/usfd1.socket"
#define CONTROLLEN CMSG_LEN(sizeof(int))

//char* paths[3] = {"/tmp/usfd1.socket"};//,"/tmp/usfd2.socket","/tmp/usfd3.socket"};

void* func(void* fd1)
{
	int fd = *(int*)fd1;
	char buffer[50];int sz;
	while(1)
	{
		if((sz = recv(fd,buffer,50,0))<0)
		{
			perror("Could not read");
		}
		else
		{
			if(sz==0)
				break;
			buffer[sz] = '\0';
			if(strcmp(buffer,"end")==0)
				break;
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
					buffer[i] = buffer[i]-'a'+'A';
			}
			send(fd,buffer,strlen(buffer),0);
		}
	}
	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd<0)
	{
		perror("Could not create Unix socket");
	}
	struct sockaddr_un addr1;
	addr1.sun_family = AF_UNIX;
	strcpy(addr1.sun_path,PATH);
	if(bind(usfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
		perror("Could not bind");
	if(listen(usfd,10)<0)
		perror("Could not listen");
	
	int nusfd = accept(usfd,NULL,NULL);
	if(nusfd<0)
		perror("Could not accept");
	else
	{
		while(1)
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
			struct cmsghdr* cmptr;
			cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
			cmptr->cmsg_level = SOL_SOCKET;
			cmptr->cmsg_type = SCM_RIGHTS;
			cmptr->cmsg_len = CONTROLLEN;
			msg.msg_control = cmptr;
			msg.msg_controllen = CONTROLLEN;
			if(recvmsg(nusfd,&msg,0)<0)
				perror("Could not receive");
			else
			{
				int fd = *(int*)CMSG_DATA(cmptr);
				pthread_t pt;
				pthread_create(&pt,NULL,func,&fd);
			}
		}
	}
	return 0;
}