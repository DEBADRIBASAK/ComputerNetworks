#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#define PATH "/tmp/backup.socket"
#define CONTROLLEN CMSG_LEN(sizeof(int))
int fd[10][255],ind[10] = {0},ind1 = 0,num[10],sfd[10];
int nusfd[10],ind2 = 0;
fd_set readset;

int find(int i)
{
	for(int k=0;k<ind1;k++)
		if(num[k]==i)
			return k;
}

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
		perror("Could not create socket");
	}
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,PATH);
	if(bind(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(usfd,10)<0)
	{
		perror("Could not listen");exit(0);
	}
	FD_ZERO(&readset);
	FD_SET(usfd,&readset);int r,sz,stoind,stoind1;char buffer[255];
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			stoind = ind2;stoind1 = ind1;
			if(FD_ISSET(usfd,&readset))
			{
				nusfd[ind2] = accept(usfd,NULL,NULL);
				FD_SET(nusfd[ind2],&readset);ind2++; //*********ind2++ must be outside the FD_SET*********
			}
			else
				FD_SET(usfd,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(FD_ISSET(nusfd[i],&readset))
				{
					int f = 0,first = 1;
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
						cmptr->cmsg_level = SOL_SOCKET;
						cmptr->cmsg_type = SCM_RIGHTS;
						cmptr->cmsg_len = CONTROLLEN;
						msg.msg_control = cmptr;
						msg.msg_controllen = CONTROLLEN;
						if(recvmsg(nusfd[i],&msg,0)<0)
						{
							perror("Could not receive fds");
						}
						else
						{
							if(buf[0]!='1')
							{
								f = (int)(buf[0]=='2');
								break;
							}
							if(!first)
							{
								fd[ind1][ind[ind1]] = *(int*)CMSG_DATA(cmptr);
								FD_SET(fd[ind1][ind[ind1]],&readset);ind[ind1]++;
							}
							else
							{
								first = 0;
								sfd[ind1] = *(int*)CMSG_DATA(cmptr);
								FD_SET(sfd[ind1],&readset);
							}
						}
					}
					if(f) //server is up
					{
						int j = find(i);
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
							*(int*)CMSG_DATA(cmptr) = sfd[j];

							FD_CLR(sfd[j],&readset);
							if(sendmsg(nusfd[i],&msg,0)<0)
								perror("Could not send fd");
						for(int i1=0;i1<ind[j];i1++)
						{
							if(fd[j][i1]==-1)
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
							*(int*)CMSG_DATA(cmptr) = fd[j][i1];

							FD_CLR(fd[j][i1],&readset);
							if(sendmsg(nusfd[i],&msg,0)<0)
								perror("Could not send fd");
						}
						//struct msghdr msg;
						//	char buf[2];
							buf[0] = '0';
							buf[1] = '0';
							//struct iovec iov[1];
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
							msg.msg_controllen = CONTROLLEN;
							*(int*)CMSG_DATA(cmptr) = fd[0][0];
							if(sendmsg(nusfd[i],&msg,0)<0)
								perror("Could not send terminator");
							ind[j] = 0;
					}
					else
					{
						num[ind1] = i;
						ind1++;
					}
				}
				else
					FD_SET(nusfd[i],&readset);
			}

			for(int i=0;i<stoind1;i++)
			{
				for(int j=0;j<ind[i];j++)
				{
					if(fd[i][j]==-1)
					continue;
					if(FD_ISSET(fd[i][j],&readset))
					{
						//printf("Selected %d-%d\n",i,j);
						sz = recv(fd[i][j],buffer,255,0);
						if(sz==0)
						{
							FD_CLR(fd[i][j],&readset);
							fd[i][j] = -1;
							continue;
						}
						buffer[sz] = '\0';
						if(strcmp(buffer,"X")==0)
						{
							FD_CLR(fd[i][j],&readset);
							fd[i][j] = -1;
							continue;
						}
						for(int k=0;k<ind[i];k++)
						{
							if(j!=k&&fd[i][k]!=-1)
							{
								//printf("Sending to: %d-%d\n",i,k);
								send(fd[i][k],buffer,strlen(buffer),0);
							}
						}
					}
					else
						FD_SET(fd[i][j],&readset);
				}
			}
			for(int i=0;i<stoind1;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					printf("Accepting on behalf of server %d\n",i);
					fd[i][ind[i]] = accept(sfd[i],NULL,NULL);
					FD_SET(fd[i][ind[i]],&readset);
					ind[i]++;
				}
				else
					FD_SET(sfd[i],&readset);
			}

		}
	}
	return 0;
}