// for testing the functionalits
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fd = open("/dev/TickTock",O_WRONLY);
	if(fd<0)
	{
		perror("Device not opened");
		exit(0);
	}
	char buff[40];
	printf("Enter something\n");
	scanf("%s",buff);
	int sz = write(fd,buff,40);
	//printf("String read = %s\nBytes read = %d\n",buff,sz);
	if(!sz)
	{
		perror("Could not write!");
	}
	close(fd);
	return 0;
}