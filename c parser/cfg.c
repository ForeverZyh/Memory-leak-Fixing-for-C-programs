#include "myheader.h"

void link(CFGnode* a,CFGnode* b)
{
	a->succ.push_back(b);
	b->prev.push_back(a);
}

pair<CFGnode*,CFGnode*> create(node* root)
{
	CFGnode* begin=new CFGnode(),*end=begin;
	for(int i=0;i<root->son.size();i++)
	{
		if (root->son[i]->str=="expression") /* maybe ues substr */
		{

		}
		/*
			if IF detected
			if WHILE detected
			if FOR detected
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