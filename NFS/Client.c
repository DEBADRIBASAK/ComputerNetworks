#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <dirent.h>
struct Pair
{
	char username[50];
	char password[50];
};
struct lnode
{
	char name[256];
	struct lnode* next;
};
struct RegOrLogIn
{
	int func;
	struct Pair p;
};
struct command
{
	char comm[20];
	char args[10];
};

int main(int argc, char const *argv[])
{
	struct command c;
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("Could not create socket");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not connect");
		exit(0);
	}
	char buffer[100];
	char* tok;
	struct RegOrLogIn A;
	printf("What do you want?\n1. for register\n2. for log in\n");
	scanf("%d",&(A.func));
	getchar();
	char prompt1[100],prompt[100];int sz;
	struct lnode* l,*q;
	if(A.func==1)
	{
		printf("Enter username:\n");
		scanf("%s",A.p.username);
		printf("Enter password:\n");
		scanf("%s",A.p.password);
		if(send(sfd,&A,sizeof(struct RegOrLogIn),0)<0)
			perror("Could not send");
		else
		{
			sz = recv(sfd,prompt1,100,0);
			prompt1[sz] = '\0';
			if(strcmp(prompt1,"Successfully registered")==0)
			{
				//memset(prompt,0,100);
				sz = recv(sfd,prompt,100,0);
				prompt[sz] = '\0';
			}
		}
	}
	else
	{
		int temp,tries = 0;
		do
		{
			printf("Enter username:\n");
			scanf("%s",A.p.username);
			printf("Enter password:\n");
			scanf("%s",A.p.password);
			printf("Username: %s Password: %s\n",A.p.username,A.p.password);
			if(send(sfd,&A,sizeof(struct RegOrLogIn),0)<0)
				perror("Could not send");
			else
			{
				recv(sfd,&temp,sizeof(int),0);
			}
			if(!temp)
				printf("Log In failed!\n");
		}while(!temp&&(tries++)<3);
		if(!temp)
		{
			printf("3 tries failed: this incident will be reported!\n");
			exit(0);
		}
		sz = recv(sfd,prompt,100,0);
		prompt[sz] = '\0';
	}
	char cm[50];int temp;
	printf("\n");
	while(getchar()!='\n');
	while(1)
	{
		printf("%s>",prompt);
		fflush(stdout);
		scanf("%[^\n]s",cm);
		// printf("got it!\n");
		while(getchar()!='\n');
		tok = strtok(cm," ");
		strcpy(c.comm,tok);
		if(strcmp(tok,"cd")==0)
		{
			tok = strtok(NULL," ");
			strcpy(c.args,tok);
			send(sfd,&c,sizeof(struct command),0);
			recv(sfd,&temp,sizeof(int),0);
			if(!temp)
				perror("Could not change directory");
			memset(prompt,0,100);
			recv(sfd,prompt,100,0);
		}
		else if(strcmp(tok,"ls")==0)
		{
			strcpy(c.args," ");
			send(sfd,&c,sizeof(struct command),0);
			char response[256];
			while(1)
			{
				sz = recv(sfd,response,256,0);
				response[sz] = '\0';
				if(strcmp(response,"over"))
				{
					printf("%s\n",response);
				}
				else
					break;
			}
		}
		else if(strcmp(tok,"file")==0)
		{
			tok = strtok(NULL," ");
			strcpy(c.args,tok);
			send(sfd,&c,sizeof(struct command),0);
			recv(sfd,&temp,sizeof(int),0);
			if(!temp)
				perror("File creation failed");
			else
				printf("File created Successfully\n");
		}
		else if(strcmp(tok,"dir")==0)
		{
			tok = strtok(NULL," ");
			strcpy(c.args,tok);
			send(sfd,&c,sizeof(struct command),0);
			recv(sfd,&temp,sizeof(int),0);
			if(!temp)
				perror("Directory creation failed");
			else
				printf("Directory created Successfully\n");
		}
		else if(strcmp(tok,"clear")==0)
		{
			system("clear");
		}
		else
		{
			send(sfd,&c,sizeof(struct command),0);
			sleep(1);
			close(sfd);
			exit(0);
		}
	}
	return 0;
}