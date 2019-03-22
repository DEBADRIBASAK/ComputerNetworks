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


int find_agent(struct Pair pr)
{
	// for(int i=0;i<255;i++)
	// {
	// 	if(ptr->agent_id[i]!=-1)
	// 		printf("%d - %d\n",i,ptr->agent_id[i]);
	// }
	return ptr->agent_id[pr.tourist_spot];
}

int main(int argc, char const *argv[])
{
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
	int sfd,nsfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);int t = 1;
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&t,sizeof(t));
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");
		exit(0);
	}
	ptr = (struct Shmstr*)shmat(shmid,NULL,0);
	if(ptr==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	struct msgbuf2 A;
	struct msgbuf1 B,C;

	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			exit(0);
		}
		else
		{
			printf("Acceptd..\n");
		}
		int len = sizeof(C.p[0].cl_addr);
		getpeername(nsfd,(struct sockaddr*)&C.p[0].cl_addr,&len);
		if(recv(nsfd,&(A.m[0]),sizeof(struct Pair),0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			printf("Location: %d\nTouristSpot: %d\n",A.m[0].location,A.m[0].tourist_spot);
			A.type = (long)find_agent(A.m[0]);
			int temp;
			if(A.type==-1)
			{
				printf("Failure..\n");
				temp =  0;
				send(nsfd,&temp,sizeof(int),0);
			}
			else
			{
				printf("Success...\n");
				temp = 1;
				send(nsfd,&temp,sizeof(int),0);
				if(msgsnd(msqid,&A,sizeof(A.m),0)<0)
				{
					perror("Could not send to message queue");
				}
				else if(msgrcv(msqid,&B,sizeof(B.p),10,0)<0)
				{
					perror("Could not read from message queue");
				}
				else
				{
					printf("PID: %d\n",B.p[0].pid);
					C.type = (long)B.p[0].pid;
					printf("Addr: %d\nPort: %d\n",(int)C.p[0].cl_addr.sin_addr.s_addr,(int)C.p[0].cl_addr.sin_port);
					if(msgsnd(msqid,&C,sizeof(C.p),0)<0)
						perror("Could not send!");
					else
						printf("Sent to taxi..\n");
				}
			}
		}

	}

	return 0;

}