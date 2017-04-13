#include "myheader.h"

void link(CFGnode* a,CFGnode* b)
{
	a->succ.push_back(b);
	b->prev.push_back(a);
}

void expression(node* root,CFGnode* x)
{

}

pair<CFGnode*,CFGnode*> declaration(node* root)
{

}

pair<CFGnode*,CFGnode*> IF(node* root,CFGnode* return_node)
{

}

pair<CFGnode*,CFGnode*> WHILE(node* root,CFGnode* return_node)
{

}

pair<CFGnode*,CFGnode*> DO_WHILE(node* root,CFGnode* return_node)
{

}

pair<CFGnode*,CFGnode*> FOR(node* root,CFGnode* return_node)
{

}

pair<CFGnode*,CFGnode*> create(node* root,CFGnode* return_node=NULL)
{
	CFGnode* begin=new CFGnode(),*pre=begin,*end=new CFGnode();
	if (!return_node) return_node=end;
	for(int i=0;i<root->son.size();i++)
	{
		if (root->son[i]->str=="expression") /* maybe use substr */
		{
			CFGnode* newnode=new CFGnode();
			expression(root,newnode);
			link(pre,newnode);
			pre=newnode;
		}
		/*
			if IF detected
			if WHILE(DO WHILE) detected
			if FOR detected
			FOR -> FOR(x;x;) FOR(x;x;x) FOR(d;x;) FOR(d;x;x)
			if declaration detected
			if function call detected
		*/
		else
		{
			pair<CFGnode*,CFGnode*> it=create(root->son[i],return_node);
			link(pre,it.first);
			pre=it.second;
		}
	}
	return make_pair(begin,end);
}