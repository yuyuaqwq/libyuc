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



#define INT_RB_TREE_ACCESSOR_GetKey(bs_entry) (((IntEntry_Rb*)bs_entry)->key)
#define INT_RB_TREE_ACCESSOR_GetParent(entry) ((IntRbEntry*)(((uintptr_t)(((IntRbEntry*)entry)->parent_color) & (~((uintptr_t)0x1)))))
#define  INT_RB_TREE_ACCESSOR_GetColor(entry) ((RbColor)(((uintptr_t)((IntRbEntry*)entry)->parent_color) & 0x1))
#define INT_RB_TREE_ACCESSOR_SetParent(entry, new_parent_id) (((IntRbEntry*)entry)->parent_color = (IntRbEntry*)(((uintptr_t)new_parent_id) | ((uintptr_t)INT_RB_TREE_ACCESSOR_GetColor(entry))));
#define INT_RB_TREE_ACCESSOR_SetColor(entry, color) (entry->parent_color = (IntRbEntry*)(((uintptr_t)INT_RB_TREE_ACCESSOR_GetParent(entry)) | ((uintptr_t)color)))
#define INT_RB_TREE_ACCESSOR INT_RB_TREE_ACCESSOR

#define INT_RB_TREE_REFERENCER_Reference(main_obj, obj_id) ((IntBsEntry*)obj_id)
#define INT_RB_TREE_REFERENCER_Dereference(main_obj, obj)
#define INT_RB_TREE_REFERENCER_InvalidId (NULL)
#define INT_RB_TREE_REFERENCER INT_RB_TREE_REFERENCER
CUTILS_CONTAINER_RB_TREE_DEFINE(Int, IntRbEntry*, int, INT_RB_TREE_REFERENCER, INT_RB_TREE_ACCESSOR, CUTILS_OBJECT_COMPARER_DEFALUT)


//

//
//

//

//
///*
//* newEntry代替entry挂接到其父节点下
//* newEntry的左右子节点不变
//* entry从树中摘除
//* entry的parent、left和right不变
//*/
//static void RbTreeHitchEntry(RbTree* tree, RbEntry* entry, RbEntry* newEntry) {
//    if (RbEntryGetParent(entry)) {
//        if (RbEntryGetParent(entry)->left == entry) {
//            RbEntryGetParent(entry)->left = newEntry;
//        }
//        else {
//            RbEntryGetParent(entry)->right = newEntry;
//        }
//    }
//    if (newEntry) {
//        RbEntrySetParent(newEntry, RbEntryGetParent(entry));
//    }
//    if (tree->root == entry) {
//        tree->root = newEntry;
//    }
//}
//
//

//
/////*
////* 向树中删除节点后的平衡操作
////*/
//void RbTreeDeleteEntryFixup(RbTree* tree, RbEntry* del_entry, RbEntry* cur, RbEntry* del_left, RbEntry* del_right, RbColor del_color, bool is_cur_left) {
//    if (del_entry) {
//        if (del_color == kRbRed) {
//            // 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
//            cur = NULL;
//        }
//        // 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
//        else if (del_left) {
//            RbEntrySetColor(del_left, kRbBlack);
//            cur = NULL;
//        }
//        else if (del_right) {
//            RbEntrySetColor(del_right, kRbBlack);
//            cur = NULL;
//        }
//    }
//
//    RbEntry* new_sub_root;
//    // 回溯维护删除黑色节点，即没有子节点(2节点)的情况
//    while (cur) {
//        RbEntry* sibling = is_cur_left ? cur->right : cur->left;
//        if (RbEntryGetColor(sibling) == kRbRed) {
//            // 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
//            // 旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点
//            RbEntry* old_sub_root = RbEntryGetParent(sibling);
//            RbEntrySetColor(old_sub_root, kRbRed);
//            RbEntrySetColor(sibling, kRbBlack);
//            if (old_sub_root->left == sibling) {
//                new_sub_root = RotateRight(old_sub_root);
//                sibling = old_sub_root->left;     // 下降后挂接过来的节点
//            }
//            else {
//                new_sub_root = RotateLeft(old_sub_root);
//                sibling = old_sub_root->right;     // 下降后挂接过来的节点
//            }
//            if (tree->root == old_sub_root) {
//                tree->root = new_sub_root;
//            }
//        }
//
//        // 至此兄弟节点一定为黑
//
//        // 侄子节点为红，即兄弟节点是3/4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点)
//        if (sibling->right && RbEntryGetColor(sibling->right) == kRbRed || sibling->left && RbEntryGetColor(sibling->left) == kRbRed) {
//            RbColor parentColor = RbEntryGetColor(cur);
//            RbEntrySetColor(cur, kRbBlack);
//            RbEntry* old_sub_root = cur;
//            if (cur->left == sibling) {
//                if (!sibling->left || RbEntryGetColor(sibling->left) == kRbBlack) {
//                    RbEntrySetColor(sibling->right, kRbBlack);
//                    sibling = RotateLeft(sibling);
//                }
//                else {
//                    RbEntrySetColor(sibling->left, kRbBlack);
//                }
//                new_sub_root = RotateRight(cur);
//            }
//            else {
//                if (!sibling->right || RbEntryGetColor(sibling->right) == kRbBlack) {
//                    RbEntrySetColor(sibling->left, kRbBlack);
//                    sibling = RotateRight(sibling);
//                }
//                else {
//                    RbEntrySetColor(sibling->right, kRbBlack);
//                }
//                new_sub_root = RotateLeft(cur);
//            }
//            // 该节点会接替原先的子根节点，也要接替颜色
//            RbEntrySetColor(sibling, parentColor);
//            if (tree->root == old_sub_root) {
//                tree->root = new_sub_root;
//            }
//            break;
//        }
//
//        if (RbEntryGetColor(cur) == kRbRed) {
//            // 父节点为红，即父节点是3/4节点，分裂下降与兄弟节点合并
//            //    |5|8|               |5|
//            //   /  |  \     ->      /   \
//            //  3   6  -9-          3   |6|8|
//            RbEntrySetColor(sibling, kRbRed);
//            RbEntrySetColor(cur, kRbBlack);
//            break;
//        }
//        else {
//            // 父节点为黑，即父节点是2节点，兄弟节点也是2节点，下降父节点与兄弟节点合并，相当于向上删除父节点，继续回溯
//            // 为什么不是3/4节点？因为黑父节点如果是3，兄弟节点是红，4的话回溯时父节点是红
//            RbEntrySetColor(sibling, kRbRed);
//        }
//        RbEntry* child = cur;
//        cur = RbEntryGetParent(cur);
//        if (cur) {
//            is_cur_left = cur->left == child;
//        }
//    }
//
//    if (tree->root && RbEntryGetColor(tree->root) == kRbRed) {
//        // 根节点染黑
//        RbEntrySetColor(tree->root, kRbBlack);
//    }
//}
//
//
///*
//* 初始化树
//*/
//void RbTreeInit(RbTree* tree, int entry_field_offset, int key_field_offset, int key_size, CmpFunc cmp_func) {
//    BsTreeInit(&tree->bst, entry_field_offset, key_field_offset, key_size, cmp_func);
//}
//
///*
//* 初始化节点
//*/
//void RbEntryInit(RbEntry* entry, RbColor color) {
//    BsEntryInit(&entry->bse);
//    RbEntrySetColor(entry, color);
//}
//
//

//bool RbTreeInsertEntryByKey(RbTree* tree, RbEntry* insertEntry) {
//    if (!BsTreeInsertEntryByKey(&tree->bst, &insertEntry->bse, NULL)) {
//        return false;
//    }
//    RbTreeInsertEntryFixup(tree, insertEntry);
//    return true;
//}
//
///*
//* 从树中删除节点
//*/
//void RbTreeDeleteEntry(RbTree* tree, RbEntry* deleteEntry) {
//    // 从entry的父节点开始回溯
//    RbEntry* cur;
//    bool isCurLeft;
//    if (deleteEntry->left != NULL && deleteEntry->right != NULL) {
//        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
//        RbEntry* minEntry = deleteEntry->right;
//        while (minEntry->left) {
//            minEntry = minEntry->left;
//        }
//
//        isCurLeft = RbEntryGetParent(minEntry)->left == minEntry;
//
//        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
//        minEntry->left = deleteEntry->left;
//        if (deleteEntry->left) {
//            RbEntrySetParent(deleteEntry->left, minEntry);
//        }
//
//        RbEntry* oldRight = minEntry->right;
//
//        // 最小节点可能是待删除节点的右节点
//        if (deleteEntry->right != minEntry) {
//            // 将minEntry从原先的位置摘除，用其右子树代替
//            RbEntryGetParent(minEntry)->left = minEntry->right;
//            if (minEntry->right) {
//                RbEntrySetParent(minEntry->right, RbEntryGetParent(minEntry));
//            }
//            // 最小节点继承待删除节点的右子树
//            minEntry->right = deleteEntry->right;
//            if (deleteEntry->right) {
//                RbEntrySetParent(deleteEntry->right, minEntry);
//            }
//            cur = RbEntryGetParent(minEntry);
//        }
//        else {
//            cur = minEntry;
//        }
//
//        RbColor oldColor = RbEntryGetColor(minEntry);
//        RbEntrySetColor(minEntry, RbEntryGetColor(deleteEntry));
//
//        // 红黑树另外需要使entry的子节点同步为minEntry原来的子节点，在判断是否存在子节点时使用
//        deleteEntry->left = NULL;
//        deleteEntry->right = oldRight;
//        RbEntrySetColor(deleteEntry, oldColor);
//
//        // 最后进行挂接
//        RbTreeHitchEntry(tree, deleteEntry, minEntry);
//    }
//    else {
//        cur = RbEntryGetParent(deleteEntry);
//        if (cur) {
//            isCurLeft = cur->left == deleteEntry;
//        } else {
//            isCurLeft = false;
//        }
//
//        if (deleteEntry->right != NULL) {
//            // 只有右子节点
//            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->right);
//        }
//        else if (deleteEntry->left != NULL) {
//            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->left);
//        }
//        else {
//            // 没有子节点，直接从父节点中摘除此节点
//            RbTreeHitchEntry(tree, deleteEntry, NULL);
//        }
//    }
//    RbTreeDeleteEntryFixup(tree, deleteEntry, cur, deleteEntry->left, deleteEntry->right, RbEntryGetColor(deleteEntry), isCurLeft);
//}
//
///*
//* 从树中按key删除节点
//* 成功返回被删除的节点，返回对应的对象，失败返回NULL
//*/
//void* RbTreeDeleteEntryByKey(RbTree* tree, void* key) {
//    void* obj = RbTreeFindEntryByKey(tree, key);
//    if (obj) {
//        RbEntry* entry = ObjectGetFieldByOffset(obj, tree->entry_field_offset, RbEntry);
//        RbTreeDeleteEntry(tree, entry);
//    }
//    return obj;
//}
//
//
//void* RbTreeFirst(RbTree* tree, RbTreeIterator* iterator) {
//    RbEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    iterator->rb_tree = tree;
//    while (cur->left)
//        cur = cur->left;
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreeLast(RbTree* tree, RbTreeIterator* iterator) {
//    RbEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    iterator->rb_tree = tree;
//    while (cur->right)
//        cur = cur->right;
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreeNext(RbTreeIterator* iterator) {
//    RbEntry* cur = iterator->cur_entry;
//    if (cur->right) {
//        cur = cur->right;
//        while (cur->left)
//            cur = cur->left;
//    }
//    else {
//        RbEntry* parent;
//        while ((parent = RbEntryGetParent(cur)) && cur == parent->right)
//            cur = parent;
//        cur = parent;
//    }
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreePrev(RbTreeIterator* iterator) {
//    RbEntry* cur = iterator->cur_entry;
//    if (cur->left) {
//        cur = cur->left;
//        while (cur->right)
//            cur = cur->right;
//    }
//    else {
//        RbEntry* parent;
//        while ((parent = RbEntryGetParent(cur)) && cur == parent->left)
//            cur = parent;
//        cur = parent;
//    }
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
//    }
//    return NULL;
//}