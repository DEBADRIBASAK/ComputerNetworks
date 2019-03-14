// tomorrow
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SCREEN "SCREEN"
#define PATH "."
#define PROJID 23

int ind,sfd,scrfd;
long pid;

struct Shmstr
{
	int n[3];
	int editor;
	int no_of_live_telecasts;
};

struct msgstr
{
	long type;
	char buffer[256];
};
struct msgstr A;
struct Shmstr *p;

int shmid,msqid,semid;

struct sembuf b;

//fd_set readset;

int is_num(char* s)
{
	int n = strlen(s);
	for(int i=0;i<n;i++)
	{
		if(s[i]<'0'||s[i]>'9')
			return 0;
	}
	return 1;
}

union semun {
    int val;    
    struct semid_ds *buf;
    unsigned short  *array; 
    struct seminfo  *__buf;  
};

int sig_rcv = 0;

void sig_handler(int alrm)
{
	printf("Arey..say something..\n");
	if(sig_rcv==0)
	{
		printf("Signal received..sig_rcv value: %d\n",sig_rcv);
		sig_rcv = (sig_rcv+1)%2;
		printf("After increment sig_rcv value: %d\n",sig_rcv);
		signal(SIGUSR1,sig_handler);
		if(pause()<0)
		{
			printf("Signal received again..\n");
		}
	}
	else
	{
		sig_rcv = (sig_rcv+1)%2;
		printf("Signal received..sig_rcv value: %d\n",sig_rcv);
		signal(SIGUSR1,sig_handler);
	}
	printf("Exiting..\n");
}

int perform_live_telecast(int portno)
{
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");			
	}
	else
	{
		//kill(p->n[(ind+1)%2],SIGUSR1);
		kill(p->editor,SIGUSR1);
		printf("Socket created successfully...\n");
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr.sin_port = htons(portno);
		printf("Sending connect request..\n");
		if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			perror("Could not connect");
		}
		else
		{
			printf("Connection established\n");
			char buffer[100];int sz;
			while(1)
			{
				if((sz = recv(sfd,buffer,100,0))<0)
				{
					perror("Could not read");
				}
				else
				{
					buffer[sz] = '\0';
					if(strcmp(buffer,"end")==0)
						break;
					if(write(scrfd,buffer,strlen(buffer))<0)
					{
						perror("Screen unaccessible");
					}
					else
					{
						printf("Showing on screen\n");
					}
				}
				sleep(1);
			}
			printf("Sending signal to my collegue..\n");
			//kill(p->n[(ind+1)%3],SIGUSR2);
			return 1;
		}
		
		//kill(p->n[(ind+1)%2],SIGUSR2);
		return 0;
	}
	return 0;
}

void sig_handler2(int a)
{
		char num[300];
		if(msgrcv(msqid,&A,sizeof(A.buffer),0,0)<0)
		{
			perror("Could not receive message");
		}
		else
		{
			if(is_num(A.buffer))
			{
				if(perform_live_telecast(atoi(A.buffer)))
				{
					printf("Live telecast completed\n");
				}
				else
				{
					printf("Live telecast failed\n");
				}
			}
			else
			{
				strcpy(num," ");
				sprintf(num,"From Reader-%d: ",ind+1);
				strcat(num,A.buffer);
				if(write(scrfd,num,strlen(num))<0)
				{
					perror("Screen unaccessible");
				}
				else
				{
					printf("Showing on screen\n");
				}
			}
			kill(p->n[(ind+1)%3],SIGUSR2);
		}
}




int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	signal(SIGUSR2,sig_handler2);
	if(argc<2)
	{
		printf("Usage: ./NewsReader [INDEX_POSITION]\n");
		exit(0);
	}
	ind = atoi(argv[1]);

	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not make the key");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not make the shared memory");
		exit(0);
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Message queue not created");
		exit(0);
	}
	// semid = semget(k,4,IPC_CREAT|0666);
	// if(semid<0)
	// {
	// 	perror("Could not create semaphore");
	// }
	//union semun tmp;
	p = (struct Shmstr*)shmat(shmid,NULL,0);
	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}

	p->n[ind] = getpid();
	pid = (long)getpid();

	
	//int sfd;

	mkfifo(SCREEN,O_CREAT|0666);
	scrfd = open(SCREEN,O_WRONLY);
	if(scrfd<0)
	{
		perror("Could not open screen!");
		exit(0);
	}
	
	if(ind==1)
	{
		sig_handler2(10);
	}
	while(1)
	{
		pause();
		kill(p->n[(ind+1)%3],SIGUSR2);
	}
	return 0;
}