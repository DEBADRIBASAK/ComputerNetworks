#include "cn.h"
#define SERVERPATH "SERVER"
#define SHMPATH "."
#define PROJ 37
struct message
{
	char p[10];
	int a,b,op;
};
struct Triplet
{
	int S,B,C;
};
pid_t p;

int shmid;
void sig_handler(int alrm)
{
	printf("The servers have reached the limit\nPress Y/n to continue/quit\n");
	char c;
	scanf("%c",&c);
	if(c=='n')
	{
		kill(p,9);
		struct Triplet* t = (struct Triplet*)shmat(shmid,NULL,0);
		kill(t->B,9);
		kill(t->B,9);
		shmdt((void*)t);
		shmctl(shmid,IPC_RMID,NULL);
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	key_t k = ftok(SHMPATH,PROJ);
	if(k<0)
	{
		perror("Key creation not successful");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Triplet),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Segment creation not successful");
		exit(0);
	}
	struct Triplet* t = (struct Triplet*)shmat(shmid,NULL,0);
	t->S = getpid();
	shmdt((void*)t);
	mkfifo(SERVERPATH,O_CREAT|0666);
	struct pollfd pfd[1];
	pfd[0].fd = open(SERVERPATH,O_RDONLY);
	p = fork();
	if(p<0)
	{
		perror("Fork failed");
		exit(0);
	}
	else if(p>0)
	{
		while(1)
		{
			poll(pfd,(nfds_t)1,-1);
			sleep(1);
			kill(p,SIGUSR1);
			pause();
		}
	}
	else
	{
		char* a[1] = {NULL};
		execv("A",a);
	}
	return 0;
}