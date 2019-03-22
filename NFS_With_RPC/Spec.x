typedef string filename<256>;
typedef string argument<20>;

struct input
{
	char command[20]; 
	argument arr[10];	
	int lenarr;
};

struct Pair
{
	string username<50>;
	string password<20>;	
};

struct lsresult
{
	filename arr[50];
	int num;
};

program NFSPROG
{
	version NFSVERSION
	{
		int register(Pair) = 1;
		int login(Pair) = 2;
		lsresult ls(input) = 3;
		int cd(input) = 4;
		int mkdir(input) = 5;
		int mkfile(input) = 6;
	} = 1;
} = 0x21111111;