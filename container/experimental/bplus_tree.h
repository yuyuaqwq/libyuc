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


#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_1(bp_tree_type_name, entry_id_type) CUTILS_CONTAINER_LIST_DECLARATION(bp_tree_type_name##BPlusLeaf, entry_id_type)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_2(bp_tree_type_name) bp_tree_type_name##BPlusLeafListHead leaf_list;
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_3(bp_tree_type_name) bp_tree_type_name##BPlusLeafListEntry list_entry;       /* 连接所有叶子节点 */

#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_1(bp_tree_type_name, entry_id_type) \
    forceinline bp_tree_type_name##BPlusLeafListEntry* bp_tree_type_name##BPlusLeafEntryReferencer_Reference(bp_tree_type_name##BPlusLeafListHead* head, entry_id_type entry_id) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, entry_id); \
        return &entry->leaf.list_entry; \
    } \
    forceinline void bp_tree_type_name##BPlusLeafEntryReferencer_Dereference(bp_tree_type_name##BPlusLeafListHead* head, bp_tree_type_name##BPlusLeafListEntry* list_entry) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusLeafEntry* entry = ObjectGetFromField(list_entry,  bp_tree_type_name##BPlusLeafEntry, list_entry); \
        entry_referencer##_Dereference(tree, (bp_tree_type_name##BPlusEntry*)entry); \
    } \
    CUTILS_CONTAINER_LIST_DEFINE(bp_tree_type_name##BPlusLeaf, entry_id_type, bp_tree_type_name##BPlusLeafEntryReferencer) \

#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_2(bp_tree_type_name) bp_tree_type_name##BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_3(bp_tree_type_name) bp_tree_type_name##BPlusLeafListDeleteEntry(&tree->leaf_list, right_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_4(bp_tree_type_name) bp_tree_type_name##BPlusLeafListInit(&tree->leaf_list); bp_tree_type_name##BPlusLeafListPutFirst(&tree->leaf_list, tree->root_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL

#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_1(bp_tree_type_name, entry_id_type)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_2(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_3(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_1(bp_tree_type_name, entry_id_type)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_2(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_3(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_4(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK


#define CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(bp_tree_type_name, leaf_link_mode, entry_id_type, key_type, value_type) \
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
    /*
    * 页内红黑树
    */\
    CUTILS_CONTAINER_RB_TREE_DECLARATION(bp_tree_type_name##BPlusEntry, int16_t, key_type) \
    leaf_link_mode##_DECLARATION_1(bp_tree_type_name, entry_id_type) \
    typedef struct _##bp_tree_type_name##BPlusTree { \
        entry_id_type root_id; \
        leaf_link_mode##_DECLARATION_2(bp_tree_type_name) \
        int16_t index_m; \
        int16_t leaf_m; \
    } bp_tree_type_name##BPlusTree; \
    /*
    * B+树
    */ \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_1(bp_tree_type_name##BPlusIndex, int16_t) \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_1(bp_tree_type_name##BPlusLeaf, int16_t) \
    typedef struct _##bp_tree_type_name##BPlusLeafElement { \
        union { \
            bp_tree_type_name##BPlusLeafStaticListEntry next; \
            bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        }; \
        key_type key; \
        value_type value; \
    } bp_tree_type_name##BPlusLeafElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexElement { \
        union { \
            bp_tree_type_name##BPlusIndexStaticListEntry next; \
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
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_2(bp_tree_type_name##BPlusIndex, int16_t, bp_tree_type_name##BPlusIndexElement, 1) \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_2(bp_tree_type_name##BPlusLeaf, int16_t, bp_tree_type_name##BPlusLeafElement, 1) \
    typedef struct _##bp_tree_type_name##BPlusIndexEntry { \
        entry_id_type tail_child_id;       /* 末尾孩子id存在这里 */ \
        bp_tree_type_name##BPlusIndexStaticList element_space; \
    } bp_tree_type_name##BPlusIndexEntry; \
    typedef struct _##bp_tree_type_name##BPlusLeafEntry { \
        leaf_link_mode##_DECLARATION_3(bp_tree_type_name) \
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
    void bp_tree_type_name##BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key); \
    \
    void bp_tree_type_name##BPlusTreeInit(bp_tree_type_name##BPlusTree* tree, uint32_t index_m, uint32_t leaf_m); \
    bool bp_tree_type_name##BPlusTreeFind(bp_tree_type_name##BPlusTree* tree, key_type* key); \
    bool bp_tree_type_name##BPlusTreeInsert(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusLeafElement* element); \
    bool bp_tree_type_name##BPlusTreeDelete(bp_tree_type_name##BPlusTree* tree, key_type* key); \



#define CUTILS_CONTAINER_BPLUS_TREE_DEFINE(bp_tree_type_name, leaf_link_mode, entry_id_type, key_type, value_type, cursor_allocator, entry_allocator, entry_referencer, rb_accessor, rb_comparer) \
    /*
    * B+树游标
    */\
    static const entry_id_type bp_tree_type_name##BPlusLeafEntryReferencer_InvalidId = entry_referencer##_InvalidId; \
    leaf_link_mode##_DEFINE_1(bp_tree_type_name, entry_id_type) \
    CUTILS_CONTAINER_VECTOR_DEFINE(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos, cursor_allocator, CUTILS_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    \
    /*
    * 页内红黑树
    */\
    static const int16_t bp_tree_type_name##BPlusEntryRbReferencer_InvalidId = (-1) ; \
    forceinline bp_tree_type_name##BPlusEntryRbEntry* bp_tree_type_name##BPlusEntryRbReferencer_Reference(bp_tree_type_name##BPlusEntryRbTree* tree, int16_t element_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
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
    /*
    * B+树
    */\
    static const int16_t bp_tree_type_name##BPlusElementStaticListReferencer_InvalidId = (-1) ; \
    forceinline int16_t bp_tree_type_name##BPlusIndexStaticAccessor_GetNext(bp_tree_type_name##BPlusIndexStaticList* list, bp_tree_type_name##BPlusIndexElement* element) { \
        return element->next.next; \
    } \
    forceinline void bp_tree_type_name##BPlusIndexStaticAccessor_SetNext(bp_tree_type_name##BPlusIndexStaticList* list, bp_tree_type_name##BPlusIndexElement* element, int16_t new_next) { \
        element->next.next = new_next; \
    } \
    CUTILS_CONTAINER_STATIC_LIST_DEFINE(bp_tree_type_name##BPlusIndex, int16_t, bp_tree_type_name##BPlusIndexElement, bp_tree_type_name##BPlusElementStaticListReferencer, bp_tree_type_name##BPlusIndexStaticAccessor, 1) \
    forceinline int16_t bp_tree_type_name##BPlusLeafStaticAccessor_GetNext(bp_tree_type_name##BPlusLeafStaticList* list, bp_tree_type_name##BPlusLeafElement* element) { \
        return element->next.next; \
    } \
    forceinline void bp_tree_type_name##BPlusLeafStaticAccessor_SetNext(bp_tree_type_name##BPlusLeafStaticList* list, bp_tree_type_name##BPlusLeafElement* element, int16_t new_next) { \
        element->next.next = new_next; \
    } \
    CUTILS_CONTAINER_STATIC_LIST_DEFINE(bp_tree_type_name##BPlusLeaf, int16_t, bp_tree_type_name##BPlusLeafElement, bp_tree_type_name##BPlusElementStaticListReferencer, bp_tree_type_name##BPlusLeafStaticAccessor, 1) \
    \
    static bp_tree_type_name##BPlusElement* bp_tree_type_name##BPlusElementGet(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, int16_t element_id) { \
          assert(element_id >= 0); \
        if (entry->type == kBPlusEntryLeaf) { \
            return (bp_tree_type_name##BPlusElement*)&entry->leaf.element_space.obj_arr[element_id]; \
        } \
        else { \
            return (bp_tree_type_name##BPlusElement*)&entry->index.element_space.obj_arr[element_id]; \
        } \
    } \
    static void bp_tree_type_name##BPlusElementSet(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, int16_t element_id, bp_tree_type_name##BPlusElement* element) { \
          assert(element_id >= 0); \
        if (entry->type == kBPlusEntryLeaf) { \
            entry->leaf.element_space.obj_arr[element_id].key = element->leaf.key; \
            entry->leaf.element_space.obj_arr[element_id].value = element->leaf.value; \
        } \
        else if (entry->type == kBPlusEntryIndex) { \
            entry->index.element_space.obj_arr[element_id].key = element->index.key; \
            entry->index.element_space.obj_arr[element_id].child_id = element->index.child_id; \
        } \
    } \
    static entry_id_type bp_tree_type_name##BPlusElementGetChildId(bp_tree_type_name##BPlusTree* tree, const bp_tree_type_name##BPlusEntry* index, int16_t element_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            return index->index.tail_child_id; \
        } \
        return index->index.element_space.obj_arr[element_id].child_id; \
    } \
    static void bp_tree_type_name##BPlusElementSetChildId(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* index, int16_t element_id, entry_id_type entry_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            index->index.tail_child_id = entry_id; \
            return; \
        } \
        index->index.element_space.obj_arr[element_id].child_id = entry_id; \
    } \
    static int16_t bp_tree_type_name##BPlusElementCreate(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry) { \
        int16_t element_id; \
        if (entry->type == kBPlusEntryLeaf) { \
            element_id = bp_tree_type_name##BPlusLeafStaticListPop(&entry->leaf.element_space, 0); \
        } \
        else { \
            element_id = bp_tree_type_name##BPlusIndexStaticListPop(&entry->index.element_space, 0); \
        } \
          assert(element_id >= 0); \
        return element_id; \
    } \
    static bp_tree_type_name##BPlusElement* bp_tree_type_name##BPlusElementRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, int16_t element_id) { \
          assert(element_id >= 0); \
        if (entry->type == kBPlusEntryLeaf) { \
            bp_tree_type_name##BPlusLeafStaticListPush(&entry->leaf.element_space, 0, element_id); \
            return (bp_tree_type_name##BPlusElement*)&entry->leaf.element_space.obj_arr[element_id]; \
        } \
        else { \
            bp_tree_type_name##BPlusIndexStaticListPush(&entry->index.element_space, 0, element_id); \
            return (bp_tree_type_name##BPlusElement*)&entry->index.element_space.obj_arr[element_id]; \
        } \
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
            bp_tree_type_name##BPlusEntry* parent_entry = entry_referencer##_Reference(tree, parent->entry_id); \
            if (parent_entry->type == kBPlusEntryLeaf) { \
                return kBPlusCursorEnd; \
            } \
            cur.entry_id = bp_tree_type_name##BPlusElementGetChildId(tree, parent_entry, parent->element_id); \
            entry_referencer##_Dereference(tree, parent_entry); \
        } \
        else { \
            cur.entry_id = tree->root_id; \
        } \
        bp_tree_type_name##BPlusEntry* cur_entry = entry_referencer##_Reference(tree, cur.entry_id); \
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
            cur.element_id = bp_tree_type_name##BPlusEntryRbReferencer_InvalidId; \
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
        entry_referencer##_Dereference(tree, cur_entry); \
        return status; \
    } \
    \
    \
    /*
    * 孩子关系对应如下
    *         3     |    6    |    8
    *      /          /        /       \
    *   1|2         4|5       7        10|11
    * 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
    */ \
    /*
    * 向节点插入元素
    * 不支持处理末尾子节点
    */ \
    static int16_t bp_tree_type_name##BPlusEntryInsertElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, bp_tree_type_name##BPlusElement* insert_element) { \
        int16_t element_id = bp_tree_type_name##BPlusElementCreate(tree, entry); \
          assert(element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
        bp_tree_type_name##BPlusElementSet(tree, entry, element_id, insert_element); \
        int16_t old_element_id = bp_tree_type_name##BPlusEntryRbTreePut(&entry->rb_tree, element_id); \
        if (old_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId && old_element_id != element_id) bp_tree_type_name##BPlusElementRelease(tree, entry, old_element_id); \
        entry->element_count++; \
        return element_id; \
    } \
    /*
    * 从节点中删除指定的元素
    * 不支持处理末尾子节点
    * 返回被删除的元素
    */ \
    static bp_tree_type_name##BPlusElement* bp_tree_type_name##BPlusEntryDeleteElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, int16_t element_id) { \
          assert(element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
        bp_tree_type_name##BPlusEntryRbTreeDelete(&entry->rb_tree, element_id); \
        entry->element_count--; \
        return bp_tree_type_name##BPlusElementRelease(tree, entry, element_id); \
    } \
    entry_id_type bp_tree_type_name##BPlusEntryCreate(bp_tree_type_name##BPlusTree* tree, BPlusEntryType type) { \
        size_t size; \
        if (type == kBPlusEntryIndex) { \
            size = (tree->index_m - 1) * sizeof(bp_tree_type_name##BPlusIndexElement); \
        } \
        else { \
            size = (tree->leaf_m - 1) * sizeof(bp_tree_type_name##BPlusLeafElement); \
        } \
        entry_id_type entry_id = entry_allocator##_CreateBySize(tree, sizeof(bp_tree_type_name##BPlusEntry) + size); \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, entry_id); \
        entry->type = type; \
        entry->element_count = 0; \
        entry_referencer##_Dereference(tree, entry); \
        bp_tree_type_name##BPlusEntryRbTreeInit(&entry->rb_tree); \
        if (type == kBPlusEntryIndex) { \
            bp_tree_type_name##BPlusIndexStaticListInit(&entry->index.element_space, tree->index_m - 1); \
        } \
        else { \
            bp_tree_type_name##BPlusLeafStaticListInit(&entry->leaf.element_space, tree->leaf_m - 1); \
        } \
        return entry_id; \
    } \
    void bp_tree_type_name##BPlusEntryRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry) { \
        entry_allocator##_Release(tree, entry); \
    } \
    \
    /*
    * 分裂节点
    * 返回上升的索引元素
    *
    * 同一个叶节点多次分裂会导致重复的key上升吗？
    * 如果叶节点不存在相同的key，那么是不会的
    * 因为分裂后选择右节点的最左元素的key作为上升的key
    * 这个时候无论怎么插入元素都不会插入到该节点最左元素的左侧(比它小的会被分到左侧节点，因为父元素key等于该元素)，该节点再分裂也就不存在最左元素再次上升的可能了
    */ \
    static bp_tree_type_name##BPlusElement bp_tree_type_name##BPlusEntrySplit(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* left, entry_id_type left_id, bp_tree_type_name##BPlusEntry* parent, int16_t parent_element_id, bp_tree_type_name##BPlusElement* insert_element, int16_t insert_id, entry_id_type* out_right_id) { \
        /* assert(insert_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); */ \
        entry_id_type right_id = bp_tree_type_name##BPlusEntryCreate(tree, left->type); \
        bp_tree_type_name##BPlusEntry* right = entry_referencer##_Reference(tree, right_id); \
        bp_tree_type_name##BPlusElement up_element; \
        int32_t mid; \
        int32_t right_count; \
        if (left->type == kBPlusEntryLeaf) { \
            leaf_link_mode##_DEFINE_2(bp_tree_type_name) \
            /* 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
             就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧 */ \
            mid = tree->leaf_m / 2; \
            right_count = left->element_count + 1 - mid;        /* +1是因为这个时候entry->count并没有把未插入元素也算上 */ \
        } \
        else { \
            /* 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了
             先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧 */ \
            mid = (tree->index_m - 1) / 2; \
            right_count = left->element_count - mid;        /* 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入 */ \
        } \
        int32_t i = right_count - 1; \
        int16_t left_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&left->rb_tree); \
        bool insert = false; \
        for (; i >= 0; i--) { \
            if (!insert && left_elemeng_id == insert_id) { \
                bp_tree_type_name##BPlusEntryInsertElement(tree, right, insert_element); \
                insert = true; \
                continue; \
            } \
              assert(left_elemeng_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
            int16_t next_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&left->rb_tree, left_elemeng_id); \
            bp_tree_type_name##BPlusEntryInsertElement(tree, right, bp_tree_type_name##BPlusEntryDeleteElement(tree, left, left_elemeng_id)); \
            left_elemeng_id = next_elemeng_id; \
        } \
        /* 新元素还没有插入，将其插入 */ \
        if (!insert) { \
            bp_tree_type_name##BPlusEntryInsertElement(tree, left, insert_element); \
        } \
        \
        if (left->type == kBPlusEntryLeaf) { \
            /* 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点 */ \
            up_element = *bp_tree_type_name##BPlusElementGet(tree, right, bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&right->rb_tree)); \
            key_type key = up_element.leaf.key; \
            up_element.index.key = key; \
        } \
        else { \
            /* 假设如下节点需要分裂
                       15            18
                     /                |
               2   4   8   12        ...
               |   |   |   |    \
               1   3   5   10   13   
             ---------------------------
               2   4          8   12
               |   |   \      |   |   \
               1   3   13     5   10   
             ---------------------------
                     4       15              18
                  /          |               |
               2             8   12
               |  \          |   |   \
               1   3         5   10    13
             此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下 */ \
            right->index.tail_child_id = left->index.tail_child_id; \
            \
            /* 最后从左节点末尾拿到上升元素，将其摘除 */ \
            up_element = *bp_tree_type_name##BPlusEntryDeleteElement(tree, left, bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&left->rb_tree)); \
            left->index.tail_child_id = up_element.index.child_id;       /* 3指定为2的右侧子节点 */ \
        } \
        /* 上升的4的子节点为左 */ \
        up_element.index.child_id = left_id; \
        \
        /* 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点) */ \
        bp_tree_type_name##BPlusElementSetChildId(tree, parent, parent_element_id, right_id); \
        \
        *out_right_id = right_id; \
        entry_referencer##_Dereference(tree, right); \
        return up_element; \
    } \
    /*
    * 合并节点
    * 释放右节点，返回时右节点不再有效
    *   2
    * 1   3
    */ \
    static void bp_tree_type_name##BPlusEntryMerge(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* left, entry_id_type left_id, bp_tree_type_name##BPlusEntry* right, entry_id_type right_id, bp_tree_type_name##BPlusEntry* parent, int16_t parent_index) { \
        int16_t right_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&right->rb_tree); \
        for (int32_t i = 0; i < right->element_count; i++) { \
              assert(right_elemeng_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
            bp_tree_type_name##BPlusEntryInsertElement(tree, left, bp_tree_type_name##BPlusElementGet(tree, right, right_elemeng_id)); \
            right_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&right->rb_tree, right_elemeng_id); \
        } \
        if (left->type == kBPlusEntryLeaf) { \
            leaf_link_mode##_DEFINE_3(bp_tree_type_name) \
            /* 是叶子节点，将right并入left中，并删除父元素 */ \
        } \
        else { \
            /* 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素 */ \
            int16_t left_element_id = bp_tree_type_name##BPlusEntryInsertElement(tree, left, bp_tree_type_name##BPlusElementGet(tree, parent, parent_index)); \
            bp_tree_type_name##BPlusElementSetChildId(tree, left, left_element_id, left->index.tail_child_id);       /* left的末尾元素此时为下降的父元素，修改其子节点 */ \
            bp_tree_type_name##BPlusElementSetChildId(tree, left, -1, right->index.tail_child_id); \
        } \
        \
        /* 父元素会被删除，父右兄弟元素的左侧子节点(或尾子节点)原先指向右节点(被删除)，此时继承左节点(合并后剩下的节点) */ \
        bp_tree_type_name##BPlusElementSetChildId(tree, parent, bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&parent->rb_tree, parent_index), left_id); \
        entry_referencer##_Dereference(tree, right); \
        bp_tree_type_name##BPlusEntryRelease(tree, right_id); \
        /* 合并部分完成，删除部分交给调用者 */ \
    } \
    \
    /*
    * 向树插入元素
    */ \
    static bool bp_tree_type_name##BPlusTreeInsertElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, bp_tree_type_name##BPlusElement* insert_element) { \
        bp_tree_type_name##BPlusElementPos* cur_pos = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        bp_tree_type_name##BPlusElementPos* parent_pos = bp_tree_type_name##BPlusCursorUp(tree, cursor); \
        entry_id_type right_id; \
        bp_tree_type_name##BPlusEntry* cur = entry_referencer##_Reference(tree, cur_pos->entry_id); \
        bool success = true, insert_up = false; \
        bp_tree_type_name##BPlusElement up_element; \
        do { \
            if (cursor->leaf_status == kBPlusCursorEq) { \
                bp_tree_type_name##BPlusElementSet(tree, cur, cur_pos->element_id, insert_element); \
                break; \
            } \
            uint32_t m = cur->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m; \
            if (cur->element_count < m - 1) { \
                /* 有空余的位置插入 */ \
                bp_tree_type_name##BPlusEntryInsertElement(tree, cur, insert_element); \
                break; \
            } \
            /* 没有多余位置，需要分裂向上插入，插入的位置需要是第一个小于key的元素，element_id指向第一个大于key的元素 */ \
            if (cur_pos->element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的元素，直接拿末尾元素 */ \
                cur_pos->element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&cur->rb_tree); \
            } else { \
                cur_pos->element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&cur->rb_tree, cur_pos->element_id); \
            } \
            if (!parent_pos) { \
                /* 没有父节点，创建 */ \
                entry_id_type parent_id = bp_tree_type_name##BPlusEntryCreate(tree, kBPlusEntryIndex); \
                bp_tree_type_name##BPlusEntry* parent = entry_referencer##_Reference(tree, parent_id); \
                up_element = bp_tree_type_name##BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, -1, insert_element, cur_pos->element_id, &right_id); \
                bp_tree_type_name##BPlusEntryInsertElement(tree, parent, &up_element); \
                tree->root_id = parent_id; \
                entry_referencer##_Dereference(tree, parent); \
                break; \
            } \
            bp_tree_type_name##BPlusEntry* parent = entry_referencer##_Reference(tree, parent_pos->entry_id); \
            up_element = bp_tree_type_name##BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, parent_pos->element_id, insert_element, cur_pos->element_id, &right_id); \
            entry_referencer##_Dereference(tree, parent); \
            insert_up = true; \
        } while (false); \
        entry_referencer##_Dereference(tree, cur); \
        if (insert_up) { \
            return bp_tree_type_name##BPlusTreeInsertElement(tree, cursor, &up_element); \
        } \
        return success; \
    } \
    \
    /*
    * 删除树中的指定元素
    */ \
    static bool bp_tree_type_name##BPlusTreeDeleteElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        bp_tree_type_name##BPlusElementPos* cur_pos = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        bp_tree_type_name##BPlusElementPos* parent_pos = bp_tree_type_name##BPlusCursorUp(tree, cursor); \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, cur_pos->entry_id); \
        entry_id_type sibling_entry_id = 0; \
        bp_tree_type_name##BPlusEntry* sibling = NULL; \
        bp_tree_type_name##BPlusEntry* parent = NULL; \
        bool success = true, delete_up = false; \
        bp_tree_type_name##BPlusEntryDeleteElement(tree, entry, cur_pos->element_id);      /* 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了 */ \
        do { \
            uint32_t m = entry->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m; \
            if (entry->element_count >= (m - 1) / 2) { \
                break; \
            } \
            if (!parent_pos) { \
                /* 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束 */ \
                if (entry->type == kBPlusEntryIndex && entry->element_count == 0) { \
                    entry_id_type childId = entry->index.tail_child_id; \
                    tree->root_id = childId; \
                    entry_referencer##_Dereference(tree, entry); \
                    bp_tree_type_name##BPlusEntryRelease(tree, cur_pos->entry_id); \
                    return true; \
                } \
                else { \
                    break; \
                } \
            } \
            parent = entry_referencer##_Reference(tree, parent_pos->entry_id); \
            bool left_sibling = true; \
            int16_t common_parent_element_id = parent_pos->element_id;     /* 始终是指向左侧元素 */ \
            int16_t sibling_element_id; \
            if (common_parent_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的父元素，当前节点是末尾节点，是左兄弟 */ \
                sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&parent->rb_tree); \
            } \
            else { \
                sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&parent->rb_tree, common_parent_element_id); \
                if (sibling_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) {     /* 当前元素已经是最小的元素 */ \
                    left_sibling = false; \
                    sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&parent->rb_tree, common_parent_element_id); \
                    if (sibling_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 直接找既没有左兄弟也没有右兄弟，那就是末尾节点是右兄弟 */ \
                        sibling_entry_id = parent->index.tail_child_id; \
                    } \
                } \
            } \
            if (sibling_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
                sibling_entry_id = parent->index.element_space.obj_arr[sibling_element_id].child_id; \
            } \
            if (left_sibling) { \
                common_parent_element_id = sibling_element_id; \
                parent_pos->element_id = sibling_element_id;     /* 更新一下，给父节点删除使用 */ \
            } \
              assert(common_parent_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
              assert(sibling_entry_id != entry_referencer##_InvalidId); \
            sibling = entry_referencer##_Reference(tree, sibling_entry_id); \
            if (sibling->element_count > (m - 1) * 10 / 25) /* 40% */ { \
                /* 向兄弟借节点 */ \
                if (entry->type == kBPlusEntryLeaf) { \
                    /* 叶子节点处理较简单，可以直接移动 */ \
                    if (left_sibling) { \
                        /* 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key */ \
                        int16_t last = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&sibling->rb_tree); \
                          assert(last != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, last); \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, element); \
                        parent->index.element_space.obj_arr[common_parent_element_id].key = element->leaf.key;       /* 更新索引 */ \
                    } \
                    else { \
                        /* 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素 */ \
                        int16_t first = bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree); \
                        int16_t new_first = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&sibling->rb_tree, first); \
                          assert(first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                          assert(new_first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, first); \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, element); \
                        /* 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的 */ \
                        parent->index.element_space.obj_arr[common_parent_element_id].key = sibling->leaf.element_space.obj_arr[new_first].key;       /* 更新索引 */ \
                    } \
                } \
                else { \
                    /* 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换) */ \
                    if (left_sibling) { \
                        /* 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧 */ \
                        int16_t last = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&sibling->rb_tree); \
                          assert(last != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* left_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, last); \
                        ObjectSwap(entry_id_type, left_element->index.child_id, sibling->index.tail_child_id);        /* 要拿的是末尾的子节点，处理一下 */ \
                        bp_tree_type_name##BPlusElement* par_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, parent, common_parent_element_id); \
                        par_element->index.child_id = left_element->index.child_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, par_element); \
                        left_element->index.child_id = sibling_entry_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, parent, left_element); \
                    } \
                    else { \
                        /* 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧 */ \
                        int16_t first = bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree); \
                          assert(first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* right_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, first); \
                        bp_tree_type_name##BPlusElement* par_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, parent, common_parent_element_id); \
                        par_element->index.child_id = right_element->index.child_id; \
                        ObjectSwap(entry_id_type, par_element->index.child_id, entry->index.tail_child_id);        /* 要插入的是末尾的子节点，处理一下 */ \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, par_element); \
                        right_element->index.child_id = cur_pos->entry_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, parent, right_element); \
                    } \
                } \
                break; \
            } \
            \
            /* 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素) */ \
            if (left_sibling) { \
                bp_tree_type_name##BPlusEntryMerge(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, common_parent_element_id); \
                entry = NULL; \
            } \
            else { \
                bp_tree_type_name##BPlusEntryMerge(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, common_parent_element_id); \
                sibling = NULL; \
            } \
            delete_up = true; \
        } while (false); \
        if (parent) { entry_referencer##_Dereference(tree, parent); } \
        if (sibling) { entry_referencer##_Dereference(tree, sibling); } \
        if (entry) { entry_referencer##_Dereference(tree, entry); } \
        \
        if (delete_up) { \
            return bp_tree_type_name##BPlusTreeDeleteElement(tree, cursor);     /* 尾递归 */ \
        } \
        return success; \
    } \
    \
    /*
    * 初始化B+树
    */ \
    void bp_tree_type_name##BPlusTreeInit(bp_tree_type_name##BPlusTree* tree, uint32_t index_m, uint32_t leaf_m) { \
        if (index_m < 3) { \
            index_m = 3;      /* 最少3阶，否则索引节点分裂会出现一侧没有节点的情况 */ \
        } \
        if (leaf_m < 3) { \
            leaf_m = 3;     /* 可以2阶，但是删除有地方需要加强判断 */ \
        } \
        tree->index_m = index_m; \
        tree->leaf_m = leaf_m; \
        tree->root_id = bp_tree_type_name##BPlusEntryCreate(tree, kBPlusEntryLeaf); \
        leaf_link_mode##_DEFINE_4(bp_tree_type_name) \
    } \
    /*
    * 从B+树中查找指定key
    */ \
    bool bp_tree_type_name##BPlusTreeFind(bp_tree_type_name##BPlusTree* tree, key_type* key) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, key); \
        } \
        return status == kBPlusCursorEq; \
    } \
    /*
    * 从B+树中插入指定key
    */ \
    bool bp_tree_type_name##BPlusTreeInsert(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusLeafElement* element) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, &element->key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, &element->key); \
        } \
        bool success = bp_tree_type_name##BPlusTreeInsertElement(tree, &cursor, (bp_tree_type_name##BPlusElement*)element); \
        bp_tree_type_name##BPlusCursorRelease(tree, &cursor); \
        return success; \
    } \
    /*
    * 从B+树中删除指定key
    */ \
    bool bp_tree_type_name##BPlusTreeDelete(bp_tree_type_name##BPlusTree* tree, key_type* key) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, key); \
        } \
        if (status == kBPlusCursorNe) { \
            return false; \
        } \
        bool success = bp_tree_type_name##BPlusTreeDeleteElement(tree, &cursor); \
        return success; \
    } \


//CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(Int, struct _IntBPlusEntry*, int, int)
//forceinline int* CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
//    if (((IntBPlusEntry*)tree)->type == kBPlusEntryLeaf) {
//        return &((IntBPlusLeafElement*)bs_entry)->key;
//    }
//    else {
//        return &((IntBPlusIndexElement*)bs_entry)->key;
//    }
//}
//#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR
//CUTILS_CONTAINER_BPLUS_TREE_DEFINE(Int, struct _IntBPlusEntry*, int, int, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_REFERENCER_DEFALUT, CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR, CUTILS_OBJECT_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_