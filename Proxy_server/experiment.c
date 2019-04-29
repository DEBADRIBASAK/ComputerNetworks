#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>

int main(int argc, char const *argv[])
{
	char name[100];
	strcpy(name,"www.geeksforgeeks.com");
	struct hostent* host;
	host = gethostbyname(name);
	if(host==NULL)
	{
		perror("Could not get host");exit(0);
	}
	else
	{
		printf("Name: %s\n",host->h_name);
		for(int i=0;i<host->h_length;i++)
		{
			printf("%d = %s\n",i,host->h_addr_list[i]);
		}
	}
	return 0;
}