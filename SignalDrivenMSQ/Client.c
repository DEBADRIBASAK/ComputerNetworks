#include "../cn.h"
#define PATH "."
#define PROJID 37
//int client_list[255],ind = 0,
int msqid;
struct msg
{
	int p;
	char buffer[255];
};
struct msgbuf
{
	long type;
	struct msg m[1];
};
struct msgbuf A,B;long p ;
void sig_handler(int alrm)
{
	if(msgrcv(msqid,&A,sizeof(struct msg),p,0)<0)
	{
		perror("Receiving error");
	}
	else
	{
		printf("Received from: %d\nMessage: %s\n",A.m[0].p,A.m[0].buffer);
	}
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1,sig_handler);
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Key not made");
		exit(0);
	}
	msqid = msgget(k,IPC_CREAT|0666);
	if(msqid<0)
	{
		perror("MSQ not created");exit(0);
	}
	else
	{
		printf("ID = %d\n",msqid);
	}
	p = (long)getpid();B.type = 10;B.m[0].p = (int)p;
	while(1)
	{
		printf("Enter a message\n");
		scanf("%s",B.m[0].buffer);
		printf("Hello: %s\n",B.m[0].buffer);
		//while(getchar()!='\n'){printf("*");};
		if(msgsnd(msqid,&B,sizeof(struct msg),IPC_NOWAIT)<0)
		{
			perror("Sending error");
		}
		else
		{
			printf("Sucessfully sent..\n");
		}
	}
	return 0;
}