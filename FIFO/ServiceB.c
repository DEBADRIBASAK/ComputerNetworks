#include "../cn.h"
#include <string.h>
#define SERVERPATH "SERVER"
#define PATH "."
#define PROJ 29
struct Request
{
	char p[10];
	int a,b,op;
};

struct Shm_str{
	int S,B,C;
};

struct Reply
{
	char serv[10];
	int res;
};
int f = 0;
char* client_list[4];
int client_fd[4] = {-1,-1,-1,-1},num = 0,ind = 0,itr;
void sig_handler(int alrm)
{
	printf("Now B will start\n");
	f = 0;
	num = 0;
	for(int i=0;i<4;i++)
	{
		if(client_fd[i]!=-1)
			num++;
	}
}



void close_fd(int i)
{
			close(client_fd[i]);
			client_fd[i] = -1;
			free(client_list[i]);
			client_list[i] = NULL;
			return;
}

int is_in(char* buf)
{
	for(int i=0;i<4;i++)
	{
		if(client_fd[i]!=-1)
		if(strcmp(buf,client_list[i])==0)
			return i+1;
	}
	return 0;
}
int operation(int a,int b,int c)
{
	switch(c)
	{
		case 1:
		return a+b;
		case 2:
		return a-b;
		case 3:
		return a*b;
		case 4:
		return a/b;
	}
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
	struct Shm_str *q = (struct Shm_str*)shmat(shmid,NULL,0);
	q->B = getpid();
	shmdt(q);
	pause();
	mkfifo("B",O_CREAT|0666);
	int fd = open("B",O_RDONLY);
	struct Request A;
	struct Reply B;
	strcpy(B.serv,"B");
	int rtn,B_pid,Bfd;
	while(1)
	{
		read(fd,&A,sizeof(struct Request));
		if(!(rtn = is_in(A.p)))
		{
			num++;
			if(num>4)
			{
				if(!f)
				{
					struct Shm_str *q = (struct Shm_str*)shmat(shmid,NULL,0);
					B_pid = q->C;
					f = 1;
					kill(SIGUSR1,B_pid);
					mkfifo("C",O_CREAT|0666);
					Bfd = open("C",O_WRONLY);
				}
				write(Bfd,&A,sizeof(struct Request));
				continue;
			}
			while(client_fd[ind]!=-1)
				ind++;
			client_list[ind] = (char*)malloc(sizeof(char));
			strcpy(client_list[ind],A.p);
			mkfifo(A.p,O_CREAT|0666);
			client_fd[ind] = open(A.p,O_WRONLY);
			B.res = operation(A.a,A.b,A.op);
			write(client_fd[ind],&B,sizeof(struct Reply));
		}
		else
		{
			if(A.op!=5)
			{
				B.res = operation(A.a,A.b,A.op);
				write(client_fd[rtn-1],&B,sizeof(struct Reply));
			}
			else
			{
				close_fd(rtn-1);
			}
		}
	}
	return 0;
}