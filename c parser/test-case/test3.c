int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
		int *z=malloc(sizeof(int));
		*z=*y;
	}
	else
	{
		x=y;
		free(y);
	}
	return 0;
}
