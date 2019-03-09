#include "../cn.h"

void sig_handler(int alrm)
{
	char buffer[20];
	scanf("%[^\n]s",buffer);
	while(getchar()!='\n');
	printf("Read: %s\n",buffer);
	signal(SIGINT,sig_handler);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT,sig_handler);
	while(1);
	return 0;
}