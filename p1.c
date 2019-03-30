#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char buffer[50];
	strcpy(buffer,"this is from child");
	while(1)
	{
		write(1,buffer,strlen(buffer));
		write(1,"\n",1);
		//printf("%s\n",buffer);
		//fflush(stdout);
		sleep(1);
	}
	return 0;
}