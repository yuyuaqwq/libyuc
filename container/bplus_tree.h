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
    kBPlusCursorNext,
    kBPlusCursorEq,
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


CUTILS_CONTAINER_RB_TREE_DECLARATION(BPlus, int16_t, Key)
CUTILS_CONTAINER_LIST_DECLARATION(BPlusLeaf, BPlusEntryId)



// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

typedef struct _BPlusLeafElement {
    union {
        // BPlusLeafStaticListEntry next;
        int16_t next;
        BPlusRbEntry rb_entry;
    };
    Key key;
    Value value;
} BPlusLeafElement;

typedef struct _BPlusIndexElement {       // Internal
    union {
        // BPlusIndexStaticListEntry next;
        int16_t next;
        BPlusRbEntry rb_entry;
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
    BPlusRbTree rb_tree;
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
    CUTILS_CONTAINER_LIST_DECLARATION(bp_tree_type_name##BPlusLeaf, entry_id_type) \
    typedef struct _##bp_tree_type_name##BPlusTree { \
        entry_id_type root_id; \
        entry_id_type leaf_list_first; \
        int16_t index_m; \
        int16_t leaf_m; \
    } bp_tree_type_name##BPlusTree; \
    typedef struct _##bp_tree_type_name##BPlusLeafElement { \
        key_type key; \
        value_type value; \
    } bp_tree_type_name##BPlusLeafElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexElement {       /* Internal */ \
        entry_id_type child_id; \
        key_type key; \
    } bp_tree_type_name##BPlusIndexElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexEntry { \
        entry_id_type tail_child_id;       /* 末尾孩子id */ \
        bp_tree_type_name##BPlusIndexElement element[]; \
    } bp_tree_type_name##BPlusIndexEntry; \
    typedef struct _##bp_tree_type_name##BPlusLeafEntry { \
         bp_tree_type_name##BPlusLeafListEntry list_entry;       /* 连接所有叶子节点 */ \
         bp_tree_type_name##BPlusLeafElement element[]; \
    } bp_tree_type_name##BPlusLeafEntry; \
    typedef struct _##bp_tree_type_name##BPlusEntry { \
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
    \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorCur(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level < 0) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[cursor->level]; \
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
    BPlusCursorStatus BPlusCursorFirst(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusCursorStackVectorInit(&cursor->stack, 8, true); \
        cursor->stack.count = 0; \
        cursor->level = -1; \
        return bp_tree_type_name##BPlusCursorNext(tree, cursor, key); \
    } \
    void bp_tree_type_name##BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->stack.capacity != 0) { \
            BPlusCursorStackVectorRelease(&cursor->stack); \
        } \
    } \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusElementPos cur; \
        bp_tree_type_name##BPlusElementPos* parent = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        if (parent) { \
            bp_tree_type_name##BPlusEntry* parent_entry = BPlusEntryGet(tree, parent->entry_id); \
            if (parent_entry->type == kBPlusEntryLeaf) { \
                return kBPlusCursorEnd; \
            } \
            cur.entry_id = bp_tree_type_name##BPlusElementGetChildId(tree, parent_entry, parent->element_idx); \
            BPlusEntryDereference(tree, parent_entry); \
        } \
        else { \
            cur.entry_id = tree->root_id; \
        } \
        bp_tree_type_name##BPlusEntry* cur_entry = bp_tree_type_name##BPlusEntryGet(tree, cur.entry_id); \
        int16_t res; \
        if (cur_entry->element_count > 0) { \
            cur.element_idx = bp_tree_type_name##BPlusBinarySearch_Range(tree, cur_entry, 0, cur_entry->element_count - 1, key); \
            res = BPlusElementCmp(tree, cur_entry, cur.element_idx, key); \
            if (cur_entry->type == kBPlusEntryIndex) { \
                if (res == 0) { \
                    /* 索引节点，查找key相等，取右兄弟元素 */ \
                    ++cur.element_idx; \
                } \
            } \
            if (res < 0) { \
                /* 查找key较大，取右兄弟元素/向右找 */ \
                ++cur.element_idx; \
            } \
            else {} /* 查找key较小，向左找 */ \
        } \
        else { \
            res = -1; \
            cur.element_idx = 0; \
        } \
        bp_tree_type_name##BPlusCursorStackVectorPushTail(&cursor->stack, &cur); \
        BPlusCursorStatus status = kBPlusCursorNext; \
        if (cur_entry->type == kBPlusEntryLeaf) { \
            if (res != 0) { \
                status = kBPlusCursorNe; \
            } \
            else { \
                status = kBPlusCursorEq; \
            } \
            cursor->leaf_status = status; \
        } \
        ++cursor->level; \
        BPlusEntryDereference(tree, cur_entry); \
        return status; \
    } \
    /*
    * 初始化B+树
    */ \
    void bp_tree_type_name##BPlusTreeInit(bp_tree_type_name##BPlusTree* tree, int16_t index_m, int16_t leaf_m) { \
        if (index_m < 3) { \
            index_m = 3;      /* 最少3阶，否则索引节点分裂会出现一侧没有节点的情况 */ \
        } \
        if (leaf_m < 3) { \
            leaf_m = 3;     /* 可以2阶，但是删除有地方需要加强判断 */ \
        } \
        tree->index_m = index_m; \
        tree->leaf_m = leaf_m; \
        tree->root_id = bp_tree_type_name##BPlusEntryCreate(tree, kBPlusEntryLeaf); \
        tree->leaf_list_first = tree->root_id; \
        bp_tree_type_name##BPlusLeafListEntryInit(tree, tree->leaf_list_first); \
    } \


//CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(Int, struct _IntBPlusEntry*, int, int)
//CUTILS_CONTAINER_BPLUS_TREE_DEFINE(Int, struct _IntBPlusEntry*, int, int, CUTILS_OBJECT_REFERENCER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_