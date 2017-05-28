int main()
{
	int *x=malloc(sizeof(int)),*y;
	y=x;
	free(x);
	return 0;
}
