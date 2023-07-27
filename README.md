# libyuc
便于做一些较为底层的开发时使用的基础库

**个人使用项目，代码可能随时调整，仅供学习参考。**

# container - 容器
## 数组
- [模板]vector
  - 动态数组
- bitmap
  - 位图
## 链表
- [模板]list
  - 双向循环链表
- [模板]SinglyList
  - 单向链表
- [模板]StaticList
  - 静态链表
- [模板]DoublyStaticList
  - 双向静态链表
## 队列
- queue
  - 基于数组的循环队列
- Heap
  - 堆
## 索引
- [模板]HashTable
  - 基于开放定址法(线性探测)的哈希表
- [模板]BsTree
  - 二叉搜索树
- [模板]AvlTree
  - 嵌入平衡因子的平衡二叉树
- [模板]RbTree
  - 以234树为原型，嵌入颜色的红黑树
- [模板]BPlusTree
  - 页内嵌入红黑树的B+树
- RadixTree
  - 映射定长key的基数树
- ArTree
  - 自适应基数树
- SkipList
  - 跳表