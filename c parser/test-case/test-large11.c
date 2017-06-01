void proc6(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc5(int *x0)
{
}

void proc4(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc3(int *x0)
{
}

void proc2(int *x0)
{
	proc3(x0);
	proc6(x0);
}

void proc1(int *x0)
{
	proc4(x0);
}

void proc0(int *x0)
{
	proc1(x0);
	proc2(x0);
	proc5(x0);
}

int main()
{
	int *x0 = malloc(sizeof(int));
	proc0(x0);
	return 0;
}
