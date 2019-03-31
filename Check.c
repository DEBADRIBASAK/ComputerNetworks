#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *b;
	b = (char*)malloc(sizeof(char)*37);
	scanf("%s",b);
	printf("No: %d\n",*(int*)(b+strlen(b)));
	return 0;
}
