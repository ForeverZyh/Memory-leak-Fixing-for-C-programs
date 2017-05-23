#include "myheader.h"
static queue<CFGnode*> h;
extern int find_next(CFGnode*,int id);
static void dfs(CFGnode* u)
{
	u->vis=u->flag;
	h.push(u);
	for(int i=0;i<u->succ.size();i++)
	{
		CFGnode* v=u->succ[i];
		if (v->vis!=v->flag) dfs(v);
	}
}
void first_analysis(CFGnode* root)
{
	CFGnode::flag++;
	dfs(root);
	while (h.size())
	{
		CFGnode*u=h.front();
		h.pop();
		u->g2.isUpdate=false;
		u->vis=0;
		for(int i=0;i<u->prev.size();i++) 
			u->g2.merge(u->prev[i]->g2.p);
		for(int i=0;i<u->defuse.pure.size();i++)
			if (u->defuse.pure[i].second.id==-MAGIC_NUMBER)
			{
				const pointer&it=u->defuse.pure[i].first;
				int cnt=0;
				for(int j=0;j<it.unary.size();j++)
					if (it.unary[j]) cnt--;
					else cnt++;
				int id=it.id;
				for(;cnt>0;cnt--) id=find_next(NULL,id);
				u->g2.merge(u->g1.f[u->g1.find(id)].p);
			}
		if (u->g2.isUpdate)
		{
			for(int i=0;i<u->succ.size();i++)
			{
				CFGnode *v=u->succ[i];
				if (v->flag!=v->vis)
				{
					h.push(v);
					v->vis=v->flag;
				}
			}
		}
	}
}