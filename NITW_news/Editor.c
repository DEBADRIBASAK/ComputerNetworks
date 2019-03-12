// I'll do it tomorrow
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "."
#define PROJID 23


struct Shmstr
{
	int n[2];
	int editor;
	int no_of_live_telecasts;
};

union semun {
    int val;    
    struct semid_ds *buf;
    unsigned short  *array; 
    struct seminfo  *__buf;  
};

struct sembuf b;

struct msgstr
{
	long type;
	char buffer[256];
};

struct Shmstr *p;

int shmid,msqid,semid;

fd_set readset;


void sig_handler(int alrm)
{
	b.sem_num = 0;
	b.sem_op = -1;
	b.sem_flg = 0;
	semop(semid,&b,1);
	p->no_of_live_telecasts++;
	b.sem_num = 0;
	b.sem_op = 1;
	b.sem_flg = 0;
	semop(semid,&b,1);
	printf("No of live telecasts: %d\n",p->no_of_live_telecasts);
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
	semid = semget(k,2,IPC_CREAT|0666);
	if(semid<0)
	{
		perror("Could not create semaphore");
	}
	union semun tmp;
	tmp.val = 1;
	if(semctl(semid,0,SETVAL,tmp)<0)
	{
		perror("Could not set values");
	}
	tmp.val = 1;
	if(semctl(semid,1,SETVAL,tmp)<0)
	{
		perror("Could not set values");
	}
	p = (struct Shmstr*)shmat(shmid,NULL,0);
	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	p->no_of_live_telecasts = 0;
	p->editor = getpid();
	int fd[3];
	FILE* fp[3];
	fp[0] = popen("./Documentation","w");
	if(fp[0]==NULL)
	{
		perror("Could not spawn");
		exit(0);
	}
	fd[0] = fileno(fp[0]);
	FD_ZERO(&readset);
	int maxfp1 = -1;
	for(int i=1;i<3;i++)
	{
		fp[i] = popen("./Reporters","r");
		if(fp[i]==NULL)
		{
			perror("Could not spawn");
			exit(0);
		}
		fd[i] = fileno(fp[i]);
		maxfp1 = (maxfp1>fd[i])?maxfp1:fd[i];
		FD_SET(fd[i],&readset);
	}
	pid_t n[2];int turn = 0,r,sz;
	n[0] = p->n[0];n[1] = p->n[1];
	struct msgstr A;
	//char buffer[100];
	while(1)
	{
		r = select(maxfp1+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			for(int i=1;i<3;i++)
			{
				if(FD_ISSET(fd[i],&readset))
				{
					fscanf(fp[i],"%[^\n]s",A.buffer);
					while(fgetc(fp[i])!='\n');
					if(strlen(A.buffer)>=2&&A.buffer[0]=='\\'&&A.buffer[1]=='d')
					{
						fprintf(fp[0],"%s\n",A.buffer+2);
						fflush(fp[0]);
					}
					else
					{
						A.type = n[turn];
						if(msgsnd(msqid,&A,sizeof(A.buffer),0)<0)
						{
							perror("Could not write to message queue");
						}
						else
						{
							printf("Message successfully set to NewsReader-%d\n",turn);
							turn = (turn+1)%2;
						}
					}
				}
				else
				{
					FD_SET(fd[i],&readset);
				}
			}
		}
	}
	return 0;
}