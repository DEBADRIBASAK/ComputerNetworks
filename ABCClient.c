#include "cn.h"
#include <string.h>
#define SERVERPATH "SERVER"
struct message
{
	char p[10];
	int a,b,op;
};
int main(int argc, char const *argv[])
{
	mkfifo(SERVERPATH,O_CREAT|0666);
	int fd = open(SERVERPATH,O_WRONLY);
	struct message A;
	pid_t p = getpid();
	sprintf(A.p,"%d",(int)p);
	mkfifo(A.p,O_CREAT|0666);
	int fd1 = open(A.p,O_RDONLY|O_NONBLOCK),sz;
	while(1)
	{
		printf("Enter values\n");
		scanf("%d%d%d",&A.a,&A.b,&A.op);
		write(fd,&A,sizeof(struct message));
		sz = read(fd1,&A,sizeof(struct message));
		while(sz<=0)
		{
			sz = read(fd1,&A,sizeof(struct message));
		}
		printf("Result = %d\n",A.op);
	}
	return 0;
}