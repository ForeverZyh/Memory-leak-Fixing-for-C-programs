int a;
int main()
{
	unsigned int *x=&a,*y,*z;
	x=y=z;
	if (x)
	{
		z=malloc(sizeof(unsigned int));
	}
	else
	{
		return 0;
	}
}
