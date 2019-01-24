/* this is a basic implementation of CYK Algorithm */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 500

struct rules
{
	char *var,*der;
}*r;

int n;

struct Pair
{
	char st;
	int num;
};

struct set
{
	int sz;
	struct Pair* ele;
};

int present(struct set* m,char c)
{
	int i;
	for(i=0;i<m->sz;i++)
	{
		if(m->ele[i].st==c)
		{
			//m->ele[i].num++;
			return i+1;
		}
	}
	return 0;
}

void find(struct set* m,char* a,int first,int n1,int n2)
{
	int i,j;
	if(first)
	{
		m->sz = 0;
		m->ele = (struct Pair*)malloc(sizeof(struct Pair)*n);
		for(i=0;i<n;i++)
		{
			m->ele[i].num = 0;
		}
	}
	for(i=0;i<n;i++)
	{
		if(strcmp(r[i].der,a)==0)
		{
			if(first)//||())
			{
				m->ele[m->sz].num = 1;
				m->ele[m->sz++].st = r[i].var[0];
			}
			else// if(!first&&!present(m,r[i].var[0]))
			{
				int f = present(m,r[i].var[0]);
				if(f)
				{
					m->ele[f-1].num+=(n1*n2);
				}
				else
				{
					m->ele[m->sz].st = r[i].var[0];
					m->ele[m->sz++].num = (n1*n2);
				}
			}
		}
	}
}

void cross_product(struct set* m1,struct set* m2,struct set* m3)
{
	int i,j;
	char helper[3];
	for(i=0;i<m2->sz;i++)
	{
		for(j=0;j<m3->sz;j++)
		{
			helper[0] = m2->ele[i].st;
			helper[1] = m3->ele[j].st;
			helper[2] = '\0';
			find(m1,helper,0,m2->ele[i].num,m3->ele[i].num);
		}
	}
}



void create_array(struct set*** m,int i,int j,int s)
{
	int i1,i2,k,d=1;
	(*m)[i][j].sz = 0;
	(*m)[i][j].ele = NULL;
	(*m)[i][j].ele = (struct Pair*)malloc(sizeof(struct Pair)*n);
	for(i1=s-1;i1>i;i1--)
	{
		cross_product(&((*m)[i][j]),&((*m)[i1][j]),&((*m)[i+d][j+d]));
		d++;
	}
}

int main()
{
	int i,j,k;
	printf("Enter number of rules\n");
	scanf("%d",&n);
	r = NULL;
	r = (struct rules*)malloc(sizeof(struct rules)*n);
	char input[50];
	for(i=0;i<n;i++)
	{
		r[i].var = NULL;
		r[i].der = NULL;
		r[i].var = (char*)malloc(sizeof(char)*2);
		r[i].der = (char*)malloc(sizeof(char));
		scanf("%s",input);
		r[i].var[0] = input[0];
		strcpy(r[i].der,input+1);
	}
	/*for(i=0;i<n;i++)
	{
		printf("%s --> %s\n",r[i].var,r[i].der);
	}*/
	printf("Enter the string to be tested\n");
	scanf("%s",input);
	int m = strlen(input);
	struct set **mat; // the 3D table for storing all values
	mat = NULL;
	mat = (struct set**)malloc(sizeof(struct set*)*m);
	for(i=0;i<m;i++)
	{
		mat[i] = NULL;
		mat[i] = (struct set*)malloc(sizeof(struct set)*m);
	}

	// initializing the lower row
	char helper[2];
	for(i=0;i<m;i++)
	{
		helper[0] = input[i];
		helper[1] = '\0';
		mat[m-1][i].ele = NULL;
		find(&mat[m-1][i],helper,1,0,0);
	}

	// the crutial part!!

	for(i=m-2;i>=0;i--)
	{
		for(j=0;j<i+1;j++)
		{
			create_array(&mat,i,j,m);
		}
	}
	printf("The lookup table = \n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<i+1;j++)
		{
			for(k=0;k<mat[i][j].sz;k++)
			{
				printf("%c = %d,",mat[i][j].ele[k].st,mat[i][j].ele[k].num);
			}
			if(mat[i][j].sz==0)
				printf(" @ = 0");
			printf(" | ");
		}
		printf("\n");
	}
	int iii;
	if(iii = present(&mat[0][0],'S'))
	{
		printf("\nThe string belongs to this CFG\n\n");
		printf("No of sequences = %d\n",mat[0][0].ele[iii-1].num);
	}
	else
	{
		printf("The string doesn't belong to this CFG\n");
	}

	return 0;
}
