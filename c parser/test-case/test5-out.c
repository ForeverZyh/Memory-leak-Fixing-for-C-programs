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
/*Created by our project*/	free(x);	/************************/
}
/*Warning created by our project
	After fixing, malloc in line12 may still cause memory leaking.
	After fixing, malloc in line9 may still cause memory leaking.
*******************************/
