## libyuc
C语言基础组件库

> *个人实践项目，项目结构不稳定，仅供学习参考。*

## 容器
### 线性容器
#### Vector
- 动态数组
#### List
- 双向循环链表
#### SinglyList
- 单向链表
#### StaticList
- 静态链表
#### DoublyStaticList
- 双向静态链表
#### RingQueue
- 环形队列
#### Heap
- 优先级队列
### 索引容器
### HashTable
- 基于robin hood hashing的哈希表
#### RbTree
- 以234树为原型，嵌入颜色的无父结点红黑树
- 相较于C++STL的map，索引所占的空间仅为其1/2、1/4
#### BPlusTree
- 为磁盘设计的，页内嵌入红黑树的B+树
### 实验
#### AvlTree
- 嵌入平衡因子的无父结点平衡二叉树
#### RadixTree
- 映射定长key的基数树
#### ArTree
- 自适应基数树
#### SkipList
- 跳表
#### BTree
- 为内存设计的B树
## 无锁容器
### 索引
### 实验
#### TsSkipList
- 无锁跳表

## 引用
1. [unordered_dense](https://github.com/martinus/unordered_dense)
    - 参考了其哈希表的实现
2. [高性能无锁数据结构探索-无锁可删除跳表]https://zhuanlan.zhihu.com/p/600729377