#include "myheader.h"
extern map<string,int> string_to_int;
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
		if (res.def.size()==1)
		{
			res.pure.push_back(make_pair(res1.def[0],res.def[0]));
		}
		else if (res.def.size()==0&&res.use.size()==1)
		{
			res.pure.push_back(make_pair(res1.def[0],res.use[0]));
		}
		res.use.insert(res.use.end(),res.def.begin(),res.def.end());
		res.merge(res1);
		return res;
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

pair<CFGnode*,CFGnode*> declaration(node* root)
{

}

/*pair<CFGnode*,CFGnode*> IF(node* root,CFGnode* return_node,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{

}

pair<CFGnode*,CFGnode*> WHILE(node* root,CFGnode* return_node,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{

}

pair<CFGnode*,CFGnode*> DO_WHILE(node* root,CFGnode* return_node,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{

}

pair<CFGnode*,CFGnode*> FOR(node* root,CFGnode* return_node,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{

}*/

pair<CFGnode*,CFGnode*> create(node* root,CFGnode* return_node=NULL,CFGnode *continue_node=NULL,CFGnode* break_node=NULL)
{
	CFGnode* begin=new CFGnode(),*pre=begin,*end=new CFGnode();
	if (!return_node) return_node=end;
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
		}
		else if (root->son[i]->str=="BREAK")
		{
			link(pre,break_node);
		}
		else if (root->son[i]->str=="RETURN")
		{
			link(pre,return_node);
		}
		else if (root->son[i]->str=="WHILE")
		{

		}
		else if (root->son[i]->str=="DO WHILE")
		{

		}
		else if (root->son[i]->str.find("FOR(d;")!=-1)
		{
			if (root->son[i]->son.size()==2)  /*FOR(d;x;)*/
			{

			}
			else	/*FOR(d;x;x)*/
			{

			}
		}
		else if (root->son[i]->str.find("FOR(x;")!=-1)
		{
			if (root->son[i]->son.size()==2)  /*FOR(x;x;)*/
			{

			}
			else	/*FOR(x;x;x)*/
			{

			}
		}
		else if (root->son[i]->str=="declaration")
		{

		}
		else if (root->son[i]->str=="function_definition")
		{

		}
		else
		{
			pair<CFGnode*,CFGnode*> it=create(root->son[i],return_node,continue_node,break_node);
			link(pre,it.first);
			pre=it.second;
		}
	}
	return make_pair(begin,end);
}