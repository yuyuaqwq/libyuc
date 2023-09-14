/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_CLASS_NAME
#define LIBYUC_CONTAINER_BS_TREE_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_Type_Key
#define LIBYUC_CONTAINER_BS_TREE_Type_Key int
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(main_obj, obj)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(main_obj, obj) ((obj)->left)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(main_obj, obj) ((obj)->right)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(main_obj, obj, new_left) ((obj)->left = new_left)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(main_obj, obj, new_right) ((obj)->right = new_right)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id struct BsEntry*
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset size_t
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId NULL
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(main_obj, reference)
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_COMPARER_Less
#define LIBYUC_CONTAINER_BS_TREE_COMPARER_Less(main_obj, obj1, obj2) (*(obj1) < *(obj2))
#endif

#ifndef LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater
#define LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater(main_obj, obj1, obj2) (*(obj1) > *(obj2))
#endif

#define BsEntry MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsEntry)
#define BsTree MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTree)


typedef struct BsEntry {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id left;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id right;
} BsEntry;
typedef struct BsTree {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id root;
} BsTree;
   

#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeStack)
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC
#define LIBYUC_CONTAINER_VECTOR_Type_Element LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_VECTOR_REFERENCER_Type_Id LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_VECTOR_Const_ElementCount (sizeof(LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset) * 8 * 2)
#include <libyuc/container/vector.h>

#define BsTreeStackVector MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeStackVector)


#define BsTreeInit MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeInit)
#define BsTreeFind MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeFind)
#define BsTreePut MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreePut)
#define BsTreeInsert MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeInsert)
#define BsTreeDelete MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeDelete)
#define BsTreeGetCount MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeGetCount)
#define BsTreeIteratorLocate MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeIteratorLocate)
#define BsTreeIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeIteratorFirst)
#define BsTreeIteratorLast MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeIteratorLast)
#define BsTreeIteratorNext MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeIteratorNext)
#define BsTreeIteratorPrev MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeIteratorPrev)

void BsTreeInit(BsTree* tree);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeFind(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreePut(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
bool BsTreeInsert(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeDelete(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset BsTreeGetCount(BsTree* tree);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLocate(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key, int8_t* cmp_status);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorFirst(BsTree* tree, BsTreeStackVector* stack);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLast(BsTree* tree, BsTreeStackVector* stack);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorNext(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorPrev(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id);


#ifdef LIBYUC_TEMPLATE_C_INCLUDE
#define BsTreeStackVectorPushTail MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeStackVectorPushTail)
#define BsTreeStackVectorGetTail MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeStackVectorGetTail)
#define BsTreeStackVectorPopTail MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeStackVectorPopTail)

#define BsTreeSetNewChild MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeSetNewChild)
#define BsTreeHitchEntry MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsTreeHitchEntry)
#define RotateLeft MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, RotateLeft)
#define RotateRight MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, RotateRight)
#define BsEntryInit MAKE_NAME(LIBYUC_CONTAINER_BS_TREE_CLASS_NAME, BsEntryInit)


static void BsTreeSetNewChild(BsTree* tree, BsEntry* entry_parent, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id new_entry_id) {
    if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, entry_parent) == entry_id) {
        LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, entry_parent, new_entry_id);
    }
    else {
            assert(LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry_parent) == entry_id);
        LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, entry_parent, new_entry_id);
    }
}
/*
* 栈指向entry的父亲
* new_entry代替entry挂接到其父节点下
* new_entry的左右子节点不变
* entry从树中摘除
* entry的parent、left和right不变
*/
static void BsTreeHitchEntry(BsTree* tree, BsEntry* entry_parent, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id new_entry_id) {
    BsEntry* entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, entry_id);
    BsEntry* new_entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, new_entry_id);
    if (entry_parent != NULL) {
        BsTreeSetNewChild(tree, entry_parent, entry_id, new_entry_id);
    }
    if (tree->root == entry_id) {
        tree->root = new_entry_id;
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, entry);
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, new_entry);
}
/*
* 栈指向sub_root的父亲
* 左旋子树
*/
static LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id RotateLeft(BsTree* tree, BsEntry* sub_root_parent, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id sub_root_id, BsEntry* sub_root) {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id new_sub_root_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, sub_root);
    if (new_sub_root_id == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        return sub_root_id;
    }
    BsEntry* new_sub_root = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, new_sub_root_id);
       
    if (sub_root_parent != NULL) {
        BsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
    }
       
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, sub_root, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, new_sub_root));
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, new_sub_root, sub_root_id);
       
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, new_sub_root);
    return new_sub_root_id;
}
/*
* 栈指向sub_root的父亲
* 右旋子树
*/
static LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id RotateRight(BsTree* tree, BsEntry* sub_root_parent, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id sub_root_id, BsEntry* sub_root) {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id new_sub_root_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, sub_root);
    if (new_sub_root_id == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        return sub_root_id;
    }
    BsEntry* new_sub_root = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, new_sub_root_id);
       
    if (sub_root_parent != NULL) {
        BsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
    }
       
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, sub_root, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, new_sub_root));
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, new_sub_root, sub_root_id);
       
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, new_sub_root);
    return new_sub_root_id;
}
/*
* 初始化节点
*/
static void BsEntryInit(BsTree* tree, BsEntry* entry) {
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, entry, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
    LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, entry, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
}
/*
* 初始化树
*/
void BsTreeInit(BsTree* tree) {
    tree->root = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
}
/*
* 从树中查找节点
* 存在返回查找到的节点对应的对象，不存在返回NULL
*/
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeFind(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key) {
    int8_t status;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id id = BsTreeIteratorLocate(tree, stack, key, &status);
    return status == 0 ? id : LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
}
/*
* 向树中插入节点
* 允许重复key，同一个节点重复插入时返回false
*/
bool BsTreeInsert(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id) {
    BsEntry* entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, entry_id);
    stack->count = 0;
    if (tree->root == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsEntryInit(tree, entry);
        tree->root = entry_id;
        return true;
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = tree->root;
    BsEntry* cur = NULL;
    bool success = true;
    while (cur_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        if (cur_id == entry_id) {
            success = false;
            break;
        }
        BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        LIBYUC_CONTAINER_BS_TREE_Type_Key* cur_key = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(tree, cur);
        LIBYUC_CONTAINER_BS_TREE_Type_Key* entry_key = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(tree, entry);
        if (LIBYUC_CONTAINER_BS_TREE_COMPARER_Less(tree, cur_key, entry_key)) {
            if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur) == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
                LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, cur, entry_id);
                break;
            }
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
        }
        else {
            if (cur_id == entry_id) break;
            if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur) == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
                LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, cur, entry_id);
                break;
            }
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    }
    if (cur) LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    if (cur_id != entry_id) {
        BsEntryInit(tree, entry);
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, entry);
    return success;
}
/*
* 向树中推入节点
* 覆盖重复key，返回被覆盖的entry_id，否则返回InvalidId，如果entry_id已经被插入过了，也会被返回
*/
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreePut(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id) {
    BsEntry* entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, entry_id);
    stack->count = 0;
    if (tree->root == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsEntryInit(tree, entry);
        tree->root = entry_id;
        return LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = tree->root;
    BsEntry* cur = NULL;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id old_id = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id parent_id = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    while (cur_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        LIBYUC_CONTAINER_BS_TREE_Type_Key* cur_key = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(tree, cur);
        LIBYUC_CONTAINER_BS_TREE_Type_Key* entry_key = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(tree, entry);
        if (LIBYUC_CONTAINER_BS_TREE_COMPARER_Less(tree, cur_key, entry_key)) {
            if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur) == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
                BsEntryInit(tree, entry);
                LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, cur, entry_id);
                break;
            }
            parent_id = cur_id;
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
        }
        else if (LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater(tree, cur_key, entry_key)) {
            if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur) == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
                BsEntryInit(tree, entry);
                LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, cur, entry_id);
                break;
            }
            parent_id = cur_id;
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
        }
        else {
            /* 相等的情况 */
            BsTreeStackVectorPopTail(stack);
            old_id = cur_id;
            if (cur_id == entry_id) break;
            BsEntryInit(tree, entry);
            if (parent_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
                BsEntry* parent = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, parent_id);
                BsTreeSetNewChild(tree, parent, cur_id, entry_id);
                LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, parent);
            }
            else {
                tree->root = entry_id;
            }
            *entry = *cur;
            break;
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    }
    if (cur) LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, entry);
    return old_id;
}
/*
* 从树中删除节点
* 返回被删除的节点(或被替换到当前位置的右子树最小节点)，构造所有回溯条件
*/
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeDelete(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id, bool* is_parent_left) {
        assert(entry_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id backtrack_id;
    BsEntry* entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, entry_id);
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id* parent_id = BsTreeStackVectorGetTail(stack);
    BsEntry* parent = NULL;
    if (parent_id != NULL) {
            assert(*parent_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
        parent = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, *parent_id);
    }
    if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, entry) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId && LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        /* 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点 */
        BsTreeStackVectorPushTail(stack, &entry_id);
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id* new_entry_id = BsTreeStackVectorGetTail(stack);
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id min_entry_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry);
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id min_entry_parent_id = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
        BsEntry* min_entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, min_entry_id);
        while (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, min_entry) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            BsTreeStackVectorPushTail(stack, &min_entry_id);
            min_entry_parent_id = min_entry_id;
            min_entry_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, min_entry);
            LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, min_entry);
            min_entry = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, min_entry_id);
        }
        /* 被替换到当前位置的最小节点，保证回溯路径的正确 */
        *new_entry_id = min_entry_id;
        BsEntry* min_entry_parent = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, min_entry_parent_id);
           
        /* 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值 */
        LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, min_entry, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, entry));
           
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id old_right_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, min_entry);
        /* 最小节点可能是待删除节点的右节点 */
        if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry) != min_entry_id) {
            /* 将min_entry从原先的位置摘除，用其右子树代替 */
            LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, min_entry_parent, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, min_entry));
               
            /* 最小节点继承待删除节点的右子树 */
            LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, min_entry, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry));
            if (is_parent_left) *is_parent_left = true;
        }
        else {
            if (is_parent_left) *is_parent_left = false;
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, min_entry_parent);
           
        /* 最后进行挂接 */
        BsTreeHitchEntry(tree, parent, entry_id, min_entry_id);
           
        /* 也可以选择直接交换两个节点的数据，但是开销不定 */
           
        entry_id = min_entry_id;
           
        /* 回溯可能需要的，entry变为原先的min_entry，只是不挂到树上(entry的父节点不指向entry) */
        LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft(tree, entry, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
        LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight(tree, entry, old_right_id);
    }
    else {
        if (is_parent_left) {
            if (parent != NULL) {
                *is_parent_left = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, parent) == entry_id;
                    assert(*is_parent_left || *is_parent_left == false && LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, parent) == entry_id);
            }
            else {
                *is_parent_left = false;
            }
        }
           
        if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry) !=    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            /* 只有右子节点 */
            BsTreeHitchEntry(tree, parent, entry_id, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, entry));
        }
        else if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, entry) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            /* 只有左子节点 */
            BsTreeHitchEntry(tree, parent, entry_id, LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, entry));
        }
        else {
            /* 没有子节点，直接从父节点中摘除此节点 */
            BsTreeHitchEntry(tree, parent, entry_id, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId);
        }
    }
    if (parent) {
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, parent);
    }
    return entry_id;
}
/*
* 获取树的节点数量
*/
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset BsTreeGetCount(BsTree* tree, BsTreeStackVector* stack) {
    size_t count = 0;
    stack->count = 0;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = BsTreeIteratorFirst(tree, stack);
    while (cur_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        count++;
        cur_id = BsTreeIteratorNext(tree, stack, cur_id);
    }
    return count;
}
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLocate(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key, int8_t* cmp_status) {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = tree->root;
    stack->count = 0;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id perv_id = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    while (cur_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        perv_id = cur_id;
        BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        LIBYUC_CONTAINER_BS_TREE_Type_Key* cur_key = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey(tree, cur);
        if (LIBYUC_CONTAINER_BS_TREE_COMPARER_Less(tree, cur_key, key)) {
            *cmp_status = 1;
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
        }
        else if (LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater(tree, cur_key, key)) {
            *cmp_status = -1;
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
        }
        else {
            LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
            *cmp_status = 0;
            return cur_id;
        }
        if (cur_id != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            BsTreeStackVectorPushTail(stack, &perv_id);
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    }
    return perv_id;
}
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorFirst(BsTree* tree, BsTreeStackVector* stack) {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = tree->root;
    if (cur_id == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        return LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    }
    BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    while (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    return cur_id;
}
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLast(BsTree* tree, BsTreeStackVector* stack) {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id = tree->root;
    if (cur_id == LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        return LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
    }
    BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    while (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    return cur_id;
}
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorNext(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id) {
    BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
        cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        while (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            BsTreeStackVectorPushTail(stack, &cur_id);
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
            LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
            cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        return cur_id;
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id* parent_id = BsTreeStackVectorPopTail(stack);
    BsEntry* parent = NULL;
    while (parent_id != NULL) {
        parent = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, *parent_id);
        if (cur_id != LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, parent)) break;
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        cur = parent;
        cur_id = *parent_id;
        parent_id = BsTreeStackVectorPopTail(stack);
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    if (parent) LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, parent);
    if (parent_id) return *parent_id;
    return LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
}
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorPrev(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id) {
    BsEntry* cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
    if (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
        BsTreeStackVectorPushTail(stack, &cur_id);
        cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, cur);
        cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        while (LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur) != LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId) {
            BsTreeStackVectorPushTail(stack, &cur_id);
            cur_id = LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight(tree, cur);
            LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
            cur = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, cur_id);
        }
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        return cur_id;
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id* parent_id = BsTreeStackVectorPopTail(stack);
    BsEntry* parent = NULL;
    while (parent_id != NULL) {
        parent = LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference(tree, *parent_id);
        if (cur_id != LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft(tree, parent)) break;
        LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
        cur = parent;
        cur_id = *parent_id;
        parent_id = BsTreeStackVectorPopTail(stack);
    }
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, cur);
    if (parent) LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference(tree, parent);
    if (parent_id) return *parent_id;
    return LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId;
}

#undef BsTreeStackVectorGetTail
#undef BsTreeStackVectorPushTail
#undef BsTreeStackVectorPopTail

#undef BsTreeSetNewChild
#undef BsTreeHitchEntry
#undef RotateLeft
#undef RotateRight
#undef BsEntryInit

#endif

#undef BsTreeStackVector

#undef BsEntry
#undef BsTree

#undef BsTreeInit
#undef BsTreeFind
#undef BsTreePut
#undef BsTreeInsert
#undef BsTreeDelete
#undef BsTreeGetCount
#undef BsTreeIteratorLocate
#undef BsTreeIteratorFirst
#undef BsTreeIteratorLast
#undef BsTreeIteratorNext
#undef BsTreeIteratorPrev

#undef LIBYUC_CONTAINER_BS_TREE_CLASS_NAME
#undef LIBYUC_CONTAINER_BS_TREE_Type_Key
#undef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey
#undef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft
#undef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight
#undef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft
#undef LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight
#undef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id
#undef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset
#undef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId
#undef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference
#undef LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference
#undef LIBYUC_CONTAINER_BS_TREE_COMPARER_Less
#undef LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater

#ifdef __cplusplus
}
#endif
