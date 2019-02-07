#include "../cn.h"
#define SERVERPATH "SERVER"
#define PATH "."
#define PROJ 29
struct Request
{
	char p[10];
	int a,b,op;
};

struct Reply
{
	char serv[10];
	int res;
};

int main(int argc, char const *argv[])
{
	mkfifo(SERVERPATH,O_CREAT|0666);
	int fd = open(SERVERPATH,O_WRONLY);
	struct Request A;
	pid_t p = getpid();
	sprintf(A.p,"%d",p);
	mkfifo(A.p,O_CREAT|0666);
	struct Reply B;
	int rfd = open(A.p,O_RDONLY|O_NONBLOCK),sz;
	for(int i=0;i<3;i++)
	{
		printf("Enter two numbers and one operation\n");
		scanf("%d%d%d",&(A.a),&(A.b),&(A.op));
		write(fd,&A,sizeof(struct Request));
		sz = read(rfd,&B,sizeof(struct Reply));
		while(sz<=0)
		{
			sz = read(rfd,&B,sizeof(struct Reply));
		}
		printf("Sent from server-%s\nResult: %d\n",B.serv,B.res);
	}
	A.op = 5;
	write(fd,&A,sizeof(struct Request));
	return 0;
}