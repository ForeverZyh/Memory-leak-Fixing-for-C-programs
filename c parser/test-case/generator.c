#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace std;

/*
 * VAR_NUM : number of different variables
 *
 * PROC_NUM : number of different procedures
 */
#define VAR_NUM 20

#define PROC_NUM 50


char arg_list[10000]={'(','\0'};
char arg_list2[10000]={'(','\0'};
char proc_list[1000][1000]={};
int fa[10000];
int main()
{
	srand(time(NULL));
	for(int i = 0; i < VAR_NUM; ++i)
		sprintf(arg_list + strlen(arg_list), "int *x%d,", i);//puts(arg_list);
	for(int i = 0; i < VAR_NUM; ++i)
		sprintf(arg_list2 + strlen(arg_list2), "x%d,", i);
	if (VAR_NUM != 0) arg_list[strlen(arg_list) - 1] = ')'; else arg_list[1] = ')';
	if (VAR_NUM != 0) arg_list2[strlen(arg_list2) - 1] = ')';else arg_list2[1] = ')';//puts(arg_list2);

	for(int proc_t = 0; proc_t < PROC_NUM; ++proc_t)
	{
		sprintf(proc_list[proc_t] + strlen(proc_list[proc_t]), "%s%d%s\n", "void proc", proc_t, arg_list);
		sprintf(proc_list[proc_t] + strlen(proc_list[proc_t]), "%s\n", "{");

		for(int i = 0; i < VAR_NUM; ++i)
			if (rand() % 100 < 10)
			{
				sprintf(proc_list[proc_t] + strlen(proc_list[proc_t]), "\t%s%d%s\n", "free(x", i, ");");
			}

		for(int i = 0; i < VAR_NUM; ++i)
			if (rand() % 100 < 20)
			{
				sprintf(proc_list[proc_t] + strlen(proc_list[proc_t]), "\t%s%d%s\n", "x", i, " = malloc(sizeof(int));");
			}
	}
	
	for(int proc_t = 1; proc_t < PROC_NUM; ++proc_t)
	{
		int fath = fa[proc_t] = rand() % proc_t;
		sprintf(proc_list[fath] + strlen(proc_list[fath]), "\t%s%d%s\n", "proc", proc_t, arg_list2);
	}
	
	for(int proc_t = 0; proc_t < PROC_NUM; ++proc_t)
	{
		sprintf(proc_list[proc_t] + strlen(proc_list[proc_t]), "%s\n", "}");
	}
	
	for(int proc_t = 0; proc_t < PROC_NUM; ++proc_t)
	{
		printf("%s\n", proc_list[proc_t]);
	}

	printf("%s\n", "int main()");
	printf("%s\n", "{");
	for(int i = 0; i < VAR_NUM; ++i)
	{
		printf("\t%s%d%s\n", "int *x", i, " = malloc(sizeof(int));");
	}
	printf("\t%s\n", "return 0;");
	printf("%s\n", "}");
	return 0;
}