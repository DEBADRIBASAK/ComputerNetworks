/* this program is for testing hte device drive */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fd = open("/dev/TickTock",O_RDONLY);
	if(fd<0)
	{
		perror("Device not opened");
		exit(0);
	}
	char buff[40];
	int sz = read(fd,buff,40);
	buff[sz] = '\0';
	printf("String read = %s\nBytes read = %d\n",buff,sz);
	close(fd);
	return 0;
}
