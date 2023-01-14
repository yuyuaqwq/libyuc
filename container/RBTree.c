/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "RBTree.h"

typedef enum _RBColor {
    kBlack,
    kRed,
} RBColor;

static inline RBColor GetColor(RBEntry* entry) {
    return (RBColor)(((uintptr_t)entry->parent_color) & 0x1);
}

static inline RBEntry* GetParent(RBEntry* entry);
static inline void SetColor(RBEntry* entry, RBColor color) {
    entry->parent_color = (RBEntry*)(((uintptr_t)GetParent(entry)) | ((uintptr_t)color));
}

static inline RBEntry* GetParent(RBEntry* entry) {
    return (RBEntry*)(((uintptr_t)entry->parent_color) & (~((uintptr_t)0x1)));
}

static inline void SetParent(RBEntry* entry, RBEntry* parent) {
    entry->parent_color = (RBEntry*)(((uintptr_t)parent) | ((uintptr_t)GetColor(entry)));
}

/*
* 左旋子树
*/
static RBEntry* RotateLeft(RBEntry* subRoot) {
    RBEntry* newSubRoot = subRoot->right;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    SetParent(newSubRoot, GetParent(subRoot));
    if (GetParent(subRoot)) {
        if (GetParent(subRoot)->left == subRoot) {
            GetParent(subRoot)->left = newSubRoot;
        }
        else {
            GetParent(subRoot)->right = newSubRoot;
        }
    }
    SetParent(subRoot, newSubRoot);

    subRoot->right = newSubRoot->left;
    if (subRoot->right) {
        SetParent(subRoot->right, subRoot);
    }

    newSubRoot->left = subRoot;

    return newSubRoot;
}

/*
* 右旋子树
*/
static RBEntry* RotateRight(RBEntry* subRoot) {
    RBEntry* newSubRoot = subRoot->left;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    SetParent(newSubRoot, GetParent(subRoot));
    if (GetParent(subRoot)) {
        if (GetParent(subRoot)->left == subRoot) {
            GetParent(subRoot)->left = newSubRoot;
        }
        else {
            GetParent(subRoot)->right = newSubRoot;
        }
    }
    SetParent(subRoot, newSubRoot);

    subRoot->left = newSubRoot->right;
    if (subRoot->left) {
        SetParent(subRoot->left, subRoot);
    }

    newSubRoot->right = subRoot;

    return newSubRoot;
}

/*
* 取兄弟节点
*/
static RBEntry* GetSiblingEntry(RBEntry* entry) {
    if (GetParent(entry)->left == entry) {
        return GetParent(entry)->right;
    }
    return GetParent(entry)->left;
}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void RBHitchEntry(RBTree* tree, RBEntry* entry, RBEntry* newEntry) {
    if (GetParent(entry)) {
        if (GetParent(entry)->left == entry) {
            GetParent(entry)->left = newEntry;
        }
        else {
            GetParent(entry)->right = newEntry;
        }
    }
    if (newEntry) {
        SetParent(newEntry, GetParent(entry));
    }
    if (tree->root == entry) {
        tree->root = newEntry;
    }
}

/*
* 初始化树
*/
void RBTreeInit(RBTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
    BSTreeInit(tree, entryFieldOffset, keyFieldOffset, keySize, cmpFunc);
}

/*
* 初始化节点
*/
void RBEntryInit(RBEntry* entry, RBColor color) {
    BSEntryInit(&entry->bs);
    SetColor(entry, color);
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
RBEntry* RBFindEntryByKey(RBTree* tree, void* key) {
    return (RBEntry*)BSFindEntryByKey(tree, key);
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool RBInsertEntry(RBTree* tree, RBEntry* entry) {
    if (!BSInsertEntry(tree, &entry->bs)) {
        return false;
    }

    RBEntry* cur = GetParent(entry);
    if (cur == NULL) {
        SetColor(entry, kBlack);
        return true;
    }
    SetColor(entry, kRed);

    if (GetColor(cur) == kBlack) {
        // 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点/3节点的插入，直接合并)
        return true;
    }

    RBEntry* newSubRoot = NULL;
    // 开始回溯维护
    while (cur) {
        if (GetParent(cur) == NULL) {
            // 没有父节点，回溯到根节点了，直接染黑
            SetColor(cur, kBlack);
            break;
        }
        RBEntry* sibling = GetSiblingEntry(cur);

        if (sibling && GetColor(sibling) == kRed) {
            // 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上合并，继续回溯
            SetColor(cur, kBlack);
            SetColor(sibling, kBlack);
            SetColor(GetParent(cur), kRed);
        }
        else {
            // 没有兄弟节点或者兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
            //         10b               5b    
            //      5r     20b  ->   !2r     10r    
            //  !2r                             20b
            RBEntry* oldSubRoot = GetParent(cur);
            if (GetParent(cur)->left == cur) {
                if (cur->right == entry) {
                    RotateLeft(cur);
                }
                newSubRoot = RotateRight(GetParent(cur));
            }
            else {
                if (cur->left == entry) {
                    RotateRight(cur);
                }
                newSubRoot = RotateLeft(GetParent(cur));
            }
            SetColor(cur, kBlack);
            SetColor(oldSubRoot, kRed);

            if (tree->root == oldSubRoot) {        // 原来的子树根节点可能是整棵树的根节点，因此要检查更新
                tree->root = newSubRoot;
            }

            break;        // 只是并入，未分裂，向上没有改变颜色，不再需要回溯
        }
        cur = GetParent(cur);
    }
    return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
RBEntry* RBDeleteEntry(RBTree* tree, RBEntry* entry) {
    RBEntry* backtrack = entry;        // 通常情况下是从被删除节点的父节点开始回溯

    // 出于性能考虑，RB找节点删除的逻辑还是单独写一份，差不了太多，但是RB回溯时需要保证entry还在树上(找兄弟节点)，回溯完才删除
    // 第二点就是颜色被嵌入到了父节点当中(不过BS也可以在每次访问的时候处理一下)
    RBEntry* newEntry;
    if (entry->left == NULL && entry->right == NULL) {
        // 没有子节点，直接从父节点中摘除此节点
        newEntry = NULL;
    }
    else if (entry->left == NULL) {
        // 挂接右子节点
        newEntry = entry->right;
    }
    else if (entry->right == NULL) {
        // 挂接左子节点
        newEntry = entry->left;
    }
    else {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        RBEntry* minEntry = entry->right;
        while (minEntry->left) {
            minEntry = minEntry->left;
        }
        RBColor temp = GetColor(minEntry);
        SetColor(minEntry, GetColor(entry));
        SetColor(entry, temp);

        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        minEntry->left = entry->left;
        if (minEntry->left) {
            SetParent(minEntry->left, minEntry);
        }

        // 这里需要临时将entry也挂接到minEntry的位置，回溯用
        RBEntry* oldParent, * oldRight = minEntry->right;
        // 最小节点可能是待删除节点的右节点
        if (GetParent(minEntry) != entry) {
            oldParent = GetParent(minEntry);        // 挂接minEntry之前记录

            // 将最小节点从原先的位置摘除，用entry代替
            GetParent(minEntry)->left = entry;
            if (minEntry->right) {
                SetParent(minEntry->right, entry);
            }
            // 最小节点继承待删除节点的右子树
            minEntry->right = entry->right;
            if (entry->right) {
                SetParent(entry->right, minEntry);
            }
        }
        else {
            oldParent = minEntry;        // 最小节点的父亲就是待删除节点，交换位置后最小节点就是待删除节点的父亲，因此从这里回溯
            minEntry->right = entry;
        }

        RBHitchEntry(tree, entry, minEntry);

        // 将entry临时挂接到minEntry的位置
        SetParent(entry, oldParent);
        entry->left = NULL; entry->right = oldRight;
        newEntry = oldRight;
    }

    if (entry) {
        if (GetColor(entry) == kRed) {
            // 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
            backtrack = NULL;
        }
        // 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
        else if (entry->left) {
            SetColor(entry->left, kBlack);
            backtrack = NULL;
        }
        else if (entry->right) {
            SetColor(entry->right, kBlack);
            backtrack = NULL;
        }
    }

    RBEntry* newSubRoot = NULL;
    // 回溯维护删除黑色节点，即没有子节点(2节点)的情况
    while (backtrack) {
        if (GetParent(backtrack) == NULL) {
            // 没有父节点，回溯到根节点了，直接染黑
            SetColor(backtrack, kBlack);
            break;
        }

        RBEntry* sibling = GetSiblingEntry(backtrack);
        if (GetColor(sibling) == kRed) {
            // 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
            // 旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，但兄弟节点变为真正的兄弟节点
            SetColor(GetParent(sibling), kRed);
            SetColor(sibling, kBlack);
            RBEntry* oldSubRoot = GetParent(sibling);
            if (GetParent(sibling)->left == sibling){
                newSubRoot = RotateRight(GetParent(sibling));
            } else {
                newSubRoot = RotateLeft(GetParent(sibling));
            }
            if (tree->root == oldSubRoot) {
                tree->root = newSubRoot;
            }
            sibling = GetSiblingEntry(backtrack);
        }

        // 至此兄弟节点一定为黑
        
        // 侄子节点为红，即兄弟节点是3/4节点的情况
        if (sibling->right && GetColor(sibling->right) == kRed || sibling->left && GetColor(sibling->left) == kRed) {
            SetColor(sibling, GetColor(GetParent(sibling)));
            SetColor(GetParent(sibling), kBlack);
            RBEntry* oldSubRoot = GetParent(sibling);
            if (GetParent(sibling)->left == sibling) {
                if (!sibling->left || GetColor(sibling->left) == kBlack) {
                    SetColor(sibling->right, kBlack);
                    sibling = RotateLeft(sibling);
                } else {
                    SetColor(sibling->left, kBlack);
                }
                newSubRoot = RotateRight(GetParent(sibling));
            }
            else {
                if (!sibling->right || GetColor(sibling->right) == kBlack) {
                    SetColor(sibling->left, kBlack);
                    sibling = RotateRight(sibling);
                } else {
                    SetColor(sibling->right, kBlack);
                }
                newSubRoot = RotateLeft(GetParent(sibling));
            }
            if (tree->root == oldSubRoot) {
                tree->root = newSubRoot;
            }
            break;
        }
        
        if (GetColor(GetParent(sibling)) == kRed) {
            // 父节点为红，即父节点是3/4节点，分裂下沉与兄弟节点合并
            SetColor(sibling, kRed);
            SetColor(GetParent(sibling), kBlack);
            break;
        }
        else {
            // 父节点为黑，即父节点是2节点，兄弟节点也是2节点，合并两个节点，此处高度-1，继续回溯寻求高度补偿
            SetColor(sibling, kRed);
        }
        backtrack = GetParent(backtrack);
    }
    RBHitchEntry(tree, entry, newEntry);
    return entry;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
RBEntry* RBDeleteEntryByKey(RBTree* tree, void* key) {
    RBEntry* entry = RBFindEntryByKey(tree, key);
    if (entry) {
        RBDeleteEntry(tree, entry);
    }
    return entry;
}