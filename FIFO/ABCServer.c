#include "../cn.h"
#define SERVERPATH "SERVER"
#define PATH "."
#define PROJ 29
struct Request
{
	char p[10];
	int req;
};

struct Shm_str{
	int S,B,C;
};

int f = 0;
void sig_handler(int alrm)
{
	printf("What do you want?\nY : Yes\nN : No\n");
	char ch;
	scanf("%c",&ch);
	if(ch=='N')
		exit(0);
	else
		f = 0;
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	key_t k = ftok(PATH,PROJ);
	if(k<0)
	{
		perror("Key not created");
		exit(0);
	}
	int shmid = shmget(k,sizeof(struct Shm_str),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Shared memory not created");
		exit(0);
	}
	struct Shm_str* q = (struct Shm_str*)shmat(shmid,NULL,0);
	q->B = getpid();
	mkfifo(SERVERPATH,O_CREAT|0666);
	struct pollfd pfd[1];
	pfd[0].fd = open(SERVERPATH,O_RDONLY);
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	struct Request A;
	int r,fd1[2];
	pipe(fd1);
	pid_t p = fork();
	if(p<0)
	{
		perror("Error in forking");
		exit(0);
	}
	else if(p>0)
	{
		close(fd1[0]);
		sleep(1);
		while(1)
		{
			r = poll(pfd,(nfds_t)1,5000);
			if(r>0)
			{
				if(!f)
				{
					kill(p,SIGUSR1);
					f = 1;
				}
				read(pfd[0].fd,&A,sizeof(struct Request));
				write(fd1[1],&A,sizeof(struct Request));
			}
		}
	}
	else
	{
		close(fd1[1]);
		dup2(fd1[0],0);
		char* a[1] = {NULL};
		execv("A",a);
	}
	return 0;
}