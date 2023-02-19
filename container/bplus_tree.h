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

typedef struct _BPlusEntry {
    BPlusEntryType type;
    uint32_t element_count;        // element计数
    union {
        struct {
            PageId tail_child_id;       // 末尾孩子id存在这里
            struct _BPlusIndexElement index_element[];
        };
        struct {
            BPlusLeafListEntry leaf_list_entry;       // 连接所有叶子节点
            struct _BPlusLeafElement leaf_element[];
        };
    };
} BPlusEntry;

typedef struct _BPlusTree {
    PageId rootId;
    PageId leaf_list_first;
    uint32_t index_m;
    uint32_t leaf_m;
    CmpFunc2 cmpFunc;
} BPlusTree;



/*
* 页面管理器相关接口
*/
extern const PageId kPageInvalidId;
PageId PageAlloc(void* bucket_, bool put_cache, size_t count);
void PageFree(void* bucket_, PageId pgid, size_t count);
void* PageGet(void* bucket_, PageId pgid, bool modify);
void PageDereference(void* bucket_, PageId pgid);


/*
* 元素相关接口
*/
void SetElement(BPlusTree* tree, BPlusEntry* entry, int i, BPlusElement* element);
int CmpEntryElement(BPlusTree* tree, BPlusEntry* entry, int i, Key* key);

/*
* B+树操作
*/
void BPlusTreeInit(BPlusTree* tree, uint32_t index_m, uint32_t leaf_m, CmpFunc2 cmpFunc);
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element);
bool BPlusTreeFind(BPlusTree* tree, Key* key);
bool BPlusTreeDelete(BPlusTree* tree, Key* key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_