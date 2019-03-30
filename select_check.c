#include "cn.h"

int main(int argc, char const *argv[])
{
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(0,&readset);
	struct timeval t;
	t.tv_sec = 0;t.tv_usec=  0;
	select(1,&readset,NULL,NULL,&t);
	if(FD_ISSET(0,&readset))
	{
		printf("Yes\n");
	}
	else
		printf("No\n");
	return 0;
}