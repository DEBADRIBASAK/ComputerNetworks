#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "."
#define PROJID 23

union semun {
    int val;    
    struct semid_ds *buf;
    unsigned short  *array; 
    struct seminfo  *__buf;  
};

int main(int argc, char const *argv[])
{
	key_t k = ftok(PATH,PROJID);
	if(k<0)
	{
		perror("Could not make the key");
		exit(0);
	}
	int semid = semget(k,3,IPC_CREAT|0666);
	if(semid<0)
	{
		perror("Could not create semaphore");
	}
	union semun tmp;
	tmp.val = 1;
	if(semctl(semid,0,SETVAL,tmp)<0)
	{
		perror("Could not set values");
	}
	tmp.val = 1;
	if(semctl(semid,1,SETVAL,tmp)<0)
	{
		perror("Could not set values");
	}
	tmp.val = 0;
	if(semctl(semid,2,SETVAL,tmp)<0)
	{
		perror("Could not set values");
	}
	return 0;
}