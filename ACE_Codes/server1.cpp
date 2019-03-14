#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include "../cn.h"

int main(int argc, char const *argv[])
{
	ACE_INET_Addr addr(8000,INADDR_LOOPBACK);
	ACE_SOCK_Stream sfd;
	printf("hello\n");
	ACE_SOCK_Acceptor acc(addr);
	acc.accept(sfd);
	printf("Connected sucessfully..\n");
	char buffer[50];int sz;
	while(1)
	{
		if((sz = sfd.recv_n(buffer,50))<0)
		{
			perror("Could not read");
		}
		else if(sz==0)
		{
			sfd.close();
			exit(0);
		}
		else
		{
			buffer[sz] = '\0';
			printf("No of bytes read: %d\nActually Read: %d\n",sz,(int)strlen(buffer));
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
					buffer[i] = buffer[i]-'a'+'A';
			}
			if(sfd.send_n(buffer,50)<0)
			{
				perror("Could not send");
			}
		}
	}
	return 0;
}