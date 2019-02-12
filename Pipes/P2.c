#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

int main()
{
	while(1)
	{
		write(1,"Message from P2",15);
		sleep(1);
	}
}