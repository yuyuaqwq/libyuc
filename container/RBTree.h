#ifndef CUTILS_RBTREE_H_
#define CUTILS_RBTREE_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 以234树为原型的红黑树实现
*/
typedef enum _RBColor {
	kBlack,
	kRed,
} RBColor;
typedef struct _RBEntry {
	struct _RBEntry* parent;
	struct _RBEntry* left;
	struct _RBEntry* right;
	RBColor color;
} RBEntry;

typedef struct _RBTree {
	RBEntry* root;
	// bool smallByteOrder;
	int objSize;
	int entryFieldOffset;
	int keyFieldOffset;
	int keyFieldSize;
} RBTree;


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
	return entry->parent->left;
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
	} else if (tree->root == entry2) {
		tree->root = entry1;
	}
}

#define RBTreeInitM(tree, objName, entryFieldName, keyFieldName) RBTreeInit((tree), sizeof(objName), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName))


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
*/

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
		} else {
			// 没有兄弟节点或者兄弟节点是黑色，说明是3节点的插入，可以合并，但需要旋转将其变为3节点
			//         10b               5b    
			//      5r     20b  ->   !2r     10r    
			//  !2r                             20b
			RBEntry* oldSubRoot = cur->parent;
			if (cur->parent->left == cur) {
				if (cur->right == entry) {
					RotateLeft(cur);
				}
				newSubRoot = RotateRight(cur->parent);
			} else {
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

			break;		// 只是合并，未分裂，向上没有改变颜色，不再需要回溯
		}
		cur = cur->parent;
	}
	return true;
}


/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
* 实现较为困难，暂时保留
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

		// 最小节点可能是待删除节点的右节点
		if (minEntry->parent != entry) {
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

		RBEntry* oldParent = minEntry->parent;		// 从删除的节点开始回溯，因此需要记录
		
		// 最后进行挂接
		RBHitchEntry(tree, entry, minEntry);

		entry->parent = oldParent;		// 回溯需要父节点形成回溯路径

		sibling = 
	}
	if (entry) {
		if (entry->color == kRed) {
			// 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
			return entry;
		}
		// 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
		else if (entry->left) {
			entry->left->color = kBlack;
			return entry;
		}
		else if (entry->right) {
			entry->right->color = kBlack;
			return entry;
		}
	}


	// 回溯维护是黑色的，没有子节点(即2节点)的情况
	while (backtrack) {
		if (!sibling) {
			sibling = GetSiblingEntry(backtrack);
		}
		sibling = NULL;
		backtrack = backtrack->parent;
	}
	return entry;
}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_RBTREE_H_