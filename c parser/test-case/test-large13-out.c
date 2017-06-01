void proc9(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x9);
	free(x10);
	free(x17);
	x1 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
	x10 = malloc(sizeof(int));
	x15 = malloc(sizeof(int));
	x19 = malloc(sizeof(int));
}

void proc8(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x2);
	free(x15);
	free(x16);
	free(x18);
	x1 = malloc(sizeof(int));
	x18 = malloc(sizeof(int));
}

void proc7(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x0);
	free(x8);
	x0 = malloc(sizeof(int));
	x5 = malloc(sizeof(int));
	x19 = malloc(sizeof(int));
	proc9(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
}

void proc6(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x10);
	free(x15);
	x1 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
	x11 = malloc(sizeof(int));
	x14 = malloc(sizeof(int));
}

void proc5(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x7);
	free(x13);
	x0 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
	x8 = malloc(sizeof(int));
	x10 = malloc(sizeof(int));
	proc7(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
}

void proc4(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x0);
	free(x9);
	free(x12);
	free(x16);
	free(x18);
	free(x19);
	x17 = malloc(sizeof(int));
	x18 = malloc(sizeof(int));
}

void proc3(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x4);
	free(x15);
	x4 = malloc(sizeof(int));
	x5 = malloc(sizeof(int));
	x9 = malloc(sizeof(int));
	x11 = malloc(sizeof(int));
	x14 = malloc(sizeof(int));
	x15 = malloc(sizeof(int));
	proc6(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
}

void proc2(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x6);
	free(x16);
	free(x19);
	x15 = malloc(sizeof(int));
	x16 = malloc(sizeof(int));
}

void proc1(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x6);
	free(x11);
	x2 = malloc(sizeof(int));
	x5 = malloc(sizeof(int));
	x9 = malloc(sizeof(int));
	x12 = malloc(sizeof(int));
	proc2(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
	proc3(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
	proc4(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
	proc8(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
}

void proc0(int *x0,int *x1,int *x2,int *x3,int *x4,int *x5,int *x6,int *x7,int *x8,int *x9,int *x10,int *x11,int *x12,int *x13,int *x14,int *x15,int *x16,int *x17,int *x18,int *x19)
{
	free(x2);
	x0 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
	x6 = malloc(sizeof(int));
	x9 = malloc(sizeof(int));
	x11 = malloc(sizeof(int));
	x16 = malloc(sizeof(int));
	x17 = malloc(sizeof(int));
	proc1(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
}

int main()
{
	int *x0 = malloc(sizeof(int));
	int *x1 = malloc(sizeof(int));
free(x1);
	int *x2 = malloc(sizeof(int));
	int *x3 = malloc(sizeof(int));
free(x3);
	int *x4 = malloc(sizeof(int));
	int *x5 = malloc(sizeof(int));
free(x5);
	int *x6 = malloc(sizeof(int));
	int *x7 = malloc(sizeof(int));
free(x7);
	int *x8 = malloc(sizeof(int));
free(x8);
	int *x9 = malloc(sizeof(int));
	int *x10 = malloc(sizeof(int));
	int *x11 = malloc(sizeof(int));
	int *x12 = malloc(sizeof(int));
	int *x13 = malloc(sizeof(int));
free(x13);
	int *x14 = malloc(sizeof(int));
free(x14);
	int *x15 = malloc(sizeof(int));
	int *x16 = malloc(sizeof(int));
	int *x17 = malloc(sizeof(int));
free(x17);
	int *x18 = malloc(sizeof(int));
	int *x19 = malloc(sizeof(int));
	proc0(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19);
	return 0;
}
