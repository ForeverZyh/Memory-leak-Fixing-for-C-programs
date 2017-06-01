int fib(int n)
{
	int *ret;
	if (n==0)
	{
		return 1;
	}
	ret=malloc(sizeof(int));
	if (n==1)
	{
free(ret);
		return 1;
	}
	*ret=fib(n-1) + fib(n-2);
	return *ret;
}
int main()
{
	fib(10);
	return 0;
}

