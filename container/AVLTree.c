#include "AVLTree.h"

/*
* 根据左右子树高度获取节点高度
*/
static int GetMaxHeight(AVLEntry* subRoot) {
	int leftHeight = -1, rightHeight = -1;
	if (subRoot->left) leftHeight = subRoot->left->height;
	if (subRoot->right) rightHeight = subRoot->right->height;
	return max(leftHeight, rightHeight) + 1;
}

/*
* 根据左右子树高度更新节点高度
* 更新完成返回true，无需更新返回false
*/
static int UpdateHeight(AVLEntry* subRoot) {
	int newHeight = GetMaxHeight(subRoot);
	if (subRoot->height != newHeight) {
		subRoot->height = newHeight;
		return true;
	}
	return false;
}

/*
* 根据左右子树高度获取节点平衡因子
*/
static int GetBalanceFactor(AVLEntry* subRoot) {
	int leftHeight = -1, rightHeight = -1;
	if (subRoot->left) leftHeight = subRoot->left->height;
	if (subRoot->right) rightHeight = subRoot->right->height;
	return leftHeight - rightHeight;
}

/*
* 左旋子树
*/
static AVLEntry* RotateLeft(AVLEntry* subRoot) {
	AVLEntry* newSubRoot = subRoot->right;
	if (newSubRoot == NULL) {
		return subRoot;
	}

	newSubRoot->parent = subRoot->parent;
	if (subRoot->parent) {
		if (subRoot->parent->left == subRoot) {
			subRoot->parent->left = newSubRoot;
		} else {
			subRoot->parent->right = newSubRoot;
		}
	}
	subRoot->parent = newSubRoot;
	
	subRoot->right = newSubRoot->left;
	if (subRoot->right) {
		subRoot->right->parent = subRoot;
	}

	newSubRoot->left = subRoot;
		
	UpdateHeight(subRoot);
	UpdateHeight(newSubRoot);

	return newSubRoot;
}

/*
* 右旋子树
*/
static AVLEntry* RotateRight(AVLEntry* subRoot) {
	AVLEntry* newSubRoot = subRoot->left;
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

	UpdateHeight(subRoot);
	UpdateHeight(newSubRoot);

	return newSubRoot;

}

/*
* 根据平衡因子来旋转子树
*/
static bool RotateByBalanceFactor(AVLTree* tree, AVLEntry** subRoot_) {
	AVLEntry* subRoot = *subRoot_;

	int factor = GetBalanceFactor(subRoot);
	bool rotate = false;
	AVLEntry* newSubRoot = NULL;
	if (factor > 1) {
		// 是失衡节点，左子树高度高于右子树高度

		// 判断需要单旋还是双旋
		if (GetBalanceFactor(subRoot->left) < 0) {
			// 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
			RotateLeft(subRoot->left);
		}
		// 此时失衡节点的左子树的左子树更深，右旋即可
		// 可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点
		newSubRoot = RotateRight(subRoot);
		rotate = true;
	} else if (factor < -1) {
		// 是失衡节点，右子树高度高于左子树高度

		// 判断需要单旋还是双旋
		if (GetBalanceFactor(subRoot->right) > 0) {
			// 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
			RotateRight(subRoot->right);
		}
		// 此时失衡节点的右子树的右子树更深，左旋即可
		newSubRoot = RotateLeft(subRoot);

		rotate = true;
	}

	if (rotate) {
		if (tree->root == subRoot) {		// 原来的子树根节点可能是整棵树的根节点，因此要检查更新
			tree->root = newSubRoot;
		}
		*subRoot_ = newSubRoot;
	}

	return rotate;
}

/*
* newEntry挂接到entry原来的位置
* entry从树中摘除，但entry的parent、left和right不变
*/
static void AVLHitchEntry(AVLTree* tree, AVLEntry* entry, AVLEntry* newEntry) {
	if (entry->parent) {
		if (entry->parent->left == entry) {
			entry->parent->left = newEntry;
		} else {
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
void AVLTreeInit(AVLTree* tree, int objSize, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc) {
	tree->root = NULL;
	tree->entryFieldOffset = entryFieldOffset;
	// head->smallByteOrder = true;
	tree->keyFieldOffset = keyFieldOffset;
	tree->keyFieldSize = keySize;
	tree->objSize = objSize;
	if (cmpFunc == NULL) {
		cmpFunc = MemoryCmpR;
	}
	tree->cmpFunc = cmpFunc;
}

/*
* 初始化节点
*/
void AVLEntryInit(AVLEntry* entry) {
	entry->height = 0;
	entry->left = NULL;
	entry->right = NULL;
	entry->parent = NULL;
}

/*
* 从树中查找节点
* 存在返回查找到的节点，不存在返回NULL
*/
AVLEntry* AVLFindEntryByKey(AVLTree* tree, void* key) {
	AVLEntry* cur = tree->root;
	while (cur) {
		void* obj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = tree->cmpFunc(GetFieldByFieldOffset(obj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
		if (res < 0) {
			cur = cur->right;
		} else if (res > 0) {
			cur = cur->left;
		} else {
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
bool AVLInsertEntry(AVLTree* tree, AVLEntry* entry) {
	AVLEntry* root = tree->root;
	AVLEntryInit(entry);
	if (root == NULL) {
		tree->root = entry;
		return true;
	}

	void* obj = GetObjByFieldOffset(entry, tree->entryFieldOffset, void);
	void* key = GetFieldByFieldOffset(obj, tree->keyFieldOffset, void);
	AVLEntry* cur = root;
	while (cur) {
		void* curObj = GetObjByFieldOffset(cur, tree->entryFieldOffset, void);
		int res = tree->cmpFunc(GetFieldByFieldOffset(curObj, tree->keyFieldOffset, void), key, tree->keyFieldSize);
		if (res < 0) {
			if (!cur->right) {
				cur->right = entry;
				break;
			}
			cur = cur->right;
		} else if (res > 0) {
			if (!cur->left) {
				cur->left = entry;
				break;
			}
			cur = cur->left;
		} else {
			return false;
		}
	}
	entry->parent = cur;

	// 插入节点后高度可能发生变化，回溯维护节点高度
	int heightCount = 1;
	while (cur) {
		if (cur->height < heightCount) {
			cur->height = heightCount;
		} else {
			break;		// 至此高度未发生变化，不再回溯
		}
		if (RotateByBalanceFactor(tree, &cur)) {
			break;		// 插入后如果旋转了，就不需要再向上回溯了，因为旋转会导致这颗子树的高度不变
		}
		cur = cur->parent;
		heightCount++;
	}
	return true;
}

/*
* 从树中删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLDeleteEntry(AVLTree* tree, AVLEntry* entry) {

	AVLEntry* backtrack = entry->parent;		// 通常情况下是从被删除节点的父节点开始回溯
	if (entry->left == NULL && entry->right == NULL) {
		// 没有子节点，直接从父节点中摘除此节点
		AVLHitchEntry(tree, entry, NULL);
	}
	else if (entry->left == NULL) {
		// 只有右子节点，那说明右子节点没有子节点(有子节点的话就已经失衡了，因为没有左子节点，右子节点还有子节点就会形成0 - 2)
		AVLHitchEntry(tree, entry, entry->right);
	}
	else if (entry->right == NULL) {
		AVLHitchEntry(tree, entry, entry->left);
	}
	else {
		// 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
		AVLEntry* minEntry = entry->right;
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
			backtrack = minEntry->parent;		// 在修改最小节点的父节点之前记录回溯节点

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
		else {
			backtrack = minEntry;		// 最小节点的父亲就是待删除节点，交换位置后最小节点就是待删除节点的父亲，因此从这里回溯
		}

		// 最后进行挂接
		AVLHitchEntry(tree, entry, minEntry);

		// 也可以选择直接交换两个节点的数据
	}
	
	
	// 删除节点后高度可能发生变化，回溯维护节点高度
	while (backtrack) {
		if (UpdateHeight(backtrack) == false) {
			// 父节点高度未变化则说明，父节点的另一子树深度更深，要检查是否失衡
			if (RotateByBalanceFactor(tree, &backtrack) == false) {
				break;		// 未失衡则停止回溯，从当前节点开始不会再影响上层节点的高度。
			}
		}
		else {}
		backtrack = backtrack->parent;
	}
	return entry;
}

/*
* 从树中按key删除节点
* 成功返回被删除的节点，失败返回NULL
*/
AVLEntry* AVLDeleteEntryByKey(AVLTree* tree, void* key) {
	AVLEntry* entry = AVLFindEntryByKey(tree, key);
	if (entry) {
		AVLDeleteEntry(tree, entry);
	}
	return entry;
}

/*
* 获取树节点数量
*/
static void AVLGetEntryCountCallback(AVLEntry* entry, void* arg) {
	int* count = arg;
	(*count)++;
}
size_t AVLGetEntryCount(AVLTree* head) {
	int count = 0;
	AVLPreorder_Callback(head->root, AVLGetEntryCountCallback, &count);
	return count;
}



/*
* 前序遍历
* 先根再右再左
*/
void AVLPreorder_Callback(AVLEntry* entry, TraversalCallback callback, void* arg) {
	if (!entry) return;
	callback(entry, arg);
	AVLPreorder_Callback(entry->left, callback, arg);
	AVLPreorder_Callback(entry->right, callback, arg);
}

/*
* 中序遍历
* 先左再根再右
*/
void AVLMiddleorder_Callback(AVLEntry* entry, TraversalCallback callback, void* arg) {
	if (!entry) return;
	AVLMiddleorder_Callback(entry->left, callback, arg);
	callback(entry, arg);
	AVLMiddleorder_Callback(entry->right, callback, arg);
}

/*
* 后序遍历
* 先左再右再根
*/
void AVLPostorder_Callback(AVLEntry* entry, TraversalCallback callback, void* arg) {
	if (!entry) return;
	AVLPostorder_Callback(entry->left, callback, arg);
	AVLPostorder_Callback(entry->right, callback, arg);
	callback(entry, arg);
}


/*
* 中序遍历
* 先左再根再右
*/
bool AVLMiddleorder_Iteration(AVLEntry** cur, bool* status_right) {
	if (*status_right == true) {
		// 当前节点和左子树已经遍历过了，准备从右子树开始
		if ((*cur)->right == NULL) {
			// 没有右子树，当前子树已经遍历完了，向上找第一个循环节点是其父节点的左节点的节点(意义在于找到第一个其左子树全部遍历完毕的子树根节点)
			while ((*cur)->parent) {
				if (*cur == (*cur)->parent->left) {
					*cur = (*cur)->parent;
					break;
				}
				*cur = (*cur)->parent;
			};
			
			return *cur ? true : false;		// 如果没有父节点则说明整棵树遍历完毕
		}
		// 有右子树，中序遍历该子树
		*cur = (*cur)->right;
	}

	if ((*cur)->left) {
		// 有左子树，找到最左的节点将其返回，同时标记左子树和子树根节点(因为返回的节点没有左子树，所以又是子树根节点)已经遍历
		do {
			(*cur) = (*cur)->left;
		} while((*cur)->left);
		*status_right = true;
		return true;
	}

	// 没有左子树，返回该节点并标记左子树和子树根节点已经遍历
	*status_right = true;
	return true;
}

/*
* 后序遍历
* 先左再右再根
*/
AVLEntry* AVLPostorder(AVLEntry* cur) {
	//if (cur->left || cur->right) {
	//	while (cur->left) {
	//		cur = cur->left;
	//		if (cur->left) {
	//			continue;
	//		}
	//		while (cur->right) {
	//			cur = cur->right;
	//		};
	//		
	//	};
	//	
	//	return cur;
	//}

	//if (cur->parent == NULL) {
	//	return cur;
	//}
	//if (cur == cur->parent->left) {
	//	// 是父节点的左节点，父节点的右子树一定还没有遍历
	//	return cur->parent->right;
	//}

	//// 是父节点的右节点，需要回溯找到第一个遍历节点是其父节点的左节点的遍历节点父节点，再找其右子树
	//cur = cur->parent;
	//if (cur->parent == NULL) {
	//	// 父节点即根节点，直接返回根节点，是最后一趟遍历
	//	return cur->parent;
	//}
	//while (cur->parent->left != cur->parent) {
	//	cur = cur->parent;
	//}
	//return cur;
	
}
