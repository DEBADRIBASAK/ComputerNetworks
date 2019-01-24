#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char* argv[])
{
	if(argc==2)
	{
		int n = atoi(argv[1]);
		if(n<6)
		{
			while(1)
			{
				write(1,"Message from",12);
				write(1,argv[1],strlen(argv[1]));
			}
		}
		else
		{
			int fd = open("f.txt",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
			char buffer[20];int sz;
			while(1)
			{
				sz = read(0,buffer,20);
				buffer[sz] = '\0';
				write(fd,buffer,sz);
				write(fd,"\n",sz);
			}
		}
	}
	return 0;
}