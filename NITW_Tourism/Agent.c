#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "."
#define PROJID 37

int shmid,msqid;

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
	printf("One car got damaged! :( \n");
}

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [LIST_OF_TOURIST_SPOTS]\n",argv[0]);
		exit(0);
	}
	signal(SIGUSR1,sig_handler);
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
	ptr = (struct Shmstr*)shmat(shmid,NULL,0);
	if(ptr==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	int self = (int)getpid(),child;
	for(int i=1;i<argc;i++)
	{
		ptr->agent_id[atoi(argv[i])] = self;
	}
	struct msgbuf2 A;
	struct msgbuf1 B;
	while(1)
	{
		if(msgrcv(msqid,&A,sizeof(A.m),(long)self,0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			printf("Received: location: %d Spot: %d\n",A.m[0].location,A.m[0].tourist_spot);
			child = fork();
			if(child<0)
				perror("Forking error");
			else if(child>0)
			{
				B.type = 10;
				B.p[0].pid = child;
				if(msgsnd(msqid,&B,sizeof(B.p),0)<0)
					perror("Could not send");
			}
			else
			{
				char location[20],tourist_spot[20];
				sprintf(location,"%d",A.m[0].location);
				sprintf(tourist_spot,"%d",A.m[0].tourist_spot);
				execlp("./Taxi","./Taxi",location,tourist_spot,NULL);
			}
		}
	}
	return 0;
}