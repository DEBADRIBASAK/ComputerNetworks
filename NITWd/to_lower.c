#include "../cn.h"
#define PATH "."
#define PROJID 23
int shmid;
struct Pair
{
	int p;
	int fd1,fd2;
};

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not make the key");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Pair),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not prepare shared memory");
		exit(0);
	}
	struct Pair* p = (struct Pair*)shmat(shmid,NULL,0);
	int sz;

	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	char buffer[20];int fd1,fd2;
	// dup2(p->fd1,fd2);
	// dup2(p->fd2,fd1);
	fd1 = open("/dev/pts/1",O_WRONLY);
	if(fd1==-1)
	{
		perror("Could not open");
	}
	//fd2 = open("")
	while(1)
	{
		if((sz = read(0,buffer,20))<0)
		{
			perror("Reading error");
		}
		buffer[sz] = '\0';
		if(strcmp(buffer,"bye")==0)
		{
			break;
		}
		else
		{
			write(fd1,"Reading: ",9);
			write(fd1,buffer,sz);
			write(fd1,"\n",1);
			for(int i=0;i<strlen(buffer);i++)
			{
				if(buffer[i]>='A'&&buffer[i]<='Z')
				buffer[i] = buffer[i]-'A'+'a';
			}
			write(1,buffer,strlen(buffer));
		}
		sleep(1);
	}
	p->p = atoi(argv[1]);
	shmdt(p);
	kill(getppid(),SIGUSR1);
	write(fd1,"Process exiting...",18);
	close(fd1);
	return 0;
}