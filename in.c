#include "cn.h"
#include <sys/select.h>

int main(int argc, char const *argv[])
{
	char buffer[20];
	while(1)
	{
		scanf("%[^\n]s",buffer);
		printf("Reading: %s\n",buffer);
		while(getchar()!='\n');
		sleep(1);
	}
	return 0;
}