static void BPlusRbBsTreeHitchEntry(BPlusRbBsTree* tree, int16_t entry_id, int16_t new_entry_id) {
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	BPlusRbBsEntry* new_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_entry_id);
	if (((int16_t)(((((BPlusRbEntry*)entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))) != (32767)) {
		BPlusRbBsEntry* entry_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ((int16_t)(((((BPlusRbEntry*)entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))));
		if (entry_parent->left == entry_id) {
			entry_parent->left = new_entry_id;
		}
		else {
			entry_parent->right = new_entry_id;
		}
		;
	}
	if (new_entry_id != (32767)) {
		(((BPlusRbEntry*)new_entry)->parent_color = (int16_t)(((int16_t)((int16_t)(((((BPlusRbEntry*)entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | (((RbColor)((((BPlusRbEntry*)new_entry)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
		;
	}
	if (tree->root == entry_id) {
		tree->root = new_entry_id;
	}
	;
	;
}
static int16_t BPlusRbRotateLeft(BPlusRbBsTree* tree, int16_t sub_root_id, BPlusRbBsEntry* sub_root) {
	int16_t new_sub_root_id = sub_root->right;
	if (new_sub_root_id == (32767)) {
		return sub_root_id;
	}
	BPlusRbBsEntry* new_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
	(((BPlusRbEntry*)new_sub_root)->parent_color = (int16_t)(((int16_t)((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | (((RbColor)((((BPlusRbEntry*)new_sub_root)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
	;
	if (((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))) != (32767)) {
		BPlusRbBsEntry* sub_root_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))));
		if (sub_root_parent->left == sub_root_id) {
			sub_root_parent->left = new_sub_root_id;
		}
		else {
			sub_root_parent->right = new_sub_root_id;
		}
		;
	}
	(((BPlusRbEntry*)sub_root)->parent_color = (int16_t)(((int16_t)new_sub_root_id) | (((RbColor)((((BPlusRbEntry*)sub_root)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
	;
	sub_root->right = new_sub_root->left;
	if (sub_root->right != (32767)) {
		(((BPlusRbEntry*)sub_root->right)->parent_color = (int16_t)(((int16_t)sub_root_id) | (((RbColor)((((BPlusRbEntry*)sub_root->right)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
		;
	}
	new_sub_root->left = sub_root_id;
	;
	return new_sub_root_id;
}
static int16_t BPlusRbRotateRight(BPlusRbBsTree* tree, int16_t sub_root_id, BPlusRbBsEntry* sub_root) {
	int16_t new_sub_root_id = sub_root->left;
	if (new_sub_root_id == (32767)) {
		return sub_root_id;
	}
	BPlusRbBsEntry* new_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
	(((BPlusRbEntry*)new_sub_root)->parent_color = (int16_t)(((int16_t)((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | (((RbColor)((((BPlusRbEntry*)new_sub_root)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
	;
	if (((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))) != (32767)) {
		BPlusRbBsEntry* sub_root_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ((int16_t)(((((BPlusRbEntry*)sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))));
		if (sub_root_parent->left == sub_root_id) {
			sub_root_parent->left = new_sub_root_id;
		}
		else {
			sub_root_parent->right = new_sub_root_id;
		}
		;
	}
	(((BPlusRbEntry*)sub_root)->parent_color = (int16_t)(((int16_t)new_sub_root_id) | (((RbColor)((((BPlusRbEntry*)sub_root)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
	;
	sub_root->left = new_sub_root->right;
	if (sub_root->left != (32767)) {
		(((BPlusRbEntry*)sub_root->left)->parent_color = (int16_t)(((int16_t)sub_root_id) | (((RbColor)((((BPlusRbEntry*)sub_root->left)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
		;
	}
	new_sub_root->right = sub_root_id;
	;
	return new_sub_root_id;
}
static void BPlusRbBsEntryInit(BPlusRbBsTree* tree, BPlusRbBsEntry* entry) {
	entry->left = (32767);
	entry->right = (32767);
	entry->parent = (32767);
}
void BPlusRbBsTreeInit(BPlusRbBsTree* tree) {
	tree->root = (32767);
}
int16_t BPlusRbBsTreeFind(BPlusRbBsTree* tree, Key* key, _Bool scope) {
	int16_t cur_id = tree->root;
	int16_t prev_cur_id = (32767);
	while (cur_id != (32767)) {
		prev_cur_id = cur_id;
		BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).ptr < (*key).ptr)) {
			cur_id = cur->right;
		}
		else if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).ptr > (*key).ptr)) {
			cur_id = cur->left;
		}
		else {
			;
			return cur_id;
		}
		;
	}
	return scope ? prev_cur_id : (32767);
}
_Bool BPlusRbBsTreePut(BPlusRbBsTree* tree, int16_t entry_id) {
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	BPlusRbBsEntryInit(tree, entry);
	if (tree->root == (32767)) {
		tree->root = entry_id;
		return 1;
	}
	int16_t cur_id = tree->root;
	while (cur_id != (32767)) {
		BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).ptr < (CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, entry)).ptr)) {
			if (cur->right == 32767) {
				cur->right = entry_id;
				break;
			}
			cur_id = cur->right;
		}
		else if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).ptr > (CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, entry)).ptr)) {
			if (cur->left == 32767) {
				cur->left = entry_id;
				break;
			}
			cur_id = cur->left;
		}
		else {
			;
			return 0;
		}
		;
	}
	(((BPlusRbEntry*)entry)->parent_color = (int16_t)(((int16_t)cur_id) | (((RbColor)((((BPlusRbEntry*)entry)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
	;
	;
	return 1;
}
int16_t BPlusRbBsTreeDelete(BPlusRbBsTree* tree, int16_t entry_id, _Bool* is_parent_left) {
	int16_t backtrack_id;
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	if (entry->left != (32767) && entry->right != (32767)) {
		int16_t min_entry_id = entry->right;
		BPlusRbBsEntry* min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry_id);
		while (min_entry->left != (32767)) {
			min_entry_id = min_entry->left;
			;
			min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry_id);
		}
		BPlusRbBsEntry* min_entry_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ((int16_t)(((((BPlusRbEntry*)min_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))));
		if (is_parent_left) {
			*is_parent_left = min_entry_parent->left == min_entry_id;
		}
		min_entry->left = entry->left;
		if (entry->left != (32767)) {
			BPlusRbBsEntry* entry_left = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry->left);
			(((BPlusRbEntry*)entry_left)->parent_color = (int16_t)(((int16_t)min_entry_id) | (((RbColor)((((BPlusRbEntry*)entry_left)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
			;
			;
		}
		int16_t old_right_id = min_entry->right;
		if (entry->right != min_entry_id) {
			min_entry_parent->left = min_entry->right;
			if (min_entry->right != (32767)) {
				BPlusRbBsEntry* min_entry_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry->right);
				(((BPlusRbEntry*)min_entry_right)->parent_color = (int16_t)(((int16_t)((int16_t)(((((BPlusRbEntry*)min_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | (((RbColor)((((BPlusRbEntry*)min_entry_right)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
				;
				;
			}
			min_entry->right = entry->right;
			if (entry->right != (32767)) {
				BPlusRbBsEntry* entry_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry->right);
				(((BPlusRbEntry*)entry_right)->parent_color = (int16_t)(((int16_t)min_entry) | (((RbColor)((((BPlusRbEntry*)entry_right)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
				;
				;
			}
			backtrack_id = ((int16_t)(((((BPlusRbEntry*)min_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
		}
		else {
			backtrack_id = min_entry_id;
		}
		;
		BPlusRbBsTreeHitchEntry(tree, entry, min_entry);
		entry_id = min_entry_id;
		entry->left = ((void*)0);
		entry->right = old_right_id;
		(((BPlusRbEntry*)entry)->parent_color = (int16_t)(((int16_t)backtrack_id) | (((RbColor)((((BPlusRbEntry*)entry)->parent_color) >> (sizeof(int16_t) * 8 - 1))))));
		;
	}
	else {
		if (is_parent_left) {
			int16_t parent_id = ((int16_t)(((((BPlusRbEntry*)entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
			if (parent_id != (32767)) {
				BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
				*is_parent_left = parent->left == entry_id;
				;
			}
			else {
				*is_parent_left = 0;
			}
		}
		if (entry->right != (32767)) {
			BPlusRbBsTreeHitchEntry(tree, entry_id, entry->right);
		}
		else if (entry->left != (32767)) {
			BPlusRbBsTreeHitchEntry(tree, entry_id, entry->left);
		}
		else {
			BPlusRbBsTreeHitchEntry(tree, entry_id, (32767));
		}
	}
	return entry_id;
}
size_t BPlusRbBsTreeGetCount(BPlusRbBsTree* tree) {
	size_t count = 0;
	int16_t cur_id = BPlusRbBsTreeIteratorFirst(tree);
	while (cur_id != (32767)) {
		count++;
		cur_id = BPlusRbBsTreeIteratorNext(tree, cur_id);
	}
	return count;
}
int16_t BPlusRbBsTreeIteratorFirst(BPlusRbBsTree* tree) {
	int16_t cur_id = tree->root;
	if (cur_id == (32767)) {
		return (32767);
	}
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur->left != (32767)) {
		cur_id = cur->left;
		;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	}
	;
	return cur_id;
}
int16_t BPlusRbBsTreeIteratorLast(BPlusRbBsTree* tree) {
	int16_t cur_id = tree->root;
	if (cur_id == (32767)) {
		return (32767);
	}
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur->right != (32767)) {
		cur_id = cur->right;
		;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	}
	;
	return cur_id;
}
int16_t BPlusRbBsTreeIteratorNext(BPlusRbBsTree* tree, int16_t cur_id) {
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	if (cur->right != (32767)) {
		cur_id = cur->right;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		while (cur->left != (32767)) {
			cur_id = cur->left;
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		}
		;
		return cur_id;
	}
	int16_t parent_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
	BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	while (parent_id != (32767) && cur_id == parent->right) {
		;
		cur = parent;
		parent_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
		parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	}
	;
	;
	return parent_id;
}
int16_t BPlusRbBsTreeIteratorPrev(BPlusRbBsTree* tree, int16_t cur_id) {
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	if (cur->left != (32767)) {
		cur_id = cur->left;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		while (cur->right != (32767)) {
			cur_id = cur->right;
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		}
		;
		return cur_id;
	}
	int16_t parent_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
	BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	while (parent_id != (32767) && cur_id == parent->left) {
		;
		cur = parent;
		parent_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
		parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	}
	;
	;
	return parent_id;
}
static int16_t BPlusGetSiblingEntry(BPlusRbTree* tree, int16_t entry_id, BPlusRbEntry* entry) {
	int16_t parent_id = ((int16_t)(((((BPlusRbEntry*)entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
	BPlusRbEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	int16_t ret;
	if (parent->left == entry_id) {
		ret = parent->right;
	}
	else {
		ret = parent->left;
	}
	;
	return ret;
}
static void BPlusRbTreeInsertFixup(BPlusRbTree* tree, int16_t ins_entry_id) {
	BPlusRbEntry* ins_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ins_entry_id);
	(((BPlusRbEntry*)ins_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
	int16_t cur_id = ((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
	if (cur_id == (32767)) {
		(((BPlusRbEntry*)ins_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
		;
		return;
	}
	(((BPlusRbEntry*)ins_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
	;
	BPlusRbEntry* cur = ((void*)0);
	while (cur_id != (32767)) {
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if (((RbColor)((((BPlusRbEntry*)cur)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbBlack) {
			break;
		}
		if (((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))) == (32767)) {
			(((BPlusRbEntry*)cur)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			break;
		}
		int16_t sibling_id = BPlusGetSiblingEntry(tree, cur_id, cur);
		BPlusRbEntry* sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
		if (sibling_id != (32767) && ((RbColor)((((BPlusRbEntry*)sibling)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed) {
			(((BPlusRbEntry*)cur)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			(((BPlusRbEntry*)sibling)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			ins_entry_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
			ins_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ins_entry_id);
			if (((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1)))))) == (32767)) {
				(((BPlusRbEntry*)ins_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
				break;
			}
			(((BPlusRbEntry*)ins_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)ins_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
			cur = ins_entry;
		}
		else {
			int16_t new_sub_root_id;
			int16_t old_sub_root_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
			BPlusRbEntry* old_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, old_sub_root_id);
			if (old_sub_root->left == cur_id) {
				if (cur->right == ins_entry_id) {
					BPlusRbRotateLeft(tree, cur_id, cur);
				}
				new_sub_root_id = BPlusRbRotateRight(tree, old_sub_root_id, old_sub_root);
			}
			else {
				if (cur->left == ins_entry_id) {
					BPlusRbRotateRight(tree, cur_id, cur);
				}
				new_sub_root_id = BPlusRbRotateLeft(tree, old_sub_root_id, old_sub_root);
			}
			BPlusRbEntry* new_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
			(((BPlusRbEntry*)new_sub_root)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)new_sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			(((BPlusRbEntry*)old_sub_root)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)old_sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
			;
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			break;
		}
		cur_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
		;
	}
	;
}
static void BPlusRbTreeDeleteFixup(BPlusRbTree* tree, int16_t del_entry_id, _Bool is_parent_left) {
	BPlusRbEntry* del_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
	int16_t cur_id = (32767);
	RbColor del_color = ((RbColor)((((BPlusRbEntry*)del_entry)->parent_color) >> (sizeof(int16_t) * 8 - 1)));
	if (del_color == kRbRed) {
	}
	else if (del_entry->left != (32767)) {
		(((BPlusRbEntry*)del_entry->left)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)del_entry->left)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
	}
	else if (del_entry->right != (32767)) {
		(((BPlusRbEntry*)del_entry->right)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)del_entry->right)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
	}
	else {
		cur_id = ((int16_t)(((((BPlusRbEntry*)del_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
	}
	int16_t new_sub_root_id;
	BPlusRbEntry* cur = ((void*)0), * sibling = ((void*)0);
	cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur_id != (32767)) {
		int16_t sibling_id = is_parent_left ? cur->right : cur->left;
		sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
		if (((RbColor)((((BPlusRbEntry*)sibling)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed) {
			int16_t old_sub_root_id = ((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
			BPlusRbEntry* old_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, old_sub_root_id);
			(((BPlusRbEntry*)old_sub_root)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)old_sub_root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
			(((BPlusRbEntry*)sibling)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			if (old_sub_root->left == sibling_id) {
				new_sub_root_id = BPlusRbRotateRight(tree, old_sub_root_id, old_sub_root);
				sibling_id = old_sub_root->left;
				;
				sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
			}
			else {
				new_sub_root_id = BPlusRbRotateLeft(tree, old_sub_root_id, old_sub_root);
				sibling_id = old_sub_root->right;
				;
				sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
			}
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			;
		}
		BPlusRbEntry* sibling_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling->right);
		BPlusRbEntry* sibling_left = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling->left);
		if (sibling->right != (32767) && ((RbColor)((((BPlusRbEntry*)sibling_right)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed || sibling->left != (32767) && ((RbColor)((((BPlusRbEntry*)sibling_left)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed) {
			RbColor parent_color = ((RbColor)((((BPlusRbEntry*)cur)->parent_color) >> (sizeof(int16_t) * 8 - 1)));
			(((BPlusRbEntry*)cur)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			int16_t old_sub_root_id = cur_id;
			if (cur->left == sibling_id) {
				if (sibling->left == (32767) || ((RbColor)((((BPlusRbEntry*)sibling_left)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbBlack) {
					(((BPlusRbEntry*)sibling_right)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling_right)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
					sibling_id = BPlusRbRotateLeft(tree, sibling_id, sibling);
				}
				else {
					(((BPlusRbEntry*)sibling_left)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling_left)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
				}
				new_sub_root_id = BPlusRbRotateRight(tree, cur_id, cur);
			}
			else {
				if (sibling->right == (32767) || ((RbColor)((((BPlusRbEntry*)sibling_right)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbBlack) {
					(((BPlusRbEntry*)sibling_left)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling_left)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
					sibling_id = BPlusRbRotateRight(tree, sibling_id, sibling);
				}
				else {
					(((BPlusRbEntry*)sibling_right)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling_right)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
				}
				new_sub_root_id = BPlusRbRotateLeft(tree, cur_id, cur);
			}
			;
			sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
			(((BPlusRbEntry*)sibling)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(parent_color))));
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			;
			;
			break;
		}
		;
		;
		if (((RbColor)((((BPlusRbEntry*)cur)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed) {
			(((BPlusRbEntry*)sibling)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
			(((BPlusRbEntry*)cur)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
			break;
		}
		else {
			(((BPlusRbEntry*)sibling)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)sibling)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbRed))));
		}
		int16_t child_id = cur_id;
		cur_id = ((int16_t)(((((BPlusRbEntry*)cur)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))));
		if (cur_id != (32767)) {
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
			is_parent_left = cur->left == child_id;
		}
	}
	;
	;
	BPlusRbEntry* root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, tree->root);
	if (root && ((RbColor)((((BPlusRbEntry*)root)->parent_color) >> (sizeof(int16_t) * 8 - 1))) == kRbRed) {
		(((BPlusRbEntry*)root)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)root)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(kRbBlack))));
	}
	;
}
void BPlusRbTreeInit(BPlusRbTree* tree) {
	BPlusRbBsTreeInit(&tree->bs_tree);
}
int16_t BPlusRbTreeFind(BPlusRbTree* tree, Key* key, _Bool scope) {
	return BPlusRbBsTreeFind(&tree->bs_tree, key, scope);
}
_Bool BPlusRbTreePut(BPlusRbTree* tree, int16_t put_entry_id) {
	if (!BPlusRbBsTreePut(&tree->bs_tree, put_entry_id)) {
		return 0;
	}
	BPlusRbTreeInsertFixup(tree, put_entry_id);
	return 1;
}
_Bool BPlusRbTreeDelete(BPlusRbTree* tree, int16_t del_entry_id) {
	_Bool is_parent_left;
	int16_t del_min_entry_id = BPlusRbBsTreeDelete(&tree->bs_tree, del_entry_id, &is_parent_left);
	if (del_min_entry_id == (32767)) {
		return 0;
	}
	if (del_min_entry_id != del_entry_id) {
		BPlusRbEntry* del_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
		BPlusRbEntry* del_min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_min_entry_id);
		;
		RbColor old_color = ((RbColor)((((BPlusRbEntry*)del_min_entry)->parent_color) >> (sizeof(int16_t) * 8 - 1)));
		(((BPlusRbEntry*)del_min_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)del_min_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(((RbColor)((((BPlusRbEntry*)del_entry)->parent_color) >> (sizeof(int16_t) * 8 - 1)))))));
		(((BPlusRbEntry*)del_entry)->parent_color = (int16_t)((((int16_t)(((((BPlusRbEntry*)del_entry)->parent_color) & (~(1 << (sizeof(int16_t) * 8 - 1))))))) | ((int16_t)(old_color))));
	}
	BPlusRbTreeDeleteFixup(tree, del_entry_id, is_parent_left);
	return 1;
}
int16_t BPlusRbTreeIteratorFirst(BPlusRbTree* tree) {
	return BPlusRbBsTreeIteratorFirst((BPlusRbBsTree*)tree);
}
int16_t BPlusRbTreeIteratorLast(BPlusRbTree* tree) {
	return BPlusRbBsTreeIteratorLast((BPlusRbBsTree*)tree);
}
int16_t BPlusRbTreeIteratorNext(BPlusRbTree* tree, int16_t cur_id) {
	return BPlusRbBsTreeIteratorNext((BPlusRbBsTree*)tree, cur_id);
}
int16_t BPlusRbTreeIteratorPrev(BPlusRbTree* tree, int16_t cur_id) {
	return BPlusRbBsTreeIteratorPrev((BPlusRbBsTree*)tree, cur_id);
}