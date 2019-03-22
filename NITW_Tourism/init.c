#include "../cn.h"
#define PATH "."
#define PROJID 37

struct Shmstr
{
	int agent_id[255];
};

struct Shmstr* ptr;
int shmid;

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not create key");
		exit(0);
	}
	shmid = shmget(k,sizeof(struct Shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");
		exit(0);
	}
	ptr = (struct Shmstr*)shmat(shmid,NULL,0);
	for(int i=0;i<255;i++)
		ptr->agent_id[i] = -1;
	shmdt(ptr);
	return 0;
}