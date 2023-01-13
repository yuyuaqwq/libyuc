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
#include "CUtils/container/list.h"

#ifdef __cplusplus
extern "C" {
#endif


// 阶(m)，4阶B树可以有4个子节点，3个内部节点，m = t * 2
// 度(t)，即除根节点外，每个节点最少有t个内部节点


typedef struct _BPlusLeafInternalEntry {
	int key;		// void* obj;
} BPlusLeafInternalEntry;

typedef struct _BPlusIndexInternalEntry {		// Internal
	struct _BPlusEntry* child;
	int key;
} BPlusIndexInternalEntry;


typedef enum _BPlusEntryType BPlusEntryType;
// 可以优化的点：child和key分别合并放到一起，可以加快查找以及避免内存对齐
typedef struct _BPlusEntry {
	BPlusEntryType type;
	int count;		// child / obj计数
	struct _BPlusEntry* parent;
	union {
		BPlusIndexInternalEntry indexInternalEntry[];
		struct {
			ListEntry leafListEntry;
			BPlusLeafInternalEntry leafInternalEntry[];
		};
	};
} BPlusEntry;

typedef struct _BPlusTree {
	BPlusEntry* root;
	ListHead listHead;
	int m;
} BPlusTree;



void BPlusTreeInit(BPlusTree* tree, int m);
bool BPlusTreeInsert(BPlusTree* tree, int key);
bool BPlusTreeFind(BPlusTree* tree, int key);
bool BPlusTreeDelete(BPlusTree* tree, int key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_