int *sum;
void foo(int x)
{
	*sum*=x;
	if (x==1)
	{
		return ;
	}
	foo(x-1);
}
int main()
{
	sum=malloc(sizeof(int));
	foo(10);
/*Created by our project*/	free(sum);	/************************/
	return 0;
}
