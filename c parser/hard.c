typedef unsigned int UI;
typedef unsigned int size_t;
static void *memheaplo;

static inline UI *pointer(UI *node) { return (UI *) (memheaplo + (*node)); }

static inline UI *pre(UI *node) { return pointer(node + 1); }

static inline UI *suc(UI *node) { return pointer(node + 2); }

static inline UI *fa(UI *node) { return pointer(node + 1); }

static inline UI *lc(UI *node) { return pointer(node + 2); }

static inline UI *rc(UI *node) { return pointer(node + 3); }

static inline int isson(UI *fa, UI *son) { return rc(fa) == son; }

static UI *list_head, *list_tail, *BST_root, *begin;
static int debug = 1, if_exit = 1;

/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
    return p <= mem_heap_hi() && p >= memheaplo;
}


static inline UI getsize(UI *p)//get pure size
{
    return (*p) >> 2 << 2;
}

static inline void set_pointer(UI *p, UI *q)
//set pointer like(pre,suc,fa,l_son,r_son) to be the offset by mem_heap_hi()
{
    *p = ((void *) q) - memheaplo;
}


static void init_node_list(UI *p, UI size, UI *pre, UI *suc)
//initialize the node in list
{
    *p = size;
    set_pointer(p + 1, pre);
    set_pointer(p + 2, suc);
    *(p + size / 4 - 1) = size;
}

static void init_node_BST(UI *p, UI size, UI *fa)
//initialize the node in BST
{
    *p = size;
    set_pointer(p + 1, fa);
    set_pointer(p + 2, 0);
    set_pointer(p + 3, 0);
    *(p + size / 4 - 1) = size;
}

int mm_init(void) {
    mem_sbrk(4 - ((unsigned long long) mem_heap_lo() % 4));    //for alignment
    memheaplo = mem_heap_lo();
    list_head = mem_sbrk(LIST_SIZE * 4);
    list_tail = mem_sbrk(LIST_SIZE * 4);
    BST_root = NULL;
    init_node_list(list_head, LIST_SIZE * 4, list_tail, list_tail);    //create list_head
    init_node_list(list_tail, LIST_SIZE * 4, list_head, list_head);    //create list_tail
    begin = memheaplo;
    *begin = 0;
    //mm_checkheap(109);
    return 0;
}

static void insert_BST(UI *r, UI *node, UI size)
//insert node into BST
{
    if (r == NULL)    //BST has no root
    {
        BST_root = node;
        init_node_BST(node, *node, memheaplo);
        return;
    }
    //printf("in %lld\n",(long long)r);
    UI rsize = getsize(r);
    int t = 2;
    if (rsize == size) t += rand() & 1;
    else if (rsize < size) t++;
    if (*(r + t)) insert_BST(*(r + t) + memheaplo, node, size);
    else {
        init_node_BST(node, *node, r);
        set_pointer(r + t, node);
    }
}

static void delete_from_list(UI *p)
//delete a free block from list
{
    UI Pre = *(p + 1), Suc = *(p + 2);
    *(pre(p) + 2) = Suc;
    *(suc(p) + 1) = Pre;
}

static UI *find_in_list(UI size)
//find a 'size' block in list
{
    if (suc(list_head) == list_tail) return NULL;
    UI *r = suc(list_head);
    if (getsize(r) >= size) {
        delete_from_list(r);
        return r;
    }
    r = pre(list_tail);
    if (getsize(r) >= size) {
        delete_from_list(r);
        return r;
    }
    return NULL;
}

static void delete_node(UI *r) {
    UI *FA = fa(r), *LC = lc(r);
    *(FA + isson(FA, r) + 2) = *(r + 2);
    if (*(r + 2)) *(LC + 1) = *(r + 1);
}

static void delete_from_BST(UI *r)
//delete r from BST
{
    UI *replace = NULL;
    if (*(r + 2) && *(r + 3)) {
        UI *t;
        for (replace = lc(r);; replace = t) {
            t = rc(replace);
            if (t == memheaplo) break;
        }
        delete_node(replace);
        *(replace + 1) = *(r + 1);
        UI *FA = fa(r);
        set_pointer((FA + isson(FA, r) + 2), replace);
        *(replace + 2) = *(r + 2);
        if (*(r + 2)) set_pointer(lc(replace) + 1, replace);
        *(replace + 3) = *(r + 3);
        if (*(r + 3)) set_pointer(rc(replace) + 1, replace);
    } else if (*(r + 2)) {
        UI *FA = fa(r);
        if (FA != memheaplo) *(FA + isson(FA, r) + 2) = *(r + 2);
        UI *C = lc(r);
        *(C + 1) = *(r + 1);
        replace = C;
    } else if (*(r + 3)) {
        UI *FA = fa(r);
        if (FA != memheaplo) *(FA + isson(FA, r) + 2) = *(r + 3);
        UI *C = rc(r);
        *(C + 1) = *(r + 1);
        replace = C;
    } else {
        UI *FA = fa(r);
        if (FA != memheaplo)
            *(FA + isson(FA, r) + 2) = 0;
    }
    //printf("delete %lld replace with %lld\n",(long long)r,(long long)replace);
    /*UI*FA=fa(r);
    if (FA!=memheaplo)
        set_pointer(FA+isson(FA,r)+2,replace);
    if (replace)
    {
        *(replace+1)=*(r+1);
        *(replace+2)=*(r+2);
        if (*(r+2)) set_pointer(lc(replace)+1,replace);
        *(replace+3)=*(r+3);
        if (*(r+3)) set_pointer(rc(replace)+1,replace);
    }*/
    if (BST_root == r) BST_root = replace;
    //mm_checkheap(0);
}

static UI *find_in_BST(UI *r, UI size)
//find a 'size' block in BST
{
    if (r == NULL) return NULL;
    if (r == memheaplo) return NULL;
    UI *ans = NULL;
    if (size > *r) return find_in_BST(rc(r), size);
    if (size < *r) ans = find_in_BST(lc(r), size);
    if (ans == NULL) {
        ans = r;
        delete_from_BST(r);
    }
    return ans;
}

static void insert_list(UI *r, UI size)
//insert r into list
{
    if (size == 16) {
        UI *Suc = suc(list_head);
        init_node_list(r, *r, list_head, Suc);
        set_pointer(list_head + 2, r);
        set_pointer(Suc + 1, r);
    } else {
        UI *Pre = pre(list_tail);
        init_node_list(r, *r, Pre, list_tail);
        set_pointer(Pre + 2, r);
        set_pointer(list_tail + 1, r);
    }
}

static UI *split(UI *r, UI size)
//split r into |----size--X-|----remain----|
{
    UI remain = getsize(r) - size;
    *r = size | ((*r) & 3);
    UI *ans = r + (size >> 2);
    *ans = remain;
    return ans;
}

static inline void delete_from_xxx(UI *r)
//delete r from xxx
{
    if (getsize(r) <= MAX_IN_LIST) delete_from_list(r);
    else delete_from_BST(r);
}

static inline void insert_xxx(UI *r) {
    UI tmp = getsize(r);
    if (tmp <= MAX_IN_LIST) insert_list(r, tmp);
    else insert_BST(BST_root, r, tmp);
}

static void *free_to_used(UI *r, UI size)
//set r to used. remember to left the status which pre block is or isn't free untouched.
//|----pre----|----free to used----|----split----|----suc----|
{
    UI free_size = getsize(r);
    UI *next = r + (free_size >> 2);
    UI *sp = NULL;
    UI remain = free_size - size;
    if (remain > MAX_IN_LIST) {
        sp = split(r, size);
        insert_BST(BST_root, sp, remain);
        *sp |= 2;
    } else if (remain >= MIN_IN_LIST) {
        sp = split(r, size);
        insert_list(sp, remain);
        *sp |= 2;
    }
    if (in_heap(next)) *next |= ((sp == NULL) << 1);
    else *begin |= ((sp == NULL) << 1);
    *r |= 1;
    return (void *) r;
}

static void SBRK(UI size)
//mem_sbrk a PAGE_SIZE and insert it to BST
{
    size = max(size, PAGE_SIZE);
    UI *p = mem_sbrk(size);
    *p = size;
    *p |= (*begin) & 2;    //begin restore the statu that the last block is or isn't used
    *begin &= ~2;
    if (((*p) & 2) == 0) {
        UI *prev = p - (*(p - 1)) / 4;
        if (prev != list_tail) {
            delete_from_xxx(prev);
            *prev += *p;
            p = prev;
        }
    }
    insert_xxx(p);
}

/*
 * malloc
 */
void *malloc(size_t size) {
    UI *ans = NULL;
    size = max(16, ALIGN(size + 4));
    if (size <= MAX_IN_LIST) ans = find_in_list(size);
    if (ans == NULL) ans = find_in_BST(BST_root, size);
    //if_exit=0;
    //mm_checkheap(0);
    //if_exit=1;
    if (ans == NULL) {
        //printf("alive!\n");
        SBRK(size);
        //mm_checkheap(276);
        ans = find_in_BST(BST_root, size);
    }
    void *ret = free_to_used(ans, size) + 4;
    //mm_checkheap(277);
    return ret;
}

static void used_to_free(UI *r)
//set r to free
{
    (*r)--;
    UI size = (*r) >> 2;
    UI *next = r + size;
    if (in_heap(next)) {
        if (is_used(next)) *next &= ~2u;
        else {
            UI tmp = getsize(next);
            delete_from_xxx(next);
            *r += tmp;
        }
    } else *begin &= ~2u;
    //printf("alive!\n");
    if (((*r) & 2) == 0) {
        //printf("size:%lld\n",(long long)*(r-1));
        UI *prev = r - (*(r - 1)) / 4;
        if (prev != list_tail) {
            //printf("size:%lld\n",(long long)r);
            delete_from_xxx(prev);
            //printf("alive!\n");
            *prev += *r;
            r = prev;
        }
        //printf("size:%lld\n",(long long)*r);
    }
    //printf("root: %lld\n",(long long)BST_root);
    insert_xxx(r);
}

/*
 * free
 */
void free(void *ptr) {
    if (ptr) {
        UI *p = ptr - 4;
        used_to_free(p);
        //mm_checkheap(329);
    }
}

static void set_end(UI *pre, int flag) {
    pre += getsize(pre) / 4;
    if (!in_heap(pre)) pre = begin;
    if (flag) *pre |= 2;
    else *pre &= ~2u;
}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *Oldptr, size_t size) {
    UI *oldptr = Oldptr;
    UI presize = size;
    if (oldptr == NULL) return malloc(size);    //if oldptr is NULL, call malloc
    else if (size == 0) free(oldptr);    //if size is zero, call free
    else    //otherwise
    {
        oldptr = oldptr - 1;
        size = max(16, ALIGN(size + 4));
        long long dlt = (long long) size - getsize(oldptr);
        if (dlt > 0)    //if the newsize is bigger than before
        {
            UI *next = oldptr + getsize(oldptr) / 4;
            if (in_heap(next) && !is_used(next) && (long long) getsize(next) >= dlt) {
                delete_from_xxx(next);
                //mm_checkheap(0);
                long long remain = getsize(next) - dlt;
                //|----used----|----new_used----|----free----|
                if (remain > MAX_IN_LIST) {
                    UI *sp = split(next, dlt);
                    *sp |= 2;
                    insert_BST(BST_root, sp, remain);
                    *oldptr += dlt;
                } else if (remain >= MIN_IN_LIST) {
                    UI *sp = split(next, dlt);
                    *sp |= 2;
                    insert_list(sp, remain);
                    *oldptr += dlt;
                } else {
                    set_end(next, 1);
                    *oldptr += getsize(next);
                }
                //mm_checkheap(369);
                return oldptr + 1;
            } else {
                UI *new = malloc(presize);
                new--;
                UI len = getsize(new) / 4;
                for (UI i = 1; i < len; i++) *(new + i) = *(oldptr + i);
                free(oldptr + 1);
                //mm_checkheap(378);
                return new + 1;
            }
        } else    //otherwise
        {
            //printf("dlt: %lld\n",-dlt);
            if (-dlt > MAX_IN_LIST) {
                UI *sp = split(oldptr, getsize(oldptr) + dlt);
                insert_BST(BST_root, sp, -dlt);
                *sp |= 2;
                set_end(sp, 0);

                /*UI ss=(*sp)>>2;
                UI*next=sp+ss;
                if (in_heap(next)&&!is_used(next))
                {
                    UI tmp=getsize(next);
                    delete_from_xxx(next);
                    *sp+=tmp;
                }*/
                //mm_checkheap(0);
                //*oldptr+=dlt;
            } else if (-dlt >= MIN_IN_LIST) {
                UI *sp = split(oldptr, getsize(oldptr) + dlt);
                insert_list(sp, -dlt);
                *sp |= 2;
                set_end(sp, 0);
                //*oldptr+=dlt;
            }
            //mm_checkheap(399);
            return oldptr + 1;
        }
    }
    return NULL;
}

/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc(size_t nmemb, size_t size) {
    UI *ans = malloc(nmemb * size);
    ans--;
    UI len = getsize(ans) / 4;
    for (UI i = 1; i < len; i++) *(ans + i) = 0;
    //mm_checkheap(416);
    return ans + 1;
}


/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
    return (size_t) ALIGN(p) == (size_t) p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int lineno) {
    if (debug) {
        fprintf(stdout, "Here in %d:\n", lineno);
        unsigned int *p = list_head;
        int cnt = 0;
        fprintf(stdout, "%lld\n", (long long) BST_root);
        int preused = 0;
        int error = 0;
        int is_pre_real = 0;
        for (; in_heap(p); p += getsize(p) / 4) {
            cnt++;
            if (getsize(p) == 0) {
                fprintf(stdout, "error!\n");
                exit(0);
            }
            if (is_used(p)) {
                fprintf(stdout, "%lld USED: Size:%lld\n", (long long) p, (long long) *p);
                if (preused ^ (((*p) & 2) >> 1)) {
                    fprintf(stdout, "status error!\n");
                    exit(0);
                }
                preused = 1;
            } else {
                if (getsize(p) <= MAX_IN_LIST)
                    fprintf(stdout, "%lld FREE: Size:%lld Pre:%lld Suc:%lld\n", (long long) p, (long long) *p,
                            (long long) pre(p), (long long) suc(p));
                else {
                    fprintf(stdout, "%lld FREE: Size:%lld Fa:%lld Lc:%lld Rc:%lld\n",
                            (long long) p, (long long) *p, (long long) fa(p), (long long) lc(p), (long long) rc(p));
                    if (BST_root == p && fa(p) != memheaplo) {
                        fprintf(stdout, "root error!\n");
                        exit(0);
                    }
                    if (BST_root != p) {
                        int t = isson(fa(p), p);
                        if ((t && rc(fa(p)) != p) || (!t && lc(fa(p)) != p)) {
                            fprintf(stdout, "father son error!\n");
                            error++;
                        }
                    }
                }
                if (preused ^ (((*p) & 2) >> 1)) {
                    fprintf(stdout, "status error!\n");
                    exit(0);
                }
                if (!preused && p != list_tail && p != list_head && is_pre_real) {
                    fprintf(stdout, "not good!\n");
                    exit(0);
                }
                preused = 0;
            }
            if (p != list_tail && p != list_head) is_pre_real = 1;
        }
        if (preused ^ (((*begin) & 2) >> 1)) {
            fprintf(stdout, "status error!\n");
            exit(0);
        }
        if (error && if_exit) exit(0);
    }
    //exit(0);
}
