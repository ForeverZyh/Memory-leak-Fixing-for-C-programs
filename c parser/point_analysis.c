#include "myheader.h"
queue<CFGnode*> h;
void dfs_add(CFGnode* root)
{
	if (root->vis==root->flag) return;
	root->vis=root->flag;
	h.push(root);
	for(int i=0;i<root->succ.size();i++)
		dfs_add(root->succ[i]);
}
void point_analysis(CFGnode* root)
{
	dfs_add(root);

	while (h.size())
	{
		CFGnode* u=h.front();
		h.pop();
		u->vis=0;
		u->g1.isUpdate=false;
		if (u->isLrac) env.left_bracket();
		for(int i=0;i<u->identifier_list.size();i++)
		{
			int id=env.add(u->identifier_list[i],u->ln);
		}
		for(int i=0;i<u->defuse.pure.size();i++)
		{
			if (u->defuse.pure[i].second.id)
			{
				
			}
		}
		if (u->isRrac) env.right_bracket();
		if (u->g1.isUpdate)
		{
			for(int i=0;i<u->succ.size();i++)
				if (u->succ[i]->vis!=u->flag)
				{
					u->succ[i]->vis=u->flag;
					h.push(u->succ[i]);
				}
		}
	}
}