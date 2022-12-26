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
	int entryFieldOffset;
	int objByteCount;
	int keyFieldOffset;
	int keyByteCount;
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


#define RBTreeInitM(tree, objName, entryFieldName, keyFieldName) RBTreeInit((tree), sizeof(objName), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName))


/*
* 初始化树
*/
void RBTreeInit(RBTree* tree, int objSize, int entryFieldOffset, int keyFieldOffset, int keySize) {
	tree->root = NULL;
	tree->entryFieldOffset = entryFieldOffset;
	// head->smallByteOrder = true;
	tree->keyFieldOffset = keyFieldOffset;
	tree->keyByteCount = keySize;
	tree->objByteCount = objSize;
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

static RBEntry* GetGrandfatherEntry(RBEntry* entry) {
	return entry->parent->parent;
}

static RBEntry* GetSiblingEntry(RBEntry* entry) {
	if (entry->parent->left == entry) {
		return entry->parent->right;
	}
	return entry->parent->left;
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
		int res = MemoryCmpR(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyByteCount);
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

	RBEntry* newSubRoot = NULL;
	// 开始回溯维护
	while (cur) {
		if (cur->color == kBlack) {
			// 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点/3节点的插入，合并)
			break;
		}
		// 当前是红节点
		if (cur->parent == NULL) {
			// 没有父节点，回溯到根节点了，直接染黑
			cur->color = kBlack;
			break;
		}
		RBEntry* sibling = GetSiblingEntry(cur);

		if (sibling && sibling->color == kRed) {
			// 兄弟节点是红色，说明是4节点的插入，分裂，父节点向上合并，继续回溯
			cur->color = kBlack;
			sibling->color = kBlack;
			cur->parent->color = kRed;
		} else {
			// 没有兄弟节点或者兄弟节点是黑色，说明是3节点的插入，可以合并，但需要旋转
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


#ifdef __cplusplus
}
#endif

#endif // CUTILS_RBTREE_H_