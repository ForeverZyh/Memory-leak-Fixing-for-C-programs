int main()
{
	int b=10;
	int *x=&b;
	for(int i=1;i<=b;i++)
	{
		int *z=malloc(sizeof(int));
		*z=*x+i;
		if (*z>i*2)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	return 0;
}
