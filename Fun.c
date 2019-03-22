#include <stdio.h>
#include <stdlib.h>
#define RED "\033[0;31m"
#define BOLDRED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLDGREEN "\033[1;32m"
#define YELLOW "\033[0;33m"
#define BOLDYELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define BOLDBLUE "\033[1;34m"
#define PURPLE "\033[0;35m"
#define BOLDPURPLE "\033[1;35m"
#define CYAN "\033[0;36m"
#define BOLDCYAN "\033[1;36m"
#define RESET "\033[0m"

int main()
{
	printf(RED);
	printf("Hello World\n");
	printf(BOLDRED);
	printf("HELLO WORLD\n");
	printf(GREEN);
	printf("Hello World\n");
	printf(BOLDGREEN);
	printf("HELLO WORLD\n");
	printf(YELLOW);
	printf("Hello World\n");
	printf(BOLDYELLOW);
	printf("HELLO WORLD\n");
	printf(BLUE);
	printf("Hello World\n");
	printf(BOLDBLUE);
	printf("HELLO WORLD\n");
	printf(PURPLE);
	printf("Hello World\n");
	printf(BOLDPURPLE);
	printf("HELLO WORLD\n");
	printf(CYAN);
	printf("Hello World\n");
	printf(BOLDCYAN);
	printf("HELLO WORLD\n");
	//printf(RESET);
	return 0;
}
