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
typedef uintptr_t PageId;
typedef uint32_t Key;
typedef uint32_t Value;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK


// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

typedef enum _BPlusEntryType {
    kBPlusEntryIndex,
    kBPlusEntryLeaf,
} BPlusEntryType;

typedef struct _BPlusLeafListEntry {
    PageId prev;
    PageId next;
} BPlusLeafListEntry;

#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef struct _BPlusLeafBlockInfo {
    Key key;
    Value value;
} BPlusLeafBlockInfo;

typedef struct _BPlusIndexBlockInfo {       // Internal 
    PageId leftChildId;
    Key key;
} BPlusIndexBlockInfo;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

typedef struct _BPlusEntry {
    BPlusEntryType type;
    int count;        // child / obj计数
    PageId parentId;
    union {
        struct _BPlusIndexBlockInfo indexElement[];
        struct {
            BPlusLeafListEntry leafListEntry;
            struct _BPlusLeafBlockInfo leafElement[];
        };
    };
} BPlusEntry;

typedef struct _BPlusTree {
    PageId rootId;
    PageId leafListFirst;
    int m;
    CmpFunc2 cmpFunc;
} BPlusTree;



/*
* 页面管理器相关接口
*/
extern const PageId kPageInvalidId;
PageId PageAlloc(void* tree);
void PageFree(void* tree, PageId id);
void* PageGet(void* tree, PageId id);
void PageDereference(void* tree, PageId id);
void PageMarkDirty(void* tree, PageId id);

/*
* 内部元素操作相关接口
*/
void* GetLeafBlockKey(BPlusTree* tree, BPlusEntry* leaf, int i, size_t* key_size);
void SetLeafBlockKey(BPlusTree* tree, BPlusEntry* leaf, int i, void* key, size_t key_size);
void SwapLeafBlockKey(BPlusTree* tree, BPlusEntry* leaf, int i, int j);
PageId GetIndexLeftChildId(BPlusTree* tree, BPlusEntry* index, int i);
void SetIndexBlockChildId(BPlusTree* tree, BPlusEntry* index, int i, PageId id);
void* GetIndexBlockKey(BPlusTree* tree, BPlusEntry* index, int i, size_t* key_size);
void SetIndexBlockKey(BPlusTree* tree, BPlusEntry* index, int i, void* key, size_t key_size);
void SwapIndexBlockKey(BPlusTree* tree, BPlusEntry* index, int i, int j);

/*
* B+树操作
*/
void BPlusTreeInit(BPlusTree* tree, int m, CmpFunc cmpFunc);
bool BPlusTreeInsert(BPlusTree* tree, void* key, size_t key_size, void* value, size_t value_size);
bool BPlusTreeFind(BPlusTree* tree, void* key, size_t key_size);
bool BPlusTreeDelete(BPlusTree* tree, void* key, size_t key_size);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_