#include <CUtils/container/avl_tree_np.h>

/*
* 获取左子节点
*/
inline AVLEntry* AVLEntryGetLeft(AVLEntry* entry) {
    return (AVLEntry*)(((uintptr_t)entry->left_balanceFactor) & (~((uintptr_t)0x3)));
}

/*
* 设置左子节点
*/
inline void AVLEntrySetLeft(AVLEntry* entry, AVLEntry* left) {
    entry->left_balanceFactor = (AVLEntry*)(((uintptr_t)left) | ((uintptr_t)entry->left_balanceFactor & 0x3));
}

/*
* 获取右子节点
*/
inline AVLEntry* AVLEntryGetRight(AVLEntry* entry) {
    return entry->right;
}

/*
* 设置右子节点
*/
inline void AVLEntrySetRight(AVLEntry* entry, AVLEntry* right) {
    entry->right = right;
}

/*
* 获取节点平衡因子
*/
inline int AVLEntryGetBalanceFactor(AVLEntry* entry) {
    int ret = (int)(((uintptr_t)entry->left_balanceFactor) & 0x3);
    if (ret == 3) {
        ret = -1;
    }
    return ret;
}

/*
* 设置节点平衡因子
*/
inline void AVLEntrySetBalanceFactor(AVLEntry* entry, int balanceFactor) {
    entry->left_balanceFactor = (AVLEntry*)(((uintptr_t)AVLEntryGetLeft(entry)) | ((uintptr_t)balanceFactor) & 0x3);
}


/*
* 左旋子树
*/
static AVLEntry* RotateLeft(AVLEntry* subRoot, AVLEntry* subRootParent) {
    AVLEntry* newSubRoot = AVLEntryGetRight(subRoot);
    if (newSubRoot == NULL) {
        return subRoot;
    }
    if (subRootParent) {
        if (AVLEntryGetLeft(subRootParent) == subRoot) {
            AVLEntrySetLeft(subRootParent, newSubRoot);
        }
        else {
            AVLEntrySetRight(subRootParent, newSubRoot);
        }
    }
    AVLEntrySetRight(subRoot, AVLEntryGetLeft(newSubRoot));
    AVLEntrySetLeft(newSubRoot, subRoot);
    return newSubRoot;
}

/*
* 右旋子树
*/
static AVLEntry* RotateRight(AVLEntry* subRoot, AVLEntry* subRootParent) {
    AVLEntry* newSubRoot = AVLEntryGetLeft(subRoot);
    if (newSubRoot == NULL) {
        return subRoot;
    }
    if (subRootParent) {
        if (AVLEntryGetLeft(subRootParent) == subRoot) {
            AVLEntrySetLeft(subRootParent, newSubRoot);
        }
        else {
            AVLEntrySetRight(subRootParent, newSubRoot);
        }
    }
    AVLEntrySetLeft(subRoot, AVLEntryGetRight(newSubRoot));
    AVLEntrySetRight(newSubRoot, subRoot);
    return newSubRoot;

}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void AVLTreeHitchEntry(AVLTree* tree, AVLEntry* entry, AVLEntry* entryParent, AVLEntry* newEntry) {
    if (entryParent) {
        if (AVLEntryGetLeft(entryParent) == entry) {
            AVLEntrySetLeft(entryParent, newEntry);
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
static bool RotateByBalanceFactor(AVLTree* tree, AVLEntry** unbalancedEntry, AVLEntry* subRoot, int curBF, AVLEntry* parent) {
    bool rotate = false;
    bool heightUpdate = true;
    AVLEntry* newSubRoot = NULL;
    if (curBF > 1) {
        AVLEntry* child = AVLEntryGetLeft(subRoot);
        int childBF = AVLEntryGetBalanceFactor(child);
        if (childBF == -1) {
            int rightBF = AVLEntryGetBalanceFactor(child->right);
            if (child->right) {
                AVLEntrySetBalanceFactor(child->right, 0);
            }
            RotateLeft(child, subRoot);
            newSubRoot = RotateRight(subRoot, parent);

            if (rightBF == -1) {
                AVLEntrySetBalanceFactor(child, 1);
                AVLEntrySetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntrySetBalanceFactor(child, 0);
                if (rightBF == 1) {
                    AVLEntrySetBalanceFactor(subRoot, -1);
                }
                else {
                    AVLEntrySetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            heightUpdate = false;
            newSubRoot = RotateRight(subRoot, parent);
            AVLEntrySetBalanceFactor(child, -1);
            AVLEntrySetBalanceFactor(subRoot, 1);
        }
        else {
            newSubRoot = RotateRight(subRoot, parent);
            AVLEntrySetBalanceFactor(child, 0);
            AVLEntrySetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else if (curBF < -1) {
        AVLEntry* child = subRoot->right;
        int childBF = AVLEntryGetBalanceFactor(child);
        if (childBF == 1) {
            int leftBF = AVLEntryGetBalanceFactor(AVLEntryGetLeft(child));
            if (AVLEntryGetLeft(child)) {
                AVLEntrySetBalanceFactor(AVLEntryGetLeft(child), 0);
            }
            RotateRight(child, subRoot);
            newSubRoot = RotateLeft(subRoot, parent);
            if (leftBF == 1) {
                AVLEntrySetBalanceFactor(child, -1);
                AVLEntrySetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntrySetBalanceFactor(child, 0);
                if (leftBF == -1) {
                    AVLEntrySetBalanceFactor(subRoot, 1);
                }
                else {
                    AVLEntrySetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            heightUpdate = false;
            newSubRoot = RotateLeft(subRoot, parent);
            AVLEntrySetBalanceFactor(child, 1);
            AVLEntrySetBalanceFactor(subRoot, -1);
        }
        else {
            newSubRoot = RotateLeft(subRoot, parent);
            AVLEntrySetBalanceFactor(child, 0);
            AVLEntrySetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else {
        heightUpdate = false;
        if (unbalancedEntry) *unbalancedEntry = subRoot;
        AVLEntrySetBalanceFactor(subRoot, curBF);
    }
    if (rotate) {
        if (tree->root == subRoot) {
            tree->root = newSubRoot;
        }
    }
    return heightUpdate;
}


/*
* 初始化AVL树
*/
void AVLTreeInit(AVLTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
    BSTreeInit(&tree->bst, entryFieldOffset, keyFieldOffset, keySize, cmpFunc);
}

/*
* 初始化节点
*/
void AVLEntryInit(AVLEntry* entry) {
    BSEntryInit(&entry->bse);
    AVLEntrySetBalanceFactor(entry, 0);
}




/*
* 向树中插入节点后的平衡操作
* 继续平衡返回true，无需平衡返回false
*/
bool AVLTreeInsertEntryFixup(AVLTree* tree, AVLEntry** unbalancedEntry, AVLEntry* cur, AVLEntry* curParent) {
    // 插入节点后平衡因子可能发生变化，回溯维护平衡因子
    int curBF = AVLEntryGetBalanceFactor(cur);
    if (AVLEntryGetLeft(cur) == *unbalancedEntry) curBF++;        // 新节点插入到当前节点的左子树
    else curBF--;       // 新节点插入到当前节点的右子树

    if (RotateByBalanceFactor(tree, unbalancedEntry, cur, curBF, curParent) || curBF == 0) {
        // 旋转后当前节点高度不变，或原先高度就不变，停止回溯
        return false;
    }
    return true;
}

/*
* 递归查找并插入节点
*/
bool AVLTreeInsertEntry(AVLTree* tree, AVLEntry** unbalancedEntry, AVLEntry* cur, AVLEntry* curParent) {
    void* obj = GetObjByFieldOffset(*unbalancedEntry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);

    void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    bool ret = true;
    if (res == 0) {
        return false;
    }
    else if (res < 0) {
        if (AVLEntryGetRight(cur)) {
            ret = AVLTreeInsertEntry(tree, unbalancedEntry, AVLEntryGetRight(cur), cur);
        }
        else {
            AVLEntrySetRight(cur, *unbalancedEntry);
        }
    }
    else {
        if (AVLEntryGetLeft(cur)) {
            ret = AVLTreeInsertEntry(tree, unbalancedEntry, AVLEntryGetLeft(cur), cur);
        }
        else {
            AVLEntrySetLeft(cur, *unbalancedEntry);
        }
    }
    if (ret) {
        ret = AVLTreeInsertEntryFixup(tree, unbalancedEntry, cur, curParent);
    }
    return ret;
}

/*
* 从树中按key插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool AVLTreeInsertEntryByKey(AVLTree* tree, AVLEntry* entry) {
    AVLEntryInit(entry);
    if (tree->root == NULL) {
        tree->root = entry;
        return true;
    }
    return AVLTreeInsertEntry(tree, &entry, tree->root, NULL);
}



/*
* 从树中删除节点的平衡操作
*/
bool AVLTreeDeleteEntryFixup(AVLTree* tree, bool isCurLeft, AVLEntry* cur, AVLEntry* curParent) {
    // 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子
    AVLEntry* newSubRoot = NULL;
    int curBF = AVLEntryGetBalanceFactor(cur);
    if (isCurLeft) curBF--;
    else curBF++;

    if (curBF != 0) {
        if (RotateByBalanceFactor(tree, NULL, cur, curBF, curParent) == false) {
            // 另一侧高度相等或更深且无需旋转，则当前节点高度不变
            return false;
        }
    }
    else {
        AVLEntrySetBalanceFactor(cur, curBF);
    }
    return true;
}


static bool DeleteMinEntry(AVLTree* tree, AVLEntry* cur, AVLEntry** curParent, AVLEntry** delete_minEntry, AVLEntry* deleteParent) {
    if (AVLEntryGetLeft(cur)) {
        if (DeleteMinEntry(tree, AVLEntryGetLeft(cur), &cur, delete_minEntry, deleteParent)) {
            return AVLTreeDeleteEntryFixup(tree, true, cur, *curParent);
        }
    } else {
        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        AVLEntrySetLeft(cur, AVLEntryGetLeft(*delete_minEntry));

        // 最小节点挂接到需要删除的位置
        AVLTreeHitchEntry(tree, *delete_minEntry, deleteParent, cur);

        AVLEntrySetBalanceFactor(cur, AVLEntryGetBalanceFactor(*delete_minEntry));

        // 最小节点可能是待删除节点的右节点
        if (AVLEntryGetRight(*delete_minEntry) != cur) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            AVLEntrySetLeft(*curParent, cur->right);

            // 最小节点继承待删除节点的右子树
            cur->right = (*delete_minEntry)->right;
        } else {
            *curParent = cur;       // curParent可能指向将要返回的minEntry，也可能是上层DeleteMinEntry的cur
        }
        *delete_minEntry = cur;     // 返回最小节点
        return true;
    }
}

/*
* 递归查找并删除节点
*/
bool AVLTreeDeleteEntry(AVLTree* tree, void* key, AVLEntry* cur, AVLEntry** curParent, AVLEntry** deleteEntry) {
    void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    bool ret;
    bool isCurLeft;
    if (res > 0) {
        ret = AVLTreeDeleteEntry(tree, key, AVLEntryGetLeft(cur), &cur, deleteEntry);
        isCurLeft = true;
    }
    else if (res < 0) {
        ret = AVLTreeDeleteEntry(tree, key, cur->right, &cur, deleteEntry);
        isCurLeft = false;
    }
    else {
        *deleteEntry = cur;
        if (AVLEntryGetLeft(cur) != NULL && AVLEntryGetRight(cur) != NULL) {
            // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
            // 递归找最左
            AVLEntry* minEntry = cur;       // 传入删除节点，返回最小节点
            // 之所以都传递minEntry的地址，是为了保证替换最小节点后，第一层调用栈上的curParent也会同步为最小节点
            ret = DeleteMinEntry(tree, AVLEntryGetRight(cur), &minEntry, &minEntry, *curParent);
            cur = minEntry;     // 当前节点变为最小节点
            isCurLeft = false;
        }
        else {
            if (cur->right != NULL) {
                // 只有右子节点
                AVLTreeHitchEntry(tree, cur, *curParent, cur->right);
                isCurLeft = false;
            }
            else if (AVLEntryGetLeft(cur) != NULL) {
                AVLTreeHitchEntry(tree, cur, *curParent, AVLEntryGetLeft(cur));
                isCurLeft = true;
            }
            else {
                // 没有子节点，直接从父节点中摘除此节点
                AVLTreeHitchEntry(tree, cur, *curParent, NULL);
                return true;
            }
            ret = true;
        }
    }
    if (ret) {
        ret = AVLTreeDeleteEntryFixup(tree, isCurLeft, cur, *curParent);
    }
    return ret;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLTreeDeleteEntryByKey(AVLTree* tree, void* key) {
    AVLEntry* deleteEntry, * curParent = NULL;
    AVLTreeDeleteEntry(tree, key, tree->root, &curParent, &deleteEntry);
    return deleteEntry;
}