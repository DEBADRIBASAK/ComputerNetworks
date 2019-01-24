#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define BLOCK 15
using namespace std;

int main()
{
	int fd = open("Text.txt",O_RDONLY);
	if(fd<0)
	{
		perror("File not openend");
		exit(0);
	}
	char buffer[16];
	int i,sz;
	cout<<"Parent reading = \n";
	for(i=0;i<5;i++)
	{
		sz = read(fd,buffer,15);
		buffer[sz] = '\0';
		cout<<buffer<<endl;
	}
	pid_t p = fork();
	if(p<0)
	{
		perror("Child not created");
		exit(0);
	}
	else if(p==0)
	{
		cout<<"Child reading = \n";
		for(i=0;i<5;i++)
		{
			sz = read(fd,buffer,15);
			buffer[sz] = '\0';
			cout<<buffer<<endl;
		}
	}
	else
	{
		wait(NULL);
	}
	return 0;
}