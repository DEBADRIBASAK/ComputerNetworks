/* implementation of inter-process communication using a single message queue */

#include "../cn.h"
#include <pthread.h>
#define PATH "."
#define PROJID 29
struct msgbuf
{
	long type;
	char buffer[100];
};
struct shm_str{
	int p[4];
};
int shmid,msqid;
struct shm_str *q;
void* read_func(void* arg)
{
	printf("Reading..\n");
	int n = *(int*)arg;
	long r;
	while((r = q->p[(n+2)%4])<=0);
	printf("r = %ld\n",r);
	struct msgbuf* bf = NULL;
	bf = (struct msgbuf*)malloc(sizeof(struct msgbuf));
	bf->type = r;
	while(1)
	{
		if(msgrcv(msqid,bf,100,r,0)<0)
		{
			perror("Message not received");
			exit(0);
		}
		else
		{
			printf("Message received = %s\n",bf->buffer);
		}
		sleep(1);
	}
}

void* write_func(void* arg)
{
	printf("Writing..\n");
	int n = *(int*)arg;
	long r = q->p[n];
	struct msgbuf* bf = NULL;
	bf = (struct msgbuf*)malloc(sizeof(struct msgbuf));
	bf->type = r;
	while(1)
	{
		printf("Enter a message\n");
		scanf("%[^\n]s",bf->buffer);
		if(msgsnd(msqid,bf,100,0)<0)
		{
			perror("Message not sent");
			exit(0);
		}
		else
		{
			printf("Successfully sent\n");
			while(getchar()!='\n');
		}
		sleep(1);
	}

}

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID),t = atoi(argv[1])-1;
	if(k<0)
	{
		perror("Key not created");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct shm_str),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Shared memory not created");
		exit(0);
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Message queue not created");
		exit(0);
	}
	q = (struct shm_str*)shmat(shmid,NULL,0);
	if(t<0)
	{
		q->p[0] = q->p[1] = q->p[2] = q->p[3] = 0;
		shmdt(q);
		exit(0);
	}
	q->p[t] = (long)getpid();
	printf("Upto\n");
	pthread_t pr,pw;
	pthread_create(&pr,NULL,read_func,(void*)&t);
	pthread_create(&pw,NULL,write_func,(void*)&t);
	pthread_join(pr,NULL);
	pthread_join(pw,NULL);
	return 0;
}