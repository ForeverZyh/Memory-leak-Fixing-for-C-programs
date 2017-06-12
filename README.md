#编译实习报告-C程序内存泄漏修复
张煜皓、李奕成、陈牧歌
#代码执行方法
**环境**

在Ubuntu下，使用

```
sudo apt-get install flex
sudo apt-get install bison
```

安装必要的flex与bison

**使用方式**

```
make clean
make
./parser in.c out.c
```

#项目描述

###**目标问题**

旨在自动修复C程序中潜在的导致内存泄漏的代码。

由于判定一段程序是否内存泄漏为图灵不可判定问题，所以我们的程序的修复目标为，尽量多地修复出错代码。

###**解决思路**

前端->中间表示(CFG)->前端

首要任务是将代码转换为中间表示从而方便分析，我们选择了适合做数据流分析的CFG来作为中间表示。

然后通过对中间表示的分析，来计算如何修复代码。

最后通过计算结果得到修改后的C代码。

#实现方案

##前端->中间表示

这部分可以看`parser.l`部分代码来看具体逻辑。

首先我们需要将代码转换为中间表示。

得到抽象语法树之后，将其分析获得CFG。

该部分代码参考`cfg.c`

我们首先对每个函数都构建了cfg，然后执行Interprocedural CFG* 过程来合并得到整个程序的cfg。

该部分代码参考`clone_build_graph.c`

###CFGnode

在此我们总得来介绍一下CFGnode中保存的内容

```c
struct CFGnode
{
    vector<CFGnode*> succ,prev;
    expr defuse;
    vector<int> identifier_list,vars_to_kill;
    vector<pair<int,int> > list_of_vars;
    int isRrac,isLrac,ln,vis,vis2,vis3,vis4,tag,call_index;
    bool cantAfter,put_back,isFirst,isLast;
    G1 g1;
    G2 g2,g3;
    static int rac_cnt;
    static int flag;
}
```
* succ和prev表示的是图中节点的后继和前驱
* defuse表示的是该节点中各个变量define和use的关系
* identifier_list保存的是该节点中声明的变量
* vars\_to_kill保存的是该节点中可以被free的变量名
* list\_of_vars保存的是当前节点作用域中的变量（也包含一些并不存在于程序中的局部变量）
* 如果该节点实际对应着原程序中的一个语句的话，ln会保存这个语句所在的行号
* isLrac(isRrac)表示该语句是不是一个左（右）大括号
* cantAfter表示该语句是否是一个return,continue,break等跳转语句，如果是，则没必要在后面添加free
* 在ln有值存在之时，isFirst(isLast)表示的是是否是该条语句的第一个点（最后一个点），如果不是最后一个点，则不能在这个点后面添加free。因为我们无法拆分一条语句
* g1表示的是指针分析的半格，g2,g3表示的是第一次分析与第二次分析的半格。


##分析部分
###Point To Analysis 

得到我们想要的cfg之后，我们立即执行`point_analysis(begin)`来进行指针分析，分析结果用于后面数据流分析。

目标是获得在每个CFG节点上，各个变量所可能指向的位置，也能够得出各个变量中可能存放着由哪些malloc语句开辟的内存地址。

传统的指向性分析算法分为两种，Steensgaard和Anderson算法。Anderson算法的特点是精度高，但是效率低下，如果变量个数为n的话，时间复杂度将达到`O(n^3)`。Steensgaard算法的特点是精度较低，但是效率非常高，分析一遍的时间复杂度为`O(n𝛼(n))`。在此我们选择Steensgaard算法作为我们指针分析的算法。

但是不幸的是，两者都是流非敏感的分析，这在我们进行分析的时候将十分不友好，简而言之会导致极低的准确度。为此，我们将其修改为流敏感的指针分析。其方法就是：**在每个CFG节点之中保存一份分析的结果，我们需要实现的就只有：1、加入一个操作；2、合并两个并查集。对于第一种操作，我们像传统的Steensgaard一样更改并查集的信息。对于第二种操作，合并并查集A和B，要保证x和y只要在A或B中是处于同一个集合，在最后的集合中C，也要保证x和y在C中属于同一个集合。这一步可以在也`O(n𝛼(n))`的时间内完成。故总时间复杂度为`O(n^2𝛼(n))`。**

该部分代码参考`point_analysis.c`

###First Analysis 
指针分析结束之后就可以进行第一次分析，这一次分析的目标旨在获得在每个CFG节点上，哪些malloc语句开辟的内存地址已被释放。

这一步实现很方便，利用我们之前指针分析得到的信息，在CFG上正向地执行类似bfs的流程（可能会有反复入队），就可以计算半格了。

该部分代码参考`first_analysis.c`
###Second Analysis (Liveness Analysis)

然后进行第二次分析，这次要分析出在每个CFG节点上，哪些malloc语句开辟的内存地址在之后还是活跃的。

这一步实现与第一步类似，我们倒着遍历CFG更新半格即可。

该部分代码参考`second_analysis.c`
###Final Pass

在做完两次分析之后，我们就可以决策在哪些地方插入free语句来修复内存泄漏了。

假设有u->v的边，如果要在u之后添加一条free(x)语句，必须要满足：

x中可能指向的malloc的地址在v之后不被使用，且在u之前未被释放

* 但是这里会有一个小问题，如果u是一个`if`语句的判断表达式，那么u之后有两条边，分别连向v1,v2，那么需要做的就是在v1和v2前都插入free(x)语句。

而且在决策之后，我们还需要更新第一次分析和第二次分析的结果，从而继续修复。

添加free语句的策略，我们选择贪心，尽可能早地添加free语句。

选择贪心是因为我们没有找到时间与空间上高效的更优秀策略。

该部分代码参考`final_pass.c`

显然这样不是最优的，参考`test-case6.c`代码


```c
int *x,*y,*z;
int foo2(int *x,int *y)
{
	return *y;
}
int foo1(int *x,int *y)
{
	
	if ((*x)*2<*y) 
	{
		return *x;
	}
	else 
	{
		return foo2(x,*y+1);
	}
	
}
int foo(int *a,int *b,int *c)
{
	if (*a>*c) 
	{
		return foo1(a,c);
	}
	else
	{
		return foo2(b,c);
	}
}
int main()
{
	x=malloc(sizeof(int));
	y=malloc(sizeof(int));
	int n=10;
	*x=3;*y=4;z=&n;
	int ans=foo(x,y,z);
}
```

因为我们会在最早能free的地方free，所以我们的程序运行结果是

```c
int *x,*y,*z;
int foo2(int *x,int *y)
{
	return *y;
}
int foo1(int *x,int *y)
{
	
	if ((*x)*2<*y) 
	{
		return *x;
	}
	else 
	{
/*Created by our project*/	free(x);	/************************/
		return foo2(x,*y+1);
	}
	
}
int foo(int *a,int *b,int *c)
{
	if (*a>*c) 
	{
		return foo1(a,c);
	}
	else
	{
/*Created by our project*/	free(a);	/************************/
		return foo2(b,c);
	}
}
int main()
{
	x=malloc(sizeof(int));
	y=malloc(sizeof(int));
	int n=10;
	*x=3;*y=4;z=&n;
/*Created by our project*/	free(y);	/************************/
	int ans=foo(x,y,z);
}
/*Warning created by our project
	After fixing, malloc in line34 may still cause memory leaking.
		Dereference when return
*******************************/

```

可以看到free(x)和free(a)的目的都是为了free main()中malloc出来的，x指向的内存区域。

这就是我们的贪心的反例。因为显然在`int ans=foo(x,y,z);`之后free(x)更加安全。

#测试用例设计

我们总共设计了13组测试数据。

其中10组是手工构造的，3组大数据由代码随机生成。

手工构造的数据中存在像test6.c这样的贪心反例，也存在一些其他的构造数据。

而且可以看到，我们的修复中还支持对于不能修复的地方报出warning，方便用户更加精确地修复。

###test1.c

目标是测试程序对多重指针的修复能力。

代码为

```c
int main()
{
	int **z=malloc(sizeof(int*));
	*z=malloc(sizeof(int));
	**z=0;
	return 0;
}
```

修复结果为

```c
int main()
{
	int **z=malloc(sizeof(int*));
	*z=malloc(sizeof(int));
	**z=0;
/*Created by our project*/	free(*z);	/************************/
/*Created by our project*/	free(z);	/************************/
	return 0;
}
```

可以看出来修复无误，这离不开指针分析与前两次分析为我们提供的各变量指向内存的信息。

###test2.c
目标为测试面对分支时程序的修复能力。

代码为

```c
int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
		int *z=malloc(sizeof(int));
		*z=*y;
	}
	else
	{
		x=y;
		//free(y);
	}
	return 0;
}
```

修复结果为

```c
int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
/*Created by our project*/	free(x);	/************************/
		int *z=malloc(sizeof(int));
		*z=*y;
/*Created by our project*/	free(y);	/************************/
/*Created by our project*/	free(z);	/************************/
	}
	else
	{
/*Created by our project*/	free(y);	/************************/
/*Created by our project*/	free(x);	/************************/
		x=y;
		//free(y);
	}
	return 0;
}
```

值得注意的是else里我们在x=y这句之前就把x,y都给free掉了，这是因为我们已经判定x,y所指向的内存空间之后再也不会被用到了。

### test3.c

与test2测试目的相同，同样也是为了测试在分支下的鲁棒性，代码几乎也与test2一样

```c
int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
		int *z=malloc(sizeof(int));
		*z=*y;
	}
	else
	{
		x=y;
		free(y);
	}
	return 0;
}
```

test3与test2最大的区别就在于else中加入了free语句，于是我们不能在x=y之前将y free掉了。

修复结果为

```c
int *x;
int main()
{
	int *y=malloc(sizeof(int));
	x=malloc(sizeof(int));
	*x=0;
	*y=1;
	if (*x>*y)
	{
/*Created by our project*/	free(x);	/************************/
		int *z=malloc(sizeof(int));
		*z=*y;
/*Created by our project*/	free(y);	/************************/
/*Created by our project*/	free(z);	/************************/
	}
	else
	{
		x=y;
		free(y);
	}
	return 0;
}
/*Warning created by our project
	After fixing, malloc in line4 may still cause memory leaking.
	After fixing, malloc in line5 may still cause memory leaking.
*******************************/
```

注意到else分支中，x一开始malloc出的那片内存可能会泄漏，但是在free(y)的时候，我们判断x和y指向的集合相同（因为Steengard的缺陷）。所以我们无法区x和y，warning也是如此。

### test4.c

主要目的是为了测试循环情况下的修复能力。

```c
int main()
{
	int b=10;
	int *x=&b;
	for(int i=1;i<=b;i++)
	{
		int *z=malloc(sizeof(int));
		*z=*x+i;
		if (*z>i*2)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	return 0;
}
```

修复结果为

```c
int main()
{
	int b=10;
	int *x=&b;
	for(int i=1;i<=b;i++)
	{
		int *z=malloc(sizeof(int));
		*z=*x+i;
		if (*z>i*2)
		{
/*Created by our project*/	free(z);	/************************/
			break;
		}
		else
		{
			i++;
		}
	}
	return 0;
}
```

会注意到我们的程序少free了else分支时的z。*但是由于对于循环的处理不是很到位，没法判断warning。*

这一点与我们建cfg时处理循环的方式有关，与递归相同，我们同样也难以判断循环执行的情况。

### test5.c

```c
void swap(int*x,int*y)
{
	int *t=x;
	x=y;
	y=x;
}
int main()
{
	int *haha=malloc(sizeof(int));
	int *youcantcatchme=haha;
	*haha=1;
	int *x=malloc(sizeof(int));
	swap(x,haha);
	*youcantcatchme=0;
	}
```

这段代码也主要是在hack我们的程序，可以看出来swap函数实际上是一个无副作用的函数。

但是加上了这个函数之后，我们修复的结果为

```c
void swap(int*x,int*y)
{
	int *t=x;
	x=y;
	y=x;
}
int main()
{
	int *haha=malloc(sizeof(int));
	int *youcantcatchme=haha;
	*haha=1;
	int *x=malloc(sizeof(int));
	swap(x,haha);
	*youcantcatchme=0;
/*Created by our project*/	free(x);	/************************/
}
/*Warning created by our project
	After fixing, malloc in line12 may still cause memory leaking.
	After fixing, malloc in line9 may still cause memory leaking.
*******************************/
```

表现出来的是我们少free了haha声明时alloc出的变量。这是因为程序分析时，我们数据流分析进行的是近似分析，所以才会出现这个情况。

###test6.c

即之前分析过的说贪心添加free可能不够优的问题。

###test7.c

```c
int *sum;
void foo(int x)
{
	*sum*=x;
	if (x==1)
	{
		return ;
	}
	foo(x-1);
}
int main()
{
	sum=malloc(sizeof(int));
	foo(10);
	return 0;
}
```

容易看出来这是一个简单的阶乘计算程序。

递归程序的主要问题在于我们难以构造出精确的CFG（因为不知道递归深度、递归分支情况等）

我们采取的是Clone-based Context-Sensitive Analysis来进行Interprocedural Analysis。

面对递归的时候会直接展开k步进行复制替换。

程序修复结果为

```c
int *sum;
void foo(int x)
{
	*sum*=x;
	if (x==1)
	{
		return ;
	}
	foo(x-1);
}
int main()
{
	sum=malloc(sizeof(int));
	foo(10);
/*Created by our project*/	free(sum);	/************************/
	return 0;
}
```

可以看出面对这样简单的递归，我们的算法工作正常。

中间调试信息可以参考`test7-extra.txt`，因为过长故不在此列出。

### test8.c

主要是测试多重循环及变量重名时程序的处理能力。

```c
int main()
{
	int n=10;
	int *x=&n;
	for(int i=1;i<=n;i++)
	{
		int *x=malloc(sizeof(int));
		for(int j=1;j<=n;j++)
		{
			int *x=malloc(sizeof(int));
		}
	}
	return 0;
}
```

结果为

```c
int main()
{
	int n=10;
	int *x=&n;
	for(int i=1;i<=n;i++)
	{
		int *x=malloc(sizeof(int));
/*Created by our project*/	free(x);	/************************/
		for(int j=1;j<=n;j++)
		{
			int *x=malloc(sizeof(int));
/*Created by our project*/	free(x);	/************************/
		}
	}
	return 0;
}
```

这个修复结果还是令人满意的。

###test9.c

同样，与test7类似，还是测试递归函数，这次选择了fibonacci函数，并且在递归时malloc。

```c
int fib(int n)
{
	int *ret;
	if (n==0)
	{
		return 1;
	}
	ret=malloc(sizeof(int));
	if (n==1)
	{
		return 1;
	}
	*ret=fib(n-1) + fib(n-2);
	return *ret;
}
int main()
{
	fib(10);
	return 0;
}
```

结果为

```c
int fib(int n)
{
	int *ret;
	if (n==0)
	{
		return 1;
	}
	ret=malloc(sizeof(int));
	if (n==1)
	{
/*Created by our project*/	free(ret);	/************************/
		return 1;
	}
	*ret=fib(n-1) + fib(n-2);
	return *ret;
}
int main()
{
	fib(10);
	return 0;
}

/*Warning created by our project
	After fixing, malloc in line8 may still cause memory leaking.
		Dereference when return
*******************************/
```

效果并不好，这也是可以预见到的，毕竟根据我们的算法，只有n==1的时候，我们可以保证free掉(ret)而又不改变语义信息。

毕竟我们所执行的修改只有添加free吗所以在return调用了*ret的话，我们完全没有机会通过添加free来释放malloc出的空间。

###test10.c

```c
int *x;
int a()
{
	
}
int b()
{
	a();
}
int main()
{
	x=malloc(sizeof(int));
	a();
	*x=1;
	b();
	return 0;
}
```

主要是为了测试在跨越几个过程中，我们的程序对一个变量的追踪能力。

结果为

```c
int *x;
int a()
{
	
}
int b()
{
	a();
}
int main()
{
	x=malloc(sizeof(int));
	a();
	*x=1;
/*Created by our project*/	free(x);	/************************/
	b();
	return 0;
}
```

结果也是可以接受的。


对于手工构造数据的分析到此结束。


#完成情况与存在的问题

我们完成了开题报告中的全部目标。但我们的项目仍存在一些问题:

* 对递归时产生的内存泄漏解决能力较差，这一点需要更好的构造CFG来解决。
* 策略上的问题，会有一些可以修复的泄漏我们没有修复，或者加入的free语句过多，期待能找到更好的策略。


#致谢

* 感谢所有成员的付出与合作
* 感谢老师和助教的指导
* 感谢其他同学的建议与支持


