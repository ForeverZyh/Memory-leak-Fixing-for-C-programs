int main()
{
	int **z=malloc(sizeof(int*));
	*z=malloc(sizeof(int));
	**z=0;
/*Created by our project*/	free(*z);	/************************/
/*Created by our project*/	free(z);	/************************/
	return 0;
}
