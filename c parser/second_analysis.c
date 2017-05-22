#include "myheader.h"
static queue<CFGnode*> h;
extern int find_next(CFGnode*,int id);
static void dfs(CFGnode* u)
{
	u->vis=u->flag;
	h.push(u);
	for(int i=0;i<u->prev.size();i++)
	{
		CFGnode* v=u->prev[i];
		if (v->vis!=v->flag) dfs(v);
	}
}
void second_analysis(CFGnode* root)
{
	CFGnode::flag++;
	dfs(root);
	while (h.size())
	{
		CFGnode*u=h.front();
		h.pop();
		u->g3.isUpdate=false;
		u->vis=0;
		for(int i=0;i<u->succ.size();i++) 
			u->g3.merge(u->succ[i]->g3.p);
		for(int i=0;i<u->defuse.use.size();i++)
		{
			int cnt=0;
			for(int j=0;j<u->defuse.use[i].unary.size();j++)
				if (u->defuse.use[i].unary[j]) cnt--;	//&
					//*
				else cnt++;
			if (cnt)
			{
				int id=u->defuse.use[i].id;
				for(;cnt>1;cnt--) id=find_next(NULL,id);
				u->g3.merge(u->g1.f[u->g1.find(id)].p);
			}
		}
		if (u->g3.isUpdate)
		{
			for(int i=0;i<u->prev.size();i++)
			{
				CFGnode *v=u->prev[i];
				if (v->flag!=v->vis)
				{
					h.push(v);
					v->vis=v->flag;
				}
			}
		}
	}
}