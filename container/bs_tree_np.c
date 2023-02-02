#include "CUtils\container\bs_tree_np.h"

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
}


/*
* 递归向树中插入节点
*/
bool BSTreeInsertEntry(BSTree* tree, BSEntry* entry, BSEntry* cur) {
    void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);

    void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    if (res == 0) {
        return false;
    }
    else if (res < 0) {
        if (cur->right) {
            return BSTreeInsertEntry(tree, entry, cur->right);
        }
        cur->right = entry;
    }
    else {
        if (cur->left) {
            return BSTreeInsertEntry(tree, entry, cur->left);
        }
        cur->left = entry;
    }
    return true;
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool BSTreeInsertEntryByKey(BSTree* tree, BSEntry* entry) {
    BSEntryInit(entry);
    if (tree->root == NULL) {
        tree->root = entry;
        return true;
    }
    return BSTreeInsertEntry(tree, entry, tree->root);
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