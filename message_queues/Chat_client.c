#include "../cn.h"
#include <pthread.h>
#define PATH "."
#define PROJID 29
#define SERVERTYPE 10
int msqid;
struct pid_pack
{
	int pid;
	char buffer[100];
};
struct msgbuf
{
	long type;
	struct pid_pack p[1];
};
int p,msqid;
void* read_func(void* arg)
{
	struct msgbuf m;
	while(1)
	{
		if(msgrcv(msqid,&m,sizeof(struct pid_pack),p,0)<0)
		{
			perror("Could not read");
		}
		else
		{
			printf("Message received from - %d :\n%s\n",m.p[0].pid,m.p[0].buffer);
		}
		sleep(1);
	}
}
void* write_func(void* arg)
{
	struct msgbuf m;
	m.p[0].pid = p;
	m.type = SERVERTYPE;
	while(1)
	{
		printf("Enter a message\n");
		scanf("%[^\n]s",m.p[0].buffer);
		if(msgsnd(msqid,&m,sizeof(struct pid_pack),0)<0)
		{
			perror("Could not send");
		}
		else
		{
			printf("Successfully sent\n");
		}
		while(getchar()!='\n');
		sleep(1);
	}
}
int main(int argc, char const *argv[])
{
	p = (int)getpid();
	struct msgbuf m;
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Key not created");
		exit(0);
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Message queue not created");
		exit(0);
	}
	pthread_t p1,p2;
	pthread_create(&p1,NULL,read_func,NULL);
	pthread_create(&p2,NULL,write_func,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}