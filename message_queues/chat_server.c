#include "../cn.h"
#include <string.h>
#include <pthread.h>
#include <time.h>
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
struct msqid_ds msg;
void* stat_info(void* arg)
{
	while(1)
	{
		if(msgctl(msqid,IPC_STAT,&msg)<0)
		{
			perror("Could not read status info");
		}
		else
		{
			printf("Key: %d\n",msg.msg_perm.__key);
			printf("Time last received : %s\n",ctime(&(msg.msg_rtime)));
			printf("Time last sent : %s\n",ctime(&(msg.msg_stime)));
			printf("No of messages left: %d\n",(int)msg.msg_qnum);
			printf("Last sent by: %d\n",msg.msg_lspid);
			printf("Last read by: %d\n",msg.msg_lrpid);
		}
		sleep(5);
	}
}

int client_tab[256],cnt = 0;
int is_in(struct msgbuf* m)
{
	int f = 0;
	struct msgbuf m1;
	m1 = *(m);
	for(int i=0;i<cnt;i++)
	{
		if(client_tab[i]==m->p[0].pid)
			f = 1;
		else
		{
			m1.type = client_tab[i];
			if(msgsnd(msqid,&m1,sizeof(struct pid_pack),0)<0)
			{
				perror("Message could not be sent");
			}
		}
	}
	return f;
}
void* operation(void* arg)
{
	struct msgbuf m;
	while(1)
	{
		if(msgrcv(msqid,&m,sizeof(struct pid_pack),SERVERTYPE,0)<0)
		{
			perror("Server could not read");exit(0);
		}
		else
		{
			if(!strcmp(m.p[0].buffer,"@init"))
			{
				client_tab[cnt++] = m.p[0].pid;
			}
			else if(!is_in(&m))
			{
				client_tab[cnt++] = m.p[0].pid;
			}
		}
		sleep(10);
	}
}
int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Key not created");
		exit(0);
	}
	else
	{
		printf("Success\n");
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("Message queue not created");
		exit(0);
	}
	else
	{
		printf("Success\n");
	}
	pthread_t p1,p2;
	pthread_create(&p1,NULL,stat_info,NULL);
	pthread_create(&p2,NULL,operation,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	return 0;
}