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
	node(const char *s);
};
struct pointer
{
	int id;
	vector<bool> unary;
	pointer()
	{
		id=0;unary.clear();
	}
	pointer(int x)
	{
		id=x;unary.clear();
	}
};
struct expr
{
	vector<pointer> def,use;
	vector<pair<pointer,pointer> > pure;
	expr()
	{
		def.clear();use.clear();pure.clear();
	}
	void merge(const expr&it)
	{
		def.insert(def.end(),it.def.begin(),it.def.end());
		use.insert(use.end(),it.use.begin(),it.use.end());
		pure.insert(pure.end(),it.pure.begin(),it.pure.end());
	}
};
struct CFGnode
{
	vector<CFGnode*> succ,prev;
	expr defuse;
	CFGnode():defuse()
	{
		succ.clear();
		prev.clear();
	}
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);