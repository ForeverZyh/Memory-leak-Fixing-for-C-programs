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

pair<CFGnode*,CFGnode*> IF(node* root)
{

}

pair<CFGnode*,CFGnode*> WHILE(node* root)
{

}

pair<CFGnode*,CFGnode*> DO_WHILE(node* root)
{

}

pair<CFGnode*,CFGnode*> FOR(node* root)
{

}

pair<CFGnode*,CFGnode*> create(node* root)
{
	CFGnode* begin=new CFGnode(),*end=begin;
	for(int i=0;i<root->son.size();i++)
	{
		if (root->son[i]->str=="expression") /* maybe use substr */
		{
			CFGnode* newnode=new CFGnode();
			expression(root,newnode);
			link(end,newnode);
			end=newnode;
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
			pair<CFGnode*,CFGnode*> it=create(root->son[i]);
			link(end,it.first);
			end=it.second;
		}
	}
	return make_pair(begin,end);
}