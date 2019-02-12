#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	while(1)
	{
		write(1,"Data from D1",12);
		sleep(1);
	}
	return 0;
}