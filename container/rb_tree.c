/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/rb_tree.h>

typedef enum {
    kRbBlack,
    kRbRed,
} RbColor;


/*
* 左旋子树
*/
static RbEntry* RotateLeft(RbEntry* subRoot) {
    RbEntry* newSubRoot = subRoot->right;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    RbEntrySetParent(newSubRoot, RbEntryGetParent(subRoot));
    if (RbEntryGetParent(subRoot)) {
        if (RbEntryGetParent(subRoot)->left == subRoot) {
            RbEntryGetParent(subRoot)->left = newSubRoot;
        }
        else {
            RbEntryGetParent(subRoot)->right = newSubRoot;
        }
    }
    RbEntrySetParent(subRoot, newSubRoot);

    subRoot->right = newSubRoot->left;
    if (subRoot->right) {
        RbEntrySetParent(subRoot->right, subRoot);
    }

    newSubRoot->left = subRoot;

    return newSubRoot;
}

/*
* 右旋子树
*/
static RbEntry* RotateRight(RbEntry* subRoot) {
    RbEntry* newSubRoot = subRoot->left;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    RbEntrySetParent(newSubRoot, RbEntryGetParent(subRoot));
    if (RbEntryGetParent(subRoot)) {
        if (RbEntryGetParent(subRoot)->left == subRoot) {
            RbEntryGetParent(subRoot)->left = newSubRoot;
        }
        else {
            RbEntryGetParent(subRoot)->right = newSubRoot;
        }
    }
    RbEntrySetParent(subRoot, newSubRoot);

    subRoot->left = newSubRoot->right;
    if (subRoot->left) {
        RbEntrySetParent(subRoot->left, subRoot);
    }

    newSubRoot->right = subRoot;

    return newSubRoot;
}

/*
* 取兄弟节点
*/
static RbEntry* GetSiblingEntry(RbEntry* entry) {
    if (RbEntryGetParent(entry)->left == entry) {
        return RbEntryGetParent(entry)->right;
    }
    return RbEntryGetParent(entry)->left;
}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void RbTreeHitchEntry(RbTree* tree, RbEntry* entry, RbEntry* newEntry) {
    if (RbEntryGetParent(entry)) {
        if (RbEntryGetParent(entry)->left == entry) {
            RbEntryGetParent(entry)->left = newEntry;
        }
        else {
            RbEntryGetParent(entry)->right = newEntry;
        }
    }
    if (newEntry) {
        RbEntrySetParent(newEntry, RbEntryGetParent(entry));
    }
    if (tree->root == entry) {
        tree->root = newEntry;
    }
}



inline RbColor RbEntryGetColor(RbEntry* entry) {
    return (RbColor)(((uintptr_t)entry->parent_color) & 0x1);
}

inline void RbEntrySetColor(RbEntry* entry, RbColor color) {
    entry->parent_color = (RbEntry*)(((uintptr_t)RbEntryGetParent(entry)) | ((uintptr_t)color));
}

inline RbEntry* RbEntryGetParent(RbEntry* entry) {
    return (RbEntry*)(((uintptr_t)entry->parent_color) & (~((uintptr_t)0x1)));
}

inline void RbEntrySetParent(RbEntry* entry, RbEntry* parent) {
    entry->parent_color = (RbEntry*)(((uintptr_t)parent) | ((uintptr_t)RbEntryGetColor(entry)));
}

inline RbEntry* RbEntryGetLeft(RbEntry* entry) {
    return entry->left;
}

inline void RbEntrySetLeft(RbEntry* entry, RbEntry* left) {
    entry->left = left;
}

inline RbEntry* RbEntryGetRight(RbEntry* entry) {
    return entry->right;
}

inline void RbEntrySetRight(RbEntry* entry, RbEntry* right) {
    entry->right = right;
}


/*
* 向树中插入节点后的平衡操作
*/
void RbTreeInsertEntryFixup(RbTree* tree, RbEntry* insertEntry) {
    RbEntry* cur = RbEntryGetParent(insertEntry);
    if (cur == NULL) {
        RbEntrySetColor(insertEntry, kRbBlack);
        return true;
    }
    RbEntrySetColor(insertEntry, kRbRed);

    // 开始回溯维护
    while (cur) {
        if (RbEntryGetColor(cur) == kRbBlack) {
            // 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点/3节点的插入，直接合并)
            break;
        }

        if (RbEntryGetParent(cur) == NULL) {
            // 没有父节点，回溯到根节点了，直接染黑
            RbEntrySetColor(cur, kRbBlack);
            break;
        }
        RbEntry* sibling = GetSiblingEntry(cur);
        
        if (sibling && RbEntryGetColor(sibling) == kRbRed) {
            // 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上插入，继续回溯
            RbEntrySetColor(cur, kRbBlack);
            RbEntrySetColor(sibling, kRbBlack);
            insertEntry = RbEntryGetParent(cur);       // 更新为该节点向上插入
            if (RbEntryGetParent(insertEntry) == NULL) {
                RbEntrySetColor(insertEntry, kRbBlack);     // 根节点，染黑并结束
                break;
            }
            RbEntrySetColor(insertEntry, kRbRed);
            cur = insertEntry;
        }
        else {
            // 没有兄弟节点或兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
            //         10b               5b    
            //      5r     20b  ->   !2r     10r    
            //  !2r                             20b
            RbEntry* newSubRoot;
            RbEntry* oldSubRoot = RbEntryGetParent(cur);
            if (RbEntryGetParent(cur)->left == cur) {
                if (cur->right == insertEntry) {
                    RotateLeft(cur);
                }
                newSubRoot = RotateRight(oldSubRoot);
            }
            else {
                if (cur->left == insertEntry) {
                    RotateRight(cur);
                }
                newSubRoot = RotateLeft(oldSubRoot);
            }
            RbEntrySetColor(newSubRoot, kRbBlack);
            RbEntrySetColor(oldSubRoot, kRbRed);

            if (tree->root == oldSubRoot) {        // 原来的子树根节点可能是整棵树的根节点，因此要检查更新
                tree->root = newSubRoot;
            }

            break;        // 只是并入，未分裂，向上没有改变颜色，不再需要回溯
        }
        cur = RbEntryGetParent(cur);
    }
}

/*
* 向树中删除节点后的平衡操作
*/
void RbTreeDeleteEntryFixup(RbTree* tree, RbEntry* deleteEntry, RbEntry* cur, RbEntry* deleteLeft, RbEntry* deleteRight, RbColor deleteColor, bool isCurLeft) {
    if (deleteEntry) {
        if (deleteColor == kRbRed) {
            // 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
            cur = NULL;
        }
        // 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
        else if (deleteLeft) {
            RbEntrySetColor(deleteLeft, kRbBlack);
            cur = NULL;
        }
        else if (deleteRight) {
            RbEntrySetColor(deleteRight, kRbBlack);
            cur = NULL;
        }
    }

    RbEntry* newSubRoot;
    // 回溯维护删除黑色节点，即没有子节点(2节点)的情况
    while (cur) {
        RbEntry* sibling = isCurLeft ? cur->right : cur->left;
        if (RbEntryGetColor(sibling) == kRbRed) {
            // 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
            // 旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点
            RbEntry* oldSubRoot = RbEntryGetParent(sibling);
            RbEntrySetColor(oldSubRoot, kRbRed);
            RbEntrySetColor(sibling, kRbBlack);
            if (oldSubRoot->left == sibling) {
                newSubRoot = RotateRight(oldSubRoot);
                sibling = oldSubRoot->left;     // 下降后挂接过来的节点
            }
            else {
                newSubRoot = RotateLeft(oldSubRoot);
                sibling = oldSubRoot->right;     // 下降后挂接过来的节点
            }
            if (tree->root == oldSubRoot) {
                tree->root = newSubRoot;
            }
        }

        // 至此兄弟节点一定为黑

        // 侄子节点为红，即兄弟节点是3/4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点)
        if (sibling->right && RbEntryGetColor(sibling->right) == kRbRed || sibling->left && RbEntryGetColor(sibling->left) == kRbRed) {
            RbColor parentColor = RbEntryGetColor(cur);
            RbEntrySetColor(cur, kRbBlack);
            RbEntry* oldSubRoot = cur;
            if (cur->left == sibling) {
                if (!sibling->left || RbEntryGetColor(sibling->left) == kRbBlack) {
                    RbEntrySetColor(sibling->right, kRbBlack);
                    sibling = RotateLeft(sibling);
                }
                else {
                    RbEntrySetColor(sibling->left, kRbBlack);
                }
                newSubRoot = RotateRight(cur);
            }
            else {
                if (!sibling->right || RbEntryGetColor(sibling->right) == kRbBlack) {
                    RbEntrySetColor(sibling->left, kRbBlack);
                    sibling = RotateRight(sibling);
                }
                else {
                    RbEntrySetColor(sibling->right, kRbBlack);
                }
                newSubRoot = RotateLeft(cur);
            }
            // 该节点会接替原先的子根节点，也要接替颜色
            RbEntrySetColor(sibling, parentColor);
            if (tree->root == oldSubRoot) {
                tree->root = newSubRoot;
            }
            break;
        }

        if (RbEntryGetColor(cur) == kRbRed) {
            // 父节点为红，即父节点是3/4节点，分裂下降与兄弟节点合并
            //    |5|8|               |5|
            //   /  |  \     ->      /   \
            //  3   6  -9-          3   |6|8|
            RbEntrySetColor(sibling, kRbRed);
            RbEntrySetColor(cur, kRbBlack);
            break;
        }
        else {
            // 父节点为黑，即父节点是2节点，兄弟节点也是2节点，下降父节点与兄弟节点合并，相当于向上删除父节点，继续回溯
            // 为什么不是3/4节点？因为黑父节点如果是3，兄弟节点是红，4的话回溯时父节点是红
            RbEntrySetColor(sibling, kRbRed);
        }
        RbEntry* child = cur;
        cur = RbEntryGetParent(cur);
        if (cur) {
            isCurLeft = cur->left == child;
        }
    }

    if (tree->root && RbEntryGetColor(tree->root) == kRbRed) {
        // 根节点染黑
        RbEntrySetColor(tree->root, kRbBlack);
    }
}


/*
* 初始化树
*/
void RbTreeInit(RbTree* tree, int entry_field_offset, int key_field_offset, int key_size, CmpFunc cmp_func) {
    BsTreeInit(&tree->bst, entry_field_offset, key_field_offset, key_size, cmp_func);
}

/*
* 初始化节点
*/
void RbEntryInit(RbEntry* entry, RbColor color) {
    BsEntryInit(&entry->bse);
    RbEntrySetColor(entry, color);
}

/*
* 从树中查找节点
* 存在返回查找到的节点对应的对象，不存在返回NULL
*/
void* RbTreeFindEntryByKey(RbTree* tree, void* key) {
    return BsTreeFindEntryByKey(&tree->bst, key);
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool RbTreeInsertEntryByKey(RbTree* tree, RbEntry* insertEntry) {
    if (!BsTreeInsertEntryByKey(&tree->bst, &insertEntry->bse, NULL)) {
        return false;
    }
    RbTreeInsertEntryFixup(tree, insertEntry);
    return true;
}

/*
* 从树中删除节点
*/
void RbTreeDeleteEntry(RbTree* tree, RbEntry* deleteEntry) {
    // 从entry的父节点开始回溯
    RbEntry* cur;
    bool isCurLeft;
    if (deleteEntry->left != NULL && deleteEntry->right != NULL) {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        RbEntry* minEntry = deleteEntry->right;
        while (minEntry->left) {
            minEntry = minEntry->left;
        }

        isCurLeft = RbEntryGetParent(minEntry)->left == minEntry;

        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        minEntry->left = deleteEntry->left;
        if (deleteEntry->left) {
            RbEntrySetParent(deleteEntry->left, minEntry);
        }

        RbEntry* oldRight = minEntry->right;

        // 最小节点可能是待删除节点的右节点
        if (deleteEntry->right != minEntry) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            RbEntryGetParent(minEntry)->left = minEntry->right;
            if (minEntry->right) {
                RbEntrySetParent(minEntry->right, RbEntryGetParent(minEntry));
            }
            // 最小节点继承待删除节点的右子树
            minEntry->right = deleteEntry->right;
            if (deleteEntry->right) {
                RbEntrySetParent(deleteEntry->right, minEntry);
            }
            cur = RbEntryGetParent(minEntry);
        }
        else {
            cur = minEntry;
        }

        RbColor oldColor = RbEntryGetColor(minEntry);
        RbEntrySetColor(minEntry, RbEntryGetColor(deleteEntry));

        // 红黑树另外需要使entry的子节点同步为minEntry原来的子节点，在判断是否存在子节点时使用
        deleteEntry->left = NULL;
        deleteEntry->right = oldRight;
        RbEntrySetColor(deleteEntry, oldColor);

        // 最后进行挂接
        RbTreeHitchEntry(tree, deleteEntry, minEntry);
    }
    else {
        cur = RbEntryGetParent(deleteEntry);
        if (cur) {
            isCurLeft = cur->left == deleteEntry;
        } else {
            isCurLeft = false;
        }

        if (deleteEntry->right != NULL) {
            // 只有右子节点
            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->right);
        }
        else if (deleteEntry->left != NULL) {
            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->left);
        }
        else {
            // 没有子节点，直接从父节点中摘除此节点
            RbTreeHitchEntry(tree, deleteEntry, NULL);
        }
    }
    RbTreeDeleteEntryFixup(tree, deleteEntry, cur, deleteEntry->left, deleteEntry->right, RbEntryGetColor(deleteEntry), isCurLeft);
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，返回对应的对象，失败返回NULL
*/
void* RbTreeDeleteEntryByKey(RbTree* tree, void* key) {
    void* obj = RbTreeFindEntryByKey(tree, key);
    if (obj) {
        RbEntry* entry = ObjectGetFieldByOffset(obj, tree->entry_field_offset, RbEntry);
        RbTreeDeleteEntry(tree, entry);
    }
    return obj;
}


void* RbTreeFirst(RbTree* tree, RbTreeIterator* iterator) {
    RbEntry* cur = tree->root;
    if (!cur) {
        return NULL;
    }
    iterator->rb_tree = tree;
    while (cur->left)
        cur = cur->left;
    void* obj = ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
    iterator->cur_entry = cur;
    return obj;
}

void* RbTreeLast(RbTree* tree, RbTreeIterator* iterator) {
    RbEntry* cur = tree->root;
    if (!cur) {
        return NULL;
    }
    iterator->rb_tree = tree;
    while (cur->right)
        cur = cur->right;
    void* obj = ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
    iterator->cur_entry = cur;
    return obj;
}

void* RbTreeNext(RbTreeIterator* iterator) {
    RbEntry* cur = iterator->cur_entry;
    if (cur->right) {
        cur = cur->right;
        while (cur->left)
            cur = cur->left;
    }
    else {
        RbEntry* parent;
        while ((parent = RbEntryGetParent(cur)) && cur == parent->right)
            cur = parent;
        cur = parent;
    }
    void* obj = ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
    iterator->cur_entry = cur;
    return obj;
}

void* RbTreePrev(RbTreeIterator* iterator) {
    RbEntry* cur = iterator->cur_entry;
    if (cur->left) {
        cur = cur->left;
        while (cur->right)
            cur = cur->right;
    }
    else {
        RbEntry* parent;
        while ((parent = RbEntryGetParent(cur)) && cur == parent->left)
            cur = parent;
        cur = parent;
    }
    void* obj = ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
    iterator->cur_entry = cur;
    return obj;
}