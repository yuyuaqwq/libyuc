/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_BS_TREE_H_
#define LIBYUC_CONTAINER_BS_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif


#define LIBYUC_CONTAINER_BS_TREE_DECLARATION(bs_tree_type_name, id_type, offset_type, key_type, stack_count) \
    LIBYUC_CONTAINER_VECTOR_DECLARATION( \
        bs_tree_type_name##BsStack, \
        LIBYUC_CONTAINER_VECTOR_MODE_STATIC, \
        offset_type, \
        offset_type, \
        id_type, \
        stack_count) \
    typedef struct _##bs_tree_type_name##BsEntry { \
        id_type left; \
        id_type right; \
    } bs_tree_type_name##BsEntry; \
    typedef struct _##bs_tree_type_name##BsTree { \
        id_type root; \
    } bs_tree_type_name##BsTree; \
    \
    void bs_tree_type_name##BsTreeInit(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeFind(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, key_type* key); \
    id_type bs_tree_type_name##BsTreePut(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id); \
    bool bs_tree_type_name##BsTreeInsert(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id); \
    id_type bs_tree_type_name##BsTreeDelete(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id); \
    offset_type bs_tree_type_name##BsTreeGetCount(bs_tree_type_name##BsTree* tree); \
    id_type bs_tree_type_name##BsTreeIteratorLocate(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, key_type* key, int8_t* cmp_status); \
    id_type bs_tree_type_name##BsTreeIteratorFirst(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack); \
    id_type bs_tree_type_name##BsTreeIteratorLast(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack); \
    id_type bs_tree_type_name##BsTreeIteratorNext(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type cur_id); \
    id_type bs_tree_type_name##BsTreeIteratorPrev(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type cur_id); \


/*
* 访问器需要提供_GetKey和_GetRight、_SetRight、_GetLeft、_SetLeft方法
*/
#define LIBYUC_CONTAINER_BS_TREE_DEFINE(bs_tree_type_name, id_type, offset_type, key_type, referencer, accessor, comparer) \
    LIBYUC_CONTAINER_VECTOR_DEFINE(bs_tree_type_name##BsStack, \
        LIBYUC_CONTAINER_VECTOR_MODE_STATIC, \
        offset_type, \
        offset_type, \
        id_type, \
        LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT, \
        LIBYUC_BASIC_ALLOCATOR_DEFALUT, \
        LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT, \
        LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID, \
        LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT, \
        ) \
    static void bs_tree_type_name##BsTreeSetNewChild(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsEntry* entry_parent, id_type entry_id, id_type new_entry_id) { \
        if (accessor##_GetLeft(tree, entry_parent) == entry_id) { \
            accessor##_SetLeft(tree, entry_parent, new_entry_id); \
        } \
        else { \
                assert(accessor##_GetRight(tree, entry_parent) == entry_id); \
            accessor##_SetRight(tree, entry_parent, new_entry_id); \
        } \
    } \
    /*
    * 栈指向entry的父亲
    * new_entry代替entry挂接到其父节点下
    * new_entry的左右子节点不变
    * entry从树中摘除
    * entry的parent、left和right不变
    */ \
    static void bs_tree_type_name##BsTreeHitchEntry(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsEntry* entry_parent, id_type entry_id, id_type new_entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        bs_tree_type_name##BsEntry* new_entry = referencer##_Reference(tree, new_entry_id); \
        if (entry_parent != NULL) { \
            bs_tree_type_name##BsTreeSetNewChild(tree, entry_parent, entry_id, new_entry_id); \
        } \
        if (tree->root == entry_id) { \
            tree->root = new_entry_id; \
        } \
        referencer##_Dereference(tree, entry); \
        referencer##_Dereference(tree, new_entry); \
    } \
    /*
    * 栈指向sub_root的父亲
    * 左旋子树
    */ \
    static id_type bs_tree_type_name##RotateLeft(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsEntry* sub_root_parent, id_type sub_root_id, bs_tree_type_name##BsEntry* sub_root) { \
        id_type new_sub_root_id = accessor##_GetRight(tree, sub_root); \
        if (new_sub_root_id == referencer##_InvalidId) { \
            return sub_root_id; \
        } \
        bs_tree_type_name##BsEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        if (sub_root_parent != NULL) { \
            bs_tree_type_name##BsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id); \
        } \
        \
        accessor##_SetRight(tree, sub_root, accessor##_GetLeft(tree, new_sub_root)); \
        accessor##_SetLeft(tree, new_sub_root, sub_root_id); \
        \
        referencer##_Dereference(tree, new_sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 栈指向sub_root的父亲
    * 右旋子树
    */ \
    static id_type bs_tree_type_name##RotateRight(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsEntry* sub_root_parent, id_type sub_root_id, bs_tree_type_name##BsEntry* sub_root) { \
        id_type new_sub_root_id = accessor##_GetLeft(tree, sub_root); \
        if (new_sub_root_id == referencer##_InvalidId) { \
            return sub_root_id; \
        } \
        bs_tree_type_name##BsEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        if (sub_root_parent != NULL) { \
            bs_tree_type_name##BsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id); \
        } \
        \
        accessor##_SetLeft(tree, sub_root, accessor##_GetRight(tree, new_sub_root)); \
        accessor##_SetRight(tree, new_sub_root, sub_root_id); \
        \
        referencer##_Dereference(tree, new_sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 初始化节点
    */ \
    static void bs_tree_type_name##BsEntryInit(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsEntry* entry) { \
        accessor##_SetLeft(tree, entry, referencer##_InvalidId); \
        accessor##_SetRight(tree, entry, referencer##_InvalidId); \
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
    id_type bs_tree_type_name##BsTreeFind(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, key_type* key) { \
        int8_t status; \
        id_type id = bs_tree_type_name##BsTreeIteratorLocate(tree, stack, key, &status); \
        return status == 0 ? id : referencer##_InvalidId; \
    } \
    /*
    * 向树中插入节点
    * 允许重复key，同一个节点重复插入时返回false
    */ \
    bool bs_tree_type_name##BsTreeInsert(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        stack->count = 0; \
        if (tree->root == referencer##_InvalidId) { \
            bs_tree_type_name##BsEntryInit(tree, entry); \
            tree->root = entry_id; \
            return true; \
        } \
        id_type cur_id = tree->root; \
        bs_tree_type_name##BsEntry* cur = NULL; \
        bool success = true; \
        while (cur_id != referencer##_InvalidId) { \
            bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
            if (cur_id == entry_id) { \
                success = false; \
                break; \
            } \
            bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
            key_type* cur_key = accessor##_GetKey(tree, cur); \
            key_type* entry_key = accessor##_GetKey(tree, entry); \
            if (comparer##_Less(tree, cur_key, entry_key)) { \
                if (accessor##_GetRight(tree, cur) == referencer##_InvalidId) { \
                    accessor##_SetRight(tree, cur, entry_id); \
                    break; \
                } \
                cur_id = accessor##_GetRight(tree, cur); \
            } \
            else { \
                if (cur_id == entry_id) break; \
                if (accessor##_GetLeft(tree, cur) == referencer##_InvalidId) { \
                    accessor##_SetLeft(tree, cur, entry_id); \
                    break; \
                } \
                cur_id = accessor##_GetLeft(tree, cur); \
            } \
            referencer##_Dereference(tree, cur); \
        } \
        if (cur) referencer##_Dereference(tree, cur); \
        if (cur_id != entry_id) { \
            bs_tree_type_name##BsEntryInit(tree, entry); \
        } \
        referencer##_Dereference(tree, entry); \
        return success; \
    } \
    /*
    * 向树中推入节点
    * 覆盖重复key，返回被覆盖的entry_id，否则返回InvalidId，如果entry_id已经被插入过了，也会被返回
    */ \
    id_type bs_tree_type_name##BsTreePut(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id) { \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        stack->count = 0; \
        if (tree->root == referencer##_InvalidId) { \
            bs_tree_type_name##BsEntryInit(tree, entry); \
            tree->root = entry_id; \
            return referencer##_InvalidId; \
        } \
        id_type cur_id = tree->root; \
        bs_tree_type_name##BsEntry* cur = NULL; \
        id_type old_id = referencer##_InvalidId; \
        id_type parent_id = referencer##_InvalidId; \
        while (cur_id != referencer##_InvalidId) { \
            bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
            cur = referencer##_Reference(tree, cur_id); \
            key_type* cur_key = accessor##_GetKey(tree, cur); \
            key_type* entry_key = accessor##_GetKey(tree, entry); \
            if (comparer##_Less(tree, cur_key, entry_key)) { \
                if (accessor##_GetRight(tree, cur) == referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntryInit(tree, entry); \
                    accessor##_SetRight(tree, cur, entry_id); \
                    break; \
                } \
                parent_id = cur_id; \
                cur_id = accessor##_GetRight(tree, cur); \
            } \
            else if (comparer##_Greater(tree, cur_key, entry_key)) { \
                if (accessor##_GetLeft(tree, cur) == referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntryInit(tree, entry); \
                    accessor##_SetLeft(tree, cur, entry_id); \
                    break; \
                } \
                parent_id = cur_id; \
                cur_id = accessor##_GetLeft(tree, cur); \
            } \
            else { \
                /* 相等的情况 */ \
                bs_tree_type_name##BsStackVectorPopTail(stack); \
                old_id = cur_id; \
                if (cur_id == entry_id) break; \
                bs_tree_type_name##BsEntryInit(tree, entry); \
                if (parent_id != referencer##_InvalidId) { \
                    bs_tree_type_name##BsEntry* parent = referencer##_Reference(tree, parent_id); \
                    bs_tree_type_name##BsTreeSetNewChild(tree, parent, cur_id, entry_id); \
                    referencer##_Dereference(tree, parent); \
                } \
                else { \
                    tree->root = entry_id; \
                } \
                *entry = *cur; \
                break; \
            } \
            referencer##_Dereference(tree, cur); \
        } \
        if (cur) referencer##_Dereference(tree, cur); \
        referencer##_Dereference(tree, entry); \
        return old_id; \
    } \
    /*
    * 从树中删除节点
    * 返回被删除的节点(或被替换到当前位置的右子树最小节点)，构造所有回溯条件
    */ \
    id_type bs_tree_type_name##BsTreeDelete(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type entry_id, bool* is_parent_left) { \
         assert(entry_id != referencer##_InvalidId); \
        id_type backtrack_id; \
        bs_tree_type_name##BsEntry* entry = referencer##_Reference(tree, entry_id); \
        id_type* parent_id = bs_tree_type_name##BsStackVectorGetTail(stack); \
        bs_tree_type_name##BsEntry* parent = NULL; \
        if (parent_id != referencer##_InvalidId) { \
                parent = referencer##_Reference(tree, *parent_id); \
        } \
        if (accessor##_GetLeft(tree, entry) != referencer##_InvalidId && accessor##_GetRight(tree, entry) != referencer##_InvalidId) { \
            /* 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点 */ \
            bs_tree_type_name##BsStackVectorPushTail(stack, &entry_id); \
            id_type* new_entry_id = bs_tree_type_name##BsStackVectorGetTail(stack); \
            id_type min_entry_id = accessor##_GetRight(tree, entry); \
            id_type min_entry_parent_id = referencer##_InvalidId; \
            bs_tree_type_name##BsEntry* min_entry = referencer##_Reference(tree, min_entry_id); \
            while (accessor##_GetLeft(tree, min_entry) != referencer##_InvalidId) { \
                bs_tree_type_name##BsStackVectorPushTail(stack, &min_entry_id); \
                min_entry_parent_id = min_entry_id; \
                min_entry_id = accessor##_GetLeft(tree, min_entry); \
                referencer##_Dereference(tree, min_entry); \
                min_entry = referencer##_Reference(tree, min_entry_id); \
            } \
            /* 被替换到当前位置的最小节点，保证回溯路径的正确 */ \
            *new_entry_id = min_entry_id; \
            bs_tree_type_name##BsEntry* min_entry_parent = referencer##_Reference(tree, min_entry_parent_id); \
            \
            /* 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值 */ \
            accessor##_SetLeft(tree, min_entry, accessor##_GetLeft(tree, entry)); \
            \
            id_type old_right_id = accessor##_GetRight(tree, min_entry); \
            /* 最小节点可能是待删除节点的右节点 */ \
            if (accessor##_GetRight(tree, entry) != min_entry_id) { \
                /* 将min_entry从原先的位置摘除，用其右子树代替 */ \
                accessor##_SetLeft(tree, min_entry_parent, accessor##_GetRight(tree, min_entry)); \
                \
                /* 最小节点继承待删除节点的右子树 */ \
                accessor##_SetRight(tree, min_entry, accessor##_GetRight(tree, entry)); \
                if (is_parent_left) *is_parent_left = true; \
            } \
            else { \
                if (is_parent_left) *is_parent_left = false; \
            } \
            referencer##_Dereference(tree, min_entry_parent); \
            \
            /* 最后进行挂接 */ \
            bs_tree_type_name##BsTreeHitchEntry(tree, parent, entry_id, min_entry_id); \
            \
            /* 也可以选择直接交换两个节点的数据，但是开销不定 */ \
            \
            entry_id = min_entry_id; \
            \
            /* 回溯可能需要的，entry变为原先的min_entry，只是不挂到树上(entry的父节点不指向entry) */ \
            accessor##_SetLeft(tree, entry, referencer##_InvalidId); \
            accessor##_SetRight(tree, entry, old_right_id); \
        } \
        else { \
            if (is_parent_left) { \
                if (parent != NULL) { \
                    *is_parent_left = accessor##_GetLeft(tree, parent) == entry_id; \
                     assert(*is_parent_left || *is_parent_left == false && accessor##_GetRight(tree, parent) == entry_id); \
                } \
                else { \
                    *is_parent_left = false; \
                } \
            } \
            \
            if (accessor##_GetRight(tree, entry) !=    referencer##_InvalidId) { \
                /* 只有右子节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, parent, entry_id, accessor##_GetRight(tree, entry)); \
            } \
            else if (accessor##_GetLeft(tree, entry) != referencer##_InvalidId) { \
                /* 只有左子节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, parent, entry_id, accessor##_GetLeft(tree, entry)); \
            } \
            else { \
                /* 没有子节点，直接从父节点中摘除此节点 */ \
                bs_tree_type_name##BsTreeHitchEntry(tree, parent, entry_id, referencer##_InvalidId); \
            } \
        } \
        if (parent) { \
            referencer##_Dereference(tree, parent); \
        } \
        return entry_id; \
    } \
    /*
    * 获取树的节点数量
    */ \
    offset_type bs_tree_type_name##BsTreeGetCount(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack) { \
        size_t count = 0; \
        stack->count = 0; \
        id_type cur_id = bs_tree_type_name##BsTreeIteratorFirst(tree, stack); \
        while (cur_id != referencer##_InvalidId) { \
            count++; \
            cur_id = bs_tree_type_name##BsTreeIteratorNext(tree, stack, cur_id); \
        } \
        return count; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorLocate(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, key_type* key, int8_t* cmp_status) { \
        id_type cur_id = tree->root; \
        stack->count = 0; \
        id_type perv_id = referencer##_InvalidId; \
        while (cur_id != referencer##_InvalidId) { \
            perv_id = cur_id; \
            bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
            key_type* cur_key = accessor##_GetKey(tree, cur); \
            if (comparer##_Less(tree, cur_key, key)) { \
                *cmp_status = 1; \
                cur_id = accessor##_GetRight(tree, cur); \
            } \
            else if (comparer##_Greater(tree, cur_key, key)) { \
                *cmp_status = -1; \
                cur_id = accessor##_GetLeft(tree, cur); \
            } \
            else { \
                referencer##_Dereference(tree, cur); \
                *cmp_status = 0; \
                return cur_id; \
            } \
            bs_tree_type_name##BsStackVectorPushTail(stack, &perv_id); \
            referencer##_Dereference(tree, cur); \
        } \
        return perv_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorFirst(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack) { \
        id_type cur_id = tree->root; \
        if (cur_id == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        while (accessor##_GetLeft(tree, cur) != referencer##_InvalidId) { \
            bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
            cur_id = accessor##_GetLeft(tree, cur); \
            referencer##_Dereference(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
        } \
        referencer##_Dereference(tree, cur); \
        return cur_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorLast(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack) { \
        id_type cur_id = tree->root; \
        if (cur_id == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        while (accessor##_GetRight(tree, cur) != referencer##_InvalidId) { \
            bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
            cur_id = accessor##_GetRight(tree, cur); \
            referencer##_Dereference(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
        } \
        referencer##_Dereference(tree, cur); \
        return cur_id; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorNext(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type cur_id) { \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        if (accessor##_GetRight(tree, cur) != referencer##_InvalidId) { \
            bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
            cur_id = accessor##_GetRight(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
            while (accessor##_GetLeft(tree, cur) != referencer##_InvalidId) { \
                bs_tree_type_name##BsStackVectorPushTail(stack, &cur_id); \
                cur_id = accessor##_GetLeft(tree, cur); \
                referencer##_Dereference(tree, cur); \
                cur = referencer##_Reference(tree, cur_id); \
            } \
            referencer##_Dereference(tree, cur); \
            return cur_id; \
        } \
        id_type* parent_id = bs_tree_type_name##BsStackVectorPopTail(stack); \
        bs_tree_type_name##BsEntry* parent = NULL; \
        while (parent_id != NULL) { \
            parent = referencer##_Reference(tree, *parent_id); \
            if (cur_id != accessor##_GetRight(tree, parent)) break; \
            referencer##_Dereference(tree, cur); \
            cur = parent; \
            cur_id = *parent_id; \
            parent_id = bs_tree_type_name##BsStackVectorPopTail(stack); \
        } \
        referencer##_Dereference(tree, cur); \
        if (parent) referencer##_Dereference(tree, parent); \
        if (parent_id) return *parent_id; \
        return referencer##_InvalidId; \
    } \
    id_type bs_tree_type_name##BsTreeIteratorPrev(bs_tree_type_name##BsTree* tree, bs_tree_type_name##BsStackVector* stack, id_type cur_id) { \
        bs_tree_type_name##BsEntry* cur = referencer##_Reference(tree, cur_id); \
        if (accessor##_GetLeft(tree, cur) != referencer##_InvalidId) { \
            cur_id = accessor##_GetLeft(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
            while (accessor##_GetRight(tree, cur) != referencer##_InvalidId) { \
                cur_id = accessor##_GetRight(tree, cur); \
                referencer##_Dereference(tree, cur); \
                cur = referencer##_Reference(tree, cur_id); \
            } \
            referencer##_Dereference(tree, cur); \
            return cur_id; \
        } \
        id_type* parent_id = bs_tree_type_name##BsStackVectorPopTail(stack); \
        bs_tree_type_name##BsEntry* parent = NULL; \
        while (parent_id != NULL) { \
            parent = referencer##_Reference(tree, *parent_id); \
            if (cur_id != accessor##_GetLeft(tree, cur)) break; \
            referencer##_Dereference(tree, cur); \
            cur = parent; \
            cur_id = *parent_id; \
            parent_id = bs_tree_type_name##BsStackVectorPopTail(stack); \
        } \
        referencer##_Dereference(tree, cur); \
        if (parent) referencer##_Dereference(tree, parent); \
        if (parent_id) return *parent_id; \
        return referencer##_InvalidId; \
    } \


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_BS_TREE_H_