//this is a comment
int main()
{
	unsigned int *x;
	*x=1;
	unsigned int *p,*q=x;
	{
		int t=*x,p;
		p=x+1;
	}
	printf("%d Hello world!\n",*x);
	return 0;
}
