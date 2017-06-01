void proc49(int *x0,int *x1)
{
}

void proc48(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc47(int *x0,int *x1)
{
}

void proc46(int *x0,int *x1)
{
	free(x1);
}

void proc45(int *x0,int *x1)
{
	free(x0);
	x1 = malloc(sizeof(int));
}

void proc44(int *x0,int *x1)
{
	free(x0);
}

void proc43(int *x0,int *x1)
{
	proc46(x0,x1);
}

void proc42(int *x0,int *x1)
{
}

void proc41(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc40(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc39(int *x0,int *x1)
{
}

void proc38(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc37(int *x0,int *x1)
{
}

void proc36(int *x0,int *x1)
{
}

void proc35(int *x0,int *x1)
{
	proc47(x0,x1);
}

void proc34(int *x0,int *x1)
{
	proc49(x0,x1);
}

void proc33(int *x0,int *x1)
{
	free(x1);
}

void proc32(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
	x1 = malloc(sizeof(int));
	proc41(x0,x1);
	proc45(x0,x1);
}

void proc31(int *x0,int *x1)
{
}

void proc30(int *x0,int *x1)
{
}

void proc29(int *x0,int *x1)
{
	proc31(x0,x1);
}

void proc28(int *x0,int *x1)
{
}

void proc27(int *x0,int *x1)
{
	free(x0);
}

void proc26(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc25(int *x0,int *x1)
{
	proc33(x0,x1);
}

void proc24(int *x0,int *x1)
{
	free(x0);
	proc28(x0,x1);
	proc36(x0,x1);
}

void proc23(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc22(int *x0,int *x1)
{
	proc37(x0,x1);
	proc39(x0,x1);
}

void proc21(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
}

void proc20(int *x0,int *x1)
{
}

void proc19(int *x0,int *x1)
{
	free(x0);
	free(x1);
	proc25(x0,x1);
}

void proc18(int *x0,int *x1)
{
	proc21(x0,x1);
	proc27(x0,x1);
	proc44(x0,x1);
}

void proc17(int *x0,int *x1)
{
	free(x1);
}

void proc16(int *x0,int *x1)
{
	x1 = malloc(sizeof(int));
}

void proc15(int *x0,int *x1)
{
	proc23(x0,x1);
	proc42(x0,x1);
}

void proc14(int *x0,int *x1)
{
	x1 = malloc(sizeof(int));
	proc20(x0,x1);
	proc26(x0,x1);
	proc43(x0,x1);
}

void proc13(int *x0,int *x1)
{
	x1 = malloc(sizeof(int));
	proc29(x0,x1);
}

void proc12(int *x0,int *x1)
{
	proc15(x0,x1);
}

void proc11(int *x0,int *x1)
{
	proc12(x0,x1);
	proc13(x0,x1);
	proc40(x0,x1);
}

void proc10(int *x0,int *x1)
{
	free(x1);
	proc11(x0,x1);
}

void proc9(int *x0,int *x1)
{
	free(x1);
	x1 = malloc(sizeof(int));
	proc10(x0,x1);
	proc19(x0,x1);
	proc35(x0,x1);
}

void proc8(int *x0,int *x1)
{
	proc14(x0,x1);
}

void proc7(int *x0,int *x1)
{
	proc32(x0,x1);
}

void proc6(int *x0,int *x1)
{
	x1 = malloc(sizeof(int));
	proc24(x0,x1);
}

void proc5(int *x0,int *x1)
{
	x0 = malloc(sizeof(int));
	proc8(x0,x1);
	proc34(x0,x1);
}

void proc4(int *x0,int *x1)
{
	proc9(x0,x1);
}

void proc3(int *x0,int *x1)
{
	x1 = malloc(sizeof(int));
	proc17(x0,x1);
}

void proc2(int *x0,int *x1)
{
	free(x0);
	x1 = malloc(sizeof(int));
	proc3(x0,x1);
	proc22(x0,x1);
}

void proc1(int *x0,int *x1)
{
	proc6(x0,x1);
	proc7(x0,x1);
	proc18(x0,x1);
	proc30(x0,x1);
}

void proc0(int *x0,int *x1)
{
	proc1(x0,x1);
	proc2(x0,x1);
	proc4(x0,x1);
	proc5(x0,x1);
	proc16(x0,x1);
	proc38(x0,x1);
	proc48(x0,x1);
}

int main()
{
	int *x0 = malloc(sizeof(int));
	int *x1 = malloc(sizeof(int));
	proc0(x0,x1);
	return 0;
}
