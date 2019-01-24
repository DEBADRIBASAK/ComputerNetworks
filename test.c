#include "cn.h"

int main(int argc, char const *argv[])
{
	FILE* fp = popen("./D1","r");
	if(fp==NULL)
	{
		perror("Not created");
		exit(0);
	}
	char buffer[10];
	while(1)
	{
		fscanf(fp,"%[^\n]s",buffer);
		printf("Reading = %s\n",buffer);
		sleep(2);
		//while(getchar()!='\n');
	}
	return 0;
}