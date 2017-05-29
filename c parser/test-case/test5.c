void swap(int*x,int*y)
{
	int *t=x;
	x=y;
	y=x;
}
int main()
{
	int *haha=malloc(sizeof(int));
	int *youcantcatchme=haha;
	*haha=1;
	int *x=malloc(sizeof(int));
	swap(x,haha);
	*youcantcatchme=0;
}
