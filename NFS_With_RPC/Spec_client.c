/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "Spec.h"


void
nfsprog_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	Pair  register_1_arg;
	int  *result_2;
	Pair  login_1_arg;
	lsresult  *result_3;
	input  ls_1_arg;
	int  *result_4;
	input  cd_1_arg;
	int  *result_5;
	input  mkdir_1_arg;
	int  *result_6;
	input  mkfile_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, NFSPROG, NFSVERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	char prompt[256];int ch;
	printf("What do you want?\n1. For Register\n2. For login\n");
	scanf("%d",&ch);
	while(getchar()!='\n');
	Pair p;
	p.username = (char*)malloc(sizeof(char)*50);
	p.password = (char*)malloc(sizeof(char)*20);
	printf("Enter Username: \n");
	scanf("%s",p.username);
	printf("Enter password: \n");
	scanf("%s",p.password);
	if(ch==1)
	{
		result_1 = register_1(&register_1_arg, clnt);
		if (result_1 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}
	}
	else
	{
		result_2 = login_1(&login_1_arg, clnt);
		if (result_2 == (int*) NULL) {
			clnt_perror (clnt, "call failed");
		}
		if(*result_2==-1)
		{
			printf("Could not log in\n");exit(0);
		}
	}
	strcpy(prompt,p.username);
	while(1)
	{
		printf("%s>",prompt);
		fflush(stdout);
		result_3 = ls_1(&ls_1_arg, clnt);
		if (result_3 == (lsresult *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		result_4 = cd_1(&cd_1_arg, clnt);
		if (result_4 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		result_5 = mkdir_1(&mkdir_1_arg, clnt);
		if (result_5 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		result_6 = mkfile_1(&mkfile_1_arg, clnt);
		if (result_6 == (int *) NULL) {
			clnt_perror (clnt, "call failed");
		}
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	nfsprog_1 (host);
exit (0);
}