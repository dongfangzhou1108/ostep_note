# Free Space

# 使用说明

```cpp
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -S HEAPSIZE, --size=HEAPSIZE
                        size of the heap
  -b BASEADDR, --baseAddr=BASEADDR
                        base address of heap
  -H HEADERSIZE, --headerSize=HEADERSIZE
                        size of the header
  -a ALIGNMENT, --alignment=ALIGNMENT
                        align allocated units to size; -1->no align
  -p POLICY, --policy=POLICY
                        list search (BEST, WORST, FIRST)
  -l ORDER, --listOrder=ORDER
                        list order (ADDRSORT, SIZESORT+, SIZESORT-, INSERT-FRONT, INSERT-BACK)
  -C, --coalesce        coalesce the free list?
  -n OPSNUM, --numOps=OPSNUM
                        number of random ops to generate
  -r OPSRANGE, --range=OPSRANGE
                        max alloc size
  -P OPSPALLOC, --percentAlloc=OPSPALLOC
                        percent of ops that are allocs
  -A OPSLIST, --allocList=OPSLIST
                        instead of random, list of ops (+10,-0,etc)
  -c, --compute         compute answers for me
```

-S 堆大小

-b 堆内存基地址

-a 内存对齐大小

-H 内存头部信息大小

> [【C/C++】内存对齐（超详细，看这一篇就够了）_c++内存对齐-CSDN博客](https://blog.csdn.net/weixin_48896613/article/details/127371045)

-p  内存分配策略

-l 空闲列表顺序 （分别是：按照地址升序，地址空间尺寸升序，地址空间尺寸降序，将空闲列表插入前面、后面）

-C 合并空闲列表

-n 随机生成测试的次数

-r 最大分配内存大小

-P 分配操作百分比

-A 内存分配列表（与随机选项互斥）

# 答案

1. 首先运行flag -n 10 -H 0 -p BEST -s 0来产生一些随机分配和释放。你能预测malloc()/free()会返回什么吗？你可以在次请求后猜测空闲列表的状态吗？随着时间的推移，你对空闲列表有什么发现？

    > 关键点在于，flag不包含【合并空闲列表选项】，随着时间推移，空闲列表会逐渐增长
    >
2. 使用最差匹配策略搜索空闲列表（-p WORST）时，结果有何不同？什么改变了？

    > 空闲列表中的小块永远没法被利用
    >
3. 如果使用首次匹配（-p FIRST）会如何？使用首次匹配时，什么变快了？

    > 内存分配变快
    >
4. 对于上述问题，列表在保持有序时，可能会影响某些策略找到空闲位置所需的时间。使用不同的空闲列表排序（-l ADDRSORT，-l SIZESORT +，-l SIZESORT-）查看策略和列表排序如何相互影响。

    > BEST SIZESORT - / SIZESORT + 均可
    >
    > WORST SIZESORT - 速度最快
    >
    > FIRST SIZESORT + 速度最快
    >
5. 合并空闲列表可能非常重要。增加随机分配的数量（比如说-n 1000）。随着时间的推移，大型分配请求会发生什么？在有和没有合并的情况下运行（即不用和采用-C标志）。你看到了什么结果差异？每种情况下的空闲列表有多大？在这种情况下，列表的排序是否重要？

    > 发现内存分配请求失败
    >
    > 加入 -C 选项，内存空闲列表中碎片部分减少
    >
    > 重要，按照 ADDRSORT 对合并比较友好
    >

6. 将已分配百分比-P改为高于50，会发生什么？它接近100时分配会怎样？接近0会怎样？

    > 发现模拟器被设置为在未allocated时不会free，因此释放内存操作概率<=50%
    >
7. 要生成高度碎片化的空闲空间，你可以提出怎样的具体请求？使用-A标志创建碎片化的空闲列表，查看不同的策略和选项如何改变空闲列表的组织。

    > 不选择 -C，且选择 -p WORST
    >
