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

void sig_handler(int alrm)
{
	printf("Now A will start\n");
}

int service(struct message* m)
{
	switch(m->op)
	{
		case 1:
		return (m->a)+(m->b);
		case 2:
		return (m->a)-(m->b);
		case 3:
		return (m->a)*(m->b);
		case 4:
		return (m->a)/(m->b);
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
	int shmid = shmget(k,sizeof(struct Triplet),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Segment creation not successful");
		exit(0);
	}
	while(1)
	{
		pause();
		struct message a;
		mkfifo(SERVERPATH,O_CREAT|0666);
		int fd = open(SERVERPATH,O_RDONLY);
		for(int i=0;i<4;i++)
		{
			read(fd,&a,sizeof(struct message));
			mkfifo(a.p,O_CREAT|0666);
			int fd1 = open(a.p,O_WRONLY);
			a.op = service(&a);
			write(fd1,&a,sizeof(struct message));
			close(fd1);
		}
		struct Triplet* t = (struct Triplet*)shmat(shmid,NULL,0);
		kill(t->B,SIGUSR1);
		shmdt((void*)t);
		close(fd);
	}
	return 0;
}