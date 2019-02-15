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

int main()
{
	struct Packet buff;int sz;
	while(1)
	{
		if((sz = read(0,&buff,sizeof(struct Packet)))<0)
		{
			perror("Reading error!");
			exit(0);
		}
		if(strcmp(buff.Opt.buffer,"bye")==0)
			break;
		int n = strlen(buff.Opt.buffer);
		for(int i=0;i<n;i++)
		{
			if(buff.Opt.buffer[i]>='a'&&buff.Opt.buffer[i]<='z')
			{
				buff.Opt.buffer[i] = buff.Opt.buffer[i]-'a'+'A';
			}
		}
		if(write(1,&buff,sizeof(struct Packet))<0)
		{
			perror("Could not send");
		}
	}
	return 0;
}