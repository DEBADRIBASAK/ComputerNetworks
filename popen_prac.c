#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	FILE* fp = popen("./p1","r");
	if(fp==NULL)
	{
		perror("popen failed");exit(0);
	}
	int fd = fileno(fp);
	int sz;char buffer[50];
	while(1)
	{
		/*if((sz = read(fd,buffer,50))<0)
		{
			perror("Could not read");
		}
		else
		{
			buffer[sz] = '\0';
			printf("Reading = %s\n",buffer);
		}*/
		fscanf(fp,"%[^\n]s",buffer);printf("Reading = %s\n",buffer);while(fgetc(fp)!='\n');
		sleep(1);
	}
	return 0;
}