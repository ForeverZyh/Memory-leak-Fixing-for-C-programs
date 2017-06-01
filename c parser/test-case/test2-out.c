int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
free(x);
		int *z=malloc(sizeof(int));
		*z=*y;
free(y);
free(z);
	}
	else
	{
free(y);
free(x);
		x=y;
		//free(y);
	}
	return 0;
}
