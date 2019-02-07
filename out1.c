#include "cn.h"

int main(int argc, char const *argv[])
{
	int n = atoi(argv[1]);
	while(1)
	{
		printf("Hello from %d\n",n);
		fflush(stdout);
		sleep(5);
	}
	return 0;
}