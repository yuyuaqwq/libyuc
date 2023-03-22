/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BS_TREE_H_
#define CUTILS_CONTAINER_BS_TREE_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif



#define CUTILS_CONTAINER_BS_TREE_DECLARATION(bs_tree_type_name, id_type, key_type) \
    typedef struct _##bs_tree_type_name##BsEntry { \
        id_type parent; \
        id_type left; \
        id_type right; \
    } bs_tree_type_name##BsEntry; \
    typedef struct _##bs_tree_type_name##BsTree { \
        id_type root; \
    } bs_tree_type_name##BsTree; \
    \
    void bs_tree_type_name##BsTreeInit(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeFind(bs_tree_type_name##BsTree* tree, key_type* key, bool scope); \
    bool bs_tree_type_name##BsTreePut(bs_tree_type_name##BsTree* tree, id_type entry_id); \
    id_type bs_tree_type_name##BsTreeDelete(bs_tree_type_name##BsTree* tree, id_type entry_id); \
    size_t bs_tree_type_name##BsTreeGetCount(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeIteratorFirst(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeIteratorLast(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeIteratorNext(bs_tree_type_name##BsTree* tree, id_type cur_id); \
    id_type bs_tree_type_name##BsTreeIteratorPrev(bs_tree_type_name##BsTree* tree, id_type cur_id); \


// 访问器需要提供_GetKey和_GetParent、_SetParent方法
#define CUTILS_CONTAINER_BS_TREE_DEFINE(bs_tree_type_name, id_type, key_type, referencer, accessor, comparer) \
    /*
    * new_entry代替entry挂接到其父节点下
    * new_entry的左右子节点不变
    * entry从树中摘除
    * entry的parent、left和right不变
    */ \
    static void bs_tree_type_name##BsTreeHitchEntry(bs_tree_type_name##BsTree* tree, id_type entry_id, id_type new_entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        bs_tree_type_name##BsEntry* new_entry = referencer##_Reference(tree, new_entry_id); \
        if (accessor##_GetParent(tree, entry) != referencer##_InvalidId) { \
            bs_tree_type_name##BsEntry* entry_parent = referencer##_Reference(tree, accessor##_GetParent(tree, entry)); \
            if (entry_parent->left == entry_id) { \
                entry_parent->left = new_entry_id; \
            } \
            else { \
                entry_parent->right = new_entry_id; \
            } \
            referencer##_Dereference(tree, entry_parent); \
        } \
        if (new_entry_id != referencer##_InvalidId) { \
            accessor##_SetParent(tree, new_entry, accessor##_GetParent(tree, entry)); \
        } \
        if (tree->root == entry_id) { \
            tree->root = new_entry_id; \
        } \
        referencer##_Dereference(tree, entry); \
        referencer##_Dereference(tree, new_entry); \
    } \
    /*
    * 左旋子树
    */ \
    static id_type bs_tree_type_name##RotateLeft(bs_tree_type_name##BsTree* tree, id_type sub_root_id, bs_tree_type_name##BsEntry* sub_root) { \
        id_type new_sub_root_id = sub_root->right; \
        if (new_sub_root_id == referencer##_InvalidId) { \
            return sub_root_id; \
        } \
        bs_tree_type_name##BsEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        accessor##_SetParent(tree, new_sub_root, accessor##_GetParent(tree, sub_root)); \
        if (accessor##_GetParent(tree, sub_root) != referencer##_InvalidId) { \
            bs_tree_type_name##BsEntry* sub_root_parent = referencer##_Reference(tree, accessor##_GetParent(tree, sub_root)); \
            if (sub_root_parent->left == sub_root_id) { \
                sub_root_parent->left = new_sub_root_id; \
            } \
            else { \
                sub_root_parent->right = new_sub_root_id; \
            } \
            referencer##_Deference(sub_root_parent); \
        } \
        accessor##_SetParent(tree, sub_root, new_sub_root_id); \
        \
        sub_root->right = new_sub_root->left; \
        if (sub_root->right != referencer##_InvalidId) { \
            accessor##_SetParent(tree, sub_root->right, sub_root_id); \
        } \
        \
        new_sub_root->left = sub_root_id; \
        \
        referencer##_Dereference(tree, new_sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 右旋子树
    */ \
    static id_type bs_tree_type_name##RotateRight(bs_tree_type_name##BsTree* tree, id_type sub_root_id, bs_tree_type_name##BsEntry* sub_root) { \
        id_type new_sub_root_id = sub_root->left; \
        if (new_sub_root_id == referencer##_InvalidId) { \
            return sub_root_id; \
        } \
        bs_tree_type_name##BsEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        accessor##_SetParent(tree, new_sub_root, accessor##_GetParent(tree, sub_root)); \
        if (accessor##_GetParent(tree, sub_root) != referencer##_InvalidId) { \
            bs_tree_type_name##BsEntry* sub_root_parent = referencer##_Reference(tree, accessor##_GetParent(tree, sub_root)); \
            if (sub_root_parent->left == sub_root_id) { \
                sub_root_parent->left = new_sub_root_id; \
            } \
            else { \
                sub_root_parent->right = new_sub_root_id; \
            } \
            referencer##_Deference(sub_root_parent); \
        } \
        accessor##_SetParent(tree, sub_root, new_sub_root_id); \
        \
        sub_root->left = new_sub_root->right; \
        if (sub_root->left != referencer##_InvalidId) { \
            accessor##_SetParent(tree, sub_root->left, sub_root_id); \
        } \
        \
        new_sub_root->right = sub_root_id; \
        \
        referencer##_Dereference(tree, new_sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 初始化节点
    */ \
    static void bs_tree_type_name##BsEntryInit(bs_tree_type_name##BsTree* tree, id_type entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        entry->left = referencer##_InvalidId; \
        entry->right = referencer##_InvalidId; \
        entry->parent = referencer##_InvalidId; \
        referencer##_Dereference(tree, entry); \
    } \
    /*
    * 初始化树
    */ \
    void bs_tree_type_name##BsTreeInit(bs_tree_type_name##BsTree* tree) { \
        tree->root = referencer##_InvalidId; \
    } \
    /*
    * 从树中查找节点
    * 存在返回查找到的节点对应的对象，不存在返回NULL
    */ \
    id_type bs_tree_type_name##BsTreeFind(bs_tree_type_name##BsTree* tree, key_type* key, bool scope) { \
        id_type cur_id = tree->root; \
        id_type prev_cur_id = referencer##_InvalidId; \
        while (cur_id != referencer##_InvalidId) { \
            bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
            if (comparer##_Less(tree, accessor##_GetKey(tree, cur), *key)) { \
                cur_id = cur->right; \
            } \
            else if (comparer##_Greater(tree, accessor##_GetKey(tree, cur), *key)) { \
                cur_id = cur->left; \
            } \
            else { \
                referencer##_Dereference(tree, cur); \
                return cur_id; \
            } \
            referencer##_Dereference(tree, cur); \
            prev_cur_id = cur_id; \
        } \
        return scope ? prev_cur_id : referencer##_InvalidId; \
    } \
    /*
    * 向树中推入节点
    * 不允许存在重复节点
    * 成功返回true，失败返回false
    */ \
    bool bs_tree_type_name##BsTreePut(bs_tree_type_name##BsTree* tree, id_type entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        bs_tree_type_name##BsEntryInit(tree, entry); \
        if (tree->root == referencer##_InvalidId) { \
            tree->root = entry_id; \
            return true; \
        } \
        id_type cur_id = tree->root; \
        while (cur_id != referencer##_InvalidId) { \
            bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
            if (comparer##_Less(tree, accessor##_GetKey(tree, cur), accessor##_GetKey(tree, entry))) { \
                if (!cur->right) { \
                    cur->right = entry_id; \
                    break; \
                } \
                cur_id = cur->right; \
            } \
            else if (comparer##_Greater(tree, accessor##_GetKey(tree, cur), accessor##_GetKey(tree, entry))) { \
                if (!cur->left) { \
                    cur->left = entry_id; \
                    break; \
                } \
                cur_id = cur->left; \
            } \
            else { \
                referencer##_Dereference(tree, cur); \
                return false; \
            } \
            referencer##_Dereference(tree, cur); \
        } \
        accessor##_SetParent(tree, entry, cur_id); \
        referencer##_Dereference(tree, entry); \
        return true; \
    } \
    /*
    * 从树中删除节点
    * 返回被删除的节点(或被替换到当前位置的右子树最小节点)，构造所有回溯条件
    */ \
    id_type bs_tree_type_name##BsTreeDelete(bs_tree_type_name##BsTree* tree, id_type entry_id, bool* is_parent_left) { \
        id_type backtrack_id; \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        if (entry->left != referencer##_InvalidId && entry->right != referencer##_InvalidId) { \
            /* 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点 */ \
            id_type min_entry_id = entry->right; \
            bs_tree_type_name##BsEntry* min_entry = referencer##_Reference(tree, min_entry_id); \
            while (min_entry->left != referencer##_InvalidId) { \
                min_entry_id = min_entry->left; \
                referencer##_Dereference(tree, min_entry); \
                min_entry = referencer##_Reference(tree, min_entry_id); \
            } \
            bs_tree_type_name##BsEntry* min_entry_parent = referencer##_Reference(tree, accessor##_GetParent(tree, min_entry)); \
            if (is_parent_left) { \
                *is_parent_left = min_entry_parent->left == min_entry_id; \
            } \
            \
            /* 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值 */ \
            min_entry->left = entry->left; \
            if (entry->left != referencer##_InvalidId) { \
                bs_tree_type_name##BsEntry* entry_left = referencer##_Reference(tree, entry->left); \
                accessor##_SetParent(tree, entry_left, min_entry_id); \
                referencer##_Dereference(tree, entry_left); \
            } \
            \
            id_type old_right_id = min_entry->right; \
            /* 最小节点可能是待删除节点的右节点 */ \
            if (entry->right != min_entry_id) { \
                /* 将min_entry从原先的位置摘除，用其右子树代替 */ \
                min_entry_parent->left = min_entry->right; \
                \
                if (min_entry->right != referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntry* min_entry_right = referencer##_Reference(tree, min_entry->right); \
                    accessor##_SetParent(tree, min_entry_right, accessor##_GetParent(tree, min_entry)); \
                    referencer##_Dereference(tree, min_entry_right); \
                } \
                /* 最小节点继承待删除节点的右子树 */ \
                min_entry->right = entry->right; \
                if (entry->right != referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntry* entry_right = referencer##_Reference(tree, entry->right); \
                    accessor##_SetParent(tree, entry_right, min_entry); \
                    referencer##_Dereference(tree, entry_right); \
                } \
                /* 如果需要回溯，这里对应entry的父亲是min_entry的父亲的情况，但不能直接修改entry的parent，因为还没挂接 */  \
                backtrack_id = accessor##_GetParent(tree, min_entry); \
            } \
            else { \
                /* 如果需要回溯，这里对应entry的父亲是min_entry的情况，但不能直接修改entry的parent，因为还没挂接 */  \
                backtrack_id = min_entry_id; \
            } \
            referencer##_Dereference(tree, min_entry_parent); \
            \
            /* 最后进行挂接 */ \
            bs_tree_type_name##BsTreeHitchEntry(tree, entry, min_entry); \
            \
            /* 也可以选择直接交换两个节点的数据，但是开销不定 */ \
            \
            entry_id = min_entry_id; \
            \
            /* 回溯可能需要的，entry变为原先的min_entry，只是不挂到树上(entry的父节点不指向entry) */ \
            entry->left = NULL; \
            entry->right = old_right_id; \
            accessor##_SetParent(tree, entry, backtrack_id); \
        } \
        else { \
            if (is_parent_left) { \
                id_type parent_id = accessor##_GetParent(tree, entry); \
                if (parent_id != referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntry* parent = referencer##_Reference(tree, parent_id); \
                    *is_parent_left = parent->left == entry_id; \
                    referencer##_Dereference(tree, parent); \
                } \
                else { \
                    *is_parent_left = false; \
                } \
            } \
            \
            if (entry->right !=  referencer##_InvalidId) { \
                /* 只有右子节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, entry_id, entry->right); \
            } \
            else if (entry->left != referencer##_InvalidId) { \
                /* 只有左子节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, entry_id, entry->left); \
            } \
            else { \
                /* 没有子节点，直接从父节点中摘除此节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, entry_id, referencer##_InvalidId); \
            } \
        } \
        return entry_id; \
    } \
    /*
    * 获取树的节点数量
    */ \
    size_t bs_tree_type_name##BsTreeGetCount(bs_tree_type_name##BsTree* tree) { \
        size_t count = 0; \
        id_type cur_id = bs_tree_type_name##BsTreeIteratorFirst(tree); \
        while (cur_id != referencer##_InvalidId) { \
            count++; \
            cur_id = bs_tree_type_name##BsTreeIteratorNext(tree, cur_id); \
        } \
        return count; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorFirst(bs_tree_type_name##BsTree* tree) { \
        id_type cur_id = tree->root; \
        if (cur_id == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        while (cur->left != referencer##_InvalidId) { \
            cur_id = cur->left; \
            referencer##_Dereference(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
        } \
        referencer##_Dereference(tree, cur); \
        return cur_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorLast(bs_tree_type_name##BsTree* tree) { \
        id_type cur_id = tree->root; \
        if (cur_id == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        while (cur->right != referencer##_InvalidId) { \
            cur_id = cur->right; \
            referencer##_Dereference(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
        } \
        referencer##_Dereference(tree, cur); \
        return cur_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorNext(bs_tree_type_name##BsTree* tree, id_type cur_id) { \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        if (cur->right != referencer##_InvalidId) { \
            cur_id = cur->right; \
            cur = referencer##_Reference(tree, cur_id); \
            while (cur->left != referencer##_InvalidId) { \
                cur_id = cur->left; \
                referencer##_Dereference(tree, cur); \
                cur = referencer##_Reference(tree, cur_id); \
            } \
            referencer##_Dereference(tree, cur); \
            return cur_id; \
        } \
        id_type parent_id = accessor##_GetParent(tree, cur); \
        bs_tree_type_name##BsEntry* parent = referencer##_Reference(tree, parent_id); \
        while (parent_id != referencer##_InvalidId && cur_id == parent->right) { \
            referencer##_Dereference(tree, cur); \
            cur = parent; \
            parent_id = accessor##_GetParent(tree, cur); \
            parent = referencer##_Reference(tree, parent_id); \
        } \
        referencer##_Dereference(tree, parent); \
        referencer##_Dereference(tree, cur); \
        return parent_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorPrev(bs_tree_type_name##BsTree* tree, id_type cur_id) { \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        if (cur->left != referencer##_InvalidId) { \
            cur_id = cur->left; \
            cur = referencer##_Reference(tree, cur_id); \
            while (cur->right != referencer##_InvalidId) { \
                cur_id = cur->right; \
                referencer##_Dereference(tree, cur); \
                cur = referencer##_Reference(tree, cur_id); \
            } \
            referencer##_Dereference(tree, cur); \
            return cur_id; \
        } \
        id_type parent_id = accessor##_GetParent(tree, cur); \
        bs_tree_type_name##BsEntry* parent = referencer##_Reference(tree, parent_id); \
        while (parent_id != referencer##_InvalidId && cur_id == parent->left) { \
            referencer##_Dereference(tree, cur); \
            cur = parent; \
            parent_id = accessor##_GetParent(tree, cur); \
            parent = referencer##_Reference(tree, parent_id); \
        } \
        referencer##_Dereference(tree, parent); \
        referencer##_Dereference(tree, cur); \
        return parent_id; \
    } \


//CUTILS_CONTAINER_BS_TREE_DECLARATION(Int, struct _IntBsEntry*, int)
//typedef struct _IntEntry {
//    IntBsEntry entry;
//    int key;
//} IntEntry;
//#define INT_BS_TREE_ACCESSOR_GetParent(bs_entry) (bs_entry->parent)
//#define INT_BS_TREE_ACCESSOR_SetParent(bs_entry, new_parent) (bs_entry->parent = new_parent)
//#define INT_BS_TREE_ACCESSOR_GetKey(bs_entry) (((IntEntry*)bs_entry)->key)
//CUTILS_CONTAINER_BS_TREE_DEFINE(Int, struct _IntBsEntry*, int, CUTILS_OBJECT_REFERENCER_DEFALUT, INT_BS_TREE_ACCESSOR, CUTILS_OBJECT_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BS_TREE_H_