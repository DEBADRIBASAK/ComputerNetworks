/* this is a sample process */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define PATH "."
#define PROJ_ID 23
// #define SHMNUM 851983 
// #define SEMNUM 163842
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

struct Pair{
	int x,y,z;
};
int main(int argc,char* argv[])
{
	key_t k = ftok(PATH,PROJ_ID);
	int SHMNUM = shmget(k,sizeof(struct Pair),0660);
	int SEMNUM = semget(k,2,0660);
	struct Pair* sh = (struct Pair*)shmat(SHMNUM,NULL,0);
	struct sembuf b;
	int n =10;
	if(atoi(argv[1])==1)
	{
		//sh->x = 5;
		while(n--)
		{
			b.sem_num = 0;
			b.sem_op = -1;
			b.sem_flg = 0;
			semop(SEMNUM,&b,1);
			printf("Process-1 reading value of x = %d and writing %d to y\n",sh->x,sh->x+1);
			sh->y = sh->x+1;
			b.sem_num = 1;
			b.sem_op = 1;
			b.sem_flg = 0;
			semop(SEMNUM,&b,1);
			sleep(1);
		}
		if(sh->z==2)
		{
			shmctl(SHMNUM,IPC_RMID,NULL);
			semctl(SEMNUM,1,IPC_RMID);
		}
	}
	if(atoi(argv[1])==2)
	{
		//sh->y = 6;

		while(n--)
		{
			b.sem_num = 1;
			b.sem_op = -1;
			b.sem_flg = 0;
			semop(SEMNUM,&b,1);
			printf("Process-2 reading value of y = %d and writing %d to x\n",sh->y,sh->y+1);
			sh->x = sh->y+1;
			b.sem_num = 0;
			b.sem_op = 1;
			b.sem_flg = 0;
			semop(SEMNUM,&b,1);
			sleep(1);
		}
		if(sh->z==1)
		{
			shmctl(SHMNUM,IPC_RMID,NULL);
			semctl(SEMNUM,1,IPC_RMID);
		}
	}
}
