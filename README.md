# 个人实践项目，仅供学习参考

## libyuc
便于做一些较为底层的开发时使用的基础库



## container - 容器
### 常规
- Vector
    - 动态数组
- List
    - 双向循环链表
- SinglyList
    - 单向链表
- StaticList
    - 静态链表
- DoublyStaticList
    - 双向静态链表
- RingQueue
    - 环形队列
- Heap
    - 优先级队列
## 索引
- HashTable
    - 基于robin hood hashing的哈希表
- RbTree
    - 以234树为原型，嵌入颜色的红黑树
- BPlusTree
    - 为磁盘设计的，页内嵌入红黑树的B+树
### 实验性
- AvlTree
    - 嵌入平衡因子的平衡二叉树
- RadixTree
    - 映射定长key的基数树
- ArTree
    - 自适应基数树
- SkipList
    - 跳表
- BTree
    - 为内存设计的B树
- TsSkipList
    - 无锁跳表