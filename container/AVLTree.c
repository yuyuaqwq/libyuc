#include "AVLTree.h"


#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
/*
* 获取父节点
*/
inline AVLEntry* AVLEntryGetParent(AVLEntry* entry) {
    return (AVLEntry*)(((uintptr_t)entry->parent_balanceFactor) & (~((uintptr_t)0x3)));
}

/*
* 设置父节点
*/
inline void AVLEntrySetParent(AVLEntry* entry, AVLEntry* parent) {
    entry->parent_balanceFactor = (AVLEntry*)(((uintptr_t)parent) | ((uintptr_t)entry->parent_balanceFactor & 0x3));
}

/*
* 获取节点平衡因子
*/
inline int AVLEntryGetBalanceFactor(AVLEntry* entry) {
    int ret = (int)(((uintptr_t)entry->parent_balanceFactor) & 0x3);
    if (ret == 3) {
        ret = -1;
    }
    return ret;
}

/*
* 设置节点平衡因子
*/
inline void AVLEntrySetBalanceFactor(AVLEntry* entry, int balanceFactor) {
    entry->parent_balanceFactor = (AVLEntry*)(((uintptr_t)AVLEntryGetParent(entry)) | ((uintptr_t)balanceFactor) & 0x3);
}

#else
/*
* 获取父节点
*/
inline AVLEntry* AVLEntryGetParent(AVLEntry* entry) {
    return entry->parent;
}

/*
* 设置父节点
*/
inline void AVLEntrySetParent(AVLEntry* entry, AVLEntry* parent) {
    entry->parent = parent;
}

/*
* 根据左右子树高度获取节点平衡因子
*/
int AVLEntryGetBalanceFactor(AVLEntry* subRoot) {
    int leftHeight = -1, rightHeight = -1;
    if (subRoot->left) leftHeight = subRoot->left->height;
    if (subRoot->right) rightHeight = subRoot->right->height;
    return leftHeight - rightHeight;
}


/*
* 根据左右子树高度获取节点高度
*/
static int GetMaxHeight(AVLEntry* subRoot) {
    int leftHeight = -1, rightHeight = -1;
    if (subRoot->left) leftHeight = subRoot->left->height;
    if (subRoot->right) rightHeight = subRoot->right->height;
    return max(leftHeight, rightHeight) + 1;
}

/*
* 根据左右子树高度更新节点高度
* 更新完成返回true，无需更新返回false
*/
static bool UpdateHeight(AVLEntry* subRoot) {
    int newHeight = GetMaxHeight(subRoot);
    if (subRoot->height != newHeight) {
        subRoot->height = newHeight;
        return true;
    }
    return false;
}

#endif // CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_

/*
* 左旋子树
*/
static AVLEntry* RotateLeft(AVLEntry* subRoot) {
    AVLEntry* newSubRoot = subRoot->right;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    AVLEntrySetParent(newSubRoot, AVLEntryGetParent(subRoot));
    if (AVLEntryGetParent(subRoot)) {
        if (AVLEntryGetParent(subRoot)->left == subRoot) {
            AVLEntryGetParent(subRoot)->left = newSubRoot;
        }
        else {
            AVLEntryGetParent(subRoot)->right = newSubRoot;
        }
    }
    AVLEntrySetParent(subRoot, newSubRoot);

    subRoot->right = newSubRoot->left;
    if (subRoot->right) {
        AVLEntrySetParent(subRoot->right, subRoot);
    }

    newSubRoot->left = subRoot;

#ifdef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
    UpdateHeight(subRoot);
    UpdateHeight(newSubRoot);
#endif // CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_


    return newSubRoot;
}

/*
* 右旋子树
*/
static AVLEntry* RotateRight(AVLEntry* subRoot) {
    AVLEntry* newSubRoot = subRoot->left;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    AVLEntrySetParent(newSubRoot, AVLEntryGetParent(subRoot));
    if (AVLEntryGetParent(subRoot)) {
        if (AVLEntryGetParent(subRoot)->left == subRoot) {
            AVLEntryGetParent(subRoot)->left = newSubRoot;
        }
        else {
            AVLEntryGetParent(subRoot)->right = newSubRoot;
        }
    }
    AVLEntrySetParent(subRoot, newSubRoot);

    subRoot->left = newSubRoot->right;
    if (subRoot->left) {
        AVLEntrySetParent(subRoot->left, subRoot);
    }

    newSubRoot->right = subRoot;

#ifdef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
    UpdateHeight(subRoot);
    UpdateHeight(newSubRoot);
#endif // CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_

    return newSubRoot;

}

/*
* newEntry代替entry挂接到其父节点下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void AVLTreeHitchEntry(AVLTree* tree, AVLEntry* entry, AVLEntry* newEntry) {
    if (AVLEntryGetParent(entry)) {
        if (AVLEntryGetParent(entry)->left == entry) {
            AVLEntryGetParent(entry)->left = newEntry;
        }
        else {
            AVLEntryGetParent(entry)->right = newEntry;
        }
    }
    if (newEntry) {
        AVLEntrySetParent(newEntry, AVLEntryGetParent(entry));
    }
    if (tree->root == entry) {
        tree->root = newEntry;
    }
}


#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
/*
* 旋转平衡因子调整(右旋为例)：
* 单旋情况：
*
* 原根节点的平衡因子是2，新根节点的平衡因子是1
* 说明原根节点的右子树深度是小于左子树2的
*
* 情况1：平衡因子置0
*                 o6, 2                    o5, 0
*         o5, 1         o3    ->      o4          o6(4), 0
*     o4     o3                                o3        o3
* 新根节点的右子树(o3)高度一定低于新根节点2，与原根节点(o6)的右子树高度相等，因此原根节点的平衡因子置0
*
*
*
* 情况2：调整平衡因子(左旋为例)
* 删除节点时可能会出现新根节点平衡因子为0的情况，如下删除的是o1
*                    o4, -2                                    o3(4), 1
*         o2(1), 0           o3, 0       ->           o4(3), -1            o2
*              -o1-       o2       o2         o2(1), 0         o2
* 此时不能简单置0
* 原根节点(o4)高度高于新根节点(o3)子树(o2, o2)1，而o4的左子树(o2(1))又低于o3子树(o2, o2)1
* 故原根节点(o4)置为-1
* o4高度高于o3子树1，而o4的左子树又低于o3子树1
*
* 因为挂接过去的节点高度可能与当前高度不同，因此需要调整
*
*
*
*
* 双旋情况：
*
* 原根节点的平衡因子是2，新根节点的平衡因子是-1
* 直接旋转：
*                 o6, 2                       o5(6), -2
*         o5, -1         o3    ->       o3                 o6(5), 1
*     o3     o4                                       o4          o3
*
*
* 先左旋：
* 情况1：
*                      o6, 2                                         o6, 2
*            o5, -1            o3    ->                  o4(5), 1               o3
*     o3            o4, -1                        o5(4),1          o3
*              o2         o3                 o3          o2
* o4的子节点高度是o2或者o3，挂到o5下可能影响o5的平衡因子，因此右旋后需要调整o5的平衡因子
*
* 依据是o4，-1时(o2和o3)置为1，0(o3和o3)和1(o3和o2)时置为0
* o5的新右子节点是o4的左子节点，o4的左子节点要么o2要么o3
*
*
* 情况2：
*
*                      o6, 2                                         o6, 2
*            o5, -1            o3    ->                  o4(5), 2               o3
*     o3            o4, 1                        o5(4),0          o2
*              o3         o2                o3          o3
* 情况2是第一次旋完之后，o4的右子节点o2与o6的右子节点o3不同高度
* 即相当于旋完后还需要更新o6的平衡因子
* 依据是o4，1时(o3和o2，o2要给o6做左子节点)置为-1
*
*/

/*
* 根据平衡因子来旋转子树
* 并负责更新平衡因子
* 子树高度变化返回true，高度未变化返回false
*/
static bool RotateByBalanceFactor(AVLTree* tree, AVLEntry** subRoot_, int curBF) {
    AVLEntry* subRoot = *subRoot_;

    bool rotate = false;
    bool heightUpdate = true;
    AVLEntry* newSubRoot = NULL;
    if (curBF > 1) {
        // 是失衡节点(最小失衡子树的根节点)，左子树高度高于右子树高度
        AVLEntry* child = subRoot->left;
        int childBF = AVLEntryGetBalanceFactor(child);
        // 判断需要单旋还是双旋
        if (childBF < 0) {      // childBF == -1
            // 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
            //      o
            //  o
            //    o
            // 需要先左旋转
            int rightBF = AVLEntryGetBalanceFactor(child->right);
            if (child->right) {
                AVLEntrySetBalanceFactor(child->right, 0);
            }

            RotateLeft(child);
            newSubRoot = RotateRight(subRoot);

            if (rightBF == -1) {
                AVLEntrySetBalanceFactor(child, 1);
                AVLEntrySetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntrySetBalanceFactor(child, 0);
                if (rightBF == 1) {
                    AVLEntrySetBalanceFactor(subRoot, -1);
                } else {
                    AVLEntrySetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            // 删除节点时可能出现的情况，即右子树的右子树高度低于左子树的左子树1，旋转过来并不完美平衡
            heightUpdate = false;       // 这种情况的旋转不会改变该子树的高度
            newSubRoot = RotateRight(subRoot);
            AVLEntrySetBalanceFactor(child, -1);
            AVLEntrySetBalanceFactor(subRoot, 1);
        }
        else {      // childBF == 1
            // 此时失衡节点的左子树的左子树更深，右旋即可
            // 可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点
            newSubRoot = RotateRight(subRoot);
            AVLEntrySetBalanceFactor(child , 0);
            AVLEntrySetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else if (curBF < -1) {
        // 是失衡节点，右子树高度高于左子树高度
        AVLEntry* child = subRoot->right;
        int childBF = AVLEntryGetBalanceFactor(child);
        // 判断需要单旋还是双旋
        if (childBF > 0) {      // childBF == 1
            // 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
            //    o
            //        o
            //      o
            // 需要先右旋转
            int leftBF = AVLEntryGetBalanceFactor(child->left);
            if (child->left) {
                AVLEntrySetBalanceFactor(child->left, 0);
            }

            RotateRight(child);
            newSubRoot = RotateLeft(subRoot);

            if (leftBF == 1) {
                AVLEntrySetBalanceFactor(child, -1);
                AVLEntrySetBalanceFactor(subRoot, 0);
            }
            else {
                AVLEntrySetBalanceFactor(child, 0);
                if (leftBF == -1) {
                    AVLEntrySetBalanceFactor(subRoot, 1);
                } else {
                    AVLEntrySetBalanceFactor(subRoot, 0);
                }
            }
        }
        else if (childBF == 0) {
            // 删除节点时可能出现的情况，即左子树的左子树高度低于右子树的右子树1，旋转过来并不完美平衡
            heightUpdate = false;       // 这种情况的旋转不会改变该子树的高度
            newSubRoot = RotateLeft(subRoot);
            AVLEntrySetBalanceFactor(child, 1);
            AVLEntrySetBalanceFactor(subRoot, -1);
        }
        else {      // childBF == -1
            // 此时失衡节点的右子树的右子树更深，左旋即可
            newSubRoot = RotateLeft(subRoot);
            AVLEntrySetBalanceFactor(child, 0);
            AVLEntrySetBalanceFactor(subRoot, 0);
        }
        rotate = true;
    }
    else {
        heightUpdate = false;
        AVLEntrySetBalanceFactor(subRoot, curBF);
    }

    if (rotate) {
        if (tree->root == subRoot) {        // 原来的子树根节点可能是整棵树的根节点，因此要检查更新
            tree->root = newSubRoot;
        }
        *subRoot_ = newSubRoot;
    }

    return heightUpdate;
}
#else
/*
* 根据平衡因子来旋转子树
* 旋转(子树高度变化)返回true，未旋转(子树高度未变化)返回false
*/
static bool RotateByBalanceFactor(AVLTree* tree, AVLEntry** subRoot_) {
    AVLEntry* subRoot = *subRoot_;

    int factor = AVLEntryGetBalanceFactor(subRoot);
    bool rotate = false;
    AVLEntry* newSubRoot = NULL;
    if (factor > 1) {
        // 是失衡节点(最小失衡子树的根节点)，左子树高度高于右子树高度

        // 判断需要单旋还是双旋
        if (AVLEntryGetBalanceFactor(subRoot->left) < 0) {
            // 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
            //      o
            //  o
            //    o
            // 需要先左旋转
            RotateLeft(subRoot->left);
        }
        // 此时失衡节点的左子树的左子树更深，右旋即可
        // 可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点
        newSubRoot = RotateRight(subRoot);
        rotate = true;
    }
    else if (factor < -1) {
        // 是失衡节点，右子树高度高于左子树高度

        // 判断需要单旋还是双旋
        if (AVLEntryGetBalanceFactor(subRoot->right) > 0) {
            // 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
            //    o
            //        o
            //      o
            // 需要先右旋转
            RotateRight(subRoot->right);
        }
        // 此时失衡节点的右子树的右子树更深，左旋即可
        newSubRoot = RotateLeft(subRoot);
        rotate = true;
    }

    if (rotate) {
        if (tree->root == subRoot) {        // 原来的子树根节点可能是整棵树的根节点，因此要检查更新
            tree->root = newSubRoot;
        }
        *subRoot_ = newSubRoot;
    }

    return rotate;
}

#endif // CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_


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
#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
    AVLEntrySetBalanceFactor(entry, 0);
#else
    entry->height = 0;
#endif
    
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
AVLEntry* AVLTreeFindEntryByKey(AVLTree* tree, void* key) {
    return (AVLEntry*)BSTreeFindEntryByKey(&tree->bst, key);
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool AVLTreeInsertEntry(AVLTree* tree, AVLEntry* entry) {
    if (!BSTreeInsertEntry(&tree->bst, &entry->bse)) {
        return false;
    }
#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
    AVLEntrySetBalanceFactor(entry, 0);
    AVLEntry* cur = AVLEntryGetParent(entry);
    AVLEntry* child = entry;
    bool rotate = false;
    // 插入节点后平衡因子可能发生变化，回溯维护平衡因子
    while (cur) {
        int curBF = AVLEntryGetBalanceFactor(cur);
        int childBF = AVLEntryGetBalanceFactor(child);
        if (child == cur->left) curBF++;        // 新节点插入到当前节点的左子树
        else curBF--;       // 新节点插入到当前节点的右子树

        if (RotateByBalanceFactor(tree, &cur, curBF) || curBF == 0) {
            // 旋转后当前节点高度不变，或原先高度就不变，停止回溯
            break;
        }
        child = cur;
        cur = AVLEntryGetParent(cur);
    }
#else
    entry->height = 0;
    AVLEntry* cur = AVLEntryGetParent(entry);

    // 插入节点后高度可能发生变化，回溯维护节点高度
    int heightCount = 1;
    while (cur) {
        if (cur->height >= heightCount) {
            break;        // 至此高度未发生变化，不再回溯
        }
        cur->height = heightCount++;
        if (RotateByBalanceFactor(tree, &cur)) {
            break;        // 插入后如果旋转了，就不需要再向上回溯了，因为旋转会导致这颗子树的高度不变
        }
        cur = AVLEntryGetParent(cur);
    }
#endif
    return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLTreeDeleteEntry(AVLTree* tree, AVLEntry* entry) {
    // 从entry的父节点开始回溯
    AVLEntry* cur;
    bool isLeft;
    if (entry->left != NULL && entry->right != NULL) {
        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
        AVLEntry* minEntry = entry->right;
        while (minEntry->left) {
            minEntry = minEntry->left;
        }

        isLeft = AVLEntryGetParent(minEntry)->left == minEntry;

#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
        AVLEntrySetBalanceFactor(minEntry, AVLEntryGetBalanceFactor(entry));
#else
        minEntry->height = entry->height;
#endif

        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
        minEntry->left = entry->left;
        if (entry->left) {
            AVLEntrySetParent(entry->left, minEntry);
        }

        // 最小节点可能是待删除节点的右节点
        if (AVLEntryGetParent(minEntry) != entry) {
            // 将minEntry从原先的位置摘除，用其右子树代替
            AVLEntryGetParent(minEntry)->left = minEntry->right;
            if (minEntry->right) {
                AVLEntrySetParent(minEntry->right, AVLEntryGetParent(minEntry));
            }
            // 最小节点继承待删除节点的右子树
            minEntry->right = entry->right;
            if (entry->right) {
                AVLEntrySetParent(entry->right, minEntry);
            }
            cur = AVLEntryGetParent(minEntry);
        }
        else {
            cur = minEntry;
        }

        // 最后进行挂接
        AVLTreeHitchEntry(tree, entry, minEntry);
    }
    else {
        cur = AVLEntryGetParent(entry);
        if (cur) {
            isLeft = cur->left == entry;
        }

        if (entry->left == NULL) {
            // 只有右子节点
            AVLTreeHitchEntry(tree, entry, entry->right);
        }
        else if (entry->right == NULL) {
            AVLTreeHitchEntry(tree, entry, entry->left);
        }
        else {
            // 没有子节点，直接从父节点中摘除此节点
            AVLTreeHitchEntry(tree, entry, NULL);
        }
    }

#ifndef CUTILS_CONTAINER_AVLTREE_STORAGE_HEIGHT_H_
    AVLEntry* child = entry;

    // 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子
    AVLEntry* newSubRoot = NULL;
    while (cur) {
        int curBF = AVLEntryGetBalanceFactor(cur);
        if (isLeft) curBF--;
        else curBF++;

        if (curBF != 0) {
            if (RotateByBalanceFactor(tree, &cur, curBF) == false) {
                // 另一侧高度相等或更深且无需旋转，则当前节点高度不变
                break;
            }
        }
        else {
            AVLEntrySetBalanceFactor(cur, curBF);
        }

        child = cur;
        cur = AVLEntryGetParent(cur);
        if (cur) {
            isLeft = cur->left == child;
        }
    }

#else
    // 删除节点后高度可能发生变化，回溯维护节点高度
    while (cur) {
        if (UpdateHeight(cur) == false) {
            // 父节点高度未变化则说明，父节点的另一子树(兄弟子树)深度相等或更深，要检查是否失衡
            if (RotateByBalanceFactor(tree, &cur) == false) {
                break;        // 未失衡则停止回溯，从当前节点开始不会再影响上层节点的高度，旋转后要继续回溯，因为旋转可能会使得当前子树高度降低。
            }
        }
        cur = AVLEntryGetParent(cur);
    }
#endif
    return entry;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLTreeDeleteEntryByKey(AVLTree* tree, void* key) {
    AVLEntry* entry = AVLTreeFindEntryByKey(tree, key);
    if (entry) {
        AVLTreeDeleteEntry(tree, entry);
    }
    return entry;
}