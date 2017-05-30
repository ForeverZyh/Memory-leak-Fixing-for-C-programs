void proc0(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x3 = malloc(sizeof(int));
	proc1(x0,x1,x2,x3,x4)
	proc2(x0,x1,x2,x3,x4)
}

void proc1(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x2 = malloc(sizeof(int));
	proc4(x0,x1,x2,x3,x4)
	proc8(x0,x1,x2,x3,x4)
	proc15(x0,x1,x2,x3,x4)
	proc25(x0,x1,x2,x3,x4)
}

void proc2(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x3);
	x0 = malloc(sizeof(int));
	proc3(x0,x1,x2,x3,x4)
	proc7(x0,x1,x2,x3,x4)
}

void proc3(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc5(x0,x1,x2,x3,x4)
	proc23(x0,x1,x2,x3,x4)
	proc45(x0,x1,x2,x3,x4)
}

void proc4(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc6(x0,x1,x2,x3,x4)
	proc37(x0,x1,x2,x3,x4)
	proc42(x0,x1,x2,x3,x4)
}

void proc5(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
	proc11(x0,x1,x2,x3,x4)
	proc12(x0,x1,x2,x3,x4)
}

void proc6(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	free(x4);
	x0 = malloc(sizeof(int));
	proc10(x0,x1,x2,x3,x4)
	proc28(x0,x1,x2,x3,x4)
	proc48(x0,x1,x2,x3,x4)
}

void proc7(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
	x2 = malloc(sizeof(int));
	proc9(x0,x1,x2,x3,x4)
	proc16(x0,x1,x2,x3,x4)
	proc24(x0,x1,x2,x3,x4)
	proc29(x0,x1,x2,x3,x4)
	proc44(x0,x1,x2,x3,x4)
}

void proc8(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
	proc13(x0,x1,x2,x3,x4)
	proc18(x0,x1,x2,x3,x4)
	proc32(x0,x1,x2,x3,x4)
}

void proc9(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc30(x0,x1,x2,x3,x4)
}

void proc10(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
	proc31(x0,x1,x2,x3,x4)
	proc35(x0,x1,x2,x3,x4)
}

void proc11(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc20(x0,x1,x2,x3,x4)
	proc47(x0,x1,x2,x3,x4)
}

void proc12(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x3 = malloc(sizeof(int));
	proc17(x0,x1,x2,x3,x4)
}

void proc13(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	x4 = malloc(sizeof(int));
	proc14(x0,x1,x2,x3,x4)
	proc36(x0,x1,x2,x3,x4)
}

void proc14(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	free(x1);
}

void proc15(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	proc21(x0,x1,x2,x3,x4)
}

void proc16(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
	x2 = malloc(sizeof(int));
	proc41(x0,x1,x2,x3,x4)
	proc49(x0,x1,x2,x3,x4)
}

void proc17(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x4 = malloc(sizeof(int));
	proc19(x0,x1,x2,x3,x4)
	proc22(x0,x1,x2,x3,x4)
	proc26(x0,x1,x2,x3,x4)
}

void proc18(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
}

void proc19(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
}

void proc20(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x2);
}

void proc21(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc34(x0,x1,x2,x3,x4)
	proc38(x0,x1,x2,x3,x4)
}

void proc22(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x3 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
}

void proc23(int *x0,int *x1,int *x2,int *x3,int *x4)
{
}

void proc24(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x3 = malloc(sizeof(int));
	proc27(x0,x1,x2,x3,x4)
}

void proc25(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x1);
	x1 = malloc(sizeof(int));
	proc33(x0,x1,x2,x3,x4)
}

void proc26(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	x2 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
}

void proc27(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x2 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
}

void proc28(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x2);
	x3 = malloc(sizeof(int));
}

void proc29(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
}

void proc30(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
}

void proc31(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc43(x0,x1,x2,x3,x4)
}

void proc32(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	proc40(x0,x1,x2,x3,x4)
}

void proc33(int *x0,int *x1,int *x2,int *x3,int *x4)
{
}

void proc34(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x1);
	x1 = malloc(sizeof(int));
}

void proc35(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x1);
	x0 = malloc(sizeof(int));
	proc39(x0,x1,x2,x3,x4)
}

void proc36(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x3);
	x4 = malloc(sizeof(int));
}

void proc37(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x1 = malloc(sizeof(int));
}

void proc38(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x2);
	x4 = malloc(sizeof(int));
	proc46(x0,x1,x2,x3,x4)
}

void proc39(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
}

void proc40(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x2 = malloc(sizeof(int));
}

void proc41(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x3);
	x1 = malloc(sizeof(int));
	x2 = malloc(sizeof(int));
	x3 = malloc(sizeof(int));
}

void proc42(int *x0,int *x1,int *x2,int *x3,int *x4)
{
}

void proc43(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
}

void proc44(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x1);
}

void proc45(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x3 = malloc(sizeof(int));
	x4 = malloc(sizeof(int));
}

void proc46(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	x0 = malloc(sizeof(int));
	x1 = malloc(sizeof(int));
}

void proc47(int *x0,int *x1,int *x2,int *x3,int *x4)
{
}

void proc48(int *x0,int *x1,int *x2,int *x3,int *x4)
{
}

void proc49(int *x0,int *x1,int *x2,int *x3,int *x4)
{
	free(x0);
	x0 = malloc(sizeof(int));
}

int main()
{
	int *x0 = malloc(sizeof(int));
	int *x1 = malloc(sizeof(int));
	int *x2 = malloc(sizeof(int));
	int *x3 = malloc(sizeof(int));
	int *x4 = malloc(sizeof(int));
	return 0;
}
