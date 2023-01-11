#include "BPlusTree.h"

static BPlusEntry* BPlusCreateIndexEntry(int m) {
	// child 应当是m个，所以最后多申请一个
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) - sizeof(ListEntry) + (m - 1) * sizeof(BPlusIndexData) + sizeof(struct _BPlusIndexEntry*));
	entry->type = kIndex;
	entry->parent = NULL;
	for (int i = 0; i < m; i++) {
		entry->indexData[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}

static BPlusEntry* BPlusCreateLeafEntry(int m) {
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) + m * sizeof(BPlusLeafData));
	entry->type = kLeaf;
	entry->parent = NULL;
	// ListEntryInit(&entry->leafListEntry);
	for (int i = 0; i < m - 1; i++) {
		// entry->entry[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}

static void BPlusInsertToArray(BPlusEntry* entry, int keyIndex, int key, BPlusEntry* newChild) {
	if (entry->type == kLeaf) {
		BPlusEntry* leaf = entry;
		for (int j = leaf->count - 1; j >= keyIndex; j--) {
			leaf->leafData[j + 1].key = leaf->leafData[j].key;
		}
		leaf->leafData[keyIndex].key = key;
		leaf->count++;
	}
	else {
		BPlusEntry* index = entry;
		for (int j = index->count - 1; j >= keyIndex; j--) {
			index->indexData[j + 1].key = index->indexData[j].key;
			index->indexData[j + 2].child = index->indexData[j + 1].child;
		}
		index->indexData[keyIndex].key = key;
		newChild->parent = index;
		index->indexData[keyIndex + 1].child = newChild;
		index->count++;
	}
}

static bool BPlusInsertToEntry(BPlusTree* tree, BPlusEntry* entry, int key, BPlusEntry* newChild);
static void BPlusSplitEntry(BPlusTree* tree, BPlusEntry* entry, int insertIndex, int key, BPlusEntry* newChild) {
	// 首先选取中间的节点[mid]作为上升节点，根据情况调整上升节点
	BPlusEntry* newEntry = NULL;
	int newCount;
	if (entry->type == kLeaf) {

		newEntry = BPlusCreateLeafEntry(tree->m);

		ListInsertHead(&entry->leafListEntry, &newEntry->leafListEntry);

		// 原地分裂思路：mid将未插入的节点也算上，好计算newCount，4节点就是2(左2右2)，5节点还是2(左2右3)
		int mid = tree->m / 2;
		newCount = entry->count + 1 - mid;		// +1是因为这个时候entry->count并没有把未插入节点也算上
		int i = newCount - 1, j = entry->count - 1;
		bool insert = false;
		for (; i >= 0; i--, j--) {
			if (!insert && j+1 == insertIndex) {		// 这里j+1是因为，循环的时候j并没有把未插入节点也算上
				entry->count++;
				newEntry->leafData[i].key = key;
				j++;		// j不动
				insert = true;
				continue;
			}
			newEntry->leafData[i].key = entry->leafData[j].key;
		}

		entry->count -= newCount;

		if (!insert) {
			// 索引还没有插入，将其插入
			BPlusInsertToArray(entry, insertIndex, key, newChild);
		}
		
		// 从mid拿到上升节点
		key = newEntry->leafData[0].key;
	}
	else {
		newEntry = BPlusCreateIndexEntry(tree->m);

		// 原地分裂思路：mid将未插入的节点和即将上升的节点都算上，好计算newCount，4节点就是2(左1右2)，5节点也是2(左2右2)，少了一个是因为上升的也算上了

		// 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
		//  1 2      3 4
		// / |  |     |  \	
		// 此时新节点缺少了一条链接，我们最终选用左侧的末尾节点作为上升节点，故末尾节点的右子树就可以挂接到新节点上

		int mid = (tree->m-1) / 2;
		newCount = entry->count - mid;		// 这个时候entry->count并没有把未插入节点也算上，但索引的mid会上升，故不变
		int i = newCount - 1, j = entry->count - 1;
		bool insert = false;
		for (; i >= 0; i--, j--) {
			if (!insert && j+1 == insertIndex) {		// 这里j+1是因为，循环的时候j并没有把未插入节点也算上
				entry->count++;
				newEntry->indexData[i].key = key;
				newEntry->indexData[i+1].child = newChild;
				newChild->parent = newEntry;
				j++;		// j不动
				insert = true;
				continue;
			}
			newEntry->indexData[i].key = entry->indexData[j].key;
			newEntry->indexData[i+1].child = entry->indexData[j+1].child;
			newEntry->indexData[i+1].child->parent = newEntry;
		}
		entry->count -= newCount;

		if (!insert) {
			// 索引还没有插入，将其插入
			BPlusInsertToArray(entry, insertIndex, key, newChild);
		}

		// 最后从entry->indexData末尾拿到上升节点，将其摘除
		entry->count--;
		key = entry->indexData[entry->count].key;

		// entry最右的链接交给newEntry
		newEntry->indexData[0].child = entry->indexData[entry->count+1].child;
		newEntry->indexData[0].child->parent = newEntry;
	}
	newEntry->count = newCount;

	// 分裂出的父节点向上传递
	if (entry->parent == NULL) {
		BPlusEntry* newIndex = BPlusCreateIndexEntry(tree->m);
		entry->parent = newIndex;
		newIndex->indexData[0].child = entry;
		BPlusInsertToArray(newIndex, 0, key, newEntry);
		tree->root = newIndex;
	}
	else {
		BPlusInsertToEntry(tree, entry->parent, key, newEntry);
	}
}

static bool BPlusInsertToEntry(BPlusTree* tree, BPlusEntry* entry, int key, BPlusEntry* newChild) {
	if (entry->count == 0) {
		BPlusInsertToArray(entry, 0, key, newChild);
		return true;
	}
	int insertIndex;
	int res;
	if (entry->type == kLeaf) {
		insertIndex = BinarySearch_Range_CustomM(entry->leafData, BPlusLeafData, key, 0, entry->count - 1, &key, MemoryCmpR);
		res = MemoryCmpR(&entry->leafData[insertIndex].key, &key, sizeof(key));
	}
	else {
		insertIndex = BinarySearch_Range_CustomM(entry->indexData, BPlusIndexData, key, 0, entry->count - 1, &key, MemoryCmpR);
		res = MemoryCmpR(&entry->indexData[insertIndex].key, &key, sizeof(key));
	}
	//if (res == 0) {		// 允许插入相同的节点
	//	return false;
	//}
	if (res < 0) {
		insertIndex++;		// 如果所有节点都小于key，那就追加到最尾部
	}

	if (entry->count < tree->m - 1) {
		// 有空余的位置插入
		BPlusInsertToArray(entry, insertIndex, key, newChild);
		return true;
	}

	// 没有多余位置，需要分裂向上合并
	BPlusSplitEntry(tree, entry, insertIndex, key, newChild);
	return true;
}

static BPlusEntry* BPlusTreeFindLeaf(BPlusTree* tree, int key) {
	BPlusEntry* cur = tree->root;
	while (cur && cur->type == kIndex) {
		//       4        8         12
		//    /       |        |       \
		// 3  4      6  7      10        15
		int i = BinarySearch_Range_CustomM(cur->indexData, BPlusIndexData, key, 0, cur->count - 1, &key, MemoryCmpR);
		if (key < cur->indexData[i].key) {
			cur = cur->indexData[i].child;
		}
		else {
			// 相等的key在右边，因为分裂的索引是右叶子的第一个节点上升的
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
	tree->m = m;
}

bool BPlusTreeInsert(BPlusTree* tree, int key) {
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key);
	return BPlusInsertToEntry(tree, leaf, key, NULL);
}

bool BPlusTreeFind(BPlusTree* tree, int key) {
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key);
	int index = BinarySearch_CustomM(leaf->leafData, BPlusLeafData, key, 0, leaf->count - 1, &key, MemoryCmpR);
	if (index == -1) {
		return false;
	}
	return true;
}