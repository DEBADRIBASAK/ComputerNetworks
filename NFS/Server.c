// an NFS Server withour RPC!!
#include "../cn.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// this is basically a multiservice concurrent server!

struct Pair
{
	char username[50];
	char password[50];
};

struct RegOrLogIn
{
	int func;
	struct Pair p;
};

struct Pair client_list[255];
int ind = 0;
void init()
{
	FILE* fp = fopen("./database.txt","r");
	if(fp==NULL)
	{
		perror("Database not connected");
		exit(0);
	}
	char buffer[110];int sz;
	char *tok;
	//fscanf(fp,"%[^\n]s",buffer);
	//while(fgetc(fp)!='\n');
	sz = fscanf(fp,"%[^\n]s",buffer);
	while(sz>0)
	{
		while(fgetc(fp)!='\n');
		tok = strtok(buffer," ");
		printf("username: %s\n",tok);
		strcpy(client_list[ind].username,tok);
		tok = strtok(NULL," ");
		strcpy(client_list[ind].password,tok);
		printf("password: %s\n",tok);
		ind++;
		sz = fscanf(fp,"%[^\n]s",buffer);
	}
	fclose(fp);
}



void register_client(struct Pair p)
{
	strcpy(client_list[ind].username,p.username);
	strcpy(client_list[ind].password,p.password);
	FILE* fp = fopen("database.txt","w+");
	if(fp==NULL)
	{
		perror("Could not open");
	}
	fprintf(fp,"%s %s\n",p.username,p.password);
	fclose(fp);
	ind++;
}

int log_in(struct Pair p)
{
	for(int i=0;i<ind;i++)
	{
		printf("%s %s\n",client_list[i].username,client_list[i].password);
		if(strcmp(client_list[i].username,p.username)==0&&strcmp(client_list[i].password,p.password)==0)
		{
			return 1;
		}
	}
	return 0;
}


int main(int argc, char const *argv[])
{
	dup2(1,11);
	init(); //reading the database
	int sfd = socket(AF_INET,SOCK_STREAM,0),nsfd;
	if(sfd<0)
	{
		perror("Socket not created");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8000);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");
		exit(0);
	}
	if(listen(sfd,10)<0)
	{
		perror("Could not listen");
		exit(0);
	}
	struct RegOrLogIn A;
	while(1)
	{
		nsfd = accept(sfd,NULL,NULL);
		if(nsfd<0)
		{
			perror("Could not accept");
			exit(0);
		}
		if(recv(nsfd,&A,sizeof(struct RegOrLogIn),0)<0)
		{
			perror("Could not receive");
		}
		else
		{
			if(A.func==1) //register
			{
				register_client(A.p);
				if(mkdir(A.p.username,0777)==0)
				{
					char dir[50];
					strcpy(dir,A.p.username);
					strcat(dir,"/");
					send(nsfd,"Successfully registered",23,0);
					send(nsfd,dir,strlen(dir),0);
				}
			}
			else
			{
				int temp;
				if(!log_in(A.p))
				{
					temp = 0;
					send(nsfd,&temp,sizeof(int),0);
					sleep(1);
					close(nsfd);
				}
				else
				{
					char dir[50];
					strcpy(dir,A.p.username);
					strcat(dir,"/");
					temp = 1;
					send(nsfd,&temp,sizeof(int),0);
					send(nsfd,dir,strlen(dir),0);
				}
			}
			pid_t c = fork();
			if(c<0)
			{
				perror("Forking error");
			}
			else if(c>0)
			{
				close(nsfd);
			}
			else
			{
				close(sfd);
				dup2(nsfd,0);
				dup2(nsfd,1);
				execlp("./Service","./Service",A.p.username,A.p.password,NULL);
			}
		}
	}
	return 0;
}