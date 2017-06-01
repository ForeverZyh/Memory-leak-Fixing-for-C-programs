int main()
{
	int **z=malloc(sizeof(int*));
	*z=malloc(sizeof(int));
	**z=0;
	return 0;
}
