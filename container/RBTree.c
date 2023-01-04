#include "RBTree.h"


/*
* 左旋子树
*/
static RBEntry* RotateLeft(RBEntry* subRoot) {
	RBEntry* newSubRoot = subRoot->right;
	if (newSubRoot == NULL) {
		return subRoot;
	}

	newSubRoot->parent = subRoot->parent;
	if (subRoot->parent) {
		if (subRoot->parent->left == subRoot) {
			subRoot->parent->left = newSubRoot;
		}
		else {
			subRoot->parent->right = newSubRoot;
		}
	}
	subRoot->parent = newSubRoot;

	subRoot->right = newSubRoot->left;
	if (subRoot->right) {
		subRoot->right->parent = subRoot;
	}

	newSubRoot->left = subRoot;

	return newSubRoot;
}

/*
* 右旋子树
*/
static RBEntry* RotateRight(RBEntry* subRoot) {
	RBEntry* newSubRoot = subRoot->left;
	if (newSubRoot == NULL) {
		return subRoot;
	}

	newSubRoot->parent = subRoot->parent;
	if (subRoot->parent) {
		if (subRoot->parent->left == subRoot) {
			subRoot->parent->left = newSubRoot;
		}
		else {
			subRoot->parent->right = newSubRoot;
		}
	}
	subRoot->parent = newSubRoot;

	subRoot->left = newSubRoot->right;
	if (subRoot->left) {
		subRoot->left->parent = subRoot;
	}

	newSubRoot->right = subRoot;

	return newSubRoot;

}

static RBEntry* GetSiblingEntry(RBEntry* entry) {
	if (entry->parent->left == entry) {
		return entry->parent->right;
	}
	else if (entry->parent->right == entry) {
		return entry->parent->left;
	}
	return entry->parent->left ? entry->parent->left : entry->parent->right;
}

/*
* newEntry挂接到entry原来的位置
* entry从树中摘除，但entry的parent、left和right不变
*/
static void RBHitchEntry(RBTree* tree, RBEntry* entry, RBEntry* newEntry) {
	if (entry->parent) {
		if (entry->parent->left == entry) {
			entry->parent->left = newEntry;
		}
		else {
			entry->parent->right = newEntry;
		}
	}
	if (newEntry) {
		newEntry->parent = entry->parent;
	}
	if (tree->root == entry) {
		tree->root = newEntry;
	}
}

/*
* 交换两个节点，包括parent、left、right、color，以及节点的parent、left、right指向节点的指针
* 可能出错，不建议使用(比如entry2的父节点是entry1，交换后entry1的父节点就指向自己了)
*/
static void RBSwapEntry(RBTree* tree, RBEntry* entry1, RBEntry* entry2) {
	MemorySwap(entry1, entry2, sizeof(RBEntry));
	if (entry1->parent) {
		if (entry1->parent->left == entry2) {
			entry1->parent->left = entry1;
		}
		else {
			entry1->parent->right = entry1;
		}
	}
	if (entry1->left) {
		entry1->left->parent = entry1;
	}
	if (entry1->right) {
		entry1->right->parent = entry1;
	}


	if (entry2->parent) {
		if (entry2->parent->left == entry1) {
			entry2->parent->left = entry2;
		}
		else {
			entry2->parent->right = entry2;
		}
	}
	if (entry2->left) {
		entry2->left->parent = entry2;
	}
	if (entry2->right) {
		entry2->right->parent = entry2;
	}

	if (tree->root == entry1) {
		tree->root = entry2;
	}
	else if (tree->root == entry2) {
		tree->root = entry1;
	}
}



/*
* 初始化树
*/
void RBTreeInit(RBTree* tree, int objSize, int entryFieldOffset, int keyFieldOffset, int keySize) {
	tree->root = NULL;
	tree->entryFieldOffset = entryFieldOffset;
	// head->smallByteOrder = true;
	tree->keyFieldOffset = keyFieldOffset;
	tree->keyFieldSize = keySize;
	tree->objSize = objSize;
}

/*
* 初始化节点
*/
void RBEntryInit(RBEntry* entry, RBColor color) {
	entry->left = NULL;
	entry->right = NULL;
	entry->parent = NULL;
	entry->color = color;
}


/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
RBEntry* RBFindEntryByKey(RBTree* tree, void* key) {
	RBEntry* cur = tree->root;
	while (cur) {
		void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = MemoryCmpR(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
		if (res < 0) {
			cur = cur->right;
		}
		else if (res > 0) {
			cur = cur->left;
		}
		else {
			return cur;
		}
	}
	return NULL;
}


/*
* 向树中插入节点
* 不允许存在重复节点
* 成功返回true，失败返回false
*/
bool RBInsertEntry(RBTree* tree, RBEntry* entry) {
	RBEntry* root = tree->root;
	if (root == NULL) {
		RBEntryInit(entry, kBlack);
		tree->root = entry;
		return true;
	}
	RBEntryInit(entry, kRed);
	void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
	void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);
	RBEntry* cur = root;
	while (cur) {
		void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = MemoryCmpR(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
		if (res < 0) {
			if (!cur->right) {
				cur->right = entry;
				break;
			}
			cur = cur->right;
		}
		else if (res > 0) {
			if (!cur->left) {
				cur->left = entry;
				break;
			}
			cur = cur->left;
		}
		else {
			return false;
		}
	}
	entry->parent = cur;
	if (cur && cur->color == kBlack) {
		// 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点/3节点的插入，直接合并)
		return true;
	}

	RBEntry* newSubRoot = NULL;
	// 开始回溯维护
	while (cur) {
		if (cur->parent == NULL) {
			// 没有父节点，回溯到根节点了，直接染黑
			cur->color = kBlack;
			break;
		}
		RBEntry* sibling = GetSiblingEntry(cur);

		if (sibling && sibling->color == kRed) {
			// 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上合并，继续回溯
			cur->color = kBlack;
			sibling->color = kBlack;
			cur->parent->color = kRed;
		}
		else {
			// 没有兄弟节点或者兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
			//         10b               5b    
			//      5r     20b  ->   !2r     10r    
			//  !2r                             20b
			RBEntry* oldSubRoot = cur->parent;
			if (cur->parent->left == cur) {
				if (cur->right == entry) {
					RotateLeft(cur);
				}
				newSubRoot = RotateRight(cur->parent);
			}
			else {
				if (cur->left == entry) {
					RotateRight(cur);
				}
				newSubRoot = RotateLeft(cur->parent);
			}
			cur->color = kBlack;
			oldSubRoot->color = kRed;

			if (tree->root == oldSubRoot) {		// 原来的子树根节点可能是整棵树的根节点，因此要检查更新
				tree->root = newSubRoot;
			}

			break;		// 只是并入，未分裂，向上没有改变颜色，不再需要回溯
		}
		cur = cur->parent;
	}
	return true;
}


/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
RBEntry* RBDeleteEntry(RBTree* tree, RBEntry* entry) {
	RBEntry* backtrack = entry;		// 通常情况下是从被删除节点的父节点开始回溯
	RBEntry* sibling = NULL;
	if (entry->left == NULL && entry->right == NULL) {
		// 没有子节点，直接从父节点中摘除此节点
		RBHitchEntry(tree, entry, NULL);
	}
	else if (entry->left == NULL) {
		// 挂接右子节点
		RBHitchEntry(tree, entry, entry->right);
	}
	else if (entry->right == NULL) {
		// 挂接左子节点
		RBHitchEntry(tree, entry, entry->left);
	}
	else {
		// 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
		RBEntry* minEntry = entry->right;
		while (minEntry) {
			if (minEntry->left) {
				minEntry = minEntry->left;
			}
			else {
				break;
			}
		}
		MemorySwap(&minEntry->color, &entry->color, sizeof(RBColor));

		
		// 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
		minEntry->left = entry->left;
		if (minEntry->left) {
			minEntry->left->parent = minEntry;
		}

		RBEntry* oldParent, * oldRight = minEntry->right;
		// 最小节点可能是待删除节点的右节点
		if (minEntry->parent != entry) {
			oldParent = minEntry->parent;		// 从删除的节点开始回溯，因此需要记录

			// 最小节点继承待删除节点的右子树
			minEntry->parent->left = minEntry->right;
			if (minEntry->right) {
				minEntry->right->parent = minEntry->parent;
			}
			minEntry->right = entry->right;
			if (minEntry->right) {
				minEntry->right->parent = minEntry;
			}
		}
		else {
			oldParent = minEntry;		// 最小节点的父亲就是待删除节点，交换位置后最小节点就是待删除节点的父亲，因此从这里回溯
		}

		// 最后进行挂接
		RBHitchEntry(tree, entry, minEntry);

		entry->parent = oldParent;		// 回溯需要父节点形成回溯路径
		entry->left = NULL; entry->right = oldRight;
	}

	if (entry) {
		if (entry->color == kRed) {
			// 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
			return entry;
		}
		// 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
		if (entry->left) {
			entry->left->color = kBlack;
			return entry;
		}
		if (entry->right) {
			entry->right->color = kBlack;
			return entry;
		}
	}

	RBEntry* newSubRoot = NULL;
	// 回溯维护删除黑色节点，即没有子节点(2节点)的情况
	while (backtrack) {
		if (backtrack->parent == NULL) {
			// 没有父节点，回溯到根节点了，直接染黑
			backtrack->color = kBlack;
			break;
		}

		sibling = GetSiblingEntry(backtrack);

		if (sibling->color == kRed) {
			// 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
			// 旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，但兄弟节点变为真正的兄弟节点
			sibling->parent->color = kRed;
			sibling->color = kBlack;
			RBEntry* oldSubRoot = sibling->parent;
			if (sibling->parent->left == sibling){
				newSubRoot = RotateRight(sibling->parent);
			} else {
				newSubRoot = RotateLeft(sibling->parent);
			}
			if (tree->root == oldSubRoot) {
				tree->root = newSubRoot;
			}
			sibling = GetSiblingEntry(backtrack);
		}

		// 至此兄弟节点一定为黑
		
		// 侄子节点为红，即兄弟节点是3/4节点的情况
		if (sibling->right && sibling->right->color == kRed || sibling->left && sibling->left->color == kRed) {
			sibling->color = sibling->parent->color;
			sibling->parent->color = kBlack;
			RBEntry* oldSubRoot = sibling->parent;
			if (sibling->parent->left == sibling) {
				if (!sibling->left || sibling->left->color == kBlack) {
					sibling->right->color = kBlack;
					sibling = RotateLeft(sibling);
				} else {
					sibling->left->color = kBlack;
				}
				newSubRoot = RotateRight(sibling->parent);
			}
			else {
				if (!sibling->right || sibling->right->color == kBlack) {
					sibling->left->color = kBlack;
					sibling = RotateRight(sibling);
				} else {
					sibling->right->color = kBlack;
				}
				newSubRoot = RotateLeft(sibling->parent);
			}

			if (tree->root == oldSubRoot) {
				tree->root = newSubRoot;
			}
			break;
		}
		
		if (sibling->parent->color == kRed) {
			// 父节点为红，即父节点是3/4节点，分裂下沉与兄弟节点合并
			sibling->color = kRed;
			sibling->parent->color = kBlack;
			break;
		}
		else {
			// 父节点为黑，即父节点是2节点，兄弟节点也是2节点，合并两个节点，此处高度-1，继续回溯寻求高度补偿
			sibling->color = kRed;
		}
		
		backtrack = backtrack->parent;
	}
	return entry;
}

RBEntry* RBDeleteEntryByKey(RBTree* tree, void* key) {
	RBEntry* entry = RBFindEntryByKey(tree, key);
	if (entry) {
		RBDeleteEntry(tree, entry);
	}
	return entry;
}

/*
* 获取树节点数量
*/
static void RBGetEntryCountCallback(RBEntry* entry, void* arg) {
	int* count = arg;
	(*count)++;
}
size_t RBGetEntryCount(RBTree* head) {
	int count = 0;
	AVLPreorder_Callback(head->root, RBGetEntryCountCallback, &count);
	return count;
}