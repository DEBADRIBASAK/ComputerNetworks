#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#define KEYPATH "."
#define CONTROLLEN CMSG_LEN(sizeof(int))
#define PATH "/tmp/file.socket"


int ports[255],pids[255],ind = 0,maxfp1 = -1,nusfd[255],nind = 0;
int sfd[255];
int map[65536];
int msqid;

struct message
{
	long type;
	int p[1];
};

fd_set readset;

void sig_handler(int alrm)
{
	unlink(PATH);
	exit(0);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT,sig_handler);
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_un  addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,PATH);
	if(bind(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
	}
	if(listen(usfd,10)<0)
	{
		perror("Could not listen");
	}

	// key_t k = ftok(KEYPATH,37);
	// if(k<0)
	// {
	// 	perror("Could not create key");exit(0);
	// }
	// int msqid = msgget(k,IPC_CREAT|0666);
	// if(msqid<0)
	// {
	// 	perror("Could not create");exit(0);
	// }
	// signal(SIGUSR1,sig_handler)

	mkfifo("FIFO",O_CREAT|0666);
	int fd = open("FIFO",O_RDONLY);
	if(fd<0)
	{
		perror("Could not open fifo");exit(0);
	}

	FD_ZERO(&readset);
	FD_SET(usfd,&readset);
	maxfp1 = (usfd>maxfp1)?usfd:maxfp1;int r;
	FD_SET(fd,&readset);
	maxfp1 = (fd>maxfp1)?fd:maxfp1;
	while(1)
	{
		int sto = ind;
		r = select(maxfp1+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			if(FD_ISSET(usfd,&readset))
			{
				nusfd[nind] = accept(usfd,NULL,NULL); // accepting new service connection
				int portno = 0;
				recv(nusfd[nind],&portno,sizeof(int),0);
				map[portno] = nind;
				recv(nusfd[nind],&pids[nind],sizeof(int),0);
				nind++;
			}
			else
				FD_SET(usfd,&readset);
			if(FD_ISSET(fd,&readset))
			{
				if(read(fd,&ports[ind],sizeof(int))<0)
				{
					perror("Could not read");
				}
				else
				{
					printf("Reading from tracer!\n");
					// sfd[ind] = socket(AF_INET,SOCK_STREAM,0);
					// struct sockaddr_in addr1;
					// addr1.sin_family = AF_INET;
					// addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
					// addr1.sin_port =htons(ports[ind]);
					// printf("Port No: %d\n",ports[ind]);
					// bind(sfd[ind],(struct sockaddr*)&addr1,sizeof(addr1));
					// listen(sfd[ind],10);
					// FD_SET(sfd[ind],&readset);
					// maxfp1 = (sfd[ind]>maxfp1)?sfd[ind]:maxfp1;
					struct msghdr msg;
					struct iovec iov[1];
					char buff[1];
					iov[0].iov_base = buff;
					iov[0].iov_len = 1;
					msg.msg_iov = iov;
					msg.msg_iovlen = 1;
					msg.msg_name = NULL;
					msg.msg_namelen = 0;
					struct cmsghdr* cmptr;
					cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
					cmptr->cmsg_type = SCM_RIGHTS;
					cmptr->cmsg_level = SOL_SOCKET;
					cmptr->cmsg_len = CONTROLLEN;
					msg.msg_control = cmptr;
					msg.msg_controllen = CONTROLLEN;
					kill(pids[map[ports[ind]]],SIGUSR1);
					if(recvmsg(nusfd[map[ports[ind]]],&msg,0)<0)
					{
						perror("Could not recevie file descriptor");
					}
					else
					{
						sfd[ind] = *(int*)CMSG_DATA(cmptr);
						FD_SET(sfd[ind],&readset);
						maxfp1 = (sfd[ind]>maxfp1)?sfd[ind]:maxfp1;
						ind++;
					}
				}
			}
			else
				FD_SET(fd,&readset);
			for(int i=0;i<sto;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					int nsfd = accept(sfd[i],NULL,NULL);
					if(nsfd<0)
					{
						perror("Could not accept");
					}
					else
					{
						printf("Accepted on behalf of port: %d\n",ports[i]);
						struct msghdr msg;
						struct iovec iov[1];
						char buff[1];
						buff[0] = 'S';
						iov[0].iov_len = 1;
						iov[0].iov_base = buff;
						msg.msg_iov = iov;
						msg.msg_iovlen = 1;
						msg.msg_name = NULL;
						msg.msg_namelen = 0;
						struct cmsghdr* cmptr;
						cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
						cmptr->cmsg_type = SCM_RIGHTS;
						cmptr->cmsg_len = CONTROLLEN;
						cmptr->cmsg_level = SOL_SOCKET;
						msg.msg_control = cmptr;
						msg.msg_controllen = CONTROLLEN;
						*(int*)CMSG_DATA(cmptr) = nsfd;
						if(sendmsg(nusfd[map[ports[i]]],&msg,0)<0)
						{
							perror("Could not send");
						}
						else
							printf("Sent successfully\n");
					}
				}
				else
					FD_SET(sfd[i],&readset);
			}
		}
	}
	return 0;
}