#include <libyuc/container/experimental/bs_tree_np.h>

/*
* newEntry代替entry挂接到parent下
* newEntry的左右子节点不变
* entry从树中摘除
* entry的left和right不变
*/
static void BsTreeNpHitchEntry(BsTreeNp* tree, BsEntryNp* entry, BsEntryNp* entryParent, BsEntryNp* newEntry) {
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
void BsTreeNpInit(BsTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
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
void BsEntryNpInit(BsEntryNp* entry) {
    entry->left = NULL;
    entry->right = NULL;
}


/*
* 递归向树中插入节点
*/
bool BsTreeNpInsertEntry(BsTreeNp* tree, BsEntryNp* insertEntry, BsEntryNp* cur) {
    void* obj = ObjectGetFromFieldOffset(insertEntry, tree->entryFieldOffset, void);
    void* key = ObjectGetFieldByOffset(obj, tree->keyFieldOffset, void);

    void* curObj = ObjectGetFromFieldOffset(cur, tree->entryFieldOffset, void);
    int res = tree->cmpFunc(ObjectGetFieldByOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
    if (res == 0) {
        return false;
    }
    else if (res < 0) {
        if (cur->right) {
            return BsTreeNpInsertEntry(tree, insertEntry, cur->right);
        }
        cur->right = insertEntry;
    }
    else {
        if (cur->left) {
            return BsTreeNpInsertEntry(tree, insertEntry, cur->left);
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
bool BsTreeNpInsertEntryByKey(BsTreeNp* tree, BsEntryNp* insertEntry) {
    BsEntryNpInit(insertEntry);
    if (tree->root == NULL) {
        tree->root = insertEntry;
        return true;
    }
    return BsTreeNpInsertEntry(tree, insertEntry, tree->root);
}