#Memory Leak Notes
###2017.5.13
---


## Input

+ `int proc_n`
> 不同的过程个数

+ `pair<CFGnode*, CFGnode*> proc_cfg[0..func_n - 1]`
> 每个过程对应的cfg

+ `int call_n`
> 调用的个数
+ `pair<int, int> call[0..call_n - 1]`
> 调用是从first号函数调用second号函数

## Output
+ 一个cfg图
> cfg图的每个结点对应 形如 call_i::call_j 的形式 



## Main

#####1. 对于每个过程对应的cfg，cfg里面有很多调用，加上一些边，每个调用对应一条这样的边。(clone完cfg之后再删掉)
#####2. 对于每个过程，clone 若干次。
#####3. 对于每个调用，删掉之前加的边，并且加一些边，对于所有可能的i,j,k ，call_j::call_i向 call_k::call_j连边