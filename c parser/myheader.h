#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
struct node
{
	string str;
	int tag;
	node * fa;
	vector<node*> son;
	node()
	{
		str="";tag=0;fa=NULL;
		son.clear();
	}
	node(char *s)
	{
		str=s;tag=0;fa=NULL;
		son.clear();
	}
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);