#include "myheader.h"
static queue<CFGnode*> h,q;
extern unordered_set<int> warning;
extern vector<pair<int,string> > res;
extern int Next[MAXN];
extern vector<string> int_to_string;
extern void link(CFGnode*,CFGnode*);
extern int id_to_line[MAXN];
extern int type[MAXN];
extern int malloc_cnt;
static void bfs_afterwards(CFGnode *root)
{
	assert(!q.size());
	if (root->g2.isUpdate)
	{
		CFGnode::flag++;
		for(int i=0;i<root->succ.size();i++)
		{
			q.push(root->succ[i]);
			root->succ[i]->vis2=root->flag;
		}
	}
	while (q.size())
	{
		CFGnode*u=q.front();
		q.pop();
		u->g2.isUpdate=false;
		u->vis2=0;
		for(int i=0;i<u->prev.size();i++) 
			u->g2.merge(u->prev[i]->g2.p);
		if (u->g2.isUpdate)
		{
			for(int i=0;i<u->succ.size();i++)
			{
				CFGnode *v=u->succ[i];
				if (v->flag!=v->vis2)
				{
					q.push(v);
					v->vis2=v->flag;
				}
			}
		}
	}
}
static void bfs_forwards(CFGnode *root)
{
	assert(!q.size());
	if (root->g3.isUpdate)
	{
		CFGnode::flag++;
		for(int i=0;i<root->prev.size();i++)
		{
			q.push(root->prev[i]);
			root->prev[i]->vis2=root->flag;
		}
	}
	while (q.size())
	{
		CFGnode*u=q.front();
		q.pop();
		u->g3.isUpdate=false;
		u->vis2=0;
		for(int i=0;i<u->succ.size();i++) 
			u->g3.merge(u->succ[i]->g3.p);
		if (u->g3.isUpdate)
		{
			for(int i=0;i<u->prev.size();i++)
			{
				CFGnode *v=u->prev[i];
				if (v->flag!=v->vis2)
				{
					q.push(v);
					v->vis2=v->flag;
				}
			}
		}
	}
}
static void dfs(CFGnode* u,int line)
{
	u->vis2=u->flag;
	if (u->ln%ln_delta_increment>=line) u->ln++;
	for(int i=0;i<u->succ.size();i++)
		if (u->succ[i]->vis2!=u->flag)
			dfs(u->succ[i],line);
}
static void addnode(CFGnode*pre,CFGnode*now,CFGnode*suc)
{
	for(int i=0;i<pre->succ.size();i++)
		if (pre->succ[i]==suc) pre->succ[i]=now;
	for(int i=0;i<suc->prev.size();i++)
		if (suc->prev[i]==pre) suc->prev[i]=now;
	now->prev.push_back(pre);
	now->succ.push_back(suc);
}
static bool bfs_find(CFGnode *root)
{
	while (h.size()) h.pop();
	int nowflag=++CFGnode::flag;
	h.push(root);
	root->vis=nowflag;
	while (h.size())
	{
		CFGnode *u=h.front();
		h.pop();
		if (u->ln%ln_delta_increment)
		{
			if (u->prev.size()==1)
				for(int i=0;i<u->prev[0]->list_of_vars.size();i++)
				{
					int id=u->prev[0]->list_of_vars[i].first;
					int base=id;
					string tmp=int_to_string[u->prev[0]->list_of_vars[i].second];
					vector<int> ids;ids.clear();ids.push_back(id);
					for(;Next[id];id=Next[id]) ids.push_back(Next[id]);
					int num=ids.size();
					while (ids.size())
					{
						string xing="";
						num--;
						for(int i=0;i<num;i++) xing+="*";
						int id=ids.back();
						ids.pop_back();
						if (u->canFreeBefore(id,Next))
						{
							fprintf(stderr,"before line%d free(%s%s)\n",u->ln%ln_delta_increment,xing.c_str(),tmp.c_str());
							int tid=u->prev[0]->g1.find(id);
							if (u->prev[0]->g1.f[tid].p.size()>1)
							{
								for(auto iter=u->prev[0]->g1.f[tid].p.begin();iter!=u->prev[0]->g1.f[tid].p.end();iter++)
								{
									warning.insert(*iter);
								}
							}
							for(int i=0;i<res.size();i++)
								if (res[i].first>=u->ln%ln_delta_increment) res[i].first++;
							for(int i=1;i<=malloc_cnt;i++)
								if (id_to_line[i]>=u->ln%ln_delta_increment) id_to_line[i]++;
							res.push_back(make_pair(u->ln%ln_delta_increment,"free("+xing+tmp+");"));
							CFGnode*free=new CFGnode(u->ln);
							CFGnode::flag++;
							dfs(root,u->ln%ln_delta_increment);
							free->g1=u->prev[0]->g1;
							free->g2=u->prev[0]->g2;
							free->g3=u->g3;
							free->list_of_vars=u->prev[0]->list_of_vars;
							free->g2.merge(u->prev[0]->g1.f[u->prev[0]->g1.find(id)].p);
							for(int i=0,ii=base;i<=num;i++,ii=Next[ii])
								free->g3.merge(u->prev[0]->g1.f[u->prev[0]->g1.find(ii)].p);
							addnode(u->prev[0],free,u);
							bfs_forwards(free);
							bfs_afterwards(free);
							return true;
						}
					}
				}
			for(int i=0;i<u->list_of_vars.size();i++)
			{
				int id=u->list_of_vars[i].first;
				int base=id;
				string tmp=int_to_string[u->list_of_vars[i].second];
				vector<int> ids;ids.clear();ids.push_back(id);
				for(;Next[id];id=Next[id]) ids.push_back(Next[id]);
				int num=ids.size();
				while (ids.size())
				{
					string xing="";
					num--;
					for(int i=0;i<num;i++) xing+="*";
					int id=ids.back();
					ids.pop_back();
					if (u->canFreeAfter(id,Next))
					{
						int tid=u->g1.find(id);
						if (u->cantAfter)
						{
							for(auto iter=u->g1.f[tid].p.begin();iter!=u->g1.f[tid].p.end();iter++)
							{
								warning.insert(*iter);
								type[-*iter]=1;
							}
							continue;
						}
						fprintf(stderr,"after line%d free(%s%s)\n",u->ln%ln_delta_increment,xing.c_str(),tmp.c_str());
						if (u->g1.f[tid].p.size()>1)
						{
							for(auto iter=u->g1.f[tid].p.begin();iter!=u->g1.f[tid].p.end();iter++)
							{
								warning.insert(*iter);
							}
						}
						for(int i=0;i<res.size();i++)
							if (res[i].first>=u->ln%ln_delta_increment+1) res[i].first++;
						for(int i=1;i<=malloc_cnt;i++)
							if (id_to_line[i]>=u->ln%ln_delta_increment+1) id_to_line[i]++;
						res.push_back(make_pair(u->ln%ln_delta_increment+1,"free("+xing+tmp+");"));
						CFGnode*free=new CFGnode(u->ln+1);
						CFGnode::flag++;
						dfs(root,u->ln%ln_delta_increment+1);
						free->g1=u->g1;
						free->g2=u->g2;
						free->g3=u->succ[0]->g3;
						free->list_of_vars=u->list_of_vars;
						free->g2.merge(u->g1.f[u->g1.find(id)].p);
						for(int i=0,ii=base;i<=num;i++,ii=Next[ii])
							free->g3.merge(u->g1.f[u->g1.find(ii)].p);
						addnode(u,free,u->succ[0]);
						bfs_forwards(free);
						bfs_afterwards(free);
						return true;
					}
				}
			}
		}
		for(int i=0;i<u->succ.size();i++)
			if (u->succ[i]->vis!=nowflag)
			{
				u->succ[i]->vis=nowflag;
				h.push(u->succ[i]);
			}
	}
	return false;
}
void final_pass(CFGnode* root)
{
	while (bfs_find(root));
}