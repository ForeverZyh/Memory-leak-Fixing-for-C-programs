#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utility>
using namespace std;
struct node
{
	string str;
	int tag,ln;
	node * fa;
	vector<node*> son;
	node();
	node(char *s);
};
struct CFGnode
{
	vector<CFGnode*> succ,prev;
	CFGnode()
	{
		succ.clear();
		prev.clear();
	}
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);