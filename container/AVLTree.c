/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "AVLTree.h"

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
static int UpdateHeight(AVLEntry* subRoot) {
    int newHeight = GetMaxHeight(subRoot);
    if (subRoot->height != newHeight) {
        subRoot->height = newHeight;
        return true;
    }
    return false;
}

/*
* 根据左右子树高度获取节点平衡因子
*/
static int GetBalanceFactor(AVLEntry* subRoot) {
    int leftHeight = -1, rightHeight = -1;
    if (subRoot->left) leftHeight = subRoot->left->height;
    if (subRoot->right) rightHeight = subRoot->right->height;
    return leftHeight - rightHeight;
}

/*
* 左旋子树
*/
static AVLEntry* RotateLeft(AVLEntry* subRoot) {
    AVLEntry* newSubRoot = subRoot->right;
    if (newSubRoot == NULL) {
        return subRoot;
    }

    newSubRoot->parent = subRoot->parent;
    if (subRoot->parent) {
        if (subRoot->parent->left == subRoot) {
            subRoot->parent->left = newSubRoot;
        } else {
            subRoot->parent->right = newSubRoot;
        }
    }
    subRoot->parent = newSubRoot;
    
    subRoot->right = newSubRoot->left;
    if (subRoot->right) {
        subRoot->right->parent = subRoot;
    }

    newSubRoot->left = subRoot;
        
    UpdateHeight(subRoot);
    UpdateHeight(newSubRoot);

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

    newSubRoot->parent = subRoot->parent;
    if (subRoot->parent) {
        if (subRoot->parent->left == subRoot) {
            subRoot->parent->left = newSubRoot;
        }
        else {
            subRoot->parent->right = newSubRoot;
        }
    }
    subRoot->parent = newSubRoot;

    subRoot->left = newSubRoot->right;
    if (subRoot->left) {
        subRoot->left->parent = subRoot;
    }

    newSubRoot->right = subRoot;

    UpdateHeight(subRoot);
    UpdateHeight(newSubRoot);

    return newSubRoot;

}

/*
* 根据平衡因子来旋转子树
*/
static bool RotateByBalanceFactor(AVLTree* tree, AVLEntry** subRoot_) {
    AVLEntry* subRoot = *subRoot_;

    int factor = GetBalanceFactor(subRoot);
    bool rotate = false;
    AVLEntry* newSubRoot = NULL;
    if (factor > 1) {
        // 是失衡节点，左子树高度高于右子树高度

        // 判断需要单旋还是双旋
        if (GetBalanceFactor(subRoot->left) < 0) {
            // 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
            RotateLeft(subRoot->left);
        }
        // 此时失衡节点的左子树的左子树更深，右旋即可
        // 可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点
        newSubRoot = RotateRight(subRoot);
        rotate = true;
    } else if (factor < -1) {
        // 是失衡节点，右子树高度高于左子树高度

        // 判断需要单旋还是双旋
        if (GetBalanceFactor(subRoot->right) > 0) {
            // 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
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

/*
* 初始化树
*/
void AVLTreeInit(AVLTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
    BSTreeInit((BSTree*)tree, entryFieldOffset, keyFieldOffset, keySize, cmpFunc);
}

/*
* 初始化节点
*/
void AVLEntryInit(AVLEntry* entry) {
    BSEntryInit(&entry->bs);
    entry->height = 0;
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
AVLEntry* AVLFindEntryByKey(AVLTree* tree, void* key) {
    return (AVLEntry*)BSFindEntryByKey((BSTree*)tree, key);
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool AVLInsertEntry(AVLTree* tree, AVLEntry* entry) {
    if (!BSInsertEntry((BSTree*)tree, &entry->bs)) {
        return false;
    }
    entry->height = 0;
    AVLEntry* cur = entry->parent;

    // 插入节点后高度可能发生变化，回溯维护节点高度
    int heightCount = 1;
    while (cur) {
        if (cur->height < heightCount) {
            cur->height = heightCount;
        } else {
            break;        // 至此高度未发生变化，不再回溯
        }
        if (RotateByBalanceFactor(tree, &cur)) {
            break;        // 插入后如果旋转了，就不需要再向上回溯了，因为旋转会导致这颗子树的高度不变
        }
        cur = cur->parent;
        heightCount++;
    }
    return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLDeleteEntry(AVLTree* tree, AVLEntry* entry) {
    AVLEntry* backtrack = (AVLEntry*)BSDeleteEntry(tree, entry);
    if (backtrack) {
        backtrack = backtrack->parent;
    }
    
    // 删除节点后高度可能发生变化，回溯维护节点高度
    while (backtrack) {
        if (UpdateHeight(backtrack) == false) {
            // 父节点高度未变化则说明，父节点的另一子树深度更深，要检查是否失衡
            if (RotateByBalanceFactor(tree, &backtrack) == false) {
                break;        // 未失衡则停止回溯，从当前节点开始不会再影响上层节点的高度。
            }
        }
        else {}
        backtrack = backtrack->parent;
    }
    return entry;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLDeleteEntryByKey(AVLTree* tree, void* key) {
    AVLEntry* entry = AVLFindEntryByKey(tree, key);
    if (entry) {
        AVLDeleteEntry(tree, entry);
    }
    return entry;
}

