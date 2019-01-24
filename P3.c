#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

int main()
{
	char buffer[20];
	int sz;
	while(1)
	{
		sz = read(0,buffer,20);
		buffer[sz] = '\0';
		printf("Inside P3 = %s\n",sz,buffer);
		sleep(1);
	}
}