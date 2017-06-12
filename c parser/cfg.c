#include "myheader.h"
extern map<string,int> string_to_int;
extern vector<func> fun;
extern int CFGnode::rac_cnt;
extern int CFGnode::flag;
stack<int> rac;
extern int malloc_cnt;
extern int total_string;
extern vector<pair<CFGnode*,CFGnode*> > ext_dec;
extern vector<string> int_to_string;
static CFGnode *begin_exp,*prev_exp,*begin_dec,*prev_dec;
static int prev_id;
extern int hash_string_to_int(const string&s);
static bool Hash[MAXN];
extern int id_to_line[MAXN];

void link(CFGnode* a,CFGnode* b)
{
	a->succ.push_back(b);
	b->prev.push_back(a);
}
pair<CFGnode*,CFGnode*> Expression(node*);
pair<CFGnode*,CFGnode*> Declaration(node*);
expr dfs_expression(node*);
CFGnode* expression(node*);

void dfs_function_call(node*root, vector<pointer>* a)
{
	if (root==NULL) return;
	int t=0;
	if (root->son.size()>1) 
	{
		t++;
		dfs_function_call(root->son[0],a);
	}
	CFGnode* tmp=expression(root->son[t]);
	if (tmp->defuse.def.size()==1) a->push_back(tmp->defuse.def[0]);
	else if (tmp->defuse.def.size()==0&&tmp->defuse.use.size()==1) a->push_back(tmp->defuse.use[0]);
	else a->push_back(pointer(0));
}
int get_func(int name)
{
	//TODO maybe performance issues
	for(int i=0;i<fun.size();i++)
		if (fun[i].id==name) return i;
	return -1;
}
void function_call(int name,int id,node* root,int ln)
{
	int tag=get_func(name);
	if (tag==-1) return;
	else tag=name;
	vector<pointer> list;list.clear();
	dfs_function_call(root,&list);
	CFGnode* res=new CFGnode(ln);
	for(int i=0;i<list.size();i++)
	{
		char tmpstr[10];
		sprintf(tmpstr,"@%d",i);
		int def=hash_string_to_int(tmpstr);
		res->identifier_list.push_back(def);
		res->defuse.def.push_back(pointer(def));
		res->defuse.use.push_back(list[i]);
		res->defuse.pure.push_back(make_pair(pointer(def),list[i]));
	}
	res->identifier_list.push_back(id);
	res->tag=tag;
	if (prev_exp) link(prev_exp,res);
	else begin_exp=res;
	prev_exp=res;
}

expr dfs_expression(node *root)
{
	if (root->str=="unary_expression"&&(root->son[0]->str=="*"||root->son[0]->str=="&"))
	{
		expr res=dfs_expression(root->son[1]);
		int mk=root->son[0]->str=="&";
		if (res.use[0].unary.size()&&res.use[0].unary.back()!=mk) res.use[0].unary.pop_back();
		else res.use[0].unary.push_back(mk);
		return res;
	}
	else if (root->str=="assignment_expression"&&root->son.size()==3)
	{
		expr res=dfs_expression(root->son[2]);
		expr res1=dfs_expression(root->son[0]);
		res1.def=res1.use;
		res1.use.clear();
		if (root->son[1]->str!="=")
		{
			res.use.push_back(res1.def[0]);
		}
		if (res.def.size()==1)
		{
			res.pure.push_back(make_pair(res1.def[0],res.def[0]));
		}
		else if (res.def.size()==0&&res.use.size()==1)
		{
			res.pure.push_back(make_pair(res1.def[0],res.use[0]));
		}
		res.merge(res1);
		return res;
	}
	else if (root->str=="function")
	{
		if (root->son[0]->son[0]->str=="malloc")
		{
			expr res;
			++malloc_cnt;
			id_to_line[malloc_cnt]=root->ln;
			pointer po(-malloc_cnt);
			po.unary.push_back(1);
			res.use.push_back(po);
			return res;
		}
		else if (root->son[0]->son[0]->str=="free")
		{
			expr res=dfs_expression(root->son[1]);
			res.def=res.use;
			pointer po(-MAGIC_NUMBER);
			po.unary.push_back(1);
			res.pure.push_back(make_pair(res.def[0],po));
			return res;
		}
		else //foo(list,...)
		{
			expr res;
			int tmp=++total_string;
			function_call(string_to_int[root->son[0]->son[0]->str],tmp,(root->son.size()>1)?root->son[1]:NULL,root->ln);
			res.use.push_back(pointer(tmp));
			return res;
		}
	}
	else 
	{
		int t=string_to_int[root->str];
		if (t) 
		{
			expr res;
			res.use.push_back(pointer(t));
			return res;
		}
		else if (root->str.find("_CONSTANT")!=-1)
		{
			expr res;
			res.use.push_back(pointer(0));
			return res;
		}
		else if (root->son.size())
		{
			expr res=dfs_expression(root->son[0]);
			for(int i=1;i<root->son.size();i++) 
				res.merge(dfs_expression(root->son[i]));
			return res;
		}
	}
	return expr();
}

CFGnode* expression(node* root)
{
	CFGnode *res=new CFGnode(root->ln);
	res->defuse=dfs_expression(root);
	if (prev_exp) link(prev_exp,res);
	else begin_exp=res;
	prev_exp=res;
	return res;
}

pair<CFGnode*,CFGnode*> Expression(node* root)
{
	prev_exp=begin_exp=NULL;
	expression(root);
	return make_pair(begin_exp,prev_exp);
}


void add_declaration(CFGnode* node)
{
	if (prev_dec) link(prev_dec,node);
	else begin_dec=node;
	prev_dec=node;
}
void add_declaration(pair<CFGnode*,CFGnode*> e)
{
	if (prev_dec) link(prev_dec,e.first);
	else begin_dec=e.first;
	prev_dec=e.second;
}
/*
 * return: the number of identifier in this subtree
 * 
 */
void dfs_declaration(node *root)
{
	if (root->str=="init_declarator")
	{
		if (root->son.size()==2)
		{
			dfs_declaration(root->son[0]);
			if (root->son[1]->str=="initializer1") 
			{
				pair<CFGnode*,CFGnode*> expr=Expression(root->son[1]->son[0]);
				int id=prev_id;
				CFGnode* tmp=new CFGnode(root->ln);
				if (expr.second->defuse.def.size()==1) 
				{
					tmp->defuse.def.push_back(pointer(id));
					tmp->defuse.use=expr.second->defuse.def;		
					tmp->defuse.pure.push_back(make_pair(pointer(id),expr.second->defuse.def[0]));
				}
				if (expr.second->defuse.use.size()==1&&expr.second->defuse.def.size()==0)
				{
					tmp->defuse.def.push_back(pointer(id));
					tmp->defuse.use=expr.second->defuse.use;
					tmp->defuse.pure.push_back(make_pair(pointer(id),expr.second->defuse.use[0]));
				}
				add_declaration(expr);
				add_declaration(tmp);
			}
		}
		else dfs_declaration(root->son[0]);
	}
	else
	{
		for(int i=0;i<root->son.size();i++)
		{
			if (root->str == "direct_declarator")
			{
				for(int i=0;i<root->son.size();i++)
				{
					string &identifier_name = root->son[i]->str;
					int identifier_number = string_to_int[identifier_name];
					if (identifier_number)		// found a IDENTIFIER
					{
						CFGnode *res=new CFGnode(root->ln);
						res->identifier_list.push_back(identifier_number);
						prev_id=identifier_number;
						add_declaration(res);
					}
				}
			}
			dfs_declaration(root->son[i]);
		}
	}
}

pair<CFGnode*,CFGnode*> Declaration(node* root)
{
	prev_dec=begin_dec=NULL;
	dfs_declaration(root);
	return make_pair(begin_dec,prev_dec);
}

void dfs_block_item_list(node*root,vector<node*> *list)
{
	for(int i=0;i<(int)root->son.size();i++)
	{
		if (root->son[i]->str=="block_item")
		{
			list->push_back(root->son[i]);
		}
		else dfs_block_item_list(root->son[i],list);
	}
}


pair<CFGnode*,CFGnode*> create(node* root,CFGnode* return_node=NULL,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{
	CFGnode* begin=new CFGnode(),*pre=begin,*end=new CFGnode();
	if (!return_node) return_node=end;
	bool flag=true;
	int tmp;
	if (root->str=="compound_statement")
	{
		begin->addL(++CFGnode::rac_cnt);
		end->addR(CFGnode::rac_cnt);
		tmp=CFGnode::rac_cnt;
	}
	for(int i=0;i<root->son.size();i++)
	{
		if (root->son[i]->str.find("expression")!=-1) /* maybe use substr */
		{
			pair<CFGnode*,CFGnode*> it=Expression(root->son[i]);
			link(pre,it.first);
			pre=it.second;
		}
		else if (root->son[i]->str=="CONTINUE")
		{
			link(pre,continue_node);
			pre->ln=root->son[i]->ln;
			pre->addR(rac.top());
			flag=false;
		}
		else if (root->son[i]->str=="BREAK")
		{
			link(pre,break_node);
			pre->ln=root->son[i]->ln;
			pre->addR(rac.top());
			flag=false;
		}
		else if (root->son[i]->str=="RETURN")
		{
			if (root->son[i]->son.size())
			{
				node* b=root->son[i]->son[0];
				node* tot=new node("assignment_expression");
				node* a=new node("@return");
				node* eq=new node("=");
				tot->ln=a->ln=eq->ln=root->son[i]->ln;
				tot->son.push_back(a);
				tot->son.push_back(eq);
				tot->son.push_back(b);
				pair<CFGnode*,CFGnode*> it=Expression(tot);
				link(pre,it.first);
				link(it.second,return_node);
				it.second->put_back=1;
				pre=it.second;
			}
			else 
				link(pre,return_node);
			flag=false;
		}
		else if (root->son[i]->str=="WHILE")
		{
			CFGnode* jump_end=new CFGnode();
			pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[0]);
			link(pre,expr.first);
			link(expr.second,jump_end);
			if (root->son[i]->son[1]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,expr.first,jump_end);
			link(expr.second,it.first);
			link(it.second,expr.first);
			pre=jump_end;
		}
		else if (root->son[i]->str=="DO WHILE")
		{
			CFGnode* jump_end=new CFGnode();
			pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[1]);
			if (root->son[i]->son[0]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[0],return_node,expr.first,jump_end);
			link(pre,it.first);
			link(it.second,expr.first);
			link(expr.second,it.first);
			link(expr.second,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str.find("FOR(d;")!=-1)
		{
			if (root->son[i]->son.size()==3)  /*FOR(d;x;)*/
			{
				CFGnode *jump_end=new CFGnode();
				pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[1]);
				pair<CFGnode*,CFGnode*> dec=Declaration(root->son[i]->son[0]);
				dec.first->addL(++CFGnode::rac_cnt);
				jump_end->addR(CFGnode::rac_cnt);
				link(pre,dec.first);
				link(dec.second,expr.first);
				link(expr.second,jump_end);
				if (root->son[i]->son[2]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[2],return_node,expr.first,jump_end);
				link(expr.second,it.first);
				link(it.second,expr.first);
				pre=jump_end;
			}
			else	/*FOR(d;x;x)*/
			{
				CFGnode *jump_end=new CFGnode();
				pair<CFGnode*,CFGnode*> dec=Declaration(root->son[i]->son[0]);
				dec.first->addL(++CFGnode::rac_cnt);
				jump_end->addR(CFGnode::rac_cnt);
				pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[1]);
				pair<CFGnode*,CFGnode*> rep=Expression(root->son[i]->son[2]);
				link(pre,dec.first);
				link(dec.second,expr.first);
				link(expr.second,jump_end);
				if (root->son[i]->son[3]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[3],return_node,rep.first,jump_end);
				link(expr.second,it.first);
				link(it.second,rep.first);
				link(rep.second,expr.first);
				pre=jump_end;
			}
		}
		else if (root->son[i]->str.find("FOR(x;")!=-1)
		{
			if (root->son[i]->son.size()==3)  /*FOR(x;x;)*/
			{
				CFGnode *jump_end=new CFGnode();
				pair<CFGnode*,CFGnode*> init=Expression(root->son[i]->son[0]);
				pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[1]);
				link(pre,init.first);
				link(init.second,expr.first);
				link(expr.second,jump_end);
				if (root->son[i]->son[2]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[2],return_node,expr.first,jump_end);
				link(expr.second,it.first);
				link(it.second,expr.first);
				pre=jump_end;
			}
			else	/*FOR(x;x;x)*/
			{
				CFGnode *jump_end=new CFGnode();
				pair<CFGnode*,CFGnode*> init=Expression(root->son[i]->son[0]);
				pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[1]);
				pair<CFGnode*,CFGnode*> rep=Expression(root->son[i]->son[2]);
				link(pre,init.first);
				link(init.second,expr.first);
				link(expr.second,jump_end);
				if (root->son[i]->son[3]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[3],return_node,rep.first,jump_end);
				link(expr.second,it.first);
				link(it.second,rep.first);
				link(rep.second,expr.first);
				pre=jump_end;
			}
		}
		else if (root->son[i]->str=="IF")
		{
			CFGnode *jump_end=new CFGnode();
			pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[0]);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,continue_node,break_node);
			link(pre,expr.first);
			link(expr.second,it.first);
			link(expr.second,jump_end);
			link(it.second,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str=="IF ELSE")
		{
			CFGnode *jump_end=new CFGnode();
			pair<CFGnode*,CFGnode*> expr=Expression(root->son[i]->son[0]);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,continue_node,break_node);
			pair<CFGnode*,CFGnode*> branch=create(root->son[i]->son[2],return_node,continue_node,break_node);
			link(pre,expr.first);
			link(expr.second,it.first);
			link(expr.second,branch.first);
			//link(expr,jump_end);
			link(it.second,jump_end);
			link(branch.second,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str=="declaration")
		{
			pair<CFGnode*,CFGnode*> dec=Declaration(root->son[i]);
			link(pre,dec.first);
			pre=dec.second;
		}
		else if (root->son[i]->str=="block_item_list")
		{
			vector<node*> list;
			list.clear();
			dfs_block_item_list(root,&list);
			for(int i=0;i<(int)list.size();i++)
			{
				pair<CFGnode*,CFGnode*> it=create(list[i],return_node,continue_node,break_node);
				link(pre,it.first);
				pre=it.second;
			}
		}
		else //i must be 0 and root->son.size() must be 1
		{
			//printf("!here:%s\n",root->str.c_str());
			delete begin;
			delete end;
			return create(root->son[i],return_node,continue_node,break_node);
		}
	}
	if (flag) link(pre,end);
	else pre->cantAfter=true;
	if (root->str=="compound_statement")
	{
		if (!rac.empty()&&rac.top()==tmp) rac.pop();
	}
	return make_pair(begin,end);
}

static void get_parms(pair<CFGnode*,CFGnode*> list,vector<int>* a)
{
	for(auto it=list.first;;it=it->succ[0])
	{
		int t;
		if (it->identifier_list.size()&&((t=it->identifier_list[0])!=0)) a->push_back(t);
		if (it==list.second) break;
	}
}

static void dfs_first(CFGnode *u)
{
	u->vis=u->flag;
	if (u->ln)
	{
		if (!Hash[u->ln])
		{
			Hash[u->ln]=true;
			u->isFirst=true;
		}
	}
	for(int i=0;i<u->succ.size();i++)
		if (u->succ[i]->vis!=u->flag) 
			dfs_first(u->succ[i]);
}
static void dfs_last(CFGnode *u)
{
	u->vis=u->flag;
	if (u->ln)
	{
		if (!Hash[u->ln])
		{
			Hash[u->ln]=true;
			u->isLast=true;
		}
	}
	for(int i=0;i<u->prev.size();i++)
		if (u->prev[i]->vis!=u->flag) 
			dfs_last(u->prev[i]);
}

void function(node* root)
{
	if (root->str=="declaration")
	{
		pair<CFGnode*,CFGnode*> dec=Declaration(root);
		ext_dec.push_back(dec);
	}
	else if (root->str=="function_definition")
	{
		fun.push_back(func());
		func&f=fun.back();
		if (root->son.size()==3)
		{
			pair<CFGnode*,CFGnode*> list=Declaration(root->son[1]);
			f.id=list.first->identifier_list[0];
			vector<int> parms;
			parms.clear();
			get_parms(list,&parms);
			f.CFG=create(root->son[2]);
			CFGnode* next=f.CFG.first;
			for(int i=1;i<parms.size();i++)
			{
				char tmpstr[10];
				sprintf(tmpstr,"@%d",i-1);
				int use=hash_string_to_int(tmpstr);
				next->identifier_list.push_back(parms[i]);
				next->defuse.def.push_back(pointer(parms[i]));
				next->defuse.use.push_back(pointer(use));
				next->defuse.pure.push_back(make_pair(pointer(parms[i]),pointer(use)));
			}
			memset(Hash,0,sizeof(Hash));
			CFGnode::flag++;
			dfs_first(f.CFG.first);
			memset(Hash,0,sizeof(Hash));
			CFGnode::flag++;
			dfs_last(f.CFG.second);
			for(auto i=list.first;i!=list.second;) 
			{
				auto next=i->succ[0];
				delete i;
				i=next;
			}
			delete list.second;
		}
		else
		{
			/*f.CFG=create(root->son[3]);
			CFGnode* tmp=Declaration(root->son[1]).first;
			pair<CFGnode*,CFGnode*> list=Declaration(root->son[2]);
			f.id=tmp->identifier_list[0];
			vector<int> parms;
			parms.clear();
			get_parms(list,&parms);
			CFGnode* next=new CFGnode(root->ln);
			for(int i=0;i<parms.size();i++)
			{
				char tmpstr[10];
				sprintf(tmpstr,"@%d",i);
				int use=hash_string_to_int(tmpstr);
				next->defuse.def.push_back(pointer(parms[i]));
				next->defuse.use.push_back(pointer(use));
				next->defuse.pure.push_back(make_pair(pointer(parms[i]),pointer(use)));
			}
			link(list.second,next);
			link(next,f.CFG.first);
			f.CFG.first=list.first;
			delete tmp;*/
			assert(0);
		}
		f.reduce();
	}
	else
	{
		for(int i=0;i<(int)root->son.size();i++)
			function(root->son[i]);
	}
}
