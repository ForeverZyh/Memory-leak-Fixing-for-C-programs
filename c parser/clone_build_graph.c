#include "myheader.h"

#define MAX_PROC_NUM 10000

#define MAX_CALL_NUM 100000

#define MAX_CALLPAIR_NUM  1000000
extern map<string,int> string_to_int;
extern vector<func> fun;
extern int get_func(int name);
extern CFGnode* CFGnode::vt_;
extern CFGnode* CFGnode::new_vt_;
extern unordered_map<CFGnode*,CFGnode*> CFGnode::Hash;
//extern pair<CFGnode*,CFGnode*> CFGnode::clone_cfg(CFGnode *vs, CFGnode* vt);

int proc_n = 0;
vector<int> G[MAX_PROC_NUM];

int current_proc = -1;

/*
 * each call is a 4-elem triple.
 *
 */
int call_n = 0;
int caller_proc_list[MAX_CALL_NUM], callee_proc_list[MAX_CALL_NUM];
CFGnode * zyhbegin_list[MAX_CALL_NUM], *zyhend_list[MAX_CALL_NUM];
pair<CFGnode*,CFGnode*> callpair[MAX_CALLPAIR_NUM];


/*
 * dfs_CFG_in_single_proc
 *
 * for each procedure, dfs it, ans find out each call to calculate our 4-elem triple list.
 */
void dfs_CFG_in_single_proc(CFGnode *u)
{
    u->vis2 = 1;
    if (u->tag != -1)
    {
        caller_proc_list[call_n] = current_proc;
        callee_proc_list[call_n] = u->tag;
        zyhbegin_list[call_n] = u;
        zyhend_list[call_n] = u->succ[0];
        
        G[current_proc].push_back(call_n);

        u->call_index = ++call_n;

        //++ call_n;

    }
    for(unsigned k = 0;k < u->succ.size(); ++k)
    {
        CFGnode *v = u->succ[k];
        if (!v->vis2)
        {
            dfs_CFG_in_single_proc(v);
        }
    }
}


int to_non_hole_proc_index(int proc_index_with_hole)
{
    return get_func(proc_index_with_hole);
}


void addedge(CFGnode *u, CFGnode *v)
{
    u->succ.push_back(v);
    v->prev.push_back(u);
}



map<pair<int,int>, int> callpair_vis;

int callpair_n = 0;



/*
 *
 * dfs_procedure3
 *
 * after a cloning, we should update the ln of each CFGnode of the new procedure.
 *
 */
int ln_delta = 0, ln_delta_increment = 100000;

void dfs_procedure3(CFGnode *u)
{
    u->vis3 = 1;
    u->ln += ln_delta;
    if (u->isRrac) u->isRrac += ln_delta;
    if (u->isLrac) u->isLrac += ln_delta;
    for(unsigned k = 0;k < u->succ.size(); ++k)
    {
        CFGnode *v = u->succ[k];
        if (!v->vis3)
        {
            dfs_procedure3(v);
        }
    }
}


/*
 *
 * dfs_procedure4
 *
 * current callpair is  call_now::call_last,  we want to dfs this procedure to find all next pairs, call_index::call_now
 *
 *
 */
void dfs_whole_CFG(int u, int call_now, int call_last, CFGnode *begin, CFGnode *end);
void dfs_procedure4(CFGnode *u, int call_now)
{
    u->vis4 = 1;
    if (u->tag != -1)
    {
        int callee_proc = u->tag;
        int call_index = u->call_index;

        dfs_whole_CFG(callee_proc, call_index, call_now, u, u->succ[0]);
    }
    for(unsigned k = 0;k < u->succ.size(); ++k)
    {
        CFGnode *v = u->succ[k];
        if (!v->vis4)
        {
            dfs_procedure4(v, call_now);
        }
    }
}

void dfs_whole_CFG(int u, int call_now, int call_last, CFGnode *begin, CFGnode *end)
{
    if (callpair_vis.find(pair<int, int>(call_now, call_last))!=callpair_vis.end())
    {
        int id=callpair_vis[pair<int, int>(call_now, call_last)];
        addedge(begin, callpair[id].first);
        addedge(callpair[id].second, end);
        return;
    }
    
    ln_delta += ln_delta_increment;

    callpair_vis[pair<int, int>(call_now, call_last)] = callpair_n;
    
        /*
         * if call_last::call_now hasn't visited, we clone the procedure.
         */
        int non_hole_proc_index = to_non_hole_proc_index(u);
        func f = fun[non_hole_proc_index];
        CFGnode *proc_begin = f.CFG.first, *proc_end = f.CFG.second;

        pair<CFGnode*, CFGnode*> cloned_CFG = CFGnode::clone_cfg(proc_begin, proc_end);
        callpair[callpair_n]=cloned_CFG;
        
        //begin = cloned_CFG.first, end = cloned_CFG.second;

        ++ callpair_n;

    dfs_procedure3(cloned_CFG.first);

    dfs_procedure4(cloned_CFG.first, call_now);

    addedge(begin, cloned_CFG.first);
    addedge(cloned_CFG.second, end);
}



void clone_build_graph_init()
{
    proc_n = 0;
    current_proc = -1;
    call_n = 0;


    ln_delta = 0;
    ln_delta_increment = 100000;

    callpair_n = 0;
    callpair_vis.clear();


    for(int i=0;i<fun.size();i++)
    {
        current_proc = fun[i].id;
        dfs_CFG_in_single_proc(fun[i].CFG.first);
    }
}

pair<CFGnode*, CFGnode*> clone_build_graph_work()
{
    CFGnode* begin=new CFGnode(),* end=new CFGnode();
    dfs_whole_CFG(string_to_int["main"], 0, 0, begin, end);
    return make_pair(begin,end);
}
