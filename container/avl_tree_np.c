#include <CUtils/container/avl_tree_np.h>

/*
* 获取左子节点
*/
inline AVLEntryNp* AVLEntryNpGetLeft(AVLEntryNp* entry) {
    return (AVLEntryNp*)(((uintptr_t)entry->left_balanceFactor) & (~((uintptr_t)0x3)));
}

/*
* 设置左子节点
*/
inline void AVLEntryNpSetLeft(AVLEntryNp* entry, AVLEntryNp* left) {
    entry->left_balanceFactor = (AVLEntryNp*)(((uintptr_t)left) | ((uintptr_t)entry->left_balanceFactor & 0x3));
}

/*
* 获取右子节点
*/
inline AVLEntryNp* AVLEntryNpGetRight(AVLEntryNp* entry) {
    return entry->right;
}

/*
* 设置右子节点
*/
inline void AVLEntryNpSetRight(AVLEntryNp* entry, AVLEntryNp* right) {
    entry->right = right;
}

/*
* 获取节点平衡因子
*/
inline int AVLEntryNpGetBalanceFactor(AVLEntryNp* entry) {
    int ret = (int)(((uintptr_t)entry->left_balanceFactor) & 0x3);
    if (ret == 3) {
        ret = -1;
    }
    return ret;
}

/*
* 设置节点平衡因子
*/
inline void AVLEntryNpSetBalanceFactor(AVLEntryNp* entry, int balanceFactor) {
    entry->left_balanceFactor = (AVLEntryNp*)(((uintptr_t)AVLEntryNpGetLeft(entry)) | ((uintptr_t)balanceFactor) & 0x3);
}


/*
* 左旋子树
*/
static AVLEntryNp* RotateLeft(AVLEntryNp* subRoot, AVLEntryNp* subRootParent) {
    AVLEntryNp* newSubRoot = AVLEntryNpGetRight(subRoot);
    if (newSubRoot == NULL) {
        return subRoot;
    }
    if (subRootParent) {
        if (AVLEntryNpGetLeft(subRootParent) == subRoot) {
            AVLEntryNpSetLeft(subRootParent, newSubRoot);
        }
        else {
            AVLEntryNpSetRight(subRootParent, newSubRoot);
        }
    }
    AVLEntryNpSetRight(subRoot, AVLEntryNpGetLeft(newSubRoot));
    AVLEntryNpSetLeft(newSubRoot, subRoot);
    return newSubRoot;
}

/*
* 右旋子树
*/
static AVLEntryNp* RotateRight(AVLEntryNp* subRoot, AVLEntryNp* subRootParent) {
    AVLEntryNp* newSubRoot = AVLEntryNpGetLeft(subRoot);
    if (newSubRoot == NULL) {
        return subRoot;
    }
    if (subRootParent) {
        if (AVLEntryNpGetLeft(subRootParent) == subRoot) {
            AVLEntryNpSetLeft(subRootParent, newSubRoot);
        }
        else {
            AVLEntryNpSetRight(subRootParent, newSubRoot);
        }
    }
    AVLEntryNpSetLeft(subRoot, AVLEntryNpGetRight(newSubRoot));
    AVLEntryNpSetRight(newSubRoot, subRoot);
    return newSubRoot;

}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void AVLTreeNpHitchEntry(AVLTreeNp* tree, AVLEntryNp* entry, AVLEntryNp* entryParent, AVLEntryNp* newEntry) {
    if (entryParent) {
        if (AVLEntryNpGetLeft(entryParent) == entry) {
            AVLEntryNpSetLeft(entryParent, newEntry);
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
static bool RotateByBalanceFactor(AVLTreeNp* tree, AVLEntryNp* subRoot, int curBF, AVLEntryNp* parent) {
    bool rotate = false;
    bool heightUpdate = true;
    AVLEntryNp* newSubRoot = NULL;
    if (curBF > 1) {
        AVLEntryNp* child = AVLEntryNpGetLeft(subRoot);
        int childBF = AVLEntryNpGetBalanceFactor(child);
        if (childBF == -1) {
            int rightBF = AVLEntryNpGetBalanceFactor(child->right);
            if (child->right) {
                AVLEntryNpSetBalanceFactor(child->right, 0);
            }
            RotateLeft(child, subRoot);
            newSubRoot = RotateRight(subRoot, parent);

            if (rightBF == -1) {
                AVLEntryNpSetBalanceFactor(child, 1);
                AVLEntryNpSetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntryNpSetBalanceFactor(child, 0);
                if (rightBF == 1) {
                    AVLEntryNpSetBalanceFactor(subRoot, -1);
                }
                else {
                    AVLEntryNpSetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            heightUpdate = false;
            newSubRoot = RotateRight(subRoot, parent);
            AVLEntryNpSetBalanceFactor(child, -1);
            AVLEntryNpSetBalanceFactor(subRoot, 1);
        }
        else {
            newSubRoot = RotateRight(subRoot, parent);
            AVLEntryNpSetBalanceFactor(child, 0);
            AVLEntryNpSetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else if (curBF < -1) {
        AVLEntryNp* child = subRoot->right;
        int childBF = AVLEntryNpGetBalanceFactor(child);
        if (childBF == 1) {
            int leftBF = AVLEntryNpGetBalanceFactor(AVLEntryNpGetLeft(child));
            if (AVLEntryNpGetLeft(child)) {
                AVLEntryNpSetBalanceFactor(AVLEntryNpGetLeft(child), 0);
            }
            RotateRight(child, subRoot);
            newSubRoot = RotateLeft(subRoot, parent);
            if (leftBF == 1) {
                AVLEntryNpSetBalanceFactor(child, -1);
                AVLEntryNpSetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntryNpSetBalanceFactor(child, 0);
                if (leftBF == -1) {
                    AVLEntryNpSetBalanceFactor(subRoot, 1);
                }
                else {
                    AVLEntryNpSetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            heightUpdate = false;
            newSubRoot = RotateLeft(subRoot, parent);
            AVLEntryNpSetBalanceFactor(child, 1);
            AVLEntryNpSetBalanceFactor(subRoot, -1);
        }
        else {
            newSubRoot = RotateLeft(subRoot, parent);
            AVLEntryNpSetBalanceFactor(child, 0);
            AVLEntryNpSetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else {
        heightUpdate = false;
        AVLEntryNpSetBalanceFactor(subRoot, curBF);
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
void AVLTreeNpInit(AVLTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
    BSTreeNpInit(&tree->bst, entryFieldOffset, keyFieldOffset, keySize, cmpFunc);
}

/*
* 初始化节点
*/
void AVLEntryNpInit(AVLEntryNp* entry) {
    BSEntryNpInit(&entry->bse);
    AVLEntryNpSetBalanceFactor(entry, 0);
}




/*
* 向树中插入节点后的平衡操作
* 继续平衡返回true，无需平衡返回false
*/
bool AVLTreeNpInsertEntryFixup(AVLTreeNp* tree, bool isCurLeft, AVLEntryNp* cur, AVLEntryNp* curParent) {
    // 插入节点后平衡因子可能发生变化，回溯维护平衡因子
    int curBF = AVLEntryNpGetBalanceFactor(cur);
    if (isCurLeft) curBF++;        // 新节点插入到当前节点的左子树
    else curBF--;       // 新节点插入到当前节点的右子树

    if (RotateByBalanceFactor(tree, cur, curBF, curParent) || curBF == 0) {
        // 旋转后当前节点高度不变，或原先高度就不变，停止回溯
        return false;
    }
    return true;
}

/*
* 递归查找并插入节点
*/
bool AVLTreeNpInsertEntry(AVLTreeNp* tree, AVLEntryNp* entry, AVLEntryNp* cur, AVLEntryNp* curParent) {
    void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);

    void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    bool ret = true;
    bool isCurLeft;
    if (res == 0) {
        return false;
    }
    else if (res < 0) {
        if (AVLEntryNpGetRight(cur)) {
            ret = AVLTreeNpInsertEntry(tree, entry, AVLEntryNpGetRight(cur), cur);
        }
        else {
            AVLEntryNpSetRight(cur, entry);
        }
        isCurLeft = false;
    }
    else {
        if (AVLEntryNpGetLeft(cur)) {
            ret = AVLTreeNpInsertEntry(tree, entry, AVLEntryNpGetLeft(cur), cur);
        }
        else {
            AVLEntryNpSetLeft(cur, entry);
        }
        isCurLeft = true;
    }
    if (ret) {
        ret = AVLTreeNpInsertEntryFixup(tree, isCurLeft, cur, curParent);
    }
    return ret;
}

/*
* 从树中按key插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool AVLTreeNpInsertEntryByKey(AVLTreeNp* tree, AVLEntryNp* entry) {
    AVLEntryNpInit(entry);
    if (tree->root == NULL) {
        tree->root = entry;
        return true;
    }
    return AVLTreeNpInsertEntry(tree, entry, tree->root, NULL);
}



/*
* 从树中删除节点的平衡操作
*/
bool AVLTreeNpDeleteEntryFixup(AVLTreeNp* tree, bool isCurLeft, AVLEntryNp* cur, AVLEntryNp* curParent) {
    // 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子
    AVLEntryNp* newSubRoot = NULL;
    int curBF = AVLEntryNpGetBalanceFactor(cur);
    if (isCurLeft) curBF--;
    else curBF++;

    if (curBF != 0) {
        if (RotateByBalanceFactor(tree, cur, curBF, curParent) == false) {
            // 另一侧高度相等或更深且无需旋转，则当前节点高度不变
            return false;
        }
    }
    else {
        AVLEntryNpSetBalanceFactor(cur, curBF);
    }
    return true;
}

/*
* 递归查找最小节点并删除待删除节点，将最小节点挂接到待删除节点的位置
* delete_minEntry传入待删除节点，返回代替其的最小节点
*/
static bool DeleteMinEntry(AVLTreeNp* tree, AVLEntryNp* cur, AVLEntryNp** curParent, AVLEntryNp** delete_minEntry, AVLEntryNp* deleteParent) {
    if (AVLEntryNpGetLeft(cur)) {
        if (DeleteMinEntry(tree, AVLEntryNpGetLeft(cur), &cur, delete_minEntry, deleteParent)) {
            return AVLTreeNpDeleteEntryFixup(tree, true, cur, *curParent);
        }
    } else {
        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        AVLEntryNpSetLeft(cur, AVLEntryNpGetLeft(*delete_minEntry));

        // 最小节点挂接到需要删除的位置
        AVLTreeNpHitchEntry(tree, *delete_minEntry, deleteParent, cur);

        AVLEntryNpSetBalanceFactor(cur, AVLEntryNpGetBalanceFactor(*delete_minEntry));

        // 最小节点可能是待删除节点的右节点
        if (AVLEntryNpGetRight(*delete_minEntry) != cur) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            AVLEntryNpSetLeft(*curParent, cur->right);

            // 最小节点继承待删除节点的右子树
            cur->right = (*delete_minEntry)->right;
        } else {
            *curParent = cur;       // curParent可能指向AVLTreeNpDeleteEntry的cur，也可能是上层DeleteMinEntry的cur
        }
        *delete_minEntry = cur;     // 返回最小节点
        return true;
    }
}

/*
* 递归查找并删除节点
*/
bool AVLTreeNpDeleteEntry(AVLTreeNp* tree, void* key, AVLEntryNp* cur, AVLEntryNp** curParent, AVLEntryNp** deleteEntry) {
    void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    bool ret;
    bool isCurLeft;
    if (res > 0) {
        ret = AVLTreeNpDeleteEntry(tree, key, AVLEntryNpGetLeft(cur), &cur, deleteEntry);
        isCurLeft = true;
    }
    else if (res < 0) {
        ret = AVLTreeNpDeleteEntry(tree, key, cur->right, &cur, deleteEntry);
        isCurLeft = false;
    }
    else {
        *deleteEntry = cur;
        if (AVLEntryNpGetLeft(cur) != NULL && AVLEntryNpGetRight(cur) != NULL) {
            // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
            // 递归删除最小节点
            // 之所以都传递cur的地址，是为了保证替换最小节点后，回溯时第一层调用栈上的curParent也会同步为最小节点
            ret = DeleteMinEntry(tree, AVLEntryNpGetRight(cur), &cur, &cur, *curParent);
            isCurLeft = false;
        }
        else {
            if (cur->right != NULL) {
                // 只有右子节点
                AVLTreeNpHitchEntry(tree, cur, *curParent, cur->right);
                isCurLeft = false;
            }
            else if (AVLEntryNpGetLeft(cur) != NULL) {
                AVLTreeNpHitchEntry(tree, cur, *curParent, AVLEntryNpGetLeft(cur));
                isCurLeft = true;
            }
            else {
                // 没有子节点，直接从父节点中摘除此节点
                AVLTreeNpHitchEntry(tree, cur, *curParent, NULL);
                return true;
            }
            ret = true;
        }
    }
    if (ret) {
        ret = AVLTreeNpDeleteEntryFixup(tree, isCurLeft, cur, *curParent);
    }
    return ret;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntryNp* AVLTreeNpDeleteEntryByKey(AVLTreeNp* tree, void* key) {
    AVLEntryNp* deleteEntry, * curParent = NULL;
    AVLTreeNpDeleteEntry(tree, key, tree->root, &curParent, &deleteEntry);
    return deleteEntry;
}