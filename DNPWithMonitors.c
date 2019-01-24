// actual monitor implementation

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#define K 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t mutex,next;
int next_count;

struct Condition
{
	sem_t x;
	int x_count;
}self[K];

int state[K];

void wait(struct Condition* s)
{
	s->x_count++;
	if(next_count>0)
	sem_post(&next);
	else
	sem_post(&mutex);
	sem_wait(&s->x);
	s->x_count--;
}

void signal(struct Condition* s)
{
	if(s->x_count>0)
	{
		next_count++;
		sem_post(&s->x);
		sem_wait(&next);
		next_count--;
	}
}

void test(int i)
{
	if((state[(i+4)%K]!=EATING)&&(state[i]==HUNGRY)&&(state[(i+1)%K]!=EATING))
	{
		printf("Philosopher-%d picking up sticks\n",i);
		state[i] = EATING;
		signal(&self[i]);
	}
}


void PickUp(int i)
{
	sem_wait(&mutex);
	printf("Philosopher-%d feeling Hungry\n",i);
	state[i] = HUNGRY;
	test(i);
	if(state[i]!=EATING)
	{
		wait(&self[i]);
	}
	if(next_count>0)
	sem_post(&next);
	else
	sem_post(&mutex);
}

void putdown(int i)
{
	sem_wait(&mutex);
	printf("Philosopher-%d putting down sticks\n",i);
	state[i] = THINKING;
	test((i+1)%K);
	test((i+4)%K);
	if(next_count>0)
	sem_post(&next);
	else
	sem_post(&mutex);
}

void* Philosopher(void* arg)
{
	int i = *(int*)arg;
	for(int j=0;j<3;j++)
	{
		PickUp(i);
		struct tm* t;
		time_t tt = time(NULL);
		t = localtime(&tt);
		int sec = t->tm_sec+3;
		while(t->tm_sec!=sec)
		{
			tt = time(NULL);
			t = localtime(&tt);
		}
		
		putdown(i);
	}
}

int main()
{
	pthread_t phil[K];
	int pi[K];
	sem_init(&mutex,0,1);
	sem_init(&next,0,0);
	next_count = 0;
	for(int i=0;i<K;i++)
	{
		state[i] = THINKING;
		self[i].x_count = 0;
		sem_init(&self[i].x,0,0);
	}
	for(int i=0;i<K;i++)
	{
		pi[i] = i;
		pthread_create(&phil[i],NULL,Philosopher,(void*)&pi[i]);
	}
	for(int i=0;i<K;i++)
	{
		pthread_join(phil[i],NULL);
	}
	return 0;
}


