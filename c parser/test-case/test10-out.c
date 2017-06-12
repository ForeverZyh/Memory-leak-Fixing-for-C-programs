int *x;
int a()
{
	
}
int b()
{
	a();
}
int main()
{
	x=malloc(sizeof(int));
	a();
	*x=1;
/*Created by our project*/	free(x);	/************************/
	b();
	return 0;
}
