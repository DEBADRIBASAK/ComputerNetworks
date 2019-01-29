#include "../cn.h"
#include <string.h>
#include <time.h>
#define PATH "."
#define PROJID 29
#define SERVERTYPE 10
int msqid;
struct pid_pack
{
	int pid,gid;
	char buffer[100];
};
struct msgbuf
{
	long type;
	struct pid_pack p[1];
};
int client_tab[256][2],cnt = 0;
int is_in(struct msgbuf* m)
{
	int f = 0;
	struct msgbuf m1;
	m1 = *(m);
	for(int i=0;i<cnt;i++)
	{
		if(client_tab[i][0]==m->p[0].pid)
			f = 1;
		else if(client_tab[i][1]==m->p[0].gid)
		{
			m1.type = client_tab[i][0];
			if(msgsnd(msqid,&m1,sizeof(struct pid_pack),0)<0)
			{
				perror("Message could not be sent");
			}
		}
	}
	return f;
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
				client_tab[cnt][0] = m.p[0].pid;
				client_tab[cnt++][1] = m.p[0].gid;
			}
			else if(!is_in(&m))
			{
				client_tab[cnt][0] = m.p[0].pid;
				client_tab[cnt++][1] = m.p[0].gid;
			}
		}
		sleep(1);
	}
	return 0;
}