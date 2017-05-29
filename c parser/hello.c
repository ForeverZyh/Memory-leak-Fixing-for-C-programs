int *sum;
void foo(int x)
{
	if (x<=1)
	{
		return ;
	}
	*sum*=x;
	foo(x-1);
}
int main()
{
	sum=malloc(sizeof(int));
	foo(10);
	return 0;
}