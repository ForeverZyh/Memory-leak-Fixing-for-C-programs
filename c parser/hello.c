int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	int a=1;
	*y=2;
	if (*y>a) 
		x=y;
	else
	{
		a++;
		y=malloc(sizeof(a));
		free(y);
	}
}
