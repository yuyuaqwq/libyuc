/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils\container\bs_tree.h>

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void BsTreeHitchEntry(BsTree* tree, BsEntry* entry, BsEntry* newEntry) {
    if (entry->parent) {
        if (entry->parent->left == entry) {
            entry->parent->left = newEntry;
        }
        else {
            entry->parent->right = newEntry;
        }
    }
    if (newEntry) {
        newEntry->parent = entry->parent;
    }
    if (tree->root == entry) {
        tree->root = newEntry;
    }
}


/*
* 初始化树
*/
void BsTreeInit(BsTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
    tree->root = NULL;
    tree->entryFieldOffset = entryFieldOffset;
    tree->keyFieldOffset = keyFieldOffset;
    tree->keyFieldSize = keySize;
    if (cmpFunc == NULL) {
        cmpFunc = MemoryCmpR;
    }
    tree->cmpFunc = cmpFunc;
}

/*
* 初始化节点
*/
void BsEntryInit(BsEntry* entry) {
    entry->left = NULL;
    entry->right = NULL;
    entry->parent = NULL;
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
BsEntry* BsTreeFindEntryByKey(BsTree* tree, void* key) {
    BsEntry* cur = tree->root;
    while (cur) {
        void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
        int res = tree->cmpFunc(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
        if (res < 0) {
            cur = cur->right;
        }
        else if (res > 0) {
            cur = cur->left;
        }
        else {
            return cur;
        }
    }
    return NULL;
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool BsTreeInsertEntryByKey(BsTree* tree, BsEntry* entry) {
    BsEntryInit(entry);
    if (tree->root == NULL) {
        tree->root = entry;
        return true;
    }
    void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);
    BsEntry* cur = tree->root;
    while (cur) {
        void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
        int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
        if (res < 0) {
            if (!cur->right) {
                cur->right = entry;
                break;
            }
            cur = cur->right;
        }
        else if (res > 0) {
            if (!cur->left) {
                cur->left = entry;
                break;
            }
            cur = cur->left;
        }
        else {
            return false;
        }
    }
    entry->parent = cur;
    return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
BsEntry* BsTreeDeleteEntry(BsTree* tree, BsEntry* entry) {
    if (entry->left != NULL && entry->right != NULL) {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        BsEntry* minEntry = entry->right;
        while (minEntry->left) {
            minEntry = minEntry->left;
        }

        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        minEntry->left = entry->left;
        if (entry->left) {
            entry->left->parent = minEntry;
        }

        // 最小节点可能是待删除节点的右节点
        if (entry->right != minEntry) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            minEntry->parent->left = minEntry->right;
            if (minEntry->right) {
                minEntry->right->parent = minEntry->parent;
            }
            // 最小节点继承待删除节点的右子树
            minEntry->right = entry->right;
            if (entry->right) {
                entry->right->parent = minEntry;
            }
            // 如果需要回溯，这里对应entry的父亲是minEntry的父亲的情况，但不能直接修改entry的parent，因为还没挂接
        }
        else {
            // 如果需要回溯，这里对应entry的父亲是minEntry的情况，但不能直接修改entry的parent，因为还没挂接
        }

        // 最后进行挂接
        BsTreeHitchEntry(tree, entry, minEntry);

        // 也可以选择直接交换两个节点的数据
        
    }
    else {
        if (entry->right != NULL) {
            // 只有右子节点
            BsTreeHitchEntry(tree, entry, entry->right);
        }
        else if (entry->left != NULL) {
            BsTreeHitchEntry(tree, entry, entry->left);
        }
        else {
            // 没有子节点，直接从父节点中摘除此节点
            BsTreeHitchEntry(tree, entry, NULL);
        }
    }
    return entry;
}


/*
* 获取树的节点数量
*/
size_t BsTreeGetEntryCount(BsTree* tree) {
    int count = 0;
    BsEntry* entry = BsTreeFirst(tree);
    while (entry) {
        count++;
        entry = BsTreeNext(entry);
    }
    return count;
}



BsEntry* BsTreeFirst(BsTree* tree) {
    BsEntry* cur = tree->root;
    if (!cur) {
        return NULL;
    }
    while (cur->left)
        cur = cur->left;
    return cur;
}

BsEntry* BsTreeLast(BsTree* tree) {
    BsEntry* cur = tree->root;
    if (!cur) {
        return NULL;
    }
    while (cur->right)
        cur = cur->right;
    return cur;
}

BsEntry* BsTreeNext(BsEntry* entry) {
    if (entry->right) {
        entry = entry->right;
        while (entry->left)
            entry = entry->left;
        return entry;
    }
    BsEntry* parent;
    while ((parent = entry->parent) && entry == parent->right)
        entry = parent;
    return parent;
}

BsEntry* BsTreePrev(BsEntry* entry) {
    if (entry->left) {
        entry = entry->left;
        while (entry->right) {
            entry = entry->right;
        }
        return entry;
    }
    BsEntry* parent;
    while ((parent = entry->parent) && entry == parent->left) {
        entry = parent;
    }
    return parent;
}



typedef bool (*BsTreeTraversalCallback)(BsEntry* entry, void* arg);
/*
* 前序遍历
* 先根再右再左
*/
static void BsTreePreorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    callback(entry, arg);
    BsTreePreorder_Callback(entry->left, callback, arg);
    BsTreePreorder_Callback(entry->right, callback, arg);
}

/*
* 中序遍历
* 先左再根再右
*/
static void BsTreeMiddleorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    BsTreeMiddleorder_Callback(entry->left, callback, arg);
    callback(entry, arg);
    BsTreeMiddleorder_Callback(entry->right, callback, arg);
}

/*
* 后序遍历
* 先左再右再根
*/
static void BsTreePostorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    BsTreePostorder_Callback(entry->left, callback, arg);
    BsTreePostorder_Callback(entry->right, callback, arg);
    callback(entry, arg);
}