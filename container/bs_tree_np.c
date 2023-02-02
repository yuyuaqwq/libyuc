#include <CUtils\container\bs_tree_np.h>

/*
* newEntry代替entry挂接到parent下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void BSTreeNpHitchEntry(BSTreeNp* tree, BSEntryNp* entry, BSEntryNp* entryParent, BSEntryNp* newEntry) {
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
void BSTreeNpInit(BSTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
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
void BSEntryNpInit(BSEntryNp* entry) {
    entry->left = NULL;
    entry->right = NULL;
}


/*
* 递归向树中插入节点
*/
bool BSTreeNpInsertEntry(BSTreeNp* tree, BSEntryNp* insertEntry, BSEntryNp* cur) {
    void* obj = GetObjByFieldOffset(insertEntry, tree->entryFieldOffset, void);
    void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);

    void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    if (res == 0) {
        return false;
    }
    else if (res < 0) {
        if (cur->right) {
            return BSTreeNpInsertEntry(tree, insertEntry, cur->right);
        }
        cur->right = insertEntry;
    }
    else {
        if (cur->left) {
            return BSTreeNpInsertEntry(tree, insertEntry, cur->left);
        }
        cur->left = insertEntry;
    }
    return true;
}

/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool BSTreeNpInsertEntryByKey(BSTreeNp* tree, BSEntryNp* insertEntry) {
    BSEntryNpInit(insertEntry);
    if (tree->root == NULL) {
        tree->root = insertEntry;
        return true;
    }
    return BSTreeNpInsertEntry(tree, insertEntry, tree->root);
}