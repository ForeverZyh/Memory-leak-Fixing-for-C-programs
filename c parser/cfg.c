#include "myheader.h"
extern map<string,int> string_to_int;
extern func fun[10000];
extern int cnt_func;
extern int CFGnode::rac_cnt;
stack<int> rac;
extern int malloc_cnt;

void link(CFGnode* a,CFGnode* b)
{
	a->succ.push_back(b);
	b->prev.push_back(a);
}

expr dfs_expression(node *root)
{
	if (root->str=="unary_expression"&&(root->son[0]->str=="*"||root->son[0]->str=="&"))
	{
		expr res=dfs_expression(root->son[1]);
		res.use[0].unary.push_back(root->son[0]->str=="&");
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
	else 
	{
		int t=string_to_int[root->str];
		if (t) 
		{
			expr res;
			if (root->str=="malloc")
			{
				++malloc_cnt;
				t=-malloc_cnt;
			}
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

void expression(node* root,CFGnode* x)
{
	x->defuse=dfs_expression(root);
}



/*
 * return: the number of identifier in this subtree
 * 
 */
void dfs_declaration(node *root, CFGnode *x)
{
	if (root->str=="init_declarator")
	{
		if (root->son.size()==2)
		{
			dfs_declaration(root->son[0],x);
			if (root->son[1]->str=="initializer1") 
			{
				expr res=dfs_expression(root->son[1]->son[0]);
				int id=x->identifier_list.back();
				expr res1;
				res1.def.push_back(id);
				if (res.def.size()==1)
				{
					res.pure.push_back(make_pair(res1.def[0],res.def[0]));
				}
				else if (res.def.size()==0&&res.use.size()==1)
				{
					res.pure.push_back(make_pair(res1.def[0],res.use[0]));
				}
				//res.use.insert(res.use.end(),res.def.begin(),res.def.end());
				res.merge(res1);
				x->defuse=res;
			}
		}
		else dfs_declaration(root->son[0], x);
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
						x->identifier_list.push_back(identifier_number);
					}
				}
			}
			dfs_declaration(root->son[i], x);
		}
	}
}

void declaration(node* root, CFGnode* x)
{
	dfs_declaration(root, x);
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
			CFGnode* newnode=new CFGnode();
			expression(root,newnode);
			link(pre,newnode);
			pre=newnode;
		}
		else if (root->son[i]->str=="CONTINUE")
		{
			link(pre,continue_node);
			pre->addR(rac.top());
			flag=false;
		}
		else if (root->son[i]->str=="BREAK")
		{
			link(pre,break_node);
			pre->addR(rac.top());
			flag=false;
		}
		else if (root->son[i]->str=="RETURN")
		{
			link(pre,return_node);
			flag=false;
		}
		else if (root->son[i]->str=="WHILE")
		{
			CFGnode* expr=new CFGnode(),*jump_end=new CFGnode();
			expression(root->son[i]->son[0],expr);
			link(pre,expr);
			link(expr,jump_end);
			if (root->son[i]->son[1]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,expr,jump_end);
			link(expr,it.first);
			link(it.second,expr);
			pre=jump_end;
		}
		else if (root->son[i]->str=="DO WHILE")
		{
			CFGnode* expr=new CFGnode(),*jump_end=new CFGnode();
			expression(root->son[i]->son[1],expr);
			if (root->son[i]->son[0]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[0],return_node,expr,jump_end);
			link(pre,it.first);
			link(it.second,expr);
			link(expr,it.first);
			link(expr,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str.find("FOR(d;")!=-1)
		{
			if (root->son[i]->son.size()==3)  /*FOR(d;x;)*/
			{
				CFGnode *dec=new CFGnode(),*jump_end=new CFGnode(),*expr=new CFGnode();
				dec->addL(++CFGnode::rac_cnt);
				jump_end->addR(CFGnode::rac_cnt);
				declaration(root->son[i]->son[0],dec);
				expression(root->son[i]->son[1],expr);
				link(pre,dec);
				link(dec,expr);
				link(expr,jump_end);
				if (root->son[i]->son[2]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[2],return_node,expr,jump_end);
				link(expr,it.first);
				link(it.second,expr);
				pre=jump_end;
			}
			else	/*FOR(d;x;x)*/
			{
				CFGnode *dec=new CFGnode(),*jump_end=new CFGnode(),*expr=new CFGnode(),*rep=new CFGnode();
				dec->addL(++CFGnode::rac_cnt);
				jump_end->addR(CFGnode::rac_cnt);
				declaration(root->son[i]->son[0],dec);
				expression(root->son[i]->son[1],expr);
				expression(root->son[i]->son[2],rep);
				link(pre,dec);
				link(dec,expr);
				link(expr,jump_end);
				if (root->son[i]->son[3]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[3],return_node,expr,jump_end);
				link(expr,it.first);
				link(it.second,rep);
				link(rep,expr);
				pre=jump_end;
			}
		}
		else if (root->son[i]->str.find("FOR(x;")!=-1)
		{
			if (root->son[i]->son.size()==3)  /*FOR(x;x;)*/
			{
				CFGnode *init=new CFGnode(),*jump_end=new CFGnode(),*expr=new CFGnode();
				expression(root->son[i]->son[0],init);
				expression(root->son[i]->son[1],expr);
				link(pre,init);
				link(init,expr);
				link(expr,jump_end);
				if (root->son[i]->son[2]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[2],return_node,expr,jump_end);
				link(expr,it.first);
				link(it.second,expr);
				pre=jump_end;
			}
			else	/*FOR(x;x;x)*/
			{
				CFGnode *init=new CFGnode(),*jump_end=new CFGnode(),*expr=new CFGnode(),*rep=new CFGnode();
				expression(root->son[i]->son[0],init);
				expression(root->son[i]->son[1],expr);
				expression(root->son[i]->son[2],rep);
				link(pre,init);
				link(init,expr);
				link(expr,jump_end);
				if (root->son[i]->son[3]->son[0]->str=="compound_statement") rac.push(CFGnode::rac_cnt+1);
				pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[3],return_node,expr,jump_end);
				link(expr,it.first);
				link(it.second,rep);
				link(rep,expr);
				pre=jump_end;
			}
		}
		else if (root->son[i]->str=="IF")
		{
			CFGnode *expr=new CFGnode(),*jump_end=new CFGnode();
			expression(root->son[i]->son[0],expr);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,continue_node,break_node);
			link(pre,expr);
			link(expr,it.first);
			link(expr,jump_end);
			link(it.second,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str=="IF ELSE")
		{
			CFGnode *expr=new CFGnode(),*jump_end=new CFGnode();
			expression(root->son[i]->son[0],expr);
			pair<CFGnode*,CFGnode*> it=create(root->son[i]->son[1],return_node,continue_node,break_node);
			pair<CFGnode*,CFGnode*> branch=create(root->son[i]->son[2],return_node,continue_node,break_node);
			link(pre,expr);
			link(expr,it.first);
			link(expr,branch.first);
			//link(expr,jump_end);
			link(it.second,jump_end);
			link(branch.second,jump_end);
			pre=jump_end;
		}
		else if (root->son[i]->str=="declaration")
		{
			CFGnode *dec=new CFGnode();
			declaration(root->son[i],dec);
			link(pre,dec);
			pre=dec;
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
	if (root->str=="compound_statement")
	{
		if (!rac.empty()&&rac.top()==tmp) rac.pop();
	}
	return make_pair(begin,end);
}

void function(node* root)
{
	if (root->str=="declaration")
	{
		CFGnode* dec=new CFGnode();
		declaration(root,dec);
	}
	else if (root->str=="function_definition")
	{
		++cnt_func;
		if (root->son.size()==3)
		{

			fun[cnt_func].CFG=create(root->son[2]);
			CFGnode* tmp=new CFGnode();
			declaration(root->son[1],tmp);
			fun[cnt_func].id=tmp->identifier_list[0];
			delete tmp;
		}
		else
		{
			fun[cnt_func].CFG=create(root->son[3]);
			CFGnode* tmp=new CFGnode(),*list=new CFGnode();
			declaration(root->son[1],tmp);
			fun[cnt_func].id=tmp->identifier_list[0];
			declaration(root->son[2],list);
			fun[cnt_func].parms=list->identifier_list;
			delete tmp;
			delete list;
		}
	}
	else
	{
		for(int i=0;i<(int)root->son.size();i++)
			function(root->son[i]);
	}
}