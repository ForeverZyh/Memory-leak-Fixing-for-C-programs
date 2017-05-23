#include "myheader.h"
static queue<CFGnode*> h;
int Next[MAXN];
environment_identifiers env;
extern int environment_identifiers::unique_identifier_count;
extern map<pair<int, int>, int> environment_identifiers::added;
static void dfs_add(CFGnode* u)
{
	u->vis=u->flag;
	h.push(u);
	if (u->isLrac) env.left_bracket(u->isLrac);
	if (u->isRrac) env.right_bracket(u->isRrac);
	for(int i=0;i<u->identifier_list.size();i++)
	{
		env.add(u->identifier_list[i],u->ln);
	}
	for(int i=0;i<u->defuse.def.size();i++)
		u->defuse.def[i].id=env.get(u->defuse.def[i].id);
	for(int i=0;i<u->defuse.use.size();i++)
		u->defuse.use[i].id=env.get(u->defuse.use[i].id);
	for(int i=0;i<u->defuse.pure.size();i++)
	{
		u->defuse.pure[i].first.id=env.get(u->defuse.pure[i].first.id);
		u->defuse.pure[i].second.id=env.get(u->defuse.pure[i].second.id);
	}
	//u->g1.init(u->act_env_cnt=u->env.get_unique_identifier_count());
	for(int i=0;i<u->succ.size();i++)
		if (u->succ[i]->vis!=u->flag)
		{
			environment_identifiers tmp=env;
			dfs_add(u->succ[i]);
			env=tmp;
		}
}
static void dfs_put_back(CFGnode* u)
{
	if (u->tag!=-1)
	{
		CFGnode* next=u->succ[0];
		int id=u->identifier_list.back();
		assert(next->prev.size()==2);
		CFGnode* return_node=next->prev[1];
		for(int i=0;i<return_node->prev.size();i++)
			if (return_node->prev[i]->put_back)
			{
				assert(return_node->prev[i]->defuse.def.size()==1);
				return_node->prev[i]->defuse.def[0].id=id;
				if (return_node->prev[i]->defuse.pure.size())
				{
					assert(return_node->prev[i]->defuse.pure.size()==1);
					return_node->prev[i]->defuse.pure[0].first.id=id;
				}
			}
		next->prev.erase(next->prev.begin()+0);
		u->succ.erase(u->succ.begin()+0);
	}
	u->vis=u->flag;
	for(int i=0;i<u->succ.size();i++)
		if (u->succ[i]->vis!=u->flag)
		{
			dfs_put_back(u->succ[i]);
		}
}
int find_next(CFGnode*u,int id)
{
	if (Next[id]==0)
	{
		Next[id]=env.plus1_unique_identifier_count();
		u->g1.init(Next[id]);
		u->g1.isUpdate=true;
		printf("=========Next[%d]->%d==========\n",id,Next[id]);
	}
	return Next[id];
}
static void solve(CFGnode* u,const pair<pointer,pointer>&pure)
{
	if (pure.first.unary.size()+pure.second.unary.size()<2)
	{
		if (pure.first.unary.size()==1)
		{
			//*a=b
			int nx_fi=find_next(u,pure.first.id);
			u->g1.Union(nx_fi,pure.second.id);
		}
		else if (pure.second.unary.size()==1)
		{
			if (pure.second.unary[0])
			{
				//a=&b
				u->g1.f[pure.first.id].p.insert(pure.second.id);
				if (pure.second.id>0) u->g1.Union(find_next(u,pure.first.id),pure.second.id);
			}
			else
			{
				//a=*b
				int nx_se=find_next(u,pure.second.id);
				u->g1.Union(pure.first.id,nx_se);
			}
		}
		else
		{
			//a=b
			u->g1.Union(pure.first.id,pure.second.id);
		}
	}
	else if (pure.first.unary.size()>1)
	{
		int tmp=env.plus1_unique_identifier_count();
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
		int tmp=env.plus1_unique_identifier_count();
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
		int tmp=env.plus1_unique_identifier_count();
		u->g1.init(tmp);
		pair<pointer,pointer> t1(pointer(tmp),pure.first),t2(pointer(tmp),pure.second);
		solve(u,t1);
		solve(u,t2);
	}
}
static vector<int> E[MAXN];
void point_analysis(CFGnode* root)
{
	env.init();
	environment_identifiers::unique_identifier_count=0;
	environment_identifiers::added.clear();
	CFGnode::flag++;
	dfs_put_back(root);
	//CFGnode::flag++;
    //printf("final!========================\n");
    //root->print();
	CFGnode::flag++;
	dfs_add(root);
	while (h.size())
	{
		CFGnode* u=h.front();
		h.pop();
		u->vis=0;
		u->g1.isUpdate=false;
		u->g1.init(env.get_unique_identifier_count());
		for(int i=0;i<u->prev.size();i++)
		{
			CFGnode *v=u->prev[i];
			for(int j=1;j<v->g1.f.size();j++)
				E[j].clear();
			for(int j=1;j<v->g1.f.size();j++)
				E[v->g1.find(j)].push_back(j);
			for(int j=1;j<v->g1.f.size();j++)
			{
				if (E[j].size())
				{
					for(int k=1;k<E[j].size();k++)
						u->g1.Union(E[j][k-1],E[j][k]);
					u->g1.merge(u->g1.find(j),v->g1.f[j].p);
				}
			}
		}
		for(int i=0;i<u->defuse.pure.size();i++)
		{
			if (u->defuse.pure[i].second.id&&u->defuse.pure[i].second.id!=-MAGIC_NUMBER) 
			{
				pair<pointer,pointer> it=u->defuse.pure[i];
				it.first.id=u->g1.find(it.first.id);
				if (it.second.id>0) it.second.id=u->g1.find(it.second.id);
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