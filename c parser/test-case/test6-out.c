int *x,*y,*z;
int foo2(int *x,int *y)
{
	return *y;
}
int foo1(int *x,int *y)
{
	
	if ((*x)*2<*y) 
	{
		return *x;
	}
	else 
	{
/*Created by our project*/	free(x);	/************************/
		return foo2(x,*y+1);
	}
	
}
int foo(int *a,int *b,int *c)
{
	if (*a>*c) 
	{
		return foo1(a,c);
	}
	else
	{
/*Created by our project*/	free(a);	/************************/
		return foo2(b,c);
	}
}
int main()
{
	x=malloc(sizeof(int));
	y=malloc(sizeof(int));
	int n=10;
	*x=3;*y=4;z=&n;
/*Created by our project*/	free(y);	/************************/
	int ans=foo(x,y,z);
}
/*Warning created by our project
	After fixing, malloc in line34 may still cause memory leaking.
		Dereference when return
*******************************/
