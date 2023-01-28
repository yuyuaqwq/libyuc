/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "BSTree.h"

#ifndef CUTILS_CONTAINER_BS_TREE_NO_PARENT
/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void BSTreeHitchEntry(BSTree* tree, BSEntry* entry, BSEntry* newEntry) {
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
#else
/*
* newEntry代替entry挂接到parent下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void BSTreeHitchEntry(BSTree* tree, BSEntry* entry, BSEntry* entryParent, BSEntry* newEntry) {
    if (entryParent) {
        if (entryParent->left == entry) {
            entryParent->left = newEntry;
        } else {
            entryParent->right = newEntry;
        }
    }
    if (tree->root == entry) {
        tree->root = newEntry;
    }
}
#endif // CUTILS_CONTAINER_BS_TREE_NO_PARENT


/*
* 初始化树
*/
void BSTreeInit(BSTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
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
void BSEntryInit(BSEntry* entry) {
    entry->left = NULL;
    entry->right = NULL;
#ifndef CUTILS_CONTAINER_BS_TREE_NO_PARENT
    entry->parent = NULL;
#endif // CUTILS_CONTAINER_BSTREE_NO_PARENT
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
BSEntry* BSTreeFindEntryByKey(BSTree* tree, void* key) {
    BSEntry* cur = tree->root;
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

#ifndef CUTILS_CONTAINER_BS_TREE_NO_PARENT
/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool BSTreeInsertEntry(BSTree* tree, BSEntry* entry) {
    BSEntryInit(entry);
    if (tree->root == NULL) {
        tree->root = entry;
        return true;
    }
    void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);
    BSEntry* cur = tree->root;
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
BSEntry* BSTreeDeleteEntry(BSTree* tree, BSEntry* entry) {
    if (entry->left != NULL && entry->right != NULL) {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        BSEntry* minEntry = entry->right;
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
        BSTreeHitchEntry(tree, entry, minEntry);

        // 也可以选择直接交换两个节点的数据
        
    }
    else {
        if (entry->right != NULL) {
            // 只有右子节点
            BSTreeHitchEntry(tree, entry, entry->right);
        }
        else if (entry->left != NULL) {
            BSTreeHitchEntry(tree, entry, entry->left);
        }
        else {
            // 没有子节点，直接从父节点中摘除此节点
            BSTreeHitchEntry(tree, entry, NULL);
        }
    }
    return entry;
}
#else
/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool BSTreeInsertEntry(BSTree* tree, BSEntry* cur, BSEntry* entry) {
    if (tree->root == NULL) {
        BSEntryInit(entry);
        tree->root = entry;
        return true;
    }
    void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);

    if (cur == NULL) cur == tree->root;
    void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    if (res < 0) {
        if (!cur->right) {
            BSEntryInit(entry);
            cur->right = entry;
            return true;
        } else {
            return BSTreeInsertEntry(tree, cur->right, entry);
        }
    }
    else if (res > 0) {
        if (!cur->left) {
            BSEntryInit(entry);
            cur->left = entry;
            return true;
        } else {
            return BSTreeInsertEntry(tree, cur->left, entry);
        }
    }
    return false;

}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
BSEntry* BSTreeDeleteEntry(BSTree* tree, BSEntry* entry, BSEntry* entryParent) {
    if (entry->left != NULL && entry->right != NULL) {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        BSEntry* minEntry = entry->right;
        BSEntry* minParent = NULL;
        while (minEntry->left) {
            minParent = minEntry;
            minEntry = minEntry->left;
        }

        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        minEntry->left = entry->left;

        // 最小节点可能是待删除节点的右节点
        if (entry->right != minEntry) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            minParent->left = minEntry->right;

            // 最小节点继承待删除节点的右子树
            minEntry->right = entry->right;

            // 如果需要回溯，这里对应entry的父亲是minEntry的父亲的情况，但不能直接修改entry的parent，因为还没挂接
        }
        else {
            // 如果需要回溯，这里对应entry的父亲是minEntry的情况，但不能直接修改entry的parent，因为还没挂接
        }

        // 最后进行挂接
        BSTreeHitchEntry(tree, entry, entryParent, minEntry);

        // 也可以选择直接交换两个节点的数据

    }
    else {
        if (entry->right != NULL) {
            // 只有右子节点
            BSTreeHitchEntry(tree, entry, entryParent, entry->right);
        }
        else if (entry->left != NULL) {
            BSTreeHitchEntry(tree, entry, entryParent, entry->left);
        }
        else {
            // 没有子节点，直接从父节点中摘除此节点
            BSTreeHitchEntry(tree, entry, entryParent, NULL);
        }
    }
    return entry;
}
#endif // CUTILS_CONTAINER_BS_TREE_NO_PARENT

/*
* 获取树的节点数量
*/
static void BSTreeGetEntryCountCallback(BSEntry* entry, void* arg) {
    int* count = arg;
    (*count)++;
}
size_t BSTreeGetEntryCount(BSTree* tree) {
    int count = 0;
    BSTreePreorder_Callback(tree->root, BSTreeGetEntryCountCallback, &count);
    return count;
}


//BSEntry* BSTreeFirst(BSTree* tree) {
//    BSEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    while (cur->left)
//        cur = cur->left;
//    return cur;
//}
//
//BSEntry* BSTreeLast(BSTree* tree) {
//    BSEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    while (cur->right)
//        cur = cur->right;
//    return cur;
//}
//
//BSEntry* BSTreeNext(BSEntry* entry) {
//    if (entry->right) {
//        entry = entry->right;
//        while (entry->left)
//            entry = entry->left;
//        return entry;
//    }
//    BSEntry* parent;
//    while ((parent = entry->parent) && entry == parent->right)
//        entry = parent;
//    return parent;
//}
//
//BSEntry* BSTreePrev(BSEntry* entry) {
//    if (entry->left) {
//        entry = entry->left;
//        while (entry->right) {
//            entry = entry->right;
//        }
//        return entry;
//    }
//    BSEntry* parent;
//    while ((parent = entry->parent) && entry == parent->left) {
//        entry = parent;
//    }
//    return parent;
//}



/*
* 前序遍历
* 先根再右再左
*/
void BSTreePreorder_Callback(BSEntry* entry, BSTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    callback(entry, arg);
    BSTreePreorder_Callback(entry->left, callback, arg);
    BSTreePreorder_Callback(entry->right, callback, arg);
}

/*
* 中序遍历
* 先左再根再右
*/
void BSTreeMiddleorder_Callback(BSEntry* entry, BSTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    BSTreeMiddleorder_Callback(entry->left, callback, arg);
    callback(entry, arg);
    BSTreeMiddleorder_Callback(entry->right, callback, arg);
}

/*
* 后序遍历
* 先左再右再根
*/
void BSTreePostorder_Callback(BSEntry* entry, BSTreeTraversalCallback callback, void* arg) {
    if (!entry) return;
    BSTreePostorder_Callback(entry->left, callback, arg);
    BSTreePostorder_Callback(entry->right, callback, arg);
    callback(entry, arg);
}