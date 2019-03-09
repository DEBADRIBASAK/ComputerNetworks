#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int flg = fcntl(0,F_GETFL,0);
	flg|=O_NONBLOCK;
	if(fcntl(0,F_SETFL,flg)<0)
	{
		perror("Could not change");
	}
	char buffer[50];
	while(1)
	{
		int sz = read(0,buffer,50);
		if(sz<=0)
		{
			printf("Could not read\n");
		}
		else
		{
			buffer[sz] = '\0';
			printf("Reading: %s\n",buffer);
		}
		sleep(1);
	}
	return 0;
}