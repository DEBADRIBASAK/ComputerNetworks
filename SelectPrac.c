/* practice of select system call */
#include "cn.h"
#include <sys/select.h>

int max(int a,int b)
{
	return (a>b)?a:b;
}

int main(int argc,char* argv[])
{
	FILE* fp = popen("./out 1","r");
	FILE* fp1 = popen("./out 2","r");
	int fd1 = fileno(fp),fd2 = fileno(fp1);
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(fd1,&readset);
	FD_SET(fd2,&readset);
	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec = 1000;
	int maxpe = max(fd1,fd2)+1,r;
	char buffer[10];
	while(1)
	{
		r = select(maxpe,&readset,NULL,NULL,&t);
		if(r<0)
		{
			perror("Error in selection");
			exit(0);
		}
		else if(r>0)
		{
			printf("Time left: %ld sec : %ld usec\n",t.tv_sec,t.tv_usec);
			if(FD_ISSET(fd1,&readset))
			{
				fscanf(fp,"%[^\n]s",buffer);
				printf("Reading: %s\n",buffer);
				while(fgetc(fp)!='\n');
			}
			else
				FD_SET(fd1,&readset);
			if(FD_ISSET(fd2,&readset))
			{
				fscanf(fp1,"%[^\n]s",buffer);
				printf("Reading: %s\n",buffer);
				while(fgetc(fp1)!='\n');
			}
			else
				FD_SET(fd2,&readset);
		}
		else
		{
			printf("No one is ready yet\n");
			FD_SET(fd1,&readset);FD_SET(fd2,&readset);
		}
		t.tv_sec = 2;
			t.tv_usec = 1000;
	}
	return 0;
}
