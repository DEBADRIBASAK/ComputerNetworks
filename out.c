#include "cn.h"

int main(int argc, char const *argv[])
{
	FILE* fp = popen("./out","r");
	if(fp==NULL)
	{
		perror("Popen error");
		exit(0);
	}
	int fd = fileno(fp),sz;
	char buffer[10];
	while(1)
	{
		sz = read(fd,buffer,10);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
		sleep(1);
	}
	return 0;
}