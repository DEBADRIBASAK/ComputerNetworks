#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <fcntl.h>
int main()
{
	while(1)
	{
		write(1,"Message from P1",15);
		sleep(1);
	}
	return 0;
}