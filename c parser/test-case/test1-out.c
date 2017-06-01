int main()
{
	int **z=malloc(sizeof(int*));
	*z=malloc(sizeof(int));
	**z=0;
free(*z);
free(z);
	return 0;
}
