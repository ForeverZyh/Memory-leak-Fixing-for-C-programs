void proc0(int *x0)
{
	proc1(x0)
	proc2(x0)
	proc3(x0)
	proc19(x0)
}

void proc1(int *x0)
{
}

void proc2(int *x0)
{
	free(x0);
	proc4(x0)
	proc10(x0)
	proc11(x0)
	proc45(x0)
}

void proc3(int *x0)
{
	proc5(x0)
	proc8(x0)
	proc15(x0)
}

void proc4(int *x0)
{
	proc6(x0)
	proc20(x0)
}

void proc5(int *x0)
{
	x0 = malloc(sizeof(int));
	proc7(x0)
	proc14(x0)
	proc24(x0)
}

void proc6(int *x0)
{
	x0 = malloc(sizeof(int));
	proc17(x0)
	proc39(x0)
}

void proc7(int *x0)
{
	proc9(x0)
	proc25(x0)
}

void proc8(int *x0)
{
	x0 = malloc(sizeof(int));
	proc43(x0)
}

void proc9(int *x0)
{
	x0 = malloc(sizeof(int));
	proc18(x0)
	proc27(x0)
}

void proc10(int *x0)
{
	x0 = malloc(sizeof(int));
	proc13(x0)
	proc30(x0)
}

void proc11(int *x0)
{
	proc12(x0)
}

void proc12(int *x0)
{
	free(x0);
	x0 = malloc(sizeof(int));
	proc33(x0)
}

void proc13(int *x0)
{
	proc16(x0)
	proc32(x0)
}

void proc14(int *x0)
{
	proc29(x0)
}

void proc15(int *x0)
{
	proc22(x0)
	proc26(x0)
}

void proc16(int *x0)
{
	proc31(x0)
}

void proc17(int *x0)
{
	proc28(x0)
}

void proc18(int *x0)
{
}

void proc19(int *x0)
{
	proc44(x0)
}

void proc20(int *x0)
{
	proc21(x0)
	proc40(x0)
}

void proc21(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc22(int *x0)
{
	proc23(x0)
}

void proc23(int *x0)
{
	free(x0);
	x0 = malloc(sizeof(int));
}

void proc24(int *x0)
{
}

void proc25(int *x0)
{
	x0 = malloc(sizeof(int));
	proc37(x0)
	proc38(x0)
}

void proc26(int *x0)
{
}

void proc27(int *x0)
{
}

void proc28(int *x0)
{
}

void proc29(int *x0)
{
}

void proc30(int *x0)
{
}

void proc31(int *x0)
{
	proc36(x0)
	proc46(x0)
	proc48(x0)
}

void proc32(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc33(int *x0)
{
	proc34(x0)
	proc35(x0)
}

void proc34(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc35(int *x0)
{
	proc42(x0)
	proc47(x0)
}

void proc36(int *x0)
{
}

void proc37(int *x0)
{
}

void proc38(int *x0)
{
}

void proc39(int *x0)
{
}

void proc40(int *x0)
{
	proc41(x0)
}

void proc41(int *x0)
{
}

void proc42(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc43(int *x0)
{
	x0 = malloc(sizeof(int));
}

void proc44(int *x0)
{
}

void proc45(int *x0)
{
	free(x0);
}

void proc46(int *x0)
{
}

void proc47(int *x0)
{
	proc49(x0)
}

void proc48(int *x0)
{
}

void proc49(int *x0)
{
	free(x0);
	x0 = malloc(sizeof(int));
}

int main()
{
	int *x0 = malloc(sizeof(int));
	return 0;
}
