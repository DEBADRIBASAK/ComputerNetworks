#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#define PATH "."
#define PROJID 23
#define MAXFD 255
struct Pair
{
	int p;
	int fd1,fd2;
};

int sfd[MAXFD],count[MAXFD]={0},ports[MAXFD],ind = 0;
char* paths[MAXFD],*types[MAXFD],*manner[MAXFD];
struct sockaddr_in addr[MAXFD];
int shmid;

fd_set readset;int maxfp1=-1;
struct Pair* p;

void* func(void* args)
{
	int i = *(int*)args;
	int nsfd = accept(sfd[i],NULL,NULL);
	if(nsfd<0)
	{
		perror("Could not accept");
		pthread_exit(NULL);
	}
	char buffer[20];int sz;
	while(1)
	{
		if((sz = recv(nsfd,buffer,20,0))<0)
		{
			perror("Reading error");
		}
		buffer[sz] = '\0';
		if(strcmp(buffer,"bye")==0)
		{
			break;
		}
		else
		{
			for(int i=0;i<strlen(buffer);i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
				buffer[i] = buffer[i]-'a'+'A';
			}
			send(nsfd,buffer,strlen(buffer),0);
		}
	}
	count[i]++;
}

void sig_handler(int alrm)
{
	count[p->p]++;
}


int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not make the key");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Pair),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not prepare shared memory");
		exit(0);
	}
	p = (struct Pair*)shmat(shmid,NULL,0);

	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	dup2(0,p->fd1);
	dup2(1,p->fd2);
	FILE* fp = fopen("Config.txt","r");
	if(fp==NULL)
	{
		perror("Could not open file");
		exit(0);
	}
	char buffer[50];
	char* tok;
	int sz = fscanf(fp,"%[^\n]s",buffer);
	while(fgetc(fp)!='\n');
	sz = fscanf(fp,"%[^\n]s",buffer);
	while(sz>0)
	{
		tok = strtok(buffer," ");
		tok = strtok(NULL," ");
		types[ind] = strdup(tok);

		tok = strtok(NULL," ");
		ports[ind] = atoi(tok);
		tok = strtok(NULL," ");
		count[ind] = atoi(tok);
		tok = strtok(NULL," ");
		paths[ind] = strdup(tok);
		tok = strtok(NULL," ");
		manner[ind] = strdup(tok);
		printf("types: %s count: %d ports: %d paths: %s manner: %s\n",types[ind],count[ind],ports[ind],paths[ind],manner[ind]);
		ind++;
		while(fgetc(fp)!='\n');
		sz = fscanf(fp,"%[^\n]s",buffer);
	}
	for(int i=0;i<ind;i++)
	{
		if(strcmp(types[i],"tcp")==0)
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);
		else
		sfd[i] = socket(AF_INET,SOCK_DGRAM,0);
		if(sfd[i]<0)
		{
			perror("Socket not created");
		}
		else
		{
			FD_SET(sfd[i],&readset);
			maxfp1 = (maxfp1<sfd[i])?sfd[i]:maxfp1;
		}
		addr[i].sin_family = AF_INET;
		addr[i].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr[i].sin_port = htons(ports[i]);
		if(bind(sfd[i],(struct sockaddr*)&addr[i],sizeof(addr[i]))<0)
		{
			perror("Could not bind");
		}
		else 
		{
			printf("Success..\n");
		}
		if(listen(sfd[i],count[i]+1)<0)
		{
			perror("Could not listen");
		}
	}
	struct timeval t;int r;
		t.tv_sec = 2;
		t.tv_usec = 1000;
		while(1)
		{
			r = select(maxfp1+1,&readset,NULL,NULL,NULL);
			if(r>0)
			{
				for(int i=0;i<ind;i++)
				{
					if(FD_ISSET(sfd[i],&readset)&&count[i]>0)
					{
						if(strcmp(manner[i],"thread")==0)
						{
							printf("Something for thread..\n");
							pthread_t pt;
							int k = i;
							pthread_create(&pt,NULL,func,(void*)&k);
							count[i]--;
						}
						else
						{
							printf("Something arrived!\n");
							int nsfd = accept(sfd[i],NULL,NULL);
							if(nsfd<0)
							{
								perror("Could not accept");
							}
							pid_t c = fork();
							if(c<0)
							{
								perror("Forking failed");
							}
							else if(c==0)
							{
								dup2(nsfd,0);
								dup2(nsfd,1);
								char* arg[3] = {NULL,NULL,NULL};
								arg[0] = strdup(paths[i]);
								arg[1] = (char*)malloc(sizeof(char)*2);
								arg[1][0] = (char)(i+'0');
								arg[1][1] = '\0';
								execv(paths[i],arg);
							}
							else
							{
								count[i]--;
							}
						}
					}
					else if(count[i]>0)
						FD_SET(sfd[i],&readset);
				}
			}

		}
	return 0;
}