#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char* argv[])
{
	 if(argc==1)
	{
		printf("Usage: ./Chain P1 P2 P3 ....\n");
		exit(0);
	}
	else if(argc==2)
	{
		char buffer[15];int sz;
		while(1)
		{
			sz = read(0,buffer,20);
			buffer[sz] = '\0';
			printf("inside last process = %s\n",buffer);
		}
	}
	char files[10];int i;
	strcpy(files,"file1.txt");
	char* arg[argc];
	int pos = (int)(argv[1][strlen(argv[1])-1]-'0');
	files[4] = argv[1][strlen(argv[1])-1];
	int pp[2],st;
	st = pipe(pp);
	if(st<0)
	{
		perror("Pipe not created");
		exit(0);
	}
	pid_t c = fork();
	if(c<0)
	{
		perror("Fork failed");
		exit(0);
	}
	else if(c>0)
	{
		if(pos==1)
		{
			close(pp[0]);
			dup2(pp[1],1);
			char buffer[15];//int sz;
			while(1)
			{
				scanf("%s",buffer);
				write(1,buffer,strlen(buffer));
				sleep(1);
			}
		}
		else
		{
			dup2(pp[1],1);
			char buffer[15];int sz;
			int fd = open(files,O_CREAT|O_WRONLY|O_TRUNC,S_IWUSR|S_IRUSR);
			while(1)
			{
				sz = read(0,buffer,15);
				buffer[sz] = '\0';
				write(fd,"Inside ",7);
				write(fd,argv[1],strlen(argv[1]));
				write(fd," = ",3);
				write(fd,buffer,sz);
				write(fd,"\n",1);
				write(1,buffer,sz);
				sleep(1);
			}
		}
	}
	else
	{
		dup2(pp[0],0);
		arg[0] = NULL;
		arg[0] = (char*)malloc(sizeof(char));
		strcpy(arg[0],argv[0]);
		for(i=1;i<argc-1;i++)
		{
			arg[i] = (char*)malloc(sizeof(char));
			strcpy(arg[i],argv[i+1]);
		}
		arg[argc-1] = NULL;
		execv("Chain",arg);
	}
	return 0;
}