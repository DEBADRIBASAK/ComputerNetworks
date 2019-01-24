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

struct set
{
	int sz;
	char* ele;
};

struct Triplet
{
	char c;
	int x,y;
}; // stores the character along with the coordinates

struct queue
{
	int f,r;
	struct Triplet ele[MAX_SIZE];
};

struct queue q;

void enqueue(char c,int x,int y)
{
	if(q.f==-1)
		q.f = 0;
	q.r = (q.r+1)%MAX_SIZE;
	q.ele[q.r].c = c;
	q.ele[q.r].x = x;
	q.ele[q.r].y = y;
	//q.ele[q.r].z = z;
}

struct Triplet dequeue()
{
	struct Triplet rtn = q.ele[q.f];
	q.f = (q.f+1)%MAX_SIZE;
	if(q.f==(q.r+1)%MAX_SIZE)
		q.f = q.r = -1;
	return rtn;
}

int present(struct set* m,char c)
{
	int i;
	for(i=0;i<m->sz;i++)
	{
		if(m->ele[i]==c)
			return 1;
	}
	return 0;
}

void find(struct set* m,char* a,int first)
{
	int i,j;
	if(first)
	{
		m->sz = 0;
		m->ele = (char*)malloc(sizeof(char)*n);
	}
	for(i=0;i<n;i++)
	{
		if(strcmp(r[i].der,a)==0)
		{
			if(first||(!first&&!present(m,r[i].var[0])))
			m->ele[m->sz++] = r[i].var[0];
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
			helper[0] = m2->ele[i];
			helper[1] = m3->ele[j];
			helper[2] = '\0';
			find(m1,helper,0);
		}
	}
}



void create_array(struct set*** m,int i,int j,int s)
{
	int i1,i2,k,d=1;
	(*m)[i][j].sz = 0;
	(*m)[i][j].ele = NULL;
	(*m)[i][j].ele = (char*)malloc(sizeof(char)*n);
	for(i1=s-1;i1>i;i1--)
	{
		cross_product(&((*m)[i][j]),&((*m)[i1][j]),&((*m)[i+d][j+d]));
		d++;
	}
}


int find1(char c,char* a)
{
	int i;
	for(i=0;i<n;i++)
	{
		if(strcmp(r[i].der,a)==0&&r[i].var[0]==c)
		{
			return 1;
		}
	}
	return 0;
}

void print(struct Triplet* arr,int m)
{
	int i;
	for(i=0;i<m;i++)
	{
		printf("%c",arr[i].c);
	}
}

void print_sequence(struct set** m,char* a,int l)
{
	int i,j,k,d;
	/*q.f = q.r = -1;
	enqueue('S',0,0);
	enqueue('|',-1,-1); // this is a special symbol
	struct Triplet helper,inp;
	char arr[l+1];int ind=0,ps = 1;
	arr[0] = 'S';
	arr[1] = '\0';*/
	struct Triplet arr[l];
	int ind = 0,ps = 1;
	arr[0].c = 'S';
	arr[0].y = arr[0].x = 0;
	printf("S --> ");
	while(ind<l)
	{
		//printf("*");
		// helper = dequeue();
		// if(helper.c=='|')
		// {
		// 	if(q.f!=-1||q.r!=-1)
		// 	{
		// 		enqueue('|',-1,-1);
		// 	}
		// 	ind = (ind+1)%ps;
		// }
		struct Triplet helper;
		helper = arr[ind];
		if(helper.x<l-1)
		{
			d = 1;
			char dummy[3];
			int f = 0;
			for(i=l-1;i>helper.x&&!f;i--)
			{
				//printf("sizes = %d %d\n",m[i][helper.y].sz,m[helper.x+d][helper.y+d].sz);
				for(j=0;j<m[i][helper.y].sz&&!f;j++)
				{
					for(k=0;k<m[helper.x+d][helper.y+d].sz&&!f;k++)
					{
						dummy[0] = m[i][helper.y].ele[j];
						dummy[1] = m[helper.x+d][helper.y+d].ele[k];
						dummy[2] = '\0';
						if(find1(helper.c,dummy))
						{
						//	printf("found dummy = %s\n",dummy);
							f = 1;
							//enqueue(dummy[0],i,helper.y);
							//enqueue(dummy[1],helper.x+d,helper.y+d);
							int j1 = ps;
							while(j1>ind+1)
							{
								arr[j1] = arr[j1-1];
								j1--;
							}
							arr[ind].c = dummy[0];
							arr[ind].x = i;
							arr[ind].y = helper.y;
							arr[ind+1].c = dummy[1];
							arr[ind+1].x = helper.x+d;
							arr[ind+1].y = helper.y+d;
							ps++;
							//arr[ps] = '\0';
						}
					}
				}
				d++;
			}
			print(arr,ps);
			printf(" --> ");
		}
		else
		{
			arr[ind].c = a[helper.y];
			//arr[ps] = '\0';
			ind = ind+1;
			print(arr,ps);
			printf(" --> ");
		}
		
	}
	printf("%s\n",a);
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
		find(&mat[m-1][i],helper,1);
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
				printf("%c,",mat[i][j].ele[k]);
			}
			if(mat[i][j].sz==0)
				printf(" @ ");
			printf(" | ");
		}
		printf("\n");
	}
	if(present(&mat[0][0],'S'))
	{
		printf("\nThe string belongs to this CFG\n\n");
		print_sequence(mat,input,m);
	}
	else
	{
		printf("The string doesn't belong to this CFG\n");
	}

	// printing the derivation sequence

	return 0;
}
