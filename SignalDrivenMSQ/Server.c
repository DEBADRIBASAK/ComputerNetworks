#include "../cn.h"
#define PATH "."
#define PROJID 37
int client_list[255],ind = 0,msqid;
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
int main(int argc, char const *argv[])
{
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
	struct msgbuf A;
	while(1)
	{
		if(msgrcv(msqid,&A,sizeof(struct msg),10,0)<0)
		{
			perror("Receiving error");exit(0);
		}
		else
		{
			if(strcmp(A.m[0].buffer,"@init")==0)
			{
				printf("Init..\n");
				client_list[ind++] = A.m[0].p;
			}
			else
			{
				for(int i=0;i<ind;i++)
				{
					if(A.m[0].p!=client_list[i])
					{
						A.type = client_list[i];
						if(msgsnd(msqid,&A,sizeof(struct msg),0)<0)
						{
							perror("Sending error");exit(0);
						}
						else
						{
							kill(client_list[i],SIGUSR1);
						}
					}
				}
			}
		}
	}
	return 0;
}