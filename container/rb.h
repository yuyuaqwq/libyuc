static void BPlusRbBsTreeHitchEntry(BPlusRbBsTree* tree, int16_t entry_id, int16_t new_entry_id) {
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	BPlusRbBsEntry* new_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_entry_id);
	if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent) != (-1)) {
		BPlusRbBsEntry* entry_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, (((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent));
		if (entry_parent->left == entry_id) {
			entry_parent->left = new_entry_id;
		}
		else {
			entry_parent->right = new_entry_id;
		}
		;
	}
	if (new_entry_id != (-1)) {
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)new_entry)->parent_color))->parent = (((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent));
	}
	if (tree->root == entry_id) {
		tree->root = new_entry_id;
	}
	;
	;
}
static int16_t BPlusRbRotateLeft(BPlusRbBsTree* tree, int16_t sub_root_id, BPlusRbBsEntry* sub_root) {
	int16_t new_sub_root_id = sub_root->right;
	if (new_sub_root_id == (-1)) {
		return sub_root_id;
	}
	BPlusRbBsEntry* new_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)new_sub_root)->parent_color))->parent = (((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent));
	if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent) != (-1)) {
		BPlusRbBsEntry* sub_root_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, (((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent));
		if (sub_root_parent->left == sub_root_id) {
			sub_root_parent->left = new_sub_root_id;
		}
		else {
			sub_root_parent->right = new_sub_root_id;
		}
		;
	}
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent = new_sub_root_id);
	sub_root->right = new_sub_root->left;
	if (sub_root->right != (-1)) {
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root->right)->parent_color))->parent = sub_root_id);
	}
	new_sub_root->left = sub_root_id;
	;
	return new_sub_root_id;
}
static int16_t BPlusRbRotateRight(BPlusRbBsTree* tree, int16_t sub_root_id, BPlusRbBsEntry* sub_root) {
	int16_t new_sub_root_id = sub_root->left;
	if (new_sub_root_id == (-1)) {
		return sub_root_id;
	}
	BPlusRbBsEntry* new_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)new_sub_root)->parent_color))->parent = (((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent));
	if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent) != (-1)) {
		BPlusRbBsEntry* sub_root_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, (((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent));
		if (sub_root_parent->left == sub_root_id) {
			sub_root_parent->left = new_sub_root_id;
		}
		else {
			sub_root_parent->right = new_sub_root_id;
		}
		;
	}
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root)->parent_color))->parent = new_sub_root_id);
	sub_root->left = new_sub_root->right;
	if (sub_root->left != (-1)) {
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)sub_root->left)->parent_color))->parent = sub_root_id);
	}
	new_sub_root->right = sub_root_id;
	;
	return new_sub_root_id;
}
static void BPlusRbBsEntryInit(BPlusRbBsTree* tree, BPlusRbBsEntry* entry) {
	entry->left = (-1);
	entry->right = (-1);
	entry->parent = (-1);
}
void BPlusRbBsTreeInit(BPlusRbBsTree* tree) {
	tree->root = (-1);
}
int16_t BPlusRbBsTreeFind(BPlusRbBsTree* tree, Key* key, _Bool scope) {
	int16_t cur_id = tree->root;
	int16_t prev_cur_id = (-1);
	while (cur_id != (-1)) {
		prev_cur_id = cur_id;
		BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).data < (*key).data)) {
			cur_id = cur->right;
		}
		else if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).data > (*key).data)) {
			cur_id = cur->left;
		}
		else {
			;
			return cur_id;
		}
		;
	}
	return scope ? prev_cur_id : (-1);
}
_Bool BPlusRbBsTreePut(BPlusRbBsTree* tree, int16_t entry_id) {
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	BPlusRbBsEntryInit(tree, entry);
	if (tree->root == (-1)) {
		tree->root = entry_id;
		return 1;
	}
	int16_t cur_id = tree->root;
	while (cur_id != (-1)) {
		BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).data < (CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, entry)).data)) {
			if (cur->right == (-1)) {
				cur->right = entry_id;
				break;
			}
			cur_id = cur->right;
		}
		else if (((CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, cur)).data > (CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(tree, entry)).data)) {
			if (cur->left == (-1)) {
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
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent = cur_id);
	;
	return 1;
}
int16_t BPlusRbBsTreeDelete(BPlusRbBsTree* tree, int16_t entry_id, _Bool* is_parent_left) {
	int16_t backtrack_id;
	BPlusRbBsEntry* entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
	if (entry->left != (-1) && entry->right != (-1)) {
		int16_t min_entry_id = entry->right;
		BPlusRbBsEntry* min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry_id);
		while (min_entry->left != (-1)) {
			min_entry_id = min_entry->left;
			;
			min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry_id);
		}
		BPlusRbBsEntry* min_entry_parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, (((BPlusRbParentColor*)&(((BPlusRbEntry*)min_entry)->parent_color))->parent));
		if (is_parent_left) {
			*is_parent_left = min_entry_parent->left == min_entry_id;
		}
		min_entry->left = entry->left;
		if (entry->left != (-1)) {
			BPlusRbBsEntry* entry_left = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry->left);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)entry_left)->parent_color))->parent = min_entry_id);
			;
		}
		int16_t old_right_id = min_entry->right;
		if (entry->right != min_entry_id) {
			min_entry_parent->left = min_entry->right;
			if (min_entry->right != (-1)) {
				BPlusRbBsEntry* min_entry_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, min_entry->right);
				(((BPlusRbParentColor*)&(((BPlusRbEntry*)min_entry_right)->parent_color))->parent = (((BPlusRbParentColor*)&(((BPlusRbEntry*)min_entry)->parent_color))->parent));
				;
			}
			min_entry->right = entry->right;
			if (entry->right != (-1)) {
				BPlusRbBsEntry* entry_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry->right);
				(((BPlusRbParentColor*)&(((BPlusRbEntry*)entry_right)->parent_color))->parent = min_entry);
				;
			}
			backtrack_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)min_entry)->parent_color))->parent);
		}
		else {
			backtrack_id = min_entry_id;
		}
		;
		BPlusRbBsTreeHitchEntry(tree, entry, min_entry);
		entry_id = min_entry_id;
		entry->left = ((void*)0);
		entry->right = old_right_id;
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent = backtrack_id);
	}
	else {
		if (is_parent_left) {
			int16_t parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent);
			if (parent_id != (-1)) {
				BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
				*is_parent_left = parent->left == entry_id;
				;
			}
			else {
				*is_parent_left = 0;
			}
		}
		if (entry->right != (-1)) {
			BPlusRbBsTreeHitchEntry(tree, entry_id, entry->right);
		}
		else if (entry->left != (-1)) {
			BPlusRbBsTreeHitchEntry(tree, entry_id, entry->left);
		}
		else {
			BPlusRbBsTreeHitchEntry(tree, entry_id, (-1));
		}
	}
	return entry_id;
}
size_t BPlusRbBsTreeGetCount(BPlusRbBsTree* tree) {
	size_t count = 0;
	int16_t cur_id = BPlusRbBsTreeIteratorFirst(tree);
	while (cur_id != (-1)) {
		count++;
		cur_id = BPlusRbBsTreeIteratorNext(tree, cur_id);
	}
	return count;
}
int16_t BPlusRbBsTreeIteratorFirst(BPlusRbBsTree* tree) {
	int16_t cur_id = tree->root;
	if (cur_id == (-1)) {
		return (-1);
	}
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur->left != (-1)) {
		cur_id = cur->left;
		;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	}
	;
	return cur_id;
}
int16_t BPlusRbBsTreeIteratorLast(BPlusRbBsTree* tree) {
	int16_t cur_id = tree->root;
	if (cur_id == (-1)) {
		return (-1);
	}
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur->right != (-1)) {
		cur_id = cur->right;
		;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	}
	;
	return cur_id;
}
int16_t BPlusRbBsTreeIteratorNext(BPlusRbBsTree* tree, int16_t cur_id) {
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	if (cur->right != (-1)) {
		cur_id = cur->right;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		while (cur->left != (-1)) {
			cur_id = cur->left;
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		}
		;
		return cur_id;
	}
	int16_t parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
	BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	while (parent_id != (-1) && cur_id == parent->right) {
		;
		cur = parent;
		parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
		parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	}
	;
	;
	return parent_id;
}
int16_t BPlusRbBsTreeIteratorPrev(BPlusRbBsTree* tree, int16_t cur_id) {
	BPlusRbBsEntry* cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	if (cur->left != (-1)) {
		cur_id = cur->left;
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		while (cur->right != (-1)) {
			cur_id = cur->right;
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		}
		;
		return cur_id;
	}
	int16_t parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
	BPlusRbBsEntry* parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	while (parent_id != (-1) && cur_id == parent->left) {
		;
		cur = parent;
		parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
		parent = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, parent_id);
	}
	;
	;
	return parent_id;
}
static int16_t BPlusGetSiblingEntry(BPlusRbTree* tree, int16_t entry_id, BPlusRbEntry* entry) {
	int16_t parent_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)entry)->parent_color))->parent);
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
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->color = kRbBlack);
	int16_t cur_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->parent);
	if (cur_id == (-1)) {
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->color = kRbBlack);
		;
		return;
	}
	(((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->color = kRbRed);
	;
	BPlusRbEntry* cur = ((void*)0);
	while (cur_id != (-1)) {
		cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
		if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color) == kRbBlack) {
			break;
		}
		if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent) == (-1)) {
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color = kRbBlack);
			break;
		}
		int16_t sibling_id = BPlusGetSiblingEntry(tree, cur_id, cur);
		BPlusRbEntry* sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
		if (sibling_id != (-1) && (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color) == kRbRed) {
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color = kRbBlack);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color = kRbBlack);
			ins_entry_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
			ins_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, ins_entry_id);
			if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->parent) == (-1)) {
				(((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->color = kRbBlack);
				break;
			}
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)ins_entry)->parent_color))->color = kRbRed);
			cur = ins_entry;
		}
		else {
			int16_t new_sub_root_id;
			int16_t old_sub_root_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
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
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)new_sub_root)->parent_color))->color = kRbBlack);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)old_sub_root)->parent_color))->color = kRbRed);
			;
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			break;
		}
		cur_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
		;
	}
	;
}
static void BPlusRbTreeDeleteFixup(BPlusRbTree* tree, int16_t del_entry_id, _Bool is_parent_left) {
	BPlusRbEntry* del_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
	int16_t cur_id = (-1);
	RbColor del_color = (((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry)->parent_color))->color);
	if (del_color == kRbRed) {
	}
	else if (del_entry->left != (-1)) {
		BPlusRbEntry* del_entry_left = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry->left);
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry_left)->parent_color))->color = kRbBlack);
		;
	}
	else if (del_entry->right != (-1)) {
		BPlusRbEntry* del_entry_right = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry->right);
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry_right)->parent_color))->color = kRbBlack);
		;
	}
	else {
		cur_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry)->parent_color))->parent);
	}
	int16_t new_sub_root_id;
	BPlusRbEntry* cur = ((void*)0), * sibling = ((void*)0);
	cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
	while (cur_id != (-1)) {
		int16_t sibling_id = is_parent_left ? cur->right : cur->left;
		sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
		if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color) == kRbRed) {
			int16_t old_sub_root_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->parent);
			BPlusRbEntry* old_sub_root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, old_sub_root_id);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)old_sub_root)->parent_color))->color = kRbRed);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color = kRbBlack);
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
		if (sibling->right != (-1) && (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_right)->parent_color))->color) == kRbRed || sibling->left != (-1) && (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_left)->parent_color))->color) == kRbRed) {
			RbColor parent_color = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color = kRbBlack);
			int16_t old_sub_root_id = cur_id;
			if (cur->left == sibling_id) {
				if (sibling->left == (-1) || (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_left)->parent_color))->color) == kRbBlack) {
					(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_right)->parent_color))->color = kRbBlack);
					sibling_id = BPlusRbRotateLeft(tree, sibling_id, sibling);
				}
				else {
					(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_left)->parent_color))->color = kRbBlack);
				}
				new_sub_root_id = BPlusRbRotateRight(tree, cur_id, cur);
			}
			else {
				if (sibling->right == (-1) || (((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_right)->parent_color))->color) == kRbBlack) {
					(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_left)->parent_color))->color = kRbBlack);
					sibling_id = BPlusRbRotateRight(tree, sibling_id, sibling);
				}
				else {
					(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling_right)->parent_color))->color = kRbBlack);
				}
				new_sub_root_id = BPlusRbRotateLeft(tree, cur_id, cur);
			}
			;
			sibling = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, sibling_id);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color = parent_color);
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			;
			;
			break;
		}
		;
		;
		if ((((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color) == kRbRed) {
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color = kRbRed);
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->color = kRbBlack);
			break;
		}
		else {
			(((BPlusRbParentColor*)&(((BPlusRbEntry*)sibling)->parent_color))->color = kRbRed);
		}
		int16_t child_id = cur_id;
		cur_id = (((BPlusRbParentColor*)&(((BPlusRbEntry*)cur)->parent_color))->parent);
		if (cur_id != (-1)) {
			;
			cur = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, cur_id);
			is_parent_left = cur->left == child_id;
		}
	}
	;
	;
	BPlusRbEntry* root = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, tree->root);
	if (root && (((BPlusRbParentColor*)&(((BPlusRbEntry*)root)->parent_color))->color) == kRbRed) {
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)root)->parent_color))->color = kRbBlack);
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
	if (del_min_entry_id == (-1)) {
		return 0;
	}
	if (del_min_entry_id != del_entry_id) {
		BPlusRbEntry* del_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
		BPlusRbEntry* del_min_entry = CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, del_min_entry_id);
		;
		RbColor old_color = (((BPlusRbParentColor*)&(((BPlusRbEntry*)del_min_entry)->parent_color))->color);
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)del_min_entry)->parent_color))->color = (((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry)->parent_color))->color));
		(((BPlusRbParentColor*)&(((BPlusRbEntry*)del_entry)->parent_color))->color = old_color);
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