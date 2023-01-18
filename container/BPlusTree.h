/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BPLUS_TREE_H_
#define CUTILS_CONTAINER_BPLUS_TREE_H_

#include "CUtils/object.h"
#include "CUtils/algorithm/search.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef uint32_t PageId;
typedef uint32_t Key;
#else 
typedef uintptr_t PageId;
typedef uint32_t Key;
#endif // CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK


/*
* 页面管理器相关接口
*/
PageId PageAlloc(int m);
void PageFree(PageId id);
struct _BPlusEntry* PageGet(PageId id);


// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点

typedef struct _BPlusLeafInternalEntry {
    Key key;
} BPlusLeafInternalEntry;

typedef struct _BPlusIndexInternalEntry {        // Internal
    PageId childId;
    Key key;
} BPlusIndexInternalEntry;

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
} BPlusTree;

void BPlusTreeInit(BPlusTree* tree, int m);
bool BPlusTreeInsert(BPlusTree* tree, Key key);
bool BPlusTreeFind(BPlusTree* tree, Key key);
bool BPlusTreeDelete(BPlusTree* tree, Key key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_