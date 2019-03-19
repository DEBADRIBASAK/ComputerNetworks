#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>

struct lnode
{
	char name[256];
	struct lnode* next;
};

struct command
{
	char comm[20];
	char args[10];
};


int main(int argc, char const *argv[])
{
	char current_directory[50];
	strcpy(current_directory,"./");
	strcat(current_directory,argv[1]);
	chdir(current_directory);
	strcpy(current_directory,argv[1]);
	int sz;
	//strcat(current_directory,"/");
	struct command c;
	while(1)
	{
		if((sz = read(0,&c,sizeof(struct command)))<0)
		{
			perror("Could not read");
		}
		else
		{
			//command[sz] = '\0';
			int temp;
			if(strcmp(c.comm,"cd")==0)
			{
				if(chdir(c.args)<0)
				{
					temp = 0;
				}
				else
				{
					if(strcmp(c.args,".."))
					{
						strcat(current_directory,"/");
						strcat(current_directory,c.args);
					}
					else
					{
						int len = strlen(current_directory);
						while(current_directory[len]!='/')
							len--;
						current_directory[len] = '\0';
					}
					temp = 1;
				}
				write(1,&temp,sizeof(int));
				write(1,current_directory,strlen(current_directory));
			}
			else if(strcmp(c.comm,"ls")==0)
			{
				DIR* drptr = opendir(".");
				if(drptr==NULL)
				{
					perror("Could not open");
				}
				else
				{
					struct dirent* drnt;
					struct lnode* head = NULL,*tail = NULL;
					drnt = readdir(drptr);
					while(drnt!=NULL)
					{
						//write(11,drnt->d_name,strlen(drnt->d_name));
						//write(11,"\n",1);
						if(strcmp(drnt->d_name,".")!=0&&strcmp(drnt->d_name,"..")!=0)
						{
							write(1,drnt->d_name,strlen(drnt->d_name));
						}
						// else
						// {
						// 	write(1,"over",4);
						// 	break;
						// }
						drnt = readdir(drptr);
						sleep(1);
					}
					write(1,"over",4);
				}
			}
			else if(strcmp(c.comm,"file")==0)
			{
				int fd = open(c.args,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR),temp;
				if(fd<0)
					temp = 0;
				else
					temp = 1;
				write(1,&temp,sizeof(int));
			}
			else if(strcmp(c.comm,"dir")==0)
			{
				int temp;
				if(mkdir(c.args,0777)<0)
				{
					temp = 0; 
				}
				else
					temp = 1;
				write(1,&temp,sizeof(int));
			}
			else
			{
				close(0);
				close(1);
				exit(0);
			}
		}
	}
	return 0;
}