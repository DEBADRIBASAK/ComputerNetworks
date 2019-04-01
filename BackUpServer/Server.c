#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#define PATH "/tmp/backup.socket"
#define CONTROLLEN CMSG_LEN(sizeof(int))

int nsfd[255],ind = 0,num;

fd_set readset;

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [SERVER_NUM] [PORT_NO]\n",argv[0]);
		exit(0);
	}
	num = atoi(argv[1]);
	int portno = atoi(argv[2]);
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	int temp = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(portno);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un addr1;
	addr1.sun_family = AF_UNIX;
	strcpy(addr1.sun_path,PATH);
	if(connect(usfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("Could not connect");exit(0);
	}
	
	FD_ZERO(&readset);
	FD_SET(sfd,&readset);int r,sz,stoind;char buffer[255];
	FD_SET(0,&readset);
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			stoind = ind;
			if(FD_ISSET(0,&readset))
			{
				scanf("%s",buffer);
				if(strcmp(buffer,"down")==0)
				{
					struct msghdr msg;
						char buf[2];
						buf[0] = '1';
						buf[1] = '1';
						struct iovec iov[1];
						iov[0].iov_base = buf;
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
						*(int*)CMSG_DATA(cmptr) = sfd;

						FD_CLR(sfd,&readset);
						if(sendmsg(usfd,&msg,0)<0)
							perror("Could not send fd");
					for(int i=0;i<ind;i++)
					{
						if(nsfd[i]==-1)
							continue;
						struct msghdr msg;
						char buf[2];
						buf[0] = '1';
						buf[1] = '1';
						struct iovec iov[1];
						iov[0].iov_base = buf;
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
						*(int*)CMSG_DATA(cmptr) = nsfd[i];

						FD_CLR(nsfd[i],&readset);
						if(sendmsg(usfd,&msg,0)<0)
							perror("Could not send fd");
					}
					//struct msghdr msg;
					//	char buf[2];
						buf[0] = '0';
						buf[1] = '0';
					//	struct iovec iov[1];
						iov[0].iov_base = buf;
						iov[0].iov_len = 2;
						msg.msg_iov = iov;
						msg.msg_iovlen = 1;
						msg.msg_name = NULL;
						msg.msg_namelen = 0;
					//	struct cmsghdr* cmptr;
						cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
						cmptr->cmsg_level = SOL_SOCKET;
						cmptr->cmsg_type = SCM_RIGHTS;
						cmptr->cmsg_len = CONTROLLEN;
						msg.msg_control = cmptr;
						*(int*)CMSG_DATA(cmptr) = nsfd[0];
						msg.msg_controllen = CONTROLLEN;
						if(sendmsg(usfd,&msg,0)<0)
							perror("Could not send terminator");
				}
				else //up
				{
						struct msghdr msg;
						char buf[2];
						buf[0] = '2';
						buf[1] = '2';
						struct iovec iov[1];
						iov[0].iov_base = buf;
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
						*(int*)CMSG_DATA(cmptr) = nsfd[0];
						if(sendmsg(usfd,&msg,0)<0)
							perror("Could not send initiator");
						else
						{
							ind = 0;int first = 1;
							while(1)
							{
								struct msghdr msg;
								char buf[2];
								struct iovec iov[1];
								iov[0].iov_base = buf;
								iov[0].iov_len = 2;
								msg.msg_iov = iov;
								msg.msg_iovlen = 1;
								msg.msg_name = NULL;
								msg.msg_namelen = 0;
								struct cmsghdr* cmptr;
								cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
								msg.msg_control = cmptr;
								msg.msg_controllen = CONTROLLEN;
								if(recvmsg(usfd,&msg,0)<0)
								{
									perror("Could not receive fds");
								}
								else
								{
									if(buf[0]=='0')
										break;
									if(!first)
									{
										nsfd[ind] = *(int*)CMSG_DATA(cmptr);
										FD_SET(nsfd[ind],&readset);ind++;
									}
									else
									{
										first = 0;
										sfd = *(int*)CMSG_DATA(cmptr);
										FD_SET(sfd,&readset);
									}
								}
							}
						}
				}
				continue;
			}
			else
				FD_SET(0,&readset);
			if(FD_ISSET(sfd,&readset))
			{
				nsfd[ind] = accept(sfd,NULL,NULL);
				if(nsfd[ind]<0)
					perror("Could not accept");
				else
				{
					printf("Accepted...\n");
					FD_SET(nsfd[ind],&readset);
					ind++;
				}
			}
			else
				FD_SET(sfd,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(nsfd[i]==-1)
					continue;
				if(FD_ISSET(nsfd[i],&readset))
				{
				//	printf("Selected: %d\n",i);
					if((sz = recv(nsfd[i],buffer,255,0))<0)
					{
						perror("Could not read");
					}
					else
					{
						if(sz==0)
						{
							FD_CLR(nsfd[i],&readset);
							nsfd[i] = -1;continue;
						}
						buffer[sz] = '\0';
						if(strcmp(buffer,"X")==0)
						{
							FD_CLR(nsfd[i],&readset);
							nsfd[i] = -1;continue;
						}
						for(int j=0;j<stoind;j++)
						{
							if(j!=i&&nsfd[j]!=-1)
							{
								//printf("Sending to: %d\n",j);
								send(nsfd[j],buffer,strlen(buffer),0);
							}
						}
					}
				}
				else
					FD_SET(nsfd[i],&readset);
			}
		}
	}
	return 0;
}