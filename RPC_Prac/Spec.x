struct Operation
{
	int op1;
	int op2;
	int op;	
};

program MATHPROG
{
	version INITIAL
	{
		int operate(Operation) = 1;
	} = 1;
} = 0x21111111;

