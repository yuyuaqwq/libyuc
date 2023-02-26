/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BPLUS_TREE_H_
#define CUTILS_CONTAINER_BPLUS_TREE_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef struct _BPlusEntry* PageId;
typedef size_t PageCount;
typedef struct {
    size_t size;
    uint32_t* ptr;
} Data;
typedef Data Key;
typedef Data Value;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK


// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

typedef struct _BPlusLeafElement {
    Key key;
    Value value;
} BPlusLeafElement;

typedef struct _BPlusIndexElement {       // Internal 
    PageId child_id;
    Key key;
} BPlusIndexElement;

typedef struct _BPlusElement {
    union {
        BPlusIndexElement index;
        BPlusLeafElement leaf;
    };
} BPlusElement;

typedef enum _BPlusEntryType {
    kBPlusEntryIndex,
    kBPlusEntryLeaf,
} BPlusEntryType;

typedef struct _BPlusLeafListEntry {
    PageId prev;
    PageId next;
} BPlusLeafListEntry;


typedef struct _BPlusIndexEntry {
    PageId tail_child_id;       // 末尾孩子id存在这里
    struct _BPlusIndexElement element[];
} BPlusIndexEntry;

typedef struct _BPlusLeafEntry {
    BPlusLeafListEntry list_entry;       // 连接所有叶子节点
    struct _BPlusLeafElement element[];
} BPlusLeafEntry;

#ifndef CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry
typedef struct _BPlusEntry {
    BPlusEntryType type;
    uint32_t element_count;
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
    PageId root_id;
    PageId leaf_list_first;
    uint32_t index_m;
    uint32_t leaf_m;
} BPlusTree;
#else
CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusTree
#endif // !CUTILS_CONTAINER_BPLUS_TREE_DEFINE_BPlusEntry

/*
* B+树游标
*/
typedef struct {
    PageId entry_id;
    int element_idx;
} BPlusElementPos;
typedef enum {
    kBPlusCursorNe,
    kBPlusCursorNext,
    kBPlusCursorEq,
    kBPlusCursorEnd,
} BPlusCursorStatus;
typedef struct _BPlusCursor {
    Array stack;
    int level;
    BPlusCursorStatus leaf_status;
} BPlusCursor;



/*
* 页面管理器相关接口
*/
extern const PageId kPageInvalidId;

BPlusTree* BPlusTreeGet(struct _Tx* tx);
BPlusEntry* BPlusEntryGet(struct _Tx* tx, PageId entry_id);
void BPlusEntryFree(struct _Tx* tx, PageId id);
void BPlusElementSet(struct _Tx* tx, BPlusEntry* entry, int i, BPlusElement* element);
ptrdiff_t BPlusKeyCmp(struct _Tx* tx, const Key* key1, const Key* key2);

/*
* B+树操作
*/
void BPlusTreeInit(struct _Tx* tx, uint32_t index_m, uint32_t leaf_m);
bool BPlusTreeInsert(struct _Tx* tx, BPlusLeafElement* element);
bool BPlusTreeFind(struct _Tx* tx, Key* key);
bool BPlusTreeDelete(struct _Tx* tx, Key* key);

bool BPlusInsertEntry(struct _Tx* tx, BPlusCursor* cursor, BPlusElement* insert_element);
bool BPlusDeleteEntry(struct _Tx* tx, BPlusCursor* cursor);

/*
* B+树游标
*/
BPlusElementPos* BPlusCursorCur(struct _Tx* tx, BPlusCursor* cursor);
BPlusElementPos* BPlusCursorUp(struct _Tx* tx, BPlusCursor* cursor);
BPlusElementPos* BPlusCursorDown(struct _Tx* tx, BPlusCursor* cursor);
BPlusCursorStatus BPlusCursorFirst(struct _Tx* tx, BPlusCursor* cursor, Key* key);
void BPlusCursorRelease(struct _Tx* tx, BPlusCursor* cursor);
BPlusCursorStatus BPlusCursorNext(struct _Tx* tx, BPlusCursor* cursor, Key* key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_