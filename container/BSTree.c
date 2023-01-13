/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "BSTree.h"


/*
* newEntry挂接到entry原来的位置
* entry从树中摘除，但entry的parent、left和right不变
*/
static void BSHitchEntry(BSTree* tree, BSEntry* entry, BSEntry* newEntry) {
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
* 初始化树
*/
void BSTreeInit(BSTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
	tree->root = NULL;
	tree->entryFieldOffset = entryFieldOffset;
	tree->keyFieldOffset = keyFieldOffset;
	tree->keyFieldSize = keySize;
	if (cmpFunc == NULL) {
		cmpFunc = MemoryCmpR;
	}
	tree->cmpFunc = cmpFunc;
}

/*
* 初始化节点
*/
void BSEntryInit(BSEntry* entry) {
	entry->left = NULL;
	entry->right = NULL;
	entry->parent = NULL;
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
BSEntry* BSFindEntryByKey(BSTree* tree, void* key) {
	BSEntry* cur = tree->root;
	while (cur) {
		void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = tree->cmpFunc(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
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
bool BSInsertEntry(BSTree* tree, BSEntry* entry) {
	BSEntry* root = tree->root;
	BSEntryInit(entry);
	if (root == NULL) {
		tree->root = entry;
		return true;
	}
	void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
	void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);
	BSEntry* cur = root;
	while (cur) {
		void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
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
	return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
BSEntry* BSDeleteEntry(BSTree* tree, BSEntry* entry) {
	if (entry->left == NULL && entry->right == NULL) {
		// 没有子节点，直接从父节点中摘除此节点
		BSHitchEntry(tree, entry, NULL);
	}
	else if (entry->left == NULL) {
		// 只有右子节点，那说明右子节点没有子节点(有子节点的话就已经失衡了，因为没有左子节点，右子节点还有子节点就会形成0 - 2)
		BSHitchEntry(tree, entry, entry->right);
	}
	else if (entry->right == NULL) {
		BSHitchEntry(tree, entry, entry->left);
	}
	else {
		// 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
		BSEntry* minEntry = entry->right;
		while (minEntry) {
			if (minEntry->left) {
				minEntry = minEntry->left;
			}
			else {
				break;
			}
		}

		// 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
		minEntry->left = entry->left;
		if (entry->left) {
			entry->left->parent = minEntry;
		}

		// 最小节点可能是待删除节点的右节点
		if (minEntry->parent != entry) {
			// 将minEntry从原先的位置摘除，用其右子树代替
			minEntry->parent->left = minEntry->right;
			if (minEntry->right) {
				minEntry->right->parent = minEntry->parent;
			}
			// 最小节点继承待删除节点的右子树
			minEntry->right = entry->right;
			if (entry->right) {
				entry->right->parent = minEntry;
			}
		}

		// 最后进行挂接
		BSHitchEntry(tree, entry, minEntry);

		// 也可以选择直接交换两个节点的数据
	}
	return entry;
}


BSEntry* BSFirst(BSTree* tree) {
	BSEntry* cur = tree->root;
	if (!cur) {
		return NULL;
	}
	while (cur->left)
		cur = cur->left;
	return cur;
}

BSEntry* BSLast(BSTree* tree) {
	BSEntry* cur = tree->root;
	if (!cur) {
		return NULL;
	}
	while (cur->right)
		cur = cur->right;
	return cur;
}

BSEntry* BSNext(BSEntry* entry) {
	if (entry->right) {
		entry = entry->right;
		while (entry->left)
			entry = entry->left;
		return entry;
	}
	BSEntry* parent;
	while ((parent = entry->parent) && entry == parent->right)
		entry = parent;
	return parent;
}

BSEntry* BSPrev(BSEntry* entry) {
	if (entry->left) {
		entry = entry->left;
		while (entry->right) {
			entry = entry->right;
		}
		return entry;
	}
	BSEntry* parent;
	while ((parent = entry->parent) && entry == parent->left) {
		entry = parent;
	}
	return parent;
}



/*
* 前序遍历
* 先根再右再左
*/
void BSPreorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg) {
	if (!entry) return;
	callback(entry, arg);
	BSPreorder_Callback(entry->left, callback, arg);
	BSPreorder_Callback(entry->right, callback, arg);
}

/*
* 中序遍历
* 先左再根再右
*/
void BSMiddleorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg) {
	if (!entry) return;
	BSMiddleorder_Callback(entry->left, callback, arg);
	callback(entry, arg);
	BSMiddleorder_Callback(entry->right, callback, arg);
}

/*
* 后序遍历
* 先左再右再根
*/
void BSPostorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg) {
	if (!entry) return;
	BSPostorder_Callback(entry->left, callback, arg);
	BSPostorder_Callback(entry->right, callback, arg);
	callback(entry, arg);
}