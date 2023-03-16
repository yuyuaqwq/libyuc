/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/rb_tree.h>

typedef enum {
    kRbBlack,
    kRbRed,
} RbColor;



#define INT_RB_TREE_ACCESSOR_GetKey(bs_entry) (((IntEntry_Rb*)bs_entry)->key)
#define INT_RB_TREE_ACCESSOR INT_RB_TREE_ACCESSOR
#define INT_RB_TREE_REFERENCER_Reference(main_obj, obj_id) ((IntBsEntry*)obj_id)
#define INT_RB_TREE_REFERENCER_Dereference(main_obj, obj)
#define INT_RB_TREE_REFERENCER_InvalidId (NULL)
#define INT_RB_TREE_REFERENCER INT_RB_TREE_REFERENCER
//CUTILS_CONTAINER_RB_TREE_DEFINE(Int, IntRbEntry*, int, INT_RB_TREE_REFERENCER, INT_RB_TREE_ACCESSOR, CUTILS_OBJECT_COMPARER_DEFALUT)


 inline IntRbEntry* INT_RB_TREE_ACCESSOR_GetParent(IntRbEntry* entry) {
	return (IntRbEntry*)(((uintptr_t)(entry->parent_color) & (~((uintptr_t)0x1))));
}
 inline RbColor INT_RB_TREE_ACCESSOR_GetColor(IntRbEntry* entry) {
	return (RbColor)(((uintptr_t)entry->parent_color) & 0x1);
}
 inline void INT_RB_TREE_ACCESSOR_SetParent(IntRbEntry* entry, IntRbEntry* new_parent_id) {
	entry->parent_color = (IntRbEntry*)(((uintptr_t)new_parent_id) | ((uintptr_t)INT_RB_TREE_ACCESSOR_GetColor(entry)));
}
 inline void INT_RB_TREE_ACCESSOR_SetColor(IntRbEntry* entry, RbColor color) {
	entry->parent_color = (IntRbEntry*)(((uintptr_t)INT_RB_TREE_ACCESSOR_GetParent(entry)) | ((uintptr_t)color));
}
static void IntBsTreeHitchEntry(IntBsTree* tree, IntRbEntry* entry_id, IntRbEntry* new_entry_id) {
	IntBsEntry* entry = ((IntBsEntry*)entry_id);
	IntBsEntry* new_entry = ((IntBsEntry*)new_entry_id);
	if (INT_RB_TREE_ACCESSOR_GetParent(entry) != (((void*)0))) {
		IntBsEntry* entry_parent = ((IntBsEntry*)INT_RB_TREE_ACCESSOR_GetParent(entry));
		if (entry_parent->left == entry_id) {
			entry_parent->left = new_entry_id;
		}
		else {
			entry_parent->right = new_entry_id;
		}
		(0);
	}
	if (new_entry_id != (((void*)0))) {
		INT_RB_TREE_ACCESSOR_SetParent(new_entry, INT_RB_TREE_ACCESSOR_GetParent(entry));
	}
	if (tree->root == entry_id) {
		tree->root = new_entry_id;
	}
	(0);
	(0);
}
static void IntBsEntryInit(IntRbEntry* entry_id) {
	IntBsEntry* entry = ((IntBsEntry*)entry_id);
	entry->left = (((void*)0));
	entry->right = (((void*)0));
	entry->parent = (((void*)0));
	(0);
}
void IntBsTreeInit(IntBsTree* tree) {
	tree->root = (((void*)0));
}
IntRbEntry* IntBsTreeFind(IntBsTree* tree, int* key) {
	IntRbEntry* cur_id = tree->root;
	while (cur_id != (((void*)0))) {
		IntBsEntry* cur = ((IntBsEntry*)cur_id);
		if ((((((IntEntry_Rb*)cur)->key)) < (*key))) {
			cur_id = cur->right;
		}
		else if ((((((IntEntry_Rb*)cur)->key)) > (*key))) {
			cur_id = cur->left;
		}
		else {
			(0);
			return cur_id;
		}
		(0);
	}
	return (((void*)0));
}
_Bool IntBsTreePut(IntBsTree* tree, IntRbEntry* entry_id) {
	IntBsEntry* entry = ((IntBsEntry*)entry_id);
	IntBsEntryInit(entry);
	if (tree->root == (((void*)0))) {
		tree->root = entry_id;
		return 1;
	}
	IntRbEntry* cur_id = tree->root;
	while (cur_id != (((void*)0))) {
		IntBsEntry* cur = ((IntBsEntry*)cur_id);
		if ((((((IntEntry_Rb*)cur)->key)) < ((((IntEntry_Rb*)entry)->key)))) {
			if (!cur->right) {
				cur->right = entry_id;
				break;
			}
			cur_id = cur->right;
		}
		else if ((((((IntEntry_Rb*)cur)->key)) > ((((IntEntry_Rb*)entry)->key)))) {
			if (!cur->left) {
				cur->left = entry_id;
				break;
			}
			cur_id = cur->left;
		}
		else {
			(0);
			return 0;
		}
		(0);
	}
	INT_RB_TREE_ACCESSOR_SetParent(entry, cur_id);
	(0);
	return 1;
}
_Bool IntBsTreeDelete(IntBsTree* tree, IntRbEntry* entry_id) {
	IntBsEntry* entry = ((IntBsEntry*)entry_id);
	if (entry->left != (((void*)0)) && entry->right != (((void*)0))) {
		IntRbEntry* min_entry_id = entry->right;
		IntBsEntry* min_entry = ((IntBsEntry*)min_entry_id);
		while (min_entry->left != (((void*)0))) {
			min_entry_id = min_entry->left;
			(0);
			min_entry = ((IntBsEntry*)min_entry_id);
		}
		min_entry->left = entry->left;
		if (entry->left != (((void*)0))) {
			IntBsEntry* entry_left = ((IntBsEntry*)entry->left);
			INT_RB_TREE_ACCESSOR_SetParent(entry_left, min_entry_id);
			(0);
		}
		if (entry->right != min_entry_id) {
			IntBsEntry* min_entry_parent = ((IntBsEntry*)INT_RB_TREE_ACCESSOR_GetParent(min_entry));
			min_entry_parent->left = min_entry->right;
			(0);
			if (min_entry->right != (((void*)0))) {
				IntBsEntry* min_entry_right = ((IntBsEntry*)min_entry->right);
				INT_RB_TREE_ACCESSOR_SetParent(min_entry_right, INT_RB_TREE_ACCESSOR_GetParent(min_entry));
				(0);
			}
			min_entry->right = entry->right;
			if (entry->right != (((void*)0))) {
				IntBsEntry* entry_right = ((IntBsEntry*)entry->right);
				INT_RB_TREE_ACCESSOR_SetParent(entry_right, min_entry);
				(0);
			}
		}
		else {
		}
		IntBsTreeHitchEntry(tree, entry, min_entry);
	}
	else {
		if (entry->right != (((void*)0))) {
			IntBsTreeHitchEntry(tree, entry_id, entry->right);
		}
		else if (entry->left != (((void*)0))) {
			IntBsTreeHitchEntry(tree, entry_id, entry->left);
		}
		else {
			IntBsTreeHitchEntry(tree, entry_id, (((void*)0)));
		}
	}
	return 1;
}
size_t IntBsTreeGetCount(IntBsTree* tree) {
	size_t count = 0;
	IntRbEntry* cur_id = IntBsTreeFirst(tree);
	while (cur_id != (((void*)0))) {
		count++;
		cur_id = IntBsTreeNext(tree, cur_id);
	}
	return count;
}
IntRbEntry* IntBsTreeFirst(IntBsTree* tree) {
	IntRbEntry* cur_id = tree->root;
	if (cur_id == (((void*)0))) {
		return (((void*)0));
	}
	IntBsEntry* cur = ((IntBsEntry*)cur_id);
	while (cur->left != (((void*)0))) {
		cur_id = cur->left;
		(0);
		cur = ((IntBsEntry*)cur_id);
	}
	(0);
	return cur_id;
}
IntRbEntry* IntBsTreeLast(IntBsTree* tree) {
	IntRbEntry* cur_id = tree->root;
	if (cur_id == (((void*)0))) {
		return (((void*)0));
	}
	IntBsEntry* cur = ((IntBsEntry*)cur_id);
	while (cur->right != (((void*)0))) {
		cur_id = cur->right;
		(0);
		cur = ((IntBsEntry*)cur_id);
	}
	(0);
	return cur_id;
}
IntRbEntry* IntBsTreeNext(IntBsTree* tree, IntRbEntry* cur_id) {
	IntBsEntry* cur = ((IntBsEntry*)cur_id);
	if (cur->right != (((void*)0))) {
		cur_id = cur->right;
		cur = ((IntBsEntry*)cur_id);
		while (cur->left != (((void*)0))) {
			cur_id = cur->left;
			(0);
			cur = ((IntBsEntry*)cur_id);
		}
		(0);
		return cur_id;
	}
	IntRbEntry* parent_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
	IntBsEntry* parent = ((IntBsEntry*)parent_id);
	while (parent_id != (((void*)0)) && cur_id == parent->right) {
		(0);
		cur = parent;
		parent_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
		parent = ((IntBsEntry*)parent_id);
	}
	return parent_id;
}
IntRbEntry* IntBsTreePrev(IntBsTree* tree, IntRbEntry* cur_id) {
	IntBsEntry* cur = ((IntBsEntry*)cur_id);
	if (cur->left != (((void*)0))) {
		cur_id = cur->left;
		cur = ((IntBsEntry*)cur_id);
		while (cur->right != (((void*)0))) {
			cur_id = cur->right;
			(0);
			cur = ((IntBsEntry*)cur_id);
		}
		(0);
		return cur_id;
	}
	IntRbEntry* parent_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
	IntBsEntry* parent = ((IntBsEntry*)parent_id);
	while (parent_id != (((void*)0)) && cur_id == parent->left) {
		(0);
		cur = parent;
		parent_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
		parent = ((IntBsEntry*)parent_id);
	}
	return parent;
}
static IntRbEntry* IntRotateLeft(IntRbEntry* sub_root_id) {
	IntRbEntry* sub_root = ((IntBsEntry*)sub_root_id);
	IntRbEntry* new_sub_root_id = sub_root->right;
	if (new_sub_root_id == (((void*)0))) {
		(0);
		return sub_root_id;
	}
	IntRbEntry* new_sub_root = ((IntBsEntry*)new_sub_root_id);
	INT_RB_TREE_ACCESSOR_SetParent(new_sub_root, INT_RB_TREE_ACCESSOR_GetParent(sub_root));
	if (INT_RB_TREE_ACCESSOR_GetParent(sub_root) != (((void*)0))) {
		if (INT_RB_TREE_ACCESSOR_GetParent(sub_root)->left == sub_root_id) {
			INT_RB_TREE_ACCESSOR_GetParent(sub_root)->left = new_sub_root_id;
		}
		else {
			INT_RB_TREE_ACCESSOR_GetParent(sub_root)->right = new_sub_root_id;
		}
	}
	INT_RB_TREE_ACCESSOR_SetParent(sub_root, new_sub_root_id);
	sub_root->right = new_sub_root->left;
	if (sub_root->right != (((void*)0))) {
		INT_RB_TREE_ACCESSOR_SetParent(sub_root->right, sub_root_id);
	}
	new_sub_root->left = sub_root_id;
	(0);
	(0);
	return new_sub_root_id;
}
static IntRbEntry* IntRotateRight(IntRbEntry* sub_root_id) {
	IntRbEntry* sub_root = ((IntBsEntry*)sub_root_id);
	IntRbEntry* new_sub_root_id = (((void*)0));
	if (new_sub_root_id == (((void*)0))) {
		(0);
		return sub_root_id;
	}
	IntRbEntry* new_sub_root = ((IntBsEntry*)new_sub_root_id);
	INT_RB_TREE_ACCESSOR_SetParent(new_sub_root, INT_RB_TREE_ACCESSOR_GetParent(sub_root));
	if (INT_RB_TREE_ACCESSOR_GetParent(sub_root) != (((void*)0))) {
		if (INT_RB_TREE_ACCESSOR_GetParent(sub_root)->left == sub_root_id) {
			INT_RB_TREE_ACCESSOR_GetParent(sub_root)->left = new_sub_root_id;
		}
		else {
			INT_RB_TREE_ACCESSOR_GetParent(sub_root)->right = new_sub_root_id;
		}
	}
	INT_RB_TREE_ACCESSOR_SetParent(sub_root, new_sub_root_id);
	sub_root->left = new_sub_root->right;
	if (sub_root->left != (((void*)0))) {
		INT_RB_TREE_ACCESSOR_SetParent(sub_root->left, sub_root_id);
	}
	new_sub_root->right = sub_root_id;
	(0);
	(0);
	return new_sub_root_id;
}
static IntRbEntry* IntGetSiblingEntry(IntRbEntry* entry_id, IntRbEntry* entry) {
	IntRbEntry* parent_id = INT_RB_TREE_ACCESSOR_GetParent(entry);
	IntRbEntry* parent = ((IntBsEntry*)parent_id);
	IntRbEntry* ret;
	if (parent->left == entry_id) {
		ret = parent->right;
	}
	else {
		ret = parent->left;
	}
	(0);
	return ret;
}
static void IntRbTreeInsertFixup(IntRbTree* tree, IntRbEntry* ins_entry_id) {
	IntRbEntry* ins_entry = ((IntBsEntry*)ins_entry_id);
	IntRbEntry* cur_id = INT_RB_TREE_ACCESSOR_GetParent(ins_entry);
	if (cur_id == (((void*)0))) {
		INT_RB_TREE_ACCESSOR_SetColor(ins_entry, kRbBlack);
		(0);
		return;
	}
	INT_RB_TREE_ACCESSOR_SetColor(ins_entry, kRbRed);
	(0);
	IntRbEntry* cur;
	while (cur_id != (((void*)0))) {
		cur = ((IntBsEntry*)cur_id);
		if (INT_RB_TREE_ACCESSOR_GetColor(cur) == kRbBlack) {
			break;
		}
		if (INT_RB_TREE_ACCESSOR_GetParent(cur) == (((void*)0))) {
			INT_RB_TREE_ACCESSOR_SetColor(cur, kRbBlack);
			break;
		}
		IntRbEntry* sibling_id = IntGetSiblingEntry(cur_id, cur);
		IntRbEntry* sibling = ((IntBsEntry*)sibling_id);
		if (sibling_id != (((void*)0)) && INT_RB_TREE_ACCESSOR_GetColor(sibling) == kRbRed) {
			INT_RB_TREE_ACCESSOR_SetColor(cur, kRbBlack);
			INT_RB_TREE_ACCESSOR_SetColor(sibling, kRbBlack);
			ins_entry_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
			ins_entry = ((IntBsEntry*)ins_entry_id);
			if (INT_RB_TREE_ACCESSOR_GetParent(ins_entry) == (((void*)0))) {
				INT_RB_TREE_ACCESSOR_SetColor(ins_entry, kRbBlack);
				break;
			}
			INT_RB_TREE_ACCESSOR_SetColor(ins_entry, kRbRed);
			cur = ins_entry;
		}
		else {
			IntRbEntry* new_sub_root_id;
			IntRbEntry* old_sub_root_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
			IntRbEntry* old_sub_root = ((IntBsEntry*)old_sub_root_id);
			if (old_sub_root->left == cur_id) {
				if (cur->right == ins_entry_id) {
					IntRotateLeft(cur_id);
				}
				new_sub_root_id = IntRotateRight(old_sub_root_id);
			}
			else {
				if (cur->left == ins_entry_id) {
					IntRotateRight(cur_id);
				}
				new_sub_root_id = IntRotateLeft(old_sub_root_id);
			}
			IntRbEntry* new_sub_root = ((IntBsEntry*)new_sub_root_id);
			INT_RB_TREE_ACCESSOR_SetColor(new_sub_root, kRbBlack);
			INT_RB_TREE_ACCESSOR_SetColor(old_sub_root, kRbRed);
			(0);
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			break;
		}
		cur_id = INT_RB_TREE_ACCESSOR_GetParent(cur);
		(0);
	}
	(0);
}
void IntRbTreeInit(IntRbTree* tree) {
	IntBsTreeInit(&tree->bs_tree);
}
IntRbEntry* IntRbTreeFind(IntRbTree* tree, int* key) {
	return IntBsTreeFind(&tree->bs_tree, key);
}
_Bool IntRbTreePut(IntRbTree* tree, IntRbEntry* put_entry_id) {
	if (!IntBsTreePut(&tree->bs_tree, put_entry_id)) {
		return 0;
	}
	IntRbTreeInsertFixup(tree, put_entry_id);
	return 1;
}





//

//
//

//

//
///*
//* newEntry代替entry挂接到其父节点下
//* newEntry的左右子节点不变
//* entry从树中摘除
//* entry的parent、left和right不变
//*/
//static void RbTreeHitchEntry(RbTree* tree, RbEntry* entry, RbEntry* newEntry) {
//    if (RbEntryGetParent(entry)) {
//        if (RbEntryGetParent(entry)->left == entry) {
//            RbEntryGetParent(entry)->left = newEntry;
//        }
//        else {
//            RbEntryGetParent(entry)->right = newEntry;
//        }
//    }
//    if (newEntry) {
//        RbEntrySetParent(newEntry, RbEntryGetParent(entry));
//    }
//    if (tree->root == entry) {
//        tree->root = newEntry;
//    }
//}
//
//

//
///*
//* 向树中删除节点后的平衡操作
//*/
//void RbTreeDeleteEntryFixup(RbTree* tree, RbEntry* deleteEntry, RbEntry* cur, RbEntry* deleteLeft, RbEntry* deleteRight, RbColor deleteColor, bool isCurLeft) {
//    if (deleteEntry) {
//        if (deleteColor == kRbRed) {
//            // 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除
//            cur = NULL;
//        }
//        // 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可
//        else if (deleteLeft) {
//            RbEntrySetColor(deleteLeft, kRbBlack);
//            cur = NULL;
//        }
//        else if (deleteRight) {
//            RbEntrySetColor(deleteRight, kRbBlack);
//            cur = NULL;
//        }
//    }
//
//    RbEntry* newSubRoot;
//    // 回溯维护删除黑色节点，即没有子节点(2节点)的情况
//    while (cur) {
//        RbEntry* sibling = isCurLeft ? cur->right : cur->left;
//        if (RbEntryGetColor(sibling) == kRbRed) {
//            // 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
//            // 旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点
//            RbEntry* oldSubRoot = RbEntryGetParent(sibling);
//            RbEntrySetColor(oldSubRoot, kRbRed);
//            RbEntrySetColor(sibling, kRbBlack);
//            if (oldSubRoot->left == sibling) {
//                newSubRoot = RotateRight(oldSubRoot);
//                sibling = oldSubRoot->left;     // 下降后挂接过来的节点
//            }
//            else {
//                newSubRoot = RotateLeft(oldSubRoot);
//                sibling = oldSubRoot->right;     // 下降后挂接过来的节点
//            }
//            if (tree->root == oldSubRoot) {
//                tree->root = newSubRoot;
//            }
//        }
//
//        // 至此兄弟节点一定为黑
//
//        // 侄子节点为红，即兄弟节点是3/4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点)
//        if (sibling->right && RbEntryGetColor(sibling->right) == kRbRed || sibling->left && RbEntryGetColor(sibling->left) == kRbRed) {
//            RbColor parentColor = RbEntryGetColor(cur);
//            RbEntrySetColor(cur, kRbBlack);
//            RbEntry* oldSubRoot = cur;
//            if (cur->left == sibling) {
//                if (!sibling->left || RbEntryGetColor(sibling->left) == kRbBlack) {
//                    RbEntrySetColor(sibling->right, kRbBlack);
//                    sibling = RotateLeft(sibling);
//                }
//                else {
//                    RbEntrySetColor(sibling->left, kRbBlack);
//                }
//                newSubRoot = RotateRight(cur);
//            }
//            else {
//                if (!sibling->right || RbEntryGetColor(sibling->right) == kRbBlack) {
//                    RbEntrySetColor(sibling->left, kRbBlack);
//                    sibling = RotateRight(sibling);
//                }
//                else {
//                    RbEntrySetColor(sibling->right, kRbBlack);
//                }
//                newSubRoot = RotateLeft(cur);
//            }
//            // 该节点会接替原先的子根节点，也要接替颜色
//            RbEntrySetColor(sibling, parentColor);
//            if (tree->root == oldSubRoot) {
//                tree->root = newSubRoot;
//            }
//            break;
//        }
//
//        if (RbEntryGetColor(cur) == kRbRed) {
//            // 父节点为红，即父节点是3/4节点，分裂下降与兄弟节点合并
//            //    |5|8|               |5|
//            //   /  |  \     ->      /   \
//            //  3   6  -9-          3   |6|8|
//            RbEntrySetColor(sibling, kRbRed);
//            RbEntrySetColor(cur, kRbBlack);
//            break;
//        }
//        else {
//            // 父节点为黑，即父节点是2节点，兄弟节点也是2节点，下降父节点与兄弟节点合并，相当于向上删除父节点，继续回溯
//            // 为什么不是3/4节点？因为黑父节点如果是3，兄弟节点是红，4的话回溯时父节点是红
//            RbEntrySetColor(sibling, kRbRed);
//        }
//        RbEntry* child = cur;
//        cur = RbEntryGetParent(cur);
//        if (cur) {
//            isCurLeft = cur->left == child;
//        }
//    }
//
//    if (tree->root && RbEntryGetColor(tree->root) == kRbRed) {
//        // 根节点染黑
//        RbEntrySetColor(tree->root, kRbBlack);
//    }
//}
//
//
///*
//* 初始化树
//*/
//void RbTreeInit(RbTree* tree, int entry_field_offset, int key_field_offset, int key_size, CmpFunc cmp_func) {
//    BsTreeInit(&tree->bst, entry_field_offset, key_field_offset, key_size, cmp_func);
//}
//
///*
//* 初始化节点
//*/
//void RbEntryInit(RbEntry* entry, RbColor color) {
//    BsEntryInit(&entry->bse);
//    RbEntrySetColor(entry, color);
//}
//
//

//bool RbTreeInsertEntryByKey(RbTree* tree, RbEntry* insertEntry) {
//    if (!BsTreeInsertEntryByKey(&tree->bst, &insertEntry->bse, NULL)) {
//        return false;
//    }
//    RbTreeInsertEntryFixup(tree, insertEntry);
//    return true;
//}
//
///*
//* 从树中删除节点
//*/
//void RbTreeDeleteEntry(RbTree* tree, RbEntry* deleteEntry) {
//    // 从entry的父节点开始回溯
//    RbEntry* cur;
//    bool isCurLeft;
//    if (deleteEntry->left != NULL && deleteEntry->right != NULL) {
//        // 有左右各有子节点，找当前节点的右子树中最小的节点，用最小节点替换到当前节点所在的位置，摘除当前节点，相当于移除了最小节点
//        RbEntry* minEntry = deleteEntry->right;
//        while (minEntry->left) {
//            minEntry = minEntry->left;
//        }
//
//        isCurLeft = RbEntryGetParent(minEntry)->left == minEntry;
//
//        // 最小节点继承待删除节点的左子树，因为最小节点肯定没有左节点，所以直接赋值
//        minEntry->left = deleteEntry->left;
//        if (deleteEntry->left) {
//            RbEntrySetParent(deleteEntry->left, minEntry);
//        }
//
//        RbEntry* oldRight = minEntry->right;
//
//        // 最小节点可能是待删除节点的右节点
//        if (deleteEntry->right != minEntry) {
//            // 将minEntry从原先的位置摘除，用其右子树代替
//            RbEntryGetParent(minEntry)->left = minEntry->right;
//            if (minEntry->right) {
//                RbEntrySetParent(minEntry->right, RbEntryGetParent(minEntry));
//            }
//            // 最小节点继承待删除节点的右子树
//            minEntry->right = deleteEntry->right;
//            if (deleteEntry->right) {
//                RbEntrySetParent(deleteEntry->right, minEntry);
//            }
//            cur = RbEntryGetParent(minEntry);
//        }
//        else {
//            cur = minEntry;
//        }
//
//        RbColor oldColor = RbEntryGetColor(minEntry);
//        RbEntrySetColor(minEntry, RbEntryGetColor(deleteEntry));
//
//        // 红黑树另外需要使entry的子节点同步为minEntry原来的子节点，在判断是否存在子节点时使用
//        deleteEntry->left = NULL;
//        deleteEntry->right = oldRight;
//        RbEntrySetColor(deleteEntry, oldColor);
//
//        // 最后进行挂接
//        RbTreeHitchEntry(tree, deleteEntry, minEntry);
//    }
//    else {
//        cur = RbEntryGetParent(deleteEntry);
//        if (cur) {
//            isCurLeft = cur->left == deleteEntry;
//        } else {
//            isCurLeft = false;
//        }
//
//        if (deleteEntry->right != NULL) {
//            // 只有右子节点
//            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->right);
//        }
//        else if (deleteEntry->left != NULL) {
//            RbTreeHitchEntry(tree, deleteEntry, deleteEntry->left);
//        }
//        else {
//            // 没有子节点，直接从父节点中摘除此节点
//            RbTreeHitchEntry(tree, deleteEntry, NULL);
//        }
//    }
//    RbTreeDeleteEntryFixup(tree, deleteEntry, cur, deleteEntry->left, deleteEntry->right, RbEntryGetColor(deleteEntry), isCurLeft);
//}
//
///*
//* 从树中按key删除节点
//* 成功返回被删除的节点，返回对应的对象，失败返回NULL
//*/
//void* RbTreeDeleteEntryByKey(RbTree* tree, void* key) {
//    void* obj = RbTreeFindEntryByKey(tree, key);
//    if (obj) {
//        RbEntry* entry = ObjectGetFieldByOffset(obj, tree->entry_field_offset, RbEntry);
//        RbTreeDeleteEntry(tree, entry);
//    }
//    return obj;
//}
//
//
//void* RbTreeFirst(RbTree* tree, RbTreeIterator* iterator) {
//    RbEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    iterator->rb_tree = tree;
//    while (cur->left)
//        cur = cur->left;
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreeLast(RbTree* tree, RbTreeIterator* iterator) {
//    RbEntry* cur = tree->root;
//    if (!cur) {
//        return NULL;
//    }
//    iterator->rb_tree = tree;
//    while (cur->right)
//        cur = cur->right;
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreeNext(RbTreeIterator* iterator) {
//    RbEntry* cur = iterator->cur_entry;
//    if (cur->right) {
//        cur = cur->right;
//        while (cur->left)
//            cur = cur->left;
//    }
//    else {
//        RbEntry* parent;
//        while ((parent = RbEntryGetParent(cur)) && cur == parent->right)
//            cur = parent;
//        cur = parent;
//    }
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
//    }
//    return NULL;
//}
//
//void* RbTreePrev(RbTreeIterator* iterator) {
//    RbEntry* cur = iterator->cur_entry;
//    if (cur->left) {
//        cur = cur->left;
//        while (cur->right)
//            cur = cur->right;
//    }
//    else {
//        RbEntry* parent;
//        while ((parent = RbEntryGetParent(cur)) && cur == parent->left)
//            cur = parent;
//        cur = parent;
//    }
//    iterator->cur_entry = cur;
//    if (cur) {
//        return ObjectGetFromFieldOffset(cur, iterator->rb_tree->entry_field_offset, void);
//    }
//    return NULL;
//}