#include "myheader.h"
queue<CFGnode*> h;
static int Next[MAXN];
extern int environment_identifiers::unique_identifier_count;
extern map<pair<int, int>, int> environment_identifiers::added;
void dfs_add(CFGnode* u)
{
	u->vis=u->flag;
	h.push(u);
	if (u->isLrac) u->env.left_bracket(u->isLrac);
	if (u->isRrac) u->env.right_bracket(u->isRrac);
	for(int i=0;i<u->identifier_list.size();i++)
	{
		u->env.add(u->identifier_list[i],u->ln);
	}
	//u->g1.init(u->act_env_cnt=u->env.get_unique_identifier_count());
	for(int i=0;i<u->succ.size();i++)
		if (u->succ[i]->vis!=u->flag)
		{
			u->succ[i]->env=u->env;
			dfs_add(u->succ[i]);
		}
}
int find_next(CFGnode*u,int id)
{
	if (Next[id]==0)
	{
		Next[id]=u->env.plus1_unique_identifier_count();
		u->g1.isUpdate=true;
		printf("=========Next[%d]->%d==========\n",id,Next[id]);
	}
	return Next[id];
}
void solve(CFGnode* u,const pair<pointer,pointer>&pure)
{
	if (pure.first.unary.size()+pure.second.unary.size()<2)
	{
		if (pure.first.unary.size()==1)
		{
			//*a=b
			u->g1.Union(find_next(u,pure.first.id),pure.second.id);
		}
		else if (pure.second.unary.size()==1)
		{
			if (pure.second.unary[0])
			{
				//a=&b
				u->g1.f[pure.first.id].p.clear();
				u->g1.f[pure.first.id].p.insert(pure.second.id);
				if (pure.second.id>0) u->g1.Union(find_next(u,pure.first.id),pure.second.id);
			}
			else
			{
				//a=*b
				u->g1.Union(pure.first.id,find_next(u,pure.second.id));
			}
		}
		else
		{
			//a=b
			u->g1.f[u->g1.find(pure.first.id)].p=u->g1.f[u->g1.find(pure.second.id)].p;
		}
	}
	else if (pure.first.unary.size()>1)
	{
		int tmp=u->env.plus1_unique_identifier_count();
		u->g1.init(tmp);
		pair<pointer,pointer> t1(pointer(tmp),pure.first),t2=pure;
		bool mk=t1.second.unary.back();
		t1.second.unary.pop_back();
		solve(u,t1);
		t2.first.init(tmp);
		t2.first.unary.push_back(mk);
		solve(u,t2);
	}
	else if (pure.second.unary.size()>1)
	{
		int tmp=u->env.plus1_unique_identifier_count();
		u->g1.init(tmp);
		pair<pointer,pointer> t1(pointer(tmp),pure.second),t2=pure;
		bool mk=t1.second.unary.back();
		t1.second.unary.pop_back();
		solve(u,t1);
		t2.second.init(tmp);
		t2.second.unary.push_back(mk);
		solve(u,t2);
	}
	else
	{
		int tmp=u->env.plus1_unique_identifier_count();
		u->g1.init(tmp);
		pair<pointer,pointer> t1(pointer(tmp),pure.first),t2(pointer(tmp),pure.second);
		solve(u,t1);
		solve(u,t2);
	}
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
		u->g1.init(u->env.get_unique_identifier_count());
		for(int i=0;i<u->prev.size();i++)
		{
			CFGnode *v=u->prev[i];
			for(int j=1;j<v->g1.f.size();j++) u->g1.merge(j,v->g1.f[j].p);
		}
		for(int i=0;i<u->defuse.pure.size();i++)
		{
			if (u->defuse.pure[i].second.id) 
			{
				pair<pointer,pointer> it=u->defuse.pure[i];
				it.first.id=u->env.get(it.first.id);
				it.second.id=u->env.get(it.second.id);
				solve(u,it);
			}
		}
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