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
		x=y;
		//free(y);
	}
	/*for(int i=1;i<=10;i++)
	{
		int *z;
		z=malloc(sizeof(int));
		break;
	}*/
	return 0;
}
