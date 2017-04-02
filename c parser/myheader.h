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
	int tag,ln;
	node * fa;
	vector<node*> son;
	node();
	node(char *s);
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);