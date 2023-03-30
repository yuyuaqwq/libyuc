# CUtils
C语言便携工具类支持  
方便我在做一些较底层的开发时使用的纯C工具支持库

**个人使用项目，代码可能随时调整，仅供学习参考。**

# 容器
## 数组
- [模板]vector
  - 动态数组
- queue
  - 基于数组的循环队列
- bitmap
  - 位图
## 链表
- [模板]list
  - 双向循环链表
- [模板]SinglyList
  - 单向链表
- [模板]StaticList
  - 静态链表
- DoublyStaticList
  - 双向静态链表
- SkipList
  - 跳表
## 散列表
- [模板]HashTable
  - 存储对象指针的链式哈希表
## 树
- [模板]BsTree
  - 二叉搜索树
- [模板]AvlTree
  - 存储高度/嵌入平衡因子/无父结点的平衡二叉树
- [模板]RbTree
  - 以234树为原型，嵌入颜色的红黑树
- [模板]BPlusTree
  - 页内嵌入红黑树的B+树
- RadixTree
  - 映射定长key的基数树
- Heap
  - 堆

