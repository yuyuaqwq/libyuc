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
typedef uint64_t Key;
typedef uint64_t Value;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK


// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef struct _BPlusLeafInternalEntry {
    Key key;
    Value value;
} BPlusLeafInternalEntry;

typedef struct _BPlusIndexInternalEntry {       // Internal 
    PageId childId;
    Key key;
} BPlusIndexInternalEntry;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

typedef struct _BPlusLeafListEntry {
    PageId prev;
    PageId next;
} BPlusLeafListEntry;

typedef enum _BPlusEntryType BPlusEntryType;
// 可以优化的点：child和key分别合并放到一起，可以加快查找以及避免内存对齐
typedef struct _BPlusEntry {
    BPlusEntryType type;
    int count;        // child / obj计数
    PageId parentId;
    int leftParentIndex;        // 父节点的左内部索引，child[0]时，leftParentIndex为-1
    union {
        BPlusIndexInternalEntry indexInternalEntry[];
        struct {
            BPlusLeafListEntry leafListEntry;
            BPlusLeafInternalEntry leafInternalEntry[];
        };
    };
} BPlusEntry;

typedef struct _BPlusTree {
    PageId rootId;
    PageId leafListFirst;
    int m;
    size_t keySize;
    CmpFunc cmpFunc;
} BPlusTree;

const PageId kBPlusInvalidPageId = -1;

/*
* 页面管理器相关接口
*/
#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
PageId PageAlloc(BPlusTree* tree);
#else
PageId PageAlloc();
#endif
void PageFree(BPlusTree* tree, PageId id);
struct _BPlusEntry* PageGet(BPlusTree* tree, PageId id);

/*
* 节点操作相关接口
*/
void* GetLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i);
void SetLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i, void* key);
void SwapLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i, int j);
PageId GetIndexInternalChildId(BPlusTree* tree, BPlusEntry* index, int i);
void SetIndexInternalChildId(BPlusTree* tree, BPlusEntry* index, int i, PageId id);
void* GetIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i);
void SetIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i, void* key);
void SwapIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i, int j);

/*
* B+树操作
*/
void BPlusTreeInit(BPlusTree* tree, int m, int keySize, CmpFunc cmpFunc);
bool BPlusTreeInsert(BPlusTree* tree, void* key);
bool BPlusTreeFind(BPlusTree* tree, void* key);
bool BPlusTreeDelete(BPlusTree* tree, void* key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_