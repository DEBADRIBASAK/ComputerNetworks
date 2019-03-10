// tomorrow
#include "../cn.h"
#include <time.h>

int main(int argc, char const *argv[])
{
	char buffer[256];
	FILE* fp = fopen("file.txt","w+");
	time_t t;char *c;
	while(1)
	{
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		t = time(NULL);
		c = ctime(&t);
		fprintf(fp,"%s\n%s\n",c,buffer);
		fflush(fp);
		sleep(1);
	}
	return 0;
}