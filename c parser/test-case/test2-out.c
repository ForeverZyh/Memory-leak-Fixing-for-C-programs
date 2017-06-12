int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
/*Created by our project*/	free(x);	/************************/
		int *z=malloc(sizeof(int));
		*z=*y;
/*Created by our project*/	free(y);	/************************/
/*Created by our project*/	free(z);	/************************/
	}
	else
	{
/*Created by our project*/	free(y);	/************************/
/*Created by our project*/	free(x);	/************************/
		x=y;
		//free(y);
	}
	return 0;
}
