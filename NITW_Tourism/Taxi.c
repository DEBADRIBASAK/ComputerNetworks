#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PATH "."
#define PROJID 37

int shmid,msqid,sfd;

struct Pair1
{
	struct sockaddr_in cl_addr;
	int pid;
};

struct msgbuf1{
	long type;
	struct Pair1 p[1];
};

struct Pair
{
	int location,tourist_spot;
};

struct Shmstr
{
	int agent_id[255];
};
struct Shmstr* ptr;
struct msgbuf2
{
	long type;
	struct Pair m[1];
};

void sig_handler(int alrm)
{
	kill(getppid(),SIGUSR1);
	char buffer[50];
	strcpy(buffer,"Sorry bro :( ... Car got damaged! You go on foot!");
	send(sfd,buffer,strlen(buffer),0);
	exit(0);
}


int main(int argc, char const *argv[])
{
	printf("Taxi started..pid = %d\n",getpid());
	signal(SIGINT,sig_handler);
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not create key");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");
		exit(0);
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Could not createmessage queue");
		exit(0);
	}
	else
	{
		printf("Msqid: %d\n",msqid);
	}
	int self = (int)getpid(),child;
	struct msgbuf2 A;
	struct msgbuf1 B;

	if(msgrcv(msqid,&B,sizeof(B.p),(long)self,0)<0)
	{
		perror("Couldn't receive");
	}
	else
	{
		printf("Address: %d\nPort: %d\n",(int)B.p[0].cl_addr.sin_addr.s_addr,(int)B.p[0].cl_addr.sin_port);
		sfd = socket(AF_INET,SOCK_STREAM,0);
		if(sfd<0)
			perror("Could not create socket");
		if(connect(sfd,(struct sockaddr*)&(B.p[0].cl_addr),sizeof(B.p[0].cl_addr))<0)
		{
			perror("Couldn,t connect");
			exit(0);
		}
		char buffer[50],buff[50];int sz;
		strcpy(buffer,"Hey! Your Taxi Here! Reaching soon.. :)");
		send(sfd,buffer,strlen(buffer),0);
		while(1)
		{
			sz = recv(sfd,buffer,50,0);
			buffer[sz] = '\0';
			if(strcmp(buffer,"end")==0)
			{
				printf("Trip got ended!\n");
				exit(0);
			}
			//sleep(sz/2);
			sprintf(buff,"Reached: %s",buffer);
			send(sfd,buff,strlen(buff),0);
		}
	}
	return 0;
}