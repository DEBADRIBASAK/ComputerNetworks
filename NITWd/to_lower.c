#include "../cn.h"
#define PATH "."
#define PROJID 23
int shmid;

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not make the key");
		exit(0);
	}
	shmid = shmget(k,sizeof(int),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not prepare shared memory");
		exit(0);
	}
	int* p = (int*)shmat(shmid,NULL,0);
	int sz;

	if(p==NULL)
	{
		perror("Could not attach");
		exit(0);
	}
	char buffer[20],buffer1[20];int fd1 = 10,fd2 = 11;
	//fd2 = open(argv[2],O_RDONLY);
	// if(fd2==-1)
	// {
	// 	perror("Could not open");
	// }
	// fd1 = open(argv[3],O_WRONLY);
	// if(fd1==-1)
	// {
	// 	perror("Could not open");
	// }
	//fd2 = open("")
	while(1)
	{
		if((sz = read(0,buffer,20))<0)
		{
			perror("Reading error");
		}
		if(sz==0)
		break;
		buffer[sz] = '\0';
		if(strcmp(buffer,"bye")==0)
		{
			break;
		}
		else
		{
			
			write(fd2,"Reading: ",9);
			write(fd2,buffer,sz);
			write(fd2,"\n",1);
			if((sz = read(fd1,buffer1,20))<0)
				perror("Could not read");
			else
			buffer1[sz] = '\0';
			write(fd2,buffer1,strlen(buffer1));
			for(int i=0;i<strlen(buffer);i++)
			{
				if(buffer[i]>='A'&&buffer[i]<='Z')
				buffer[i] = buffer[i]-'A'+'a';
			}
			strcat(buffer," ");
			strcat(buffer,buffer1);
			write(1,buffer,strlen(buffer));
		}
		sleep(1);
	}
	*p = atoi(argv[1]);
	shmdt(p);
	kill(getppid(),SIGUSR1);
	write(fd2,"Process exiting...",18);
	return 0;
}
