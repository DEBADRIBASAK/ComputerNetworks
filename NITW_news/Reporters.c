// tomorrow
//
#include "../cn.h"

int main(int argc, char const *argv[])
{
	char buffer[256];
	while(1)
	{
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		printf("%s\n",buffer);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}