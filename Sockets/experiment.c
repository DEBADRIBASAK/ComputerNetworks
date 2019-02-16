#include "../cn.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <syslog.h>

int main(int argc, char const *argv[])
{
	unsigned char buff[INET_ADDRSTRLEN];
	int src = htonl(INADDR_LOOPBACK);
	printf("Number: %u\n",htonl(src));
	uint32_t dst;
	int domain = AF_INET;
	const char* add = inet_ntop(domain,&src,buff,sizeof(buff));
	if(add==NULL)
	{
		printf("Not in presentation format\n");
	}
	else
	{
		//printf("s = %d\nINADDR_LOOPBACK = %d\n",add,INADDR_LOOPBACK);
		printf("Address: %s\n",add);
	}
	if(inet_pton(AF_INET,"127.0.0.1",&dst))
	{
		printf("Number: %u\n",htonl(dst));
	}
	return 0;
}