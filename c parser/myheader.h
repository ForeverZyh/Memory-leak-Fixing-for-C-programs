#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
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



/*
 *
 * maintain identifiers in nested environments.
 *
 * Author : Yicheng Lee
 */
namespace environment_identifiers
{
	const int IDENTIFIER_NUMBER_LIMIT = 1000010;
	
	vector<int> identifier_list[IDENTIFIER_NUMBER_LIMIT];
	
	/*
	 * number in identifier_stack:
	 *
	 * -> positive : = identifier_number
	 * -> negative : = -bracket_number
	 *
	 */
	int identifier_stack[IDENTIFIER_NUMBER_LIMIT], identifier_stack_top;

	int unique_identifier_count;

	map<pair<int, int>, int> MAP;

	int added[IDENTIFIER_NUMBER_LIMIT];

	void init()
	{
		identifier_stack_top = 0;
		/*
		 * to do : add some initial global identifier.
		 */
		unique_identifier_count = 0;

		for(int i = 0; i < IDENTIFIER_NUMBER_LIMIT; ++i)
			added[i] = 0;
		for(int i = 0; i < IDENTIFIER_NUMBER_LIMIT; ++i)
			identifier_list[i].clear();
	}
	int get_unique_identifier_count()
	{
		return unique_identifier_count;
	}
	int plus1_unique_identifier_count()
	{
		return ++unique_identifier_count;
	}
	int add(int identifier_num, int line_no)
	{
		int unique_identifier_num;
		
		if (!added[line_no])
			added[line_no] = ++unique_identifier_count;
		
		unique_identifier_num = added[line_no];

		identifier_list[identifier_num].push_back(unique_identifier_num);
		return unique_identifier_num;
	}
	int get(int identifier_num)
	{
		if (identifier_list[identifier_num].empty())
		{
			fprintf(stderr, "%s %d\n", "No such identifier :  required identifier num =", identifier_num);
			return -1;
		}
		return identifier_list[identifier_num].back();
	}
	void left_bracket(int bracket_num)
	{
		identifier_stack[++identifier_stack_top] = -bracket_num;
	}
	void right_bracket(int bracket_num)
	{
		while (identifier_stack[identifier_stack_top] != -bracket_num)
		{
			int identifier_num = identifier_stack[identifier_stack_top];
			identifier_list[identifier_num].pop_back();
			--identifier_stack_top;
		}
		--identifier_stack_top;
	}
}