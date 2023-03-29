/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BPLUS_TREE_H_
#define CUTILS_CONTAINER_BPLUS_TREE_H_

#include <CUtils/object.h>
#include <CUtils/container/list.h>
#include <CUtils/container/vector.h>
#include <CUtils/container/static_list.h>
#include <CUtils/container/rb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum _BPlusEntryType {
    kBPlusEntryIndex = 0,
    kBPlusEntryLeaf = 1,
} BPlusEntryType;
typedef enum {
    kBPlusCursorNe,
    kBPlusCursorEq,
    kBPlusCursorNext,
    kBPlusCursorEnd,
} BPlusCursorStatus;





// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点






#define CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(bp_tree_type_name, entry_id_type, key_type, value_type) \
    /*
    * B+树游标
    */ \
    typedef struct { \
        entry_id_type entry_id; \
        int16_t element_id; \
    } bp_tree_type_name##BPlusElementPos; \
    CUTILS_CONTAINER_VECTOR_DECLARATION(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos) \
    typedef struct _##bp_tree_type_name##BPlusCursor { \
        bp_tree_type_name##BPlusCursorStackVector stack; \
        int16_t level; \
        BPlusCursorStatus leaf_status; \
    } bp_tree_type_name##BPlusCursor; \
    \
    \
    CUTILS_CONTAINER_RB_TREE_DECLARATION(bp_tree_type_name##BPlusEntry, int16_t, key_type) \
    CUTILS_CONTAINER_LIST_DECLARATION(bp_tree_type_name##BPlusLeaf, entry_id_type) \
    typedef struct _##bp_tree_type_name##BPlusTree { \
        entry_id_type root_id; \
        bp_tree_type_name##BPlusLeafListHead leaf_list; \
        int16_t index_m; \
        int16_t leaf_m; \
    } bp_tree_type_name##BPlusTree; \
    typedef struct _##bp_tree_type_name##BPlusLeafElement { \
        union { \
            /* BPlusLeafStaticListEntry next; */ \
            int16_t next; \
            bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        }; \
        key_type key; \
        value_type value; \
    } bp_tree_type_name##BPlusLeafElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexElement { \
        union { \
            /* BPlusIndexStaticListEntry next; */ \
            int16_t next; \
            bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        }; \
        entry_id_type child_id; \
        key_type key; \
    } bp_tree_type_name##BPlusIndexElement; \
    typedef struct _##bp_tree_type_name##BPlusElement { \
        union { \
            bp_tree_type_name##BPlusIndexElement index; \
            bp_tree_type_name##BPlusLeafElement leaf; \
        }; \
    } bp_tree_type_name##BPlusElement; \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION(bp_tree_type_name##BPlusIndex, int16_t, bp_tree_type_name##BPlusIndexElement, 1) \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION(bp_tree_type_name##BPlusLeaf, int16_t, bp_tree_type_name##BPlusLeafElement, 1) \
    typedef struct _##bp_tree_type_name##BPlusIndexEntry { \
        entry_id_type tail_child_id;       /* 末尾孩子id存在这里 */ \
        bp_tree_type_name##BPlusIndexStaticList element_space; \
    } bp_tree_type_name##BPlusIndexEntry; \
    typedef struct _##bp_tree_type_name##BPlusLeafEntry { \
        bp_tree_type_name##BPlusLeafListEntry list_entry;       /* 连接所有叶子节点 */ \
        bp_tree_type_name##BPlusLeafStaticList element_space; \
    } bp_tree_type_name##BPlusLeafEntry; \
    typedef struct _##bp_tree_type_name##BPlusEntry { \
        bp_tree_type_name##BPlusEntryRbTree rb_tree; \
        uint16_t type : 1; \
        uint16_t element_count : 15; \
        union { \
            bp_tree_type_name##BPlusIndexEntry index; \
            bp_tree_type_name##BPlusLeafEntry leaf; \
        }; \
    } bp_tree_type_name##BPlusEntry; \
    \
    \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorCur(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorUp(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorDown(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorFirst(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key); \
    void BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key); \
    \



CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(Int, struct _IntBPlusEntry*, int, int)


#define CUTILS_CONTAINER_BPLUS_TREE_DEFINE(bp_tree_type_name, entry_id_type, key_type, value_type, cursor_allocator, bp_referencer, rb_accessor, rb_comparer) \
    static const entry_id_type bp_tree_type_name##BPlusLeafEntryReferencer_InvalidId = bp_referencer##_InvalidId; \
    forceinline bp_tree_type_name##BPlusLeafListEntry* bp_tree_type_name##BPlusLeafEntryReferencer_Reference(bp_tree_type_name##BPlusLeafListHead* head, entry_id_type entry_id) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusEntry* entry = bp_referencer##_Reference(tree, entry_id); \
        return &entry->leaf.list_entry; \
    } \
    forceinline void bp_tree_type_name##BPlusLeafEntryReferencer_Dereference(bp_tree_type_name##BPlusLeafListHead* head, bp_tree_type_name##BPlusLeafListEntry* list_entry) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusLeafEntry* entry = ObjectGetFromField(list_entry,  bp_tree_type_name##BPlusLeafEntry, list_entry); \
        bp_referencer##_Dereference(tree, (bp_tree_type_name##BPlusEntry*)entry); \
    } \
    CUTILS_CONTAINER_LIST_DEFINE(bp_tree_type_name##BPlusLeaf, entry_id_type, bp_tree_type_name##BPlusLeafEntryReferencer) \
    CUTILS_CONTAINER_VECTOR_DEFINE(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos, cursor_allocator) \
    \
    static const int16_t bp_tree_type_name##BPlusEntryRbReferencer_InvalidId = (-1) ; \
    forceinline bp_tree_type_name##BPlusEntryRbEntry* bp_tree_type_name##BPlusEntryRbReferencer_Reference(bp_tree_type_name##BPlusEntryRbTree* tree, int16_t element_id) { \
        if (element_id == bp_tree_type_name##BPlusLeafEntryReferencer_InvalidId) { \
            return NULL; \
        } \
        bp_tree_type_name##BPlusEntry* entry = ObjectGetFromField(tree, bp_tree_type_name##BPlusEntry, rb_tree); \
        if (entry->type == kBPlusEntryIndex) { \
            return &entry->index.element_space.obj_arr[element_id].rb_entry; \
        } \
        else { \
            return &entry->leaf.element_space.obj_arr[element_id].rb_entry; \
        } \
    } \
    forceinline void bp_tree_type_name##BPlusEntryRbReferencer_Dereference(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbEntry* entry) {  } \
    \
    typedef struct { \
        int16_t color : 1; \
        int16_t parent : sizeof(int16_t) * 8 - 1; \
    } bp_tree_type_name##BPlusEntryRbParentColor; \
    forceinline int16_t rb_accessor##_GetParent(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry) { \
        return (((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->parent); \
    } \
    forceinline RbColor rb_accessor##_GetColor(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry) { \
        return (((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->color == -1 ? 1 : 0); \
    } \
    forceinline void rb_accessor##_SetParent(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry, int16_t new_id) { \
        ((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->parent = new_id; \
    } \
    forceinline void rb_accessor##_SetColor(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry, RbColor new_color) { \
        return ((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->color = new_color; \
    } \
    CUTILS_CONTAINER_RB_TREE_DEFINE(bp_tree_type_name##BPlusEntry, int16_t, key_type, bp_tree_type_name##BPlusEntryRbReferencer, rb_accessor, rb_comparer) \
    \
    \
    static entry_id_type bp_tree_type_name##BPlusElementGetChildId(bp_tree_type_name##BPlusTree* tree, const bp_tree_type_name##BPlusEntry* index, int16_t element_id) { \
        if (element_id == -1) { \
            return index->index.tail_child_id; \
        } \
        return index->index.element_space.obj_arr[element_id].child_id; \
    } \
    static void bp_tree_type_name##BPlusElementSetChildId(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* index, int16_t element_id, entry_id_type entry_id) { \
        if (element_id == -1) { \
            index->index.tail_child_id = entry_id; \
            return; \
        } \
        index->index.element_space.obj_arr[element_id].child_id = entry_id; \
    } \
    \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorCur(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level < 0) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[cursor->level];\
    } \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorUp(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level <= 0) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[--cursor->level]; \
    } \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorDown(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level + 1 >= cursor->stack.count) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[++cursor->level]; \
    } \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorFirst(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusCursorStackVectorInit(&cursor->stack, 8, true); \
        cursor->stack.count = 0; \
        cursor->level = -1; \
        return bp_tree_type_name##BPlusCursorNext(tree, cursor, key); \
    } \
    void bp_tree_type_name##BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->stack.capacity != 0) { \
            bp_tree_type_name##BPlusCursorStackVectorRelease(&cursor->stack); \
        } \
    } \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusElementPos cur; \
        bp_tree_type_name##BPlusElementPos* parent = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        if (parent) { \
            bp_tree_type_name##BPlusEntry* parent_entry = bp_referencer##_Reference(tree, parent->entry_id); \
            if (parent_entry->type == kBPlusEntryLeaf) { \
                return kBPlusCursorEnd; \
            } \
            cur.entry_id = bp_tree_type_name##BPlusElementGetChildId(tree, parent_entry, parent->element_id); \
            bp_referencer##_Dereference(tree, parent_entry); \
        } \
        else { \
            cur.entry_id = tree->root_id; \
        } \
        bp_tree_type_name##BPlusEntry* cur_entry = bp_referencer##_Reference(tree, cur.entry_id); \
        int8_t cmp_status = -1; \
        if (cur_entry->element_count > 0) { \
            cur.element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLocate(&cur_entry->rb_tree, key, &cmp_status); \
            if (cmp_status == -1) { /* key小于当前定位元素 */ } \
            else { /* key大于等于当前定位元素 */ \
                if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) { \
                    cur.element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&cur_entry->rb_tree, cur.element_id); \
                } \
            } \
        } \
        else { \
            cur.element_id = -1; \
        } \
        bp_tree_type_name##BPlusCursorStackVectorPushTail(&cursor->stack, &cur); \
        BPlusCursorStatus status = kBPlusCursorNext; \
        if (cur_entry->type == kBPlusEntryLeaf) { \
            if (cmp_status != 0) { \
                status = kBPlusCursorNe; \
            } \
            else { \
                status = kBPlusCursorEq; \
            } \
            cursor->leaf_status = status; \
        } \
        ++cursor->level; \
        bp_referencer##_Dereference(tree, cur_entry); \
        return status; \
    } \
    \
    \






forceinline int CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
    if (((IntBPlusEntry*)tree)->type == kBPlusEntryLeaf) {
        return ((IntBPlusLeafElement*)bs_entry)->key;
    }
    else {
        return ((IntBPlusIndexElement*)bs_entry)->key;
    }
}
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR

#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Equal(TREE, OBJ1, OBJ2) ((OBJ1) == (OBJ2))
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Greater(TREE, OBJ1, OBJ2) ((OBJ1) > (OBJ2))
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Less(TREE, OBJ1, OBJ2) ((OBJ1) < (OBJ2))
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE

CUTILS_CONTAINER_BPLUS_TREE_DEFINE(Int, struct _IntBPlusEntry*, int, int, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_REFERENCER_DEFALUT, CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR, CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_