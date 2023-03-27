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




#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef struct _BPlusEntry* BPlusEntryId;
typedef size_t PageCount;
typedef struct {
    uint32_t data;
} Data;
typedef Data Key;
typedef Data Value;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK


CUTILS_CONTAINER_RB_TREE_DECLARATION(BPlusEntry, int16_t, Key)
CUTILS_CONTAINER_LIST_DECLARATION(BPlusLeaf, BPlusEntryId)



// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

typedef struct _BPlusLeafElement {
    union {
        // BPlusLeafStaticListEntry next;
        int16_t next;
        BPlusEntryRbEntry rb_entry;
    };
    Key key;
    Value value;
} BPlusLeafElement;

typedef struct _BPlusIndexElement {       // Internal
    union {
        // BPlusIndexStaticListEntry next;
        int16_t next;
        BPlusEntryRbEntry rb_entry;
    };
    BPlusEntryId child_id;
    Key key;
} BPlusIndexElement;

typedef struct _BPlusElement {
    union {
        BPlusIndexElement index;
        BPlusLeafElement leaf;
    };
} BPlusElement;

CUTILS_CONTAINER_STATIC_LIST_DECLARATION(BPlusIndex, int16_t, BPlusIndexElement, 1)
CUTILS_CONTAINER_STATIC_LIST_DECLARATION(BPlusLeaf, int16_t, BPlusLeafElement, 1)

typedef struct _BPlusIndexEntry {
    BPlusEntryId tail_child_id;       // 末尾孩子id存在这里
    BPlusIndexStaticList element_space;
} BPlusIndexEntry;

typedef struct _BPlusLeafEntry {
    BPlusLeafListEntry list_entry;       // 连接所有叶子节点
    BPlusLeafStaticList element_space;
} BPlusLeafEntry;

#ifndef CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry
typedef struct _BPlusEntry {
    BPlusEntryRbTree rb_tree;
    uint16_t type : 1;
    uint16_t element_count : 15;
    union {
        BPlusIndexEntry index;
        BPlusLeafEntry leaf;
    };
} BPlusEntry;
#else
CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry
#endif // !CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry


#ifndef CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusTree
typedef struct _BPlusTree {
    BPlusEntryId root_id;
    BPlusLeafListHead leaf_list;
    int16_t index_m;
    int16_t leaf_m;
} BPlusTree;
#else
CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusTree
#endif // !CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry




/*
* B+树游标
*/
typedef struct {
    BPlusEntryId entry_id;
    int16_t element_idx;
} BPlusElementPos;

CUTILS_CONTAINER_VECTOR_DECLARATION(BPlusCursorStack, BPlusElementPos)

typedef struct _BPlusCursor {
    BPlusCursorStackVector stack;
    BPlusElementPos pos[8];
    int level;
    BPlusCursorStatus leaf_status;
} BPlusCursor;



/*
* 实现接口
*/
extern const BPlusEntryId kBPlusEntryInvalidId;


BPlusEntry* BPlusEntryReference(BPlusTree* tree, BPlusEntryId id);
void BPlusEntryDereference(BPlusTree* tree, BPlusEntry* entry);
void BPlusElementSet(BPlusTree* tree, BPlusEntry* entry, int i, BPlusElement* element);
ptrdiff_t BPlusKeyCmp(BPlusTree* tree, const Key* key1, const Key* key2);

/*
* B+树操作
*/
void BPlusTreeInit(BPlusTree* tree, uint32_t index_m, uint32_t leaf_m);
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element);
bool BPlusTreeFind(BPlusTree* tree, Key* key);
bool BPlusTreeDelete(BPlusTree* tree, Key* key);

bool BPlusInsertEntry(BPlusTree* tree, BPlusCursor* cursor, BPlusElement* insert_element);
bool BPlusDeleteEntry(BPlusTree* tree, BPlusCursor* cursor);

/*
* B+树游标
*/
BPlusElementPos* BPlusCursorCur(BPlusTree* tree, BPlusCursor* cursor);
BPlusElementPos* BPlusCursorUp(BPlusTree* tree, BPlusCursor* cursor);
BPlusElementPos* BPlusCursorDown(BPlusTree* tree, BPlusCursor* cursor);
BPlusCursorStatus BPlusCursorFirst(BPlusTree* tree, BPlusCursor* cursor, Key* key);
void BPlusCursorRelease(BPlusTree* tree, BPlusCursor* cursor);
BPlusCursorStatus BPlusCursorNext(BPlusTree* tree, BPlusCursor* cursor, Key* key);







#define CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(bp_tree_type_name, entry_id_type, key_type, value_type) \
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
    typedef struct _BPlusIndexElement { \
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
    /*
    * B+树游标
    */ \
    typedef struct { \
        entry_id_type entry_id; \
        int16_t element_idx; \
    } bp_tree_type_name##BPlusElementPos; \
    CUTILS_CONTAINER_VECTOR_DECLARATION(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos) \
    typedef struct _##bp_tree_type_name##BPlusCursor { \
        bp_tree_type_name##BPlusCursorStackVector stack; \
        int16_t level; \
        BPlusCursorStatus leaf_status; \
    } bp_tree_type_name##BPlusCursor; \


#define CUTILS_CONTAINER_BPLUS_TREE_DEFINE(bp_tree_type_name, entry_id_type, key_type, value_type, referencer, comparer) \
    CUTILS_CONTAINER_LIST_DEFINE(bp_tree_type_name##BPlusLeaf, entry_id_type, referencer) \
    CUTILS_CONTAINER_VECTOR_DEFINE(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos, CUTILS_OBJECT_ALLOCATOR_DEFALUT) \
    CUTILS_CONTAINER_RB_TREE_DEFINE(BPlusEntry, int16_t, key_type, CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER, CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR, CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE) \
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
            bp_tree_type_name##BPlusEntry* parent_entry = bp_tree_type_name##BPlusEntryReference(tree, parent->entry_id); \
            if (parent_entry->type == kBPlusEntryLeaf) { \
                return kBPlusCursorEnd; \
            } \
            cur.entry_id = bp_tree_type_name##BPlusElementGetChildId(tree, parent_entry, parent->element_idx); \
            bp_tree_type_name##BPlusEntryDereference(tree, parent_entry); \
        } \
        else { \
            cur.entry_id = tree->root_id; \
        } \
        bp_tree_type_name##BPlusEntry* cur_entry = bp_tree_type_name##BPlusEntryReference(tree, cur.entry_id); \
        int8_t cmp_status = -1; \
        if (cur_entry->element_count > 0) { \
            cur.element_idx = bp_tree_type_name##BPlusRbTreeIteratorLocate(&cur_entry->rb_tree, key, &cmp_status); \
            if (cmp_status == -1) { /* key小于当前定位元素 */ } \
            else { /* key大于等于当前定位元素 */ \
                if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) { \
                    cur.element_idx = bp_tree_type_name##BPlusRbTreeIteratorNext(&cur_entry->rb_tree, cur.element_idx); \
                } \
            } \
        } \
        else { \
            cur.element_idx = -1; \
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
        bp_tree_type_name##BPlusEntryDereference(tree, cur_entry); \
        return status; \
    } \


CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(Int, struct _IntBPlusEntry*, int, int)
CUTILS_CONTAINER_BPLUS_TREE_DEFINE(Int, struct _IntBPlusEntry*, int, int, CUTILS_OBJECT_REFERENCER_DEFALUT, )


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_