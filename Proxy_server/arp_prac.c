#include "../cn.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>

int main(int argc, char const *argv[])
{
	int rsfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))
	return 0;
}