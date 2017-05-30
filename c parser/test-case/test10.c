nt main()
{
	int **z1, **z2, y, x;
	z1=malloc(sizeof(int*));
	z2=malloc(sizeof(int*));
	y=malloc(sizeof(int));
	x=5;
	*z1=y, *z2=y;
	*y=x;
	return 0;
}

