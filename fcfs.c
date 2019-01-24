// this is a program to simulate the first-come-first-serve algorithm of process scheduling

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct process
{
	int pid,cpu_burst;
	struct process *next,*prev;
};

struct process *queue = NULL,*tail = NULL;

void addEnd(int p,int c)
{
	if(queue==NULL)
	{
		queue = (struct process*)malloc(sizeof(struct process));
		queue->pid = p;
		queue->cpu_burst = c;
		queue->prev = NULL;
		queue->next = NULL;
		tail = queue;
	}
	else
	{
		tail->next = (struct process*)malloc(sizeof(struct process));
		tail->next->pid = p;
		tail->next->cpu_burst = c;
		tail->next->prev = tail;
		tail->next->next = NULL;
		tail = tail->next;
	}
	//sleep(1);
}

struct process* removeBegin()
{
	if(queue==NULL)
	{
		return NULL;
	}
	else
	{
		struct process *rtn = queue;
		queue = queue->next;
		if(queue!=NULL)
		queue->prev = NULL;
		else
		{
			tail = NULL;
		}
		return rtn;
	}
}

void* CreateProcess(void *n)
{
	int k = *(int*)n,i,c;
	struct process *t = NULL;
	for(i=0;i<k;i++)
	{
		c = rand()%2+1;
		printf("process id = %d cpu burst time = %d\n",i+1,c);
		addEnd(i+1,c);
		sleep(1);
	}
	printf("all processes created!\n");
}

void* shedule_process(void* k)
{
	struct process *p;
	int n = *(int*)k;
	int i = 0;
	do
	{
		p = removeBegin();
		if(p!=NULL)
		{
			sleep(p->cpu_burst);
			printf("process %d completed\n",p->pid);
			i++;
		}
	}while(i!=n);
	printf("scheduling process completed!\n");
}

int main()
{
	pthread_t pth1,pth2;
	srand(time(NULL));
	printf("enter required number of processes\n");
	int n;
	scanf("%d",&n);
	pthread_create(&pth1,NULL,CreateProcess,(void*)&n);
	pthread_create(&pth2,NULL,shedule_process,(void*)&n);
	pthread_join(pth2,NULL);
	pthread_join(pth1,NULL);
	return 0;
}
