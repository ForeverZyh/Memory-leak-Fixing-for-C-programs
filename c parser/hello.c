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