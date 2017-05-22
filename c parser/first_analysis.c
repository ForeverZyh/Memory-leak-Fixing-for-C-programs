#include "myheader.h"
static int vis[MAXN];
static int flag=0;
static void dfs(CFGnode* u)
{
	u->vis=u->flag;
	flag++;
	for(int j=1;j<u->g1.f.size();j++)
	{
		int id=u->g1.find(j);
		if (vis[id]!=flag)
		{
			vis[id]=flag;
			if (u->g1.f[id].p.find(-MAGIC_NUMBER)==u->g1.f[id].p.end())
			{
				for(auto it=u->g1.f[id].p.begin();it!=u->g1.f[id].p.end();it++)
				{
					u->g2.insert(*it);
				}
			}
		}
	}
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
}