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
#include <unordered_map>
#include <algorithm>
#include <fstream>
#define MAXN 1000
#define MAGIC_NUMBER 999999
#define MAX_PROC_NUM 10000
#define MAX_CALL_NUM 100000
#define MAX_CALLPAIR_NUM  1000000
#define ln_delta_increment  100000
#define IDENTIFIER_NUMBER_LIMIT MAXN
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
			printf("[%d unary:%d] ",def[i].id,def[i].unary.size());
		printf("\n");
		printf("===use===\n");
		for(int i=0;i<use.size();i++)
			printf("[%d unary:%d] ",use[i].id,use[i].unary.size());
		printf("\n");
		printf("===pure===\n");
		for(int i=0;i<pure.size();i++)
			printf("([%d unary:%d],[%d unary:%d]) ",pure[i].first.id,pure[i].first.unary.size(),pure[i].second.id,pure[i].second.unary.size());
		printf("\n");
	}
	bool empty()
	{
		return def.size()==0&&use.size()==0&&pure.size()==0;
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

	static int identifier_to_var[IDENTIFIER_NUMBER_LIMIT];

	void init()
	{
		identifier_stack_top = 0;
		/*
		 * to do : add some initial global identifier.
		 */

		for(int i = 0; i < IDENTIFIER_NUMBER_LIMIT; ++i)
			identifier_list[i].clear();
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
		{
			added[PII(identifier_num, line_no)] = ++unique_identifier_count;
			identifier_to_var[unique_identifier_count]=identifier_num;
		}
		
		unique_identifier_num = added[PII(identifier_num, line_no)];

		identifier_list[identifier_num].push_back(unique_identifier_num);
		identifier_stack[++identifier_stack_top]=identifier_num;
		return unique_identifier_num;
	}
	int get(int identifier_num)
	{
		if (identifier_num<=0) return identifier_num;
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
	void right_bracket(int bracket_num,vector<int>& a)
	{
		while (identifier_stack[identifier_stack_top] != -bracket_num)
		{
			int identifier_num = identifier_stack[identifier_stack_top];
			if (identifier_num>0)
			{
				a.push_back(identifier_list[identifier_num].back());
				identifier_list[identifier_num].pop_back();
			}
			--identifier_stack_top;
		}
		--identifier_stack_top;
	}
	void inside(vector<pair<int,int> >&a,const vector<string>&int_to_string)
	{
		unordered_set<int> hash;
		hash.clear();
		for(int i=identifier_stack_top;i>1;i--)
			if (identifier_stack[i]>0&&hash.find(identifier_stack[i])==hash.end())
			{
				int id=identifier_stack[i];
				hash.insert(id);
				if (int_to_string[id].size()&&int_to_string[id][0]!='@')
					a.push_back(make_pair(get(id),id));
			}
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
		void init(int f)
		{
			fa=f;p.clear();
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
			int t=find(i);
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
	void erase(int id)
	{
		int newfa=0;
		for(int j=1;j<f.size();j++)
			if (j!=id&&find(j)==id) newfa=j;
		if (newfa)
		{
			for(int j=1;j<f.size();j++)
				if (j!=id&&f[j].fa==id) f[j].fa=newfa;
			f[newfa].p=f[id].p;
		}
		f[id].init(id);
	}
};
struct G2
{
	unordered_set<int> p;
	bool isUpdate;
	G2()
	{
		p.clear();
		isUpdate=false;
	}
	void clear()
	{
		if (p.size()) isUpdate=true;
		p.clear();
	}
	void insert(int x)
	{
		if (p.find(x)!=p.end()) isUpdate=true;
		p.insert(x);
	}
	void merge(const unordered_set<int>&q)
	{
		int pre=p.size();
		p.insert(q.begin(),q.end());
		if (pre!=p.size()) isUpdate=true;
	}
	bool find(int x)
	{
		return p.find(x)!=p.end();
	}
	void print()
	{
		auto it=p.begin();
		for(;it!=p.end();it++)
			printf(" %d",*it);
		printf("\n");
	}
};
struct CFGnode
{
	vector<CFGnode*> succ,prev;
	expr defuse;
	vector<int> identifier_list,vars_to_kill;
	vector<pair<int,int> > list_of_vars;
	int isRrac,isLrac,ln,vis,vis2,vis3,vis4,tag,call_index;
	bool cantAfter,put_back,isFirst,isLast;
	G1 g1;
	G2 g2,g3;
	static int rac_cnt;
	static int flag;
	CFGnode():defuse(),g1(),g2(),g3()
	{
		isRrac=isLrac=ln=vis=vis2=vis3=vis4=put_back=0; call_index = -1;
		cantAfter=isFirst=isLast=0;
		tag=-1;
		succ.clear();
		prev.clear();
		identifier_list.clear();
		vars_to_kill.clear();
		list_of_vars.clear();
	}
	CFGnode(int line):defuse(),g1(),g2(),g3()
	{
		isRrac=isLrac=vis=vis2=vis3=vis4=put_back=0; call_index = -1;
		cantAfter=isFirst=isLast=0;
		tag=-1;
		ln=line;
		succ.clear();
		prev.clear();
		identifier_list.clear();
		vars_to_kill.clear();
		list_of_vars.clear();
	}
	bool empty()
	{
		return ln%ln_delta_increment==0&&defuse.empty()&&isRrac==0&&isLrac==0&&tag==-1&&put_back==0;
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
		printf("tag=%d\n",tag);
		printf("call_index=%d\n",call_index);
		printf("===G1===\n");
		g1.print();
		printf("===G2===\n");
		g2.print();
		printf("===G3===\n");
		g3.print();
		printf("===dec===\n");
		for(int i=0;i<(int)identifier_list.size();i++)
			printf("%d ",identifier_list[i]);
		printf("\n");
		defuse.print();
		printf("===you can ues===\n");
		for(int i=0;i<(int)list_of_vars.size();i++) printf("%d ",list_of_vars[i].second);
		printf("\n");
		for(int i=0;i<(int)succ.size();i++)
			printf("  To %x\n",succ[i]);
		for(int i=0;i<(int)succ.size();i++)
			succ[i]->print();
	}
	void initvis()
	{
		prev.clear();succ.clear();
		vis=vis2=vis3=vis4=0;
	}
	bool canFreeBefore(int id,int*Next)
	{
		if (prev.size()!=1) return 0;
		if (!isFirst) return 0;
		const unordered_set<int> &p=prev[0]->g1.f[prev[0]->g1.find(id)].p;
		if (p.size()==0) return 0;
		for(auto it=p.begin();it!=p.end();it++)
		{
			int item=*it;
			if (item>=0) return 0;
			if (prev[0]->g2.find(item)) return 0;
			if (g3.find(item)) return 0;
		}
		return 1;
	}
	bool canFreeAfter(int id,int*Next)
	{
		if (succ.size()!=1) return 0;
		if (!isLast) return 0;
		if (cantAfter) return 0;
		const unordered_set<int> &p=g1.f[g1.find(id)].p;
		if (p.size()==0) return 0;
		for(auto it=p.begin();it!=p.end();it++)
		{
			int item=*it;
			if (item>=0) return 0;
			if (g2.find(item)) return 0;
			if (succ[0]->g3.find(item)) return 0;
		}
		return 1;
	}

	/*
	 * clone the cfg
	 *
	 * Author : Yicheng Lee
	 *
	 */
	static CFGnode *vt_, *new_vt_;
	static unordered_map<CFGnode*,CFGnode*> Hash;
	static void clone_cfg_dfs(CFGnode *u, CFGnode *new_u)
	{
		if (u == vt_)
		{
			new_vt_ = new_u;
			return;
		}
		for(int i=0;i<(int)u->succ.size();i++)
		{
			CFGnode *v = u->succ[i];
			if (v->vis != flag)
			{
				v->vis = flag;
				CFGnode *new_v = new CFGnode();
				*new_v = *v; new_v->initvis();
				Hash[v]=new_v;
				new_u->succ.push_back(new_v);
				new_v->prev.push_back(new_u);
				clone_cfg_dfs(v, new_v);
			}
			else
			{
				new_u->succ.push_back(Hash[v]);
				Hash[v]->prev.push_back(new_u);
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
		CFGnode *new_vs = new CFGnode();
		*new_vs = *vs; new_vs->initvis();
		Hash.clear();
		Hash[vs]=new_vs;
		vt_ = vt;
		new_vt_=NULL;
		clone_cfg_dfs(vs, new_vs);
		assert(new_vt_ != NULL);
		return pair<CFGnode*, CFGnode*> (new_vs, new_vt_);
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
	void dfs(CFGnode*u)
	{
		u->vis=u->flag;
		if (u->succ.size()==1&&u->prev.size()==1&&u->empty())
		{
			CFGnode *pre=u->prev[0],*suc=u->succ[0];
			for(int i=0;i<pre->succ.size();i++)
				if (pre->succ[i]==u) pre->succ[i]=suc;
			for(int i=0;i<suc->prev.size();i++)
				if (suc->prev[i]==u) suc->prev[i]=pre;
		}
		for(int i=0;i<u->succ.size();i++)
			if (u->succ[i]->vis!=u->flag)
				dfs(u->succ[i]);
	}
	void reduce()
	{
		CFGnode::flag++;
		dfs(CFG.first);
	}
};
typedef node* myYYSTYPE;
int hash_string_to_int(const string&);
