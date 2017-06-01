int main()
{
	int n=10;
	int *x=&n;
	for(int i=1;i<=n;i++)
	{
		int *x=malloc(sizeof(int));
free(x);
		for(int j=1;j<=n;j++)
		{
			int *x=malloc(sizeof(int));
free(x);
		}
	}
	return 0;
}

