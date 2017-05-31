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
	b();
	return 0;
}
