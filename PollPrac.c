#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	FILE* fp[3];
	int i,fd[3];
	char buffer[5] = {'.','/','P','1','\0'};
	for(i=0;i<3;i++)
	{
		buffer[3] = (char)(i+1+'0');
		if(i==0||i==1)
		fp[i] = popen(buffer,"r");
		else
		fp[i] = popen(buffer,"w");
		if(fp[i]==NULL)
		{
			perror("popen failed");
		}
		else
		{
			printf("Success = %s\n",buffer);
			fd[i] = fileno(fp[i]);
		}
	}
	int saved_fd,r;
	//dup2(0,saved_fd);
	//dup2(fd[0],0);
	struct pollfd pf[3];

	dup2(fd[0],pf[0].fd);
	dup2(fd[1],pf[1].fd);
	dup2(fd[2],pf[2].fd);

	pf[0].events = POLLIN;
	pf[1].events = POLLIN;
	pf[2].events = POLLOUT;
	pf[0].revents = pf[1].revents = pf[2].revents = 0;

	char buffer1[21],buffer2[21];

	while(1)
	{
		//pf[0].revents = printf("*");
		r = poll(pf,(nfds_t)(3),0);
		if(r>0)
		{
			if(pf[0].revents&POLLIN)
			{
				int sz = read(fd[0],buffer1,15);
				//printf("size = %d\n",sz);
				buffer1[sz] = '\0';
				//printf("Here\n");
				write(fd[2],buffer1,strlen(buffer1));
				sleep(2);
			}
			if(pf[1].revents&POLLIN)
			{
				int sz = read(fd[1],buffer2,15);
				buffer2[sz] = '\0';
				printf("Inside parent = %s\n",buffer2);
			}
		}
	}

	return 0;
}