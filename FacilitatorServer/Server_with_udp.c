// 5th question
#include "cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "."
#define PROJID 37
#define PORT 8000




fd_set readset;
int sfd[255],clone[255],ind = 0,fsfd;
pid_t p[255];
struct ClientList *pt;




struct Services
{
	char name[20];
	int portno;
	char description[100];
};

struct ClientList
{
	struct sockaddr_in addr[255];
	int i;
};

struct Services arr[255];

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Key could not be created");
		exit(0);
	}
	int shmid = shmget(k,sizeof(struct ClientList),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");
		exit(0);
	}
	pt = (struct ClientList*)shmat(shmid,NULL,0);
	if(pt==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	pt->i = 0;
	fsfd = socket(AF_INET,SOCK_DGRAM,0);
	FD_SET(0,&readset);
	int maxfp1 = 0;
	char buffer[255];
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(PORT);
	if(bind(fsfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
	}
	FD_SET(fsfd,&readset);
	maxfp1 = (maxfp1<fsfd)?fsfd:maxfp1;
	int r,sz,portno,stoind;
	while(1)
	{
		r = select(maxfp1+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			stoind = ind;
			if(FD_ISSET(0,&readset))
			{
				scanf("%d",&portno);
				scanf("%s",buffer);
				printf("portno: %d path: %s\n",portno,buffer);
				arr[ind].portno = portno;
				scanf("%s",arr[ind].name);
				scanf("%s",arr[ind].description);
				sfd[ind] = socket(AF_INET,SOCK_STREAM,0);
				if(sfd[ind]<0)
				{
					perror("Could not create socket");
				}
				int temp;
				addr.sin_port = htons(portno);
				if(bind(sfd[ind],(struct sockaddr*)&addr,sizeof(addr))<0)
				{
					perror("Could not bind");
				}
				else if(listen(sfd[ind],10)<0)
				{
					perror("Listening error");
				}
				else
				{
					printf("Successful\n");
					clone[ind] = ind+10;
					dup2(sfd[ind],clone[ind]);
					char num[2];
					sprintf(num,"%d",ind);
					p[ind] = fork();
					if(p[ind]<0)
					{
						perror("Forking error");
					}
					else if(p[ind]==0)
					{
						printf("%s\n",buffer);
						execlp(buffer,buffer,num,NULL);
					}
					else
					{
						printf("portno: %d path: %s\n",portno,buffer);
						FD_SET(sfd[ind],&readset);
						maxfp1 = (maxfp1<sfd[ind])?sfd[ind]:maxfp1;
						for(int j=0;j<pt->i;j++)
						{
							if(sendto(fsfd,&arr[ind],sizeof(arr[ind]),0,(struct sockaddr*)&pt->addr[j],sizeof(pt->addr[j]))<0)
							{
								perror("Could not send");
							}
						}
						ind++;
					}
				}
			}
			else
				FD_SET(0,&readset);
			for(int i=0;i<stoind;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					printf("Request arrived for Service-%d\n",i+1);
					kill(p[i],SIGUSR1);
				}
				else
					FD_SET(sfd[i],&readset);
			}
			if(FD_ISSET(fsfd,&readset))
			{
				// new service enquiry request has arrived
				char buffer1[20];
				int len = sizeof(pt->addr[pt->i]),sz;
				if((sz = recvfrom(fsfd,buffer1,20,0,(struct sockaddr*)&(pt->addr[pt->i]),&len))<0)
				{
					perror("Coul not receive");
				}
				else
				{
					buffer1[sz] = '\0';
					printf("Received request from port no: %d buffer: %s\n",pt->addr[pt->i].sin_port,buffer1);
					if(strcmp(buffer1,"services")==0)
					{
						if(sendto(fsfd,arr,sizeof(arr),0,(struct sockaddr*)&(pt->addr[pt->i]),sizeof(pt->addr[pt->i]))<0)
						{
							perror("Could not send");
						}
						if(sendto(fsfd,&ind,sizeof(int),0,(struct sockaddr*)&(pt->addr[pt->i]),sizeof(pt->addr[pt->i]))<0)
						{
							perror("Could not send");
						}
						printf("So far no of clients: %d\n",pt->i+1);
						pt->i++;
					}
					else
					{
						if(sendto(fsfd,"invalid",7,0,(struct sockaddr*)&(pt->addr[pt->i]),sizeof(pt->addr[pt->i]))<0)
						{
							perror("Could not send");
						}
					}
				}
				
			}
			else
				FD_SET(fsfd,&readset);
		}
		sleep(1);
	}
	return 0;
}
