#include "BPlusTree.h"
#include "CUtils/container/array.h"

typedef enum _BPlusEntryType {
	kIndex,
	kLeaf,
} BPlusEntryType;

static BPlusEntry* BPlusCreateIndexEntry(int m) {
	// child 应当是m个，所以最后多申请一个
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) - sizeof(ListEntry) + (m - 1) * sizeof(BPlusIndexInternalEntry) + sizeof(struct _BPlusIndexEntry*));
	entry->type = kIndex;
	entry->parent = NULL;
	for (int i = 0; i < m; i++) {
		entry->indexInternalEntry[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}

static BPlusEntry* BPlusCreateLeafEntry(int m) {
	BPlusEntry* entry = (BPlusEntry*)MemoryAlloc(sizeof(BPlusEntry) + m * sizeof(BPlusLeafInternalEntry));
	entry->type = kLeaf;
	entry->parent = NULL;
	// ListEntryInit(&entry->leafListEntry);
	for (int i = 0; i < m - 1; i++) {
		// entry->entry[i].child = NULL;
	}
	entry->count = 0;
	return entry;
}

/*
* 没有明确标准的索引是key索引
* 
* 通常情况下，父节点内部节点是左父索引，子节点是右子节点
* 即默认是key[0] - child[1]、key[1] - child[2]...
*         3     |    6    |    8
*      /        \         \        \
*   1|2         4|5        7        10|11
* 如上面画的示例，默认是3对应4|5，6对应7、8对应10|11
*/

/*
* 获取左兄弟节点，没有则返回NULL
*/
static BPlusEntry* BPlusGetLeftSiblingEntry(BPlusTree* tree, BPlusEntry* entry, int leftParentIndex) {
	BPlusEntry* parent = entry->parent;
	if (!parent) {
		return NULL;
	}
	if (leftParentIndex > -1) {
		return parent->indexInternalEntry[leftParentIndex].child;
	}
	return NULL;
}

/*
* 获取右兄弟节点，没有则返回NULL
*/
static BPlusEntry* BPlusGetRightSiblingEntry(BPlusTree* tree, BPlusEntry* entry, int leftParentIndex) {
	BPlusEntry* parent = entry->parent;
	if (!parent) {
		return NULL;
	}
	if (leftParentIndex < parent->count) {
		return parent->indexInternalEntry[leftParentIndex + 2].child;
	}
	return NULL;
}

/*
* 向内部节点数组指定位置插入key及子节点(是索引节点时)
*/
static void BPlusInsertInternalEntry(BPlusEntry* entry, int keyIndex, int key, BPlusEntry* child, bool rightChild) {
	if (entry->type == kLeaf) {
		BPlusEntry* leaf = entry;
		for (int j = leaf->count - 1; j >= keyIndex; j--) {
			leaf->leafInternalEntry[j + 1].key = leaf->leafInternalEntry[j].key;
		}
		leaf->leafInternalEntry[keyIndex].key = key;
		leaf->count++;
	}
	else {
		BPlusEntry* index = entry;
		for (int j = index->count - 1; j >= keyIndex; j--) {
			index->indexInternalEntry[j + 1].key = index->indexInternalEntry[j].key;
			index->indexInternalEntry[j + 2].child = index->indexInternalEntry[j + 1].child;
		}
		index->indexInternalEntry[keyIndex].key = key;
		child->parent = index;
		if (!rightChild) {
			// 是右子节点，多挪一个，并 keyIndex-- 使其指向右子节点
			index->indexInternalEntry[keyIndex + 1].child = index->indexInternalEntry[keyIndex].child;
			keyIndex--;
		}
		index->indexInternalEntry[keyIndex + 1].child = child;
		index->count++;
	}
}

/*
* 从内部节点数组中删除指定索引的内部节点
* 返回被删除内部节点的子节点
*/
static BPlusEntry* BPlusDeleteInternalEntry(BPlusEntry* entry, int keyIndex, int* key, bool rightChild) {
	if (entry->type == kLeaf) {
		BPlusEntry* leaf = entry;
		if (key) {
			*key = leaf->leafInternalEntry[keyIndex].key;
		}
		for (int i = keyIndex + 1; i < leaf->count; i++) {
			leaf->leafInternalEntry[i - 1].key = leaf->leafInternalEntry[i].key;
		}
		leaf->count--;
		return NULL;
	}
	else {
		BPlusEntry* index = entry;
		if (key) {
			*key = index->indexInternalEntry[keyIndex].key;
		}
		BPlusEntry* deleteEntry;
		if (rightChild) {
			deleteEntry = index->indexInternalEntry[keyIndex + 1].child;
			for (int i = keyIndex + 1; i < index->count; i++) {
				index->indexInternalEntry[i - 1].key = index->indexInternalEntry[i].key;
				index->indexInternalEntry[i].child = index->indexInternalEntry[i + 1].child;
			}
		} else {
			deleteEntry = index->indexInternalEntry[keyIndex].child;
			for (int i = keyIndex + 1; i < index->count; i++) {
				index->indexInternalEntry[i - 1].key = index->indexInternalEntry[i].key;
				index->indexInternalEntry[i - 1].child = index->indexInternalEntry[i].child;
			}
			index->indexInternalEntry[index->count-1].child = index->indexInternalEntry[index->count].child;
		}
		index->count--;
		return deleteEntry;
	}
}

/*
* 分裂节点
* 同一个叶节点多次分裂会导致重复的key上升吗？
* 如果叶节点不存在相同的key，那么是不会的
* 因为分裂后选择右节点的最左内部节点的key作为上升的key
* 这个时候无论怎么插入内部节点都不会插入到该节点最左内部节点的左侧(比它小的会被分到左侧节点，因为父索引内部节点key等于该内部节点)，该节点再分裂也就不存在最左内部节点再次上升的可能了
*/
static bool BPlusInsertEntry(BPlusTree* tree, BPlusEntry* entry, int key, BPlusEntry* rightChild);
static void BPlusSplitEntry(BPlusTree* tree, BPlusEntry* entry, int insertIndex, int key, BPlusEntry* rightChild) {
	BPlusEntry* newEntry = NULL;
	int newCount;
	if (entry->type == kLeaf) {

		newEntry = BPlusCreateLeafEntry(tree->m);

		ListInsertHead(&entry->leafListEntry, &newEntry->leafListEntry);

		// 原地分裂思路：mid将未插入的内部节点也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
		// 就是提前算好右侧应当有多少个内部节点，拷贝过去，中间遇到新内部节点插入就代替这一次的拷贝，没插入再插入到左侧
		int mid = tree->m / 2;
		newCount = entry->count + 1 - mid;		// +1是因为这个时候entry->count并没有把未插入内部节点也算上
		int i = newCount - 1, j = entry->count - 1;
		bool insert = false;
		for (; i >= 0; i--, j--) {
			if (!insert && j+1 == insertIndex) {		// 这里j+1是因为，循环的时候j并没有把未插入内部节点也算上
				entry->count++;
				newEntry->leafInternalEntry[i].key = key;
				j++;		// j不动
				insert = true;
				continue;
			}
			newEntry->leafInternalEntry[i].key = entry->leafInternalEntry[j].key;
		}

		entry->count -= newCount;

		if (!insert) {
			// 新内部节点还没有插入，将其插入
			BPlusInsertInternalEntry(entry, insertIndex, key, rightChild, true);
		}
		
		// 从mid拿到上升内部节点
		key = newEntry->leafInternalEntry[0].key;
	}
	else {
		newEntry = BPlusCreateIndexEntry(tree->m);

		// 原地分裂思路：mid将未插入的内部节点和即将上升的内部节点都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了

		// 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
		//  1 2       3 4
		// / |  |      |  \	
		// 此时新节点缺少了一条链接，我们最终选用旧节点的末尾内部节点作为上升内部节点，故旧节点的左侧的末尾内部节点的右子树就可以挂接到新节点上

		int mid = (tree->m-1) / 2;
		newCount = entry->count - mid;		// 这个时候entry->count并没有把未插入内部节点也算上，但是会上升一个内部节点，抵消故不计入
		int i = newCount - 1, j = entry->count - 1;
		bool insert = false;
		for (; i >= 0; i--, j--) {
			if (!insert && j+1 == insertIndex) {		// 这里j+1是因为，循环的时候j并没有把未插入内部节点也算上
				entry->count++;
				newEntry->indexInternalEntry[i].key = key;
				newEntry->indexInternalEntry[i+1].child = rightChild;
				rightChild->parent = newEntry;
				j++;		// j不动
				insert = true;
				continue;
			}
			newEntry->indexInternalEntry[i].key = entry->indexInternalEntry[j].key;
			newEntry->indexInternalEntry[i+1].child = entry->indexInternalEntry[j+1].child;
			newEntry->indexInternalEntry[i+1].child->parent = newEntry;
		}
		entry->count -= newCount;

		if (!insert) {
			// 新内部节点还没有插入，将其插入
			BPlusInsertInternalEntry(entry, insertIndex, key, rightChild, true);
		}

		// 最后从entry->indexData末尾拿到上升内部节点，将其摘除
		entry->count--;
		key = entry->indexInternalEntry[entry->count].key;

		// entry最右的链接交给newEntry
		newEntry->indexInternalEntry[0].child = entry->indexInternalEntry[entry->count+1].child;
		newEntry->indexInternalEntry[0].child->parent = newEntry;
	}
	newEntry->count = newCount;

	// 分裂出的内部节点向上传递
	if (entry->parent == NULL) {
		BPlusEntry* newIndex = BPlusCreateIndexEntry(tree->m);
		entry->parent = newIndex;
		newIndex->indexInternalEntry[0].child = entry;
		BPlusInsertInternalEntry(newIndex, 0, key, newEntry, true);
		tree->root = newIndex;
	}
	else {
		BPlusInsertEntry(tree, entry->parent, key, newEntry);
	}
}

/*
* 插入节点
*/
static bool BPlusInsertEntry(BPlusTree* tree, BPlusEntry* entry, int key, BPlusEntry* rightChild) {
	if (entry->count == 0) {
		BPlusInsertInternalEntry(entry, 0, key, rightChild, true);
		return true;
	}
	int insertIndex;
	int res;
	if (entry->type == kLeaf) {
		insertIndex = BinarySearch_Range_CustomM(entry->leafInternalEntry, BPlusLeafInternalEntry, key, 0, entry->count - 1, &key, MemoryCmpR);
		res = MemoryCmpR(&entry->leafInternalEntry[insertIndex].key, &key, sizeof(key));
	}
	else {
		insertIndex = BinarySearch_Range_CustomM(entry->indexInternalEntry, BPlusIndexInternalEntry, key, 0, entry->count - 1, &key, MemoryCmpR);
		res = MemoryCmpR(&entry->indexInternalEntry[insertIndex].key, &key, sizeof(key));
	}
	//if (res == 0) {		// 允许插入相同的节点
	//	return false;
	//}
	if (res < 0) {
		insertIndex++;		// 如果所有节点都小于key，那就追加到最尾部
	}

	if (entry->count < tree->m - 1) {
		// 有空余的位置插入
		BPlusInsertInternalEntry(entry, insertIndex, key, rightChild, true);
		return true;
	}

	// 没有多余位置，需要分裂向上合并
	BPlusSplitEntry(tree, entry, insertIndex, key, rightChild);
	return true;
}

/*
* 合并节点
*   2
* 1   3
* 父、左、右的关系如下
*/
static bool BPlusDeleteEntry(BPlusTree* tree, BPlusEntry* entry, int deleteIndex, Array* stack);
static void BPlusMergeEntry(BPlusTree* tree, BPlusEntry* left, BPlusEntry* right, int commonParentIndex, Array* stack) {
	if (left->type == kLeaf) {
		// 是叶子节点，将right并入left中，并删除right的父索引内部节点
		for (int i = 0; i < right->count; i++) {
			left->leafInternalEntry[left->count++].key = right->leafInternalEntry[i].key;
		}
	} else {
		// 是索引节点，将即将被删除的父索引内部节点(子节点丢弃，因为父索引子节点就指向左和右)和right都并入到left中，删除right的父索引内部节点
		left->indexInternalEntry[left->count++].key = right->parent->indexInternalEntry[commonParentIndex].key;
		// 右节点会多出一个子节点，因为父节点不需要带子节点下降，所以可以直接并到左节点尾部
		left->indexInternalEntry[left->count].child = right->indexInternalEntry[0].child;		
		right->indexInternalEntry[0].child->parent = left;
		for (int i = 0; i < right->count; i++) {
			left->indexInternalEntry[left->count++].key = right->indexInternalEntry[i].key;
			left->indexInternalEntry[left->count].child = right->indexInternalEntry[i+1].child;
			right->indexInternalEntry[i + 1].child->parent = left;
		}
	}
	BPlusDeleteEntry(tree, left->parent, commonParentIndex, stack);
	DeleteObject_(right);
}

/*
* 删除指定节点
*/
static bool BPlusDeleteEntry(BPlusTree* tree, BPlusEntry* entry, int deleteIndex, Array* stack) {
	BPlusDeleteInternalEntry(entry, deleteIndex, NULL, true);
	if (entry->count >= (tree->m-1) / 2) {
		return true;
	}

	int* leftParentIndex = ArrayPopTail(stack);
	if (!leftParentIndex) {
		// 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
		if (entry->type == kIndex && entry->count == 0) {
			BPlusEntry* child = entry->indexInternalEntry[0].child;
			child->parent = NULL;
			tree->root = child;
			DeleteObject_(entry);
		}
		return true;
	}

	BPlusEntry* sibling = BPlusGetLeftSiblingEntry(tree, entry, *leftParentIndex);
	bool leftSibling = true;
	if (!sibling) {
		sibling = BPlusGetRightSiblingEntry(tree, entry, *leftParentIndex);
		leftSibling = false;
	}
	if (sibling->count > (tree->m-1) / 2) {
		// 向兄弟借节点
		int key;
		if (entry->type == kLeaf) {
			// 叶子节点处理较简单，可以直接移动
			if (leftSibling) {
				// 从左兄弟节点的末尾的内部节点插入到当前节点的头部并更新父内部节点key为借来的key
				BPlusEntry* child = BPlusDeleteInternalEntry(sibling, sibling->count - 1, &key, true);
				BPlusInsertInternalEntry(entry, 0, key, child, true);
			}
			else {
				// 从右兄弟节点的头部的内部节点插入到当前节点的尾部并更新父内部节点key为右兄弟的新首内部节点
				BPlusEntry* child = BPlusDeleteInternalEntry(sibling, 0, &key, true);
				BPlusInsertInternalEntry(entry, entry->count, key, child, true);
				key = sibling->leafInternalEntry[0].key;		// 右节点的头内部节点key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首内部节点是最好的
				++*leftParentIndex;		// 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
			}
			
		} else {
			// 索引节点处理较复杂，需要下降父节点的中当前节点和兄弟节点的共同内部节点，上升兄弟节点到共同父节点(即交换)
			if (leftSibling) {
				// 左兄弟节点的末尾内部节点上升到父节点的头部，父节点的对应内部节点下降到当前节点的头部，上升内部节点其右子节点挂在下降的父节点内部节点的右侧
				BPlusEntry* rightChild = BPlusDeleteInternalEntry(sibling, sibling->count - 1, &key, true);
				BPlusInsertInternalEntry(entry, 0, entry->parent->indexInternalEntry[*leftParentIndex].key, rightChild, false);
			} else {
				// 右兄弟节点的头内部节点上升到父节点的头部，父节点的对应内部节点下降到当前节点的尾部，上升内部节点其左子节点挂在下降的父节点内部节点的左侧
				BPlusEntry* leftChild = BPlusDeleteInternalEntry(sibling, 0, &key, false);
				++*leftParentIndex;		// 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
				BPlusInsertInternalEntry(entry, entry->count, entry->parent->indexInternalEntry[*leftParentIndex].key, leftChild, true);
			}
		}
		entry->parent->indexInternalEntry[*leftParentIndex].key = key;
		return true;
	}

	// 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点合并也足够下降一个父内部节点)
	if (leftSibling) {
		BPlusMergeEntry(tree, sibling, entry, *leftParentIndex, stack);
	} else {
		BPlusMergeEntry(tree, entry, sibling, *leftParentIndex + 1, stack);		// 要求共同父索引
	}
	return true;
}

/*
* 根据key查找到指定的叶子节点
* stack返回每一层节点的父节点内部节点(左父)索引
*/
static BPlusEntry* BPlusTreeFindLeaf(BPlusTree* tree, int key, Array* stack) {
	BPlusEntry* cur = tree->root;
	while (cur && cur->type == kIndex) {
		//       4        8         12
		//    /       |        |       \
		// 3  4      6  7      10        15
		int i = BinarySearch_Range_CustomM(cur->indexInternalEntry, BPlusIndexInternalEntry, key, 0, cur->count - 1, &key, MemoryCmpR);
		
		if (key < cur->indexInternalEntry[i].key) {
			cur = cur->indexInternalEntry[i].child;
		}
		else {
			// 相等的key在右边，因为分裂的索引是右叶子的第一个节点上升的
			cur = cur->indexInternalEntry[++i].child;
		}
		if (stack) {
			i--;		// 修正入栈的父节点内部节点索引
			ArrayPushTail(stack, &i);
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

bool BPlusTreeFind(BPlusTree* tree, int key) {
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key, NULL);
	int index = BinarySearch_CustomM(leaf->leafInternalEntry, BPlusLeafInternalEntry, key, 0, leaf->count - 1, &key, MemoryCmpR);
	if (index == -1) {
		return false;
	}
	return true;
}

bool BPlusTreeInsert(BPlusTree* tree, int key) {
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key, NULL);
	return BPlusInsertEntry(tree, leaf, key, NULL);
}

bool BPlusTreeDelete(BPlusTree* tree, int key) {
	Array stack;
	ArrayInit(&stack, 16, sizeof(int));
	BPlusEntry* leaf = BPlusTreeFindLeaf(tree, key, &stack);

	int deleteIndex = BinarySearch_CustomM(leaf->leafInternalEntry, BPlusLeafInternalEntry, key, 0, leaf->count - 1, &key, MemoryCmpR);
	if (deleteIndex == -1) {
		ArrayRelease(&stack);
		return false;
	}
	bool success = BPlusDeleteEntry(tree, leaf, deleteIndex, &stack);

	ArrayRelease(&stack);

	return success;
}
