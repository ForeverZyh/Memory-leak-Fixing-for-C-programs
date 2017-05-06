#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utility>
#include <stack>
#include <queue>
#include <unordered_set>
#define MAXN 10000
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
	void print()
	{
		printf("===def===\n");
		for(int i=0;i<def.size();i++)
			printf("%d ",def[i].id);
		printf("\n");
		printf("===use===\n");
		for(int i=0;i<use.size();i++)
			printf("%d ",use[i].id);
		printf("\n");
		printf("===pure===\n");
		for(int i=0;i<pure.size();i++)
			printf("(%d,%d) ",pure[i].first.id,pure[i].second.id);
		printf("\n");
	}
};
struct G1
{
	struct ele
	{
		int fa;
		unordered_set<int> p;
	}f[MAXN];
	bool isUpdate;
	int find(int x){return (f[x].fa==x)?x:f[x].fa=find(f[x].fa);}
	void Union(int x,int y)
	{
		int u=find(x),v=find(y);
		if (u!=v)
		{
			isUpdate=true;
			f[v].fa=u;
			f[u].p.insert(f[v].p.begin(),f[v].p.end());
		}
	}
	G1()
	{
		isUpdate=false;
	}
};
struct CFGnode
{
	vector<CFGnode*> succ,prev;
	expr defuse;
	vector<int> identifier_list;
	int isRrac,isLrac,ln,vis;
	G1 g1;
	static int rac_cnt;
	static int flag;
	CFGnode():defuse(),g1()
	{
		isRrac=isLrac=ln=vis=0;
		succ.clear();
		prev.clear();
		identifier_list.clear();
	}
	CFGnode(int line):defuse(),g1()
	{
		isRrac=isLrac=vis=0;
		ln=line;
		succ.clear();
		prev.clear();
		identifier_list.clear();
	}
	void addL(int id)
	{
		isLrac=id;
	}
	void addR(int id)
	{
		isRrac=id;
	}
	void print()
	{
		if (vis==flag) return;
		vis=flag;
		printf("=====%x=====\n",this);
		if (isRrac) printf("!!Right:%d!!\n",isRrac);
		if (isLrac) printf("!!Left:%d!!\n",isLrac);
		printf("line=%d\n",ln);
		printf("===dec===\n");
		for(int i=0;i<(int)identifier_list.size();i++)
			printf("%d ",identifier_list[i]);
		printf("\n");
		defuse.print();
		for(int i=0;i<(int)succ.size();i++)
			printf("  To %x\n",succ[i]);
		for(int i=0;i<(int)succ.size();i++)
			succ[i]->print();
	}
};
struct func
{
	pair<CFGnode*,CFGnode*> CFG;
	int id;
	CFGnode* parms;
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);