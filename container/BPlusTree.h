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


typedef struct _BPlusLeafData {
	int key;		// void* obj;
} BPlusLeafData;

typedef struct _BPlusIndexData {		// Internal
	struct _BPlusEntry* child;
	int key;
} BPlusIndexData;

typedef enum {
	kIndex,
	kLeaf,
} BPlusEntryType;

// 可以优化的点：child和key分别合并放到一起，可以加快查找以及避免内存对齐
typedef struct _BPlusEntry {
	BPlusEntryType type;
	int count;		// child / obj计数
	struct _BPlusEntry* parent;
	union {
		BPlusIndexData indexData[];
		struct {
			ListEntry leafListEntry;
			BPlusLeafData leafData[];
		};
	};
} BPlusEntry;

typedef struct _BPlusTree {
	BPlusEntry* root;
	ListHead listHead;
	int m;
} BPlusTree;


static BPlusEntry* BPlusCreateIndexEntry(int m) {
	// child 应当是m个，所以最后多申请一个
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) - sizeof(ListEntry) + (m - 1) * sizeof(BPlusIndexData) + sizeof(struct _BPlusIndexEntry*));
	entry->type = kIndex;
	for (int i = 0; i < m; i++) {
		entry->indexData[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}

static BPlusEntry* BPlusCreateLeafEntry(int m) {
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) + m * sizeof(BPlusLeafData));
	entry->type = kLeaf;
	// ListEntryInit(&entry->leafListEntry);
	for (int i = 0; i < m - 1; i++) {
		// entry->entry[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}


static void BPlusInsertKeyToLeaf(BPlusEntry* leaf, int keyIndex, int key) {
	for (int j = leaf->count - 1; j >= keyIndex; j--) {
		leaf->leafData[j + 1].key = leaf->leafData[j].key;
	}
	leaf->leafData[keyIndex].key = key;
}

static void BPlusLeafSplit(BPlusTree* tree, BPlusEntry* leaf, int insertIndex, int key){
	int minT = tree->m / 2;
	int maxT = tree->m - minT;

	if (insertIndex < maxT) {

	}

	BPlusEntry* newLeaf = BPlusCreateLeafEntry(tree->m);

	
	// 拷贝后半部分内部节点到条目
	for (int i = 0; i < t; i++) {
		newLeaf->leafData[i].key = leaf->leafData[t + i].key;
	}
	leaf->count -= t;
	newLeaf->count = t;
	
}

static BPlusEntry* BPlusTreeFindLeaf(BPlusTree* tree, int key) {
	BPlusEntry* cur = tree->root;
	while (cur && cur->type == kIndex) {
		//       4        8         12
		//    /       |        |       \
		// 3  4      6  7      10        15
		int i = BinarySearch_Range_CustomM(cur->indexData, BPlusIndexData, key, 0, cur->count - 1, &key, MemoryCmpR);
		if (cur->indexData[i].key <= key) {
			// 相等的key也放左边
			cur = cur->indexData[i].child;
		}
		else {
			cur = cur->indexData[i + 1].child;
		}
	}
	return cur;
}


void BPlusTreeInit(BPlusTree* tree, int m) {
	if (m < 2) {
		m = 2;
	}
	tree->root = BPlusCreateLeafEntry(m);
	ListHeadInit(&tree->listHead);
	ListInsertHead(&tree->listHead, &tree->root->leafListEntry);
	tree->root->parent = NULL;
	tree->m = m;
}



bool BPlusTreeInsert(BPlusTree* tree, int key) {
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key);

	if (leaf->count == 0) {
		BPlusInsertKeyToLeaf(leaf, 0, key);
		leaf->count++;
		return true;
	}

	int insertIndex = BinarySearch_Range_CustomM(leaf->leafData, BPlusLeafData, key, 0, leaf->count - 1, &key, MemoryCmpR);
	int res = MemoryCmpR(&leaf->leafData[insertIndex].key, &key, sizeof(key));
	if (res == 0) {
		return false;
	}
	if (leaf->count > 0 && res < 0) {
		insertIndex++;		// 如果所有节点都小于key，那就追加到最尾部
	}

	if (leaf->count < tree->m - 1) {
		// 有空余的位置插入
		BPlusInsertKeyToLeaf(leaf, insertIndex, key);
		leaf->count++;
		return true;
	}

	// 没有多余位置，需要分裂向上合并
	BPlusLeafSplit(tree, leaf, insertIndex, key);

	return true;
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_