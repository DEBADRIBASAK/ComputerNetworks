#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#define KEYPATH "."
#define CONTROLLEN CMSG_LEN(sizeof(int))
#define PATH "/tmp/file1.socket"
struct shmstr
{
	int ports[255];
	int ind;
};

 union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };
int main(int argc, char const *argv[])
{
	key_t k = ftok(KEYPATH,37);
	if(k<0)
	{
		perror("Could not create");exit(0);
	}
	int shmid = shmget(k,sizeof(struct shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");exit(0);
	}
	struct shmstr* ptr = (struct shmstr*)shmat(shmid,NULL,0);
	ptr->ind = 0;
	shmdt(ptr);
	int semid = semget(k,1,IPC_CREAT|0666);
	if(semid<0)
	{
		perror("Could not create semaphore");exit(0);
	}
	union semun tmp;
	tmp.val = 1;
	semctl(semid,0,SETVAL,tmp);
	return 0;
}