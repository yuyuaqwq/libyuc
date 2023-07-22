#include <libyuc/container/experimental/avl_tree_np.h>

/*
* 获取左子节点
*/
inline AvlEntryNp* AvlEntryNpGetLeft(AvlEntryNp* entry) {
  return (AvlEntryNp*)(((uintptr_t)entry->left_balanceFactor) & (~((uintptr_t)0x3)));
}

/*
* 设置左子节点
*/
inline void AvlEntryNpSetLeft(AvlEntryNp* entry, AvlEntryNp* left) {
  entry->left_balanceFactor = (AvlEntryNp*)(((uintptr_t)left) | ((uintptr_t)entry->left_balanceFactor & 0x3));
}

/*
* 获取右子节点
*/
inline AvlEntryNp* AvlEntryNpGetRight(AvlEntryNp* entry) {
  return entry->right;
}

/*
* 设置右子节点
*/
inline void AvlEntryNpSetRight(AvlEntryNp* entry, AvlEntryNp* right) {
  entry->right = right;
}

/*
* 获取节点平衡因子
*/
inline int AvlEntryNpGetBalanceFactor(AvlEntryNp* entry) {
  int ret = (int)(((uintptr_t)entry->left_balanceFactor) & 0x3);
  if (ret == 3) {
    ret = -1;
  }
  return ret;
}

/*
* 设置节点平衡因子
*/
inline void AvlEntryNpSetBalanceFactor(AvlEntryNp* entry, int balanceFactor) {
  entry->left_balanceFactor = (AvlEntryNp*)(((uintptr_t)AvlEntryNpGetLeft(entry)) | ((uintptr_t)balanceFactor) & 0x3);
}



/*
* 左旋子树
*/
static AvlEntryNp* RotateLeft(AvlEntryNp* subRoot, AvlEntryNp* subRootParent) {
  AvlEntryNp* newSubRoot = AvlEntryNpGetRight(subRoot);
  if (newSubRoot == NULL) {
    return subRoot;
  }
  if (subRootParent) {
    if (AvlEntryNpGetLeft(subRootParent) == subRoot) {
      AvlEntryNpSetLeft(subRootParent, newSubRoot);
    }
    else {
      AvlEntryNpSetRight(subRootParent, newSubRoot);
    }
  }
  AvlEntryNpSetRight(subRoot, AvlEntryNpGetLeft(newSubRoot));
  AvlEntryNpSetLeft(newSubRoot, subRoot);
  return newSubRoot;
}

/*
* 右旋子树
*/
static AvlEntryNp* RotateRight(AvlEntryNp* subRoot, AvlEntryNp* subRootParent) {
  AvlEntryNp* newSubRoot = AvlEntryNpGetLeft(subRoot);
  if (newSubRoot == NULL) {
    return subRoot;
  }
  if (subRootParent) {
    if (AvlEntryNpGetLeft(subRootParent) == subRoot) {
      AvlEntryNpSetLeft(subRootParent, newSubRoot);
    }
    else {
      AvlEntryNpSetRight(subRootParent, newSubRoot);
    }
  }
  AvlEntryNpSetLeft(subRoot, AvlEntryNpGetRight(newSubRoot));
  AvlEntryNpSetRight(newSubRoot, subRoot);
  return newSubRoot;

}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void AvlTreeNpHitchEntry(AvlTreeNp* tree, AvlEntryNp* entry, AvlEntryNp* entryParent, AvlEntryNp* newEntry) {
  if (entryParent) {
    if (AvlEntryNpGetLeft(entryParent) == entry) {
      AvlEntryNpSetLeft(entryParent, newEntry);
    }
    else {
      entryParent->right = newEntry;
    }
  }
  if (tree->root == entry) {
    tree->root = newEntry;
  }
}

/*
* 根据平衡因子来旋转子树
* 并负责更新平衡因子
* 子树高度变化返回true，高度未变化返回false
*/
static bool RotateByBalanceFactor(AvlTreeNp* tree, AvlEntryNp* subRoot, int curBF, AvlEntryNp* parent) {
  bool rotate = false;
  bool heightUpdate = true;
  AvlEntryNp* newSubRoot = NULL;
  if (curBF > 1) {
    AvlEntryNp* child = AvlEntryNpGetLeft(subRoot);
    int childBF = AvlEntryNpGetBalanceFactor(child);
    if (childBF == -1) {
      int rightBF = AvlEntryNpGetBalanceFactor(child->right);
      if (child->right) {
        AvlEntryNpSetBalanceFactor(child->right, 0);
      }
      RotateLeft(child, subRoot);
      newSubRoot = RotateRight(subRoot, parent);

      if (rightBF == -1) {
        AvlEntryNpSetBalanceFactor(child, 1);
        AvlEntryNpSetBalanceFactor(subRoot, 0);
      }
      else {
        AvlEntryNpSetBalanceFactor(child, 0);
        if (rightBF == 1) {
          AvlEntryNpSetBalanceFactor(subRoot, -1);
        }
        else {
          AvlEntryNpSetBalanceFactor(subRoot, 0);
        }
      }
    }
    else if (childBF == 0) {
      heightUpdate = false;
      newSubRoot = RotateRight(subRoot, parent);
      AvlEntryNpSetBalanceFactor(child, -1);
      AvlEntryNpSetBalanceFactor(subRoot, 1);
    }
    else {
      newSubRoot = RotateRight(subRoot, parent);
      AvlEntryNpSetBalanceFactor(child, 0);
      AvlEntryNpSetBalanceFactor(subRoot, 0);
    }
    rotate = true;
  }
  else if (curBF < -1) {
    AvlEntryNp* child = subRoot->right;
    int childBF = AvlEntryNpGetBalanceFactor(child);
    if (childBF == 1) {
      int leftBF = AvlEntryNpGetBalanceFactor(AvlEntryNpGetLeft(child));
      if (AvlEntryNpGetLeft(child)) {
        AvlEntryNpSetBalanceFactor(AvlEntryNpGetLeft(child), 0);
      }
      RotateRight(child, subRoot);
      newSubRoot = RotateLeft(subRoot, parent);
      if (leftBF == 1) {
        AvlEntryNpSetBalanceFactor(child, -1);
        AvlEntryNpSetBalanceFactor(subRoot, 0);
      }
      else {
        AvlEntryNpSetBalanceFactor(child, 0);
        if (leftBF == -1) {
          AvlEntryNpSetBalanceFactor(subRoot, 1);
        }
        else {
          AvlEntryNpSetBalanceFactor(subRoot, 0);
        }
      }
    }
    else if (childBF == 0) {
      heightUpdate = false;
      newSubRoot = RotateLeft(subRoot, parent);
      AvlEntryNpSetBalanceFactor(child, 1);
      AvlEntryNpSetBalanceFactor(subRoot, -1);
    }
    else {
      newSubRoot = RotateLeft(subRoot, parent);
      AvlEntryNpSetBalanceFactor(child, 0);
      AvlEntryNpSetBalanceFactor(subRoot, 0);
    }
    rotate = true;
  }
  else {
    heightUpdate = false;
    AvlEntryNpSetBalanceFactor(subRoot, curBF);
  }
  if (rotate) {
    if (tree->root == subRoot) {
      tree->root = newSubRoot;
    }
  }
  return heightUpdate;
}



/*
* 向树中插入节点后的平衡操作
* 继续平衡返回true，无需平衡返回false
*/
bool AvlTreeNpInsertEntryFixup(AvlTreeNp* tree, bool isCurLeft, AvlEntryNp* cur, AvlEntryNp* curParent) {
  // 插入节点后平衡因子可能发生变化，回溯维护平衡因子
  int curBF = AvlEntryNpGetBalanceFactor(cur);
  if (isCurLeft) curBF++;    // 新节点插入到当前节点的左子树
  else curBF--;     // 新节点插入到当前节点的右子树

  if (RotateByBalanceFactor(tree, cur, curBF, curParent) || curBF == 0) {
    // 旋转后当前节点高度不变，或原先高度就不变，停止回溯
    return false;
  }
  return true;
}

/*
* 递归查找并插入节点
*/
bool AvlTreeNpInsertEntry(AvlTreeNp* tree, AvlEntryNp* entry, AvlEntryNp* cur, AvlEntryNp* curParent) {
  void* obj = ObjectGetFromFieldOffset(entry, tree->entryFieldOffset, void);
  void* key = ObjectGetFieldByOffset(obj, tree->keyFieldOffset, void);

  void* curObj = ObjectGetFromFieldOffset(cur, tree->entryFieldOffset, void);
  int res = tree->cmpFunc(ObjectGetFieldByOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
  bool ret = true;
  bool isCurLeft;
  if (res == 0) {
    return false;
  }
  else if (res < 0) {
    if (AvlEntryNpGetRight(cur)) {
      ret = AvlTreeNpInsertEntry(tree, entry, AvlEntryNpGetRight(cur), cur);
    }
    else {
      AvlEntryNpSetRight(cur, entry);
    }
    isCurLeft = false;
  }
  else {
    if (AvlEntryNpGetLeft(cur)) {
      ret = AvlTreeNpInsertEntry(tree, entry, AvlEntryNpGetLeft(cur), cur);
    }
    else {
      AvlEntryNpSetLeft(cur, entry);
    }
    isCurLeft = true;
  }
  if (ret) {
    ret = AvlTreeNpInsertEntryFixup(tree, isCurLeft, cur, curParent);
  }
  return ret;
}

/*
* 从树中删除节点的平衡操作
*/
bool AvlTreeNpDeleteEntryFixup(AvlTreeNp* tree, bool isCurLeft, AvlEntryNp* cur, AvlEntryNp* curParent) {
  // 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子
  AvlEntryNp* newSubRoot = NULL;
  int curBF = AvlEntryNpGetBalanceFactor(cur);
  if (isCurLeft) curBF--;
  else curBF++;

  if (curBF != 0) {
    if (RotateByBalanceFactor(tree, cur, curBF, curParent) == false) {
      // 另一侧高度相等或更深且无需旋转，则当前节点高度不变
      return false;
    }
  }
  else {
    AvlEntryNpSetBalanceFactor(cur, curBF);
  }
  return true;
}

/*
* 递归查找最小节点并删除待删除节点，将最小节点挂接到待删除节点的位置
* delete_minEntry传入待删除节点，返回代替其的最小节点
*/
static bool DeleteMinEntry(AvlTreeNp* tree, AvlEntryNp* cur, AvlEntryNp** curParent, AvlEntryNp** delete_minEntry, AvlEntryNp* deleteParent) {
  if (AvlEntryNpGetLeft(cur)) {
    if (DeleteMinEntry(tree, AvlEntryNpGetLeft(cur), &cur, delete_minEntry, deleteParent)) {
      return AvlTreeNpDeleteEntryFixup(tree, true, cur, *curParent);
    }
    return false;
  }

  // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
  AvlEntryNpSetLeft(cur, AvlEntryNpGetLeft(*delete_minEntry));

  // 最小节点挂接到需要删除的位置
  AvlTreeNpHitchEntry(tree, *delete_minEntry, deleteParent, cur);

  AvlEntryNpSetBalanceFactor(cur, AvlEntryNpGetBalanceFactor(*delete_minEntry));

  // 最小节点可能是待删除节点的右节点
  if (AvlEntryNpGetRight(*delete_minEntry) != cur) {
    // 将minEntry从原先的位置摘除，用其右子树代替
    AvlEntryNpSetLeft(*curParent, cur->right);

    // 最小节点继承待删除节点的右子树
    cur->right = (*delete_minEntry)->right;
  }
  else {
    *curParent = cur;     // curParent可能指向AvlTreeNpDeleteEntry的cur，也可能是上层DeleteMinEntry的cur
  }
  *delete_minEntry = cur;   // 返回最小节点
  return true;

}

/*
* 递归查找并删除节点
*/
bool AvlTreeNpDeleteEntry(AvlTreeNp* tree, void* key, AvlEntryNp* cur, AvlEntryNp** curParent, AvlEntryNp** deleteEntry) {
  if (cur == NULL) {
    return false;
  }
  void* obj = ObjectGetFromFieldOffset(cur, tree->entryFieldOffset, void);
  int res = tree->cmpFunc(ObjectGetFieldByOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
  bool ret;
  bool isCurLeft;
  if (res > 0) {
    ret = AvlTreeNpDeleteEntry(tree, key, AvlEntryNpGetLeft(cur), &cur, deleteEntry);
    isCurLeft = true;
  }
  else if (res < 0) {
    ret = AvlTreeNpDeleteEntry(tree, key, cur->right, &cur, deleteEntry);
    isCurLeft = false;
  }
  else {
    *deleteEntry = cur;
    if (AvlEntryNpGetLeft(cur) != NULL && AvlEntryNpGetRight(cur) != NULL) {
      // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
      // 递归删除最小节点
      // 之所以都传递cur的地址，是为了保证替换最小节点后，回溯时第一层调用栈上的curParent也会同步为最小节点
      ret = DeleteMinEntry(tree, AvlEntryNpGetRight(cur), &cur, &cur, *curParent);
      isCurLeft = false;
    }
    else {
      if (cur->right != NULL) {
        // 只有右子节点
        AvlTreeNpHitchEntry(tree, cur, *curParent, cur->right);
        isCurLeft = false;
      }
      else if (AvlEntryNpGetLeft(cur) != NULL) {
        AvlTreeNpHitchEntry(tree, cur, *curParent, AvlEntryNpGetLeft(cur));
        isCurLeft = true;
      }
      else {
        // 没有子节点，直接从父节点中摘除此节点
        AvlTreeNpHitchEntry(tree, cur, *curParent, NULL);
        return true;
      }
      ret = true;
    }
  }
  if (ret) {
    ret = AvlTreeNpDeleteEntryFixup(tree, isCurLeft, cur, *curParent);
  }
  return ret;
}



/*
* 初始化Avl树
*/
void AvlTreeNpInit(AvlTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
  BsTreeNpInit(&tree->bst, entryFieldOffset, keyFieldOffset, keySize, cmpFunc);
}

/*
* 初始化节点
*/
void AvlEntryNpInit(AvlEntryNp* entry) {
  BsEntryNpInit(&entry->bse);
  AvlEntryNpSetBalanceFactor(entry, 0);
}

/*
* 从树中查找节点
*/
void* AvlTreeNpFindEntryByKey(AvlTreeNp* tree, void* key) {
  AvlEntryNp* cur = tree->root;
  while (cur) {
    void* obj = ObjectGetFromFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(ObjectGetFieldByOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    if (res < 0) {
      cur = cur->right;
    }
    else if (res > 0) {
      cur = AvlEntryNpGetLeft(cur);
    }
    else {
      return ObjectGetFromFieldOffset(cur, tree->entryFieldOffset, void);
    }
  }
  return NULL;
}

/*
* 从树中按key插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool AvlTreeNpInsertEntryByKey(AvlTreeNp* tree, AvlEntryNp* entry) {
  AvlEntryNpInit(entry);
  if (tree->root == NULL) {
    tree->root = entry;
    return true;
  }
  return AvlTreeNpInsertEntry(tree, entry, tree->root, NULL);
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
void* AvlTreeNpDeleteEntryByKey(AvlTreeNp* tree, void* key) {
  AvlEntryNp* deleteEntry, * curParent = NULL;
  AvlTreeNpDeleteEntry(tree, key, tree->root, &curParent, &deleteEntry);
  return ObjectGetFromFieldOffset(deleteEntry, tree->entryFieldOffset, void);
}