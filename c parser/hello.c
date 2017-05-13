int *x;
void foo(int* x,int *y)
{
	int t=*x;
	*x=*y;
	*y=t;
}
int* foo1(int*a,int*b)
{
	a=malloc(sizeof(int));
	foo(a,b);
	return a;
}
int main()
{
	int *y=malloc(sizeof(int));
	foo(x,y);
	int *x=malloc(sizeof(int)),*z=foo1(x,y);
	foo(foo1(z,x),y);
}
