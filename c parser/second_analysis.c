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
static void process(CFGnode*u,const pointer&it,bool flag=0)
{
	int cnt=0;
	for(int j=0;j<it.unary.size();j++)
		if (it.unary[j]) cnt--;	//&
			//*
		else cnt++;
	cnt+=flag;
	if (cnt)
	{
		int id=it.id;
		for(;cnt>=0;cnt--)
		{
			u->g3.merge(u->g1.f[u->g1.find(id)].p);
			if (cnt>1) id=find_next(NULL,id);
		}
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
		for(int i=0;i<u->defuse.use.size();i++) process(u,u->defuse.use[i]);
		for(int i=0;i<u->defuse.def.size();i++) process(u,u->defuse.def[i]);
		for(int i=0;i<u->defuse.pure.size();i++)
			if (u->defuse.pure[i].second.id==-MAGIC_NUMBER) 
				process(u,u->defuse.pure[i].first,1);
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