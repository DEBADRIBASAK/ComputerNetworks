#include <stdio.h>
#include <string.h>
#include "../cn.h"
struct triplet
{
	int a,b,c;
};

union ops
{
	struct triplet t;
	char buffer[50];
};

struct Packet
{
	int ind;
	union ops Opt;
};

void encrypt(char* s)
{
	for(int i=0;i<strlen(s);i++)
	{
		s[i] = s[i]-'a'+'1';
	}
}

int main(int argc, char const *argv[])
{
	struct Packet buff;
	while(1)
	{
		if(read(0,&buff,sizeof(struct Packet))<0)
		{
			perror("Could not read");
			exit(0);
		}
		if(strcmp(buff.Opt.buffer,"bye")==0)
			break;
		encrypt(buff.Opt.buffer);
		write(1,&buff,sizeof(struct Packet));
	}
	return 0;
}