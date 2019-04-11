#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

int main(int argc, char const *argv[])
{
	char buffer[255];int sz;
	while(1)
	{
		if((sz = read(0,buffer,255))<0)
		{
			perror("Could not read");
		}
		if(sz==0)
			break;
		if(write(1,buffer,sz)<0)
		{
			perror("Could not write");
		}
		sleep(1);
	}
	return 0;
}