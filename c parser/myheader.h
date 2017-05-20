#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <iostream>
#include <utility>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_set>
#define MAXN 10000
#define MAGIC_NUMBER 999999
using namespace std;
struct environment_identifiers;
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
	void init(int x)
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
/*
 *
 * maintain identifiers in nested environments.
 *
 * Author : Yicheng Lee
 *
 * rule: for every line, two definitions of identifier must have different identifier name.
 */
struct environment_identifiers
{
	#define IDENTIFIER_NUMBER_LIMIT MAXN
	#define PII pair<int, int>
	#define c0 first
	#define c1 second
	
	vector<int> identifier_list[IDENTIFIER_NUMBER_LIMIT];
	
	/*
	 * number in identifier_stack:
	 *
	 * -> positive : = identifier_number
	 * -> negative : = -bracket_number
	 *
	 */
	int identifier_stack[IDENTIFIER_NUMBER_LIMIT], identifier_stack_top;

	static int unique_identifier_count;

	static map<pair<int, int>, int> added;

	void init()
	{
		identifier_stack_top = 0;
		/*
		 * to do : add some initial global identifier.
		 */
		unique_identifier_count = 0;

		for(int i = 0; i < IDENTIFIER_NUMBER_LIMIT; ++i)
			identifier_list[i].clear();

		added.clear();
	}
	static int get_unique_identifier_count()
	{
		return unique_identifier_count;
	}
	static int plus1_unique_identifier_count()
	{
		return ++unique_identifier_count;
	}
	int add(int identifier_num, int line_no)
	{
		int unique_identifier_num;
		
		if (!added[PII(identifier_num, line_no)])
			added[PII(identifier_num, line_no)] = ++unique_identifier_count;
		
		unique_identifier_num = added[PII(identifier_num, line_no)];

		identifier_list[identifier_num].push_back(unique_identifier_num);
		return unique_identifier_num;
	}
	int get(int identifier_num)
	{
		if (identifier_num<0) return identifier_num;
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
};

struct G1
{
	struct ele
	{
		int fa;
		unordered_set<int> p;
		ele(int x)
		{
			fa=x;
			p.clear();
		}
	};
	vector<ele> f;
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
		f.clear();
		isUpdate=false;
	}
	void init(int len)
	{
		if (f.size()<=len) isUpdate=true;
		for(int i=f.size();i<=len;i++)
		{
			f.push_back(ele(i));
		}
	}
	void print()
	{
		for(int i=1;i<f.size();i++)
		{
			int t=find(f[i].fa);
			printf("  %d:",i);
			auto it=f[t].p.begin();
			for(;it!=f[t].p.end();it++)
				printf(" %d",*it);
			printf("\n");
		}
	}
	void merge(int x,const unordered_set<int>&q)
	{
		int pre=f[x].p.size();
		f[x].p.insert(q.begin(),q.end());
		if (pre!=f[x].p.size()) isUpdate=true;
	}
};
struct CFGnode
{
	vector<CFGnode*> succ,prev;
	expr defuse;
	vector<int> identifier_list;
	int isRrac,isLrac,ln,vis,tag;
	G1 g1;
	static int rac_cnt;
	static int flag;
	CFGnode():defuse(),g1()
	{
		isRrac=isLrac=ln=vis=0;
		tag=-1;
		succ.clear();
		prev.clear();
		identifier_list.clear();
	}
	CFGnode(int line):defuse(),g1()
	{
		isRrac=isLrac=vis=0;
		tag=-1;
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
		printf("===G1===\n");
		g1.print();
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

	/*
	 * clone the cfg
	 *
	 * Author : Yicheng Lee
	 *
	 */
	static CFGnode *vt_, *new_vt_;
	static void clone_cfg_dfs(CFGnode *u, CFGnode *new_u)
	{
		if (u == vt_)
		{
			new_vt_ = u;
			return;
		}
		for(int i=0;i<(int)u->succ.size();i++)
		{
			CFGnode *v = u->succ[i];
			if (v->vis != flag)
			{
				v->vis = flag;
				CFGnode *new_v = new CFGnode(); *new_v = *v; new_v->prev.clear(); new_v->succ.clear();
				new_u->succ.push_back(new_v);
				new_v->prev.push_back(new_u);
				clone_cfg_dfs(v, new_v);
			}
		}
	}
	/*
	 * param
	 * - vs : source of the original cfg
	 * - vt : destination of the original cfg
	 *
	 * return
	 * - pair<new_vt, new_vt>
	 * 		- new_vs : source of the new cfg
	 * 		- new_vt : destination of the new cfg
	 *
	 *
	 * ensure it before call this function:
	 * 		for each node u in the original cfg,
	 *	there's no edge(u, v) that v is not in the original cfg.
	 *
	 */
	static pair<CFGnode*, CFGnode*> clone_cfg(CFGnode *vs, CFGnode* vt)
	{
		++flag;
		CFGnode *new_vs = new CFGnode(), *new_vt = NULL; *new_vs = *vs; new_vs->prev.clear(); new_vs->succ.clear();
		vt_ = vt;
		dfs(vs, new_vs);
		assert(new_vt != NULL);
		return pair<CFGnode*, CFGnode*> (new_vs, new_vt);
	}
};
struct func
{
	pair<CFGnode*,CFGnode*> CFG;
	int id;
	func()
	{
		CFG=make_pair((CFGnode*)NULL,(CFGnode*)NULL);
		id=0;
	}
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);
