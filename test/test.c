#include "test.h"


#include <libyuc/object.c>


LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(, int32_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_OBJECT_ALLOCATOR_DEFALUT)


//typedef struct _IntLru_Entry {
//	IntHashListEntry lru_entry;
//	int key;
//} IntLru_Entry;
//#define IntLruListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (&((IntLru_Entry*)(LRU_LIST_ENTRY))->key)
//#define IntLruListEntryAccessor IntLruListEntryAccessor
//LIBYUC_CONTAINER_LRU_LIST_DEFINE(Int, int, IntLruListEntryAccessor, LIBYUC_OBJECT_ALLOCATOR_DEFALUT, LIBYUC_OBJECT_HASHER_DEFALUT, LIBYUC_OBJECT_COMPARER_DEFALUT)


#define TEST_COMPARER_DEFALUT_Equal(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) == 0)
#define TEST_COMPARER_DEFALUT_Greater(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) > 0)
#define TEST_COMPARER_DEFALUT_Less(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) < 0)
#define TEST_COMPARER_DEFALUT_Subrrac(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16))
#define TEST_COMPARER_DEFALUT TEST_COMPARER_DEFALUT
    LIBYUC_CONTAINER_RB_TREE_DEFINE(Int, IntRbEntry*, uint32_t, int64_t, INT_RB_TREE_REFERENCER, INT_RB_TREE_ACCESSOR, LIBYUC_OBJECT_COMPARER_DEFALUT)//TEST_COMPARER_DEFALUT

    //LIBYUC_CONTAINER_AVL_TREE_DEFINE(Int, IntAvlEntry*, uint32_t, int64_t, INT_AVL_TREE_REFERENCER, INT_AVL_TREE_ACCESSOR, LIBYUC_OBJECT_COMPARER_DEFALUT)
	void IntAvlBsStackVectorResetCapacity(IntAvlBsStackVector* arr, uint32_t capacity) {
	IntAvlEntry** new_buf = ((IntAvlEntry**)MemoryAlloc(sizeof(IntAvlEntry*) * (capacity)));
	if (arr->obj_arr) {
		memcpy((void*)(new_buf), (void*)(arr->obj_arr), (sizeof(IntAvlEntry*) * arr->count));
		if (arr->is_vector_alloc) {
			(MemoryFree(arr->obj_arr));
		}
	}
	arr->is_vector_alloc = 1;
	arr->obj_arr = new_buf;
	arr->capacity = capacity;
}
void IntAvlBsStackVectorExpand(IntAvlBsStackVector* arr, uint32_t add_count) {
	uint32_t old_capacity = arr->capacity;
	uint32_t cur_capacity = old_capacity;
	uint32_t target_count = cur_capacity + add_count;
	if (cur_capacity == 0) {
		cur_capacity = 1;
	}
	while (cur_capacity < target_count) {
		cur_capacity *= 2;
	}
	IntAvlBsStackVectorResetCapacity(arr, cur_capacity);
	;
}
void IntAvlBsStackVectorInit(IntAvlBsStackVector* arr, uint32_t capacity, IntAvlEntry** external_arr) {
	arr->obj_arr = ((void*)0);
	arr->is_vector_alloc = !external_arr;
	if (capacity != 0 && !external_arr) {
		IntAvlBsStackVectorResetCapacity(arr, capacity);
	}
	else {
		arr->capacity = capacity;
		arr->obj_arr = external_arr;
	}
	arr->count = 0;
}
void IntAvlBsStackVectorRelease(IntAvlBsStackVector* arr) {
	if (arr->obj_arr && arr->is_vector_alloc) {
		(MemoryFree(arr->obj_arr));
		arr->obj_arr = ((void*)0);
	}
	arr->capacity = 0;
	arr->count = 0;
}
ptrdiff_t IntAvlBsStackVectorPushTail(IntAvlBsStackVector* arr, const IntAvlEntry** obj) {
	if (arr->capacity <= arr->count) {
		IntAvlBsStackVectorExpand(arr, 1);
	}
	memcpy((void*)(&arr->obj_arr[arr->count++]), (void*)(obj), (sizeof(IntAvlEntry*)));
	return arr->count - 1;
}
ptrdiff_t IntAvlBsStackVectorPushMultipleTail(IntAvlBsStackVector* arr, const IntAvlEntry** obj, uint32_t count) {
	if (arr->capacity <= arr->count + count) {
		IntAvlBsStackVectorExpand(arr, count);
	}
	memcpy((void*)(&arr->obj_arr[arr->count]), (void*)(obj), (sizeof(IntAvlEntry*) * count));
	arr->count += count;
	return arr->count - count;
}
IntAvlEntry** IntAvlBsStackVectorGetTail(IntAvlBsStackVector* arr) {
	if (arr->count == 0) {
		return ((void*)0);
	}
	return &arr->obj_arr[arr->count - 1];
}
IntAvlEntry** IntAvlBsStackVectorPopTail(IntAvlBsStackVector* arr) {
	if (arr->count == 0) {
		return ((void*)0);
	}
	return &arr->obj_arr[--arr->count];
}
static void IntAvlBsTreeSetNewChild(IntAvlBsTree* tree, IntAvlBsEntry* entry_parent, IntAvlEntry* entry_id, IntAvlEntry* new_entry_id) {
	if (((IntAvlEntry*)((uintptr_t)(entry_parent)->left & ~3)) == entry_id) {
		((entry_parent)->left = (uintptr_t)new_entry_id | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)entry_parent)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)entry_parent)->left) & 0x3))) & 0x3);
		;
	}
	else {
		{
			if (!(((entry_parent)->right) == entry_id)) {
				*(int*)0 = 0;
			}
		}
		;
		((entry_parent)->right = new_entry_id);
		;
	}
}
static void IntAvlBsTreeHitchEntry(IntAvlBsTree* tree, IntAvlBsEntry* entry_parent, IntAvlEntry* entry_id, IntAvlEntry* new_entry_id) {
	IntAvlBsEntry* entry = ((IntAvlBsEntry*)entry_id);
	IntAvlBsEntry* new_entry = ((IntAvlBsEntry*)new_entry_id);
	if (entry_parent != ((void*)0)) {
		IntAvlBsTreeSetNewChild(tree, entry_parent, entry_id, new_entry_id);
	}
	if (tree->root == entry_id) {
		tree->root = new_entry_id;
	}
	;
	;
}
static IntAvlEntry* IntAvlRotateLeft(IntAvlBsTree* tree, IntAvlBsEntry* sub_root_parent, IntAvlEntry* sub_root_id, IntAvlBsEntry* sub_root) {
	IntAvlEntry* new_sub_root_id = ((sub_root)->right);
	if (new_sub_root_id == (((void*)0))) {
		return sub_root_id;
	}
	IntAvlBsEntry* new_sub_root = ((IntAvlBsEntry*)new_sub_root_id);
	if (sub_root_parent != ((void*)0)) {
		IntAvlBsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
	}
	((sub_root)->right = ((IntAvlEntry*)((uintptr_t)(new_sub_root)->left & ~3)));
	;
	((new_sub_root)->left = (uintptr_t)sub_root_id | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)new_sub_root)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)new_sub_root)->left) & 0x3))) & 0x3);
	;
	;
	return new_sub_root_id;
}
static IntAvlEntry* IntAvlRotateRight(IntAvlBsTree* tree, IntAvlBsEntry* sub_root_parent, IntAvlEntry* sub_root_id, IntAvlBsEntry* sub_root) {
	IntAvlEntry* new_sub_root_id = ((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3));
	if (new_sub_root_id == (((void*)0))) {
		return sub_root_id;
	}
	IntAvlBsEntry* new_sub_root = ((IntAvlBsEntry*)new_sub_root_id);
	if (sub_root_parent != ((void*)0)) {
		IntAvlBsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
	}
	((sub_root)->left = (uintptr_t)((new_sub_root)->right) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)sub_root)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)sub_root)->left) & 0x3))) & 0x3);
	;
	((new_sub_root)->right = sub_root_id);
	;
	;
	return new_sub_root_id;
}
static void IntAvlBsEntryInit(IntAvlBsTree* tree, IntAvlBsEntry* entry) {
	((entry)->left = (uintptr_t)(((void*)0)) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3))) & 0x3);
	;
	((entry)->right = (((void*)0)));
	;
}
void IntAvlBsTreeInit(IntAvlBsTree* tree) {
	tree->root = (((void*)0));
}
IntAvlEntry* IntAvlBsTreeFind(IntAvlBsTree* tree, IntAvlBsStackVector* stack, int64_t* key) {
	int8_t status;
	IntAvlEntry* id = IntAvlBsTreeIteratorLocate(tree, stack, key, &status);
	return status == 0 ? id : (((void*)0));
}
_Bool IntAvlBsTreeInsert(IntAvlBsTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* entry_id) {
	IntAvlBsEntry* entry = ((IntAvlBsEntry*)entry_id);
	stack->count = 0;
	if (tree->root == (((void*)0))) {
		IntAvlBsEntryInit(tree, entry);
		tree->root = entry_id;
		return 1;
	}
	IntAvlEntry* cur_id = tree->root;
	IntAvlBsEntry* cur = ((void*)0);
	_Bool success = 1;
	while (cur_id != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &cur_id);
		if (cur_id == entry_id) {
			success = 0;
			break;
		}
		IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
		int64_t* cur_key = (&((IntEntry_Avl*)cur)->key);
		int64_t* entry_key = (&((IntEntry_Avl*)entry)->key);
		if ((*(cur_key) < *(entry_key))) {
			if (((cur)->right) == (((void*)0))) {
				((cur)->right = entry_id);
				;
				break;
			}
			cur_id = ((cur)->right);
		}
		else {
			if (cur_id == entry_id) break;
			if (((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) == (((void*)0))) {
				((cur)->left = (uintptr_t)entry_id | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3))) & 0x3);
				;
				break;
			}
			cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
		}
		;
	}
	if (cur);
	if (cur_id != entry_id) {
		IntAvlBsEntryInit(tree, entry);
	}
	;
	return success;
}
IntAvlEntry* IntAvlBsTreePut(IntAvlBsTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* entry_id) {
	IntAvlBsEntry* entry = ((IntAvlBsEntry*)entry_id);
	stack->count = 0;
	if (tree->root == (((void*)0))) {
		IntAvlBsEntryInit(tree, entry);
		tree->root = entry_id;
		return (((void*)0));
	}
	IntAvlEntry* cur_id = tree->root;
	IntAvlBsEntry* cur = ((void*)0);
	IntAvlEntry* old_id = (((void*)0));
	IntAvlEntry* parent_id = (((void*)0));
	while (cur_id != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &cur_id);
		cur = ((IntAvlBsEntry*)cur_id);
		int64_t* cur_key = (&((IntEntry_Avl*)cur)->key);
		int64_t* entry_key = (&((IntEntry_Avl*)entry)->key);
		if ((*(cur_key) < *(entry_key))) {
			if (((cur)->right) == (((void*)0))) {
				IntAvlBsEntryInit(tree, entry);
				((cur)->right = entry_id);
				;
				break;
			}
			parent_id = cur_id;
			cur_id = ((cur)->right);
		}
		else if ((*(cur_key) > *(entry_key))) {
			if (((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) == (((void*)0))) {
				IntAvlBsEntryInit(tree, entry);
				((cur)->left = (uintptr_t)entry_id | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3))) & 0x3);
				;
				break;
			}
			parent_id = cur_id;
			cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
		}
		else {
			IntAvlBsStackVectorPopTail(stack);
			old_id = cur_id;
			if (cur_id == entry_id) break;
			IntAvlBsEntryInit(tree, entry);
			if (parent_id != (((void*)0))) {
				IntAvlBsEntry* parent = ((IntAvlBsEntry*)parent_id);
				IntAvlBsTreeSetNewChild(tree, parent, cur_id, entry_id);
				;
			}
			else {
				tree->root = entry_id;
			}
			*entry = *cur;
			break;
		}
		;
	}
	if (cur);
	;
	return old_id;
}
IntAvlEntry* IntAvlBsTreeDelete(IntAvlBsTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* entry_id, _Bool* is_parent_left) {
	IntAvlEntry* backtrack_id;
	IntAvlBsEntry* entry = ((IntAvlBsEntry*)entry_id);
	IntAvlEntry** parent_id = IntAvlBsStackVectorGetTail(stack);
	IntAvlBsEntry* parent = ((void*)0);
	if (parent_id != ((void*)0)) {
		parent = ((IntAvlBsEntry*)*parent_id);
	}
	if (((IntAvlEntry*)((uintptr_t)(entry)->left & ~3)) != (((void*)0)) && ((entry)->right) != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &entry_id);
		IntAvlEntry** new_entry_id = IntAvlBsStackVectorGetTail(stack);
		IntAvlEntry* min_entry_id = ((entry)->right);
		IntAvlEntry* min_entry_parent_id = (((void*)0));
		IntAvlBsEntry* min_entry = ((IntAvlBsEntry*)min_entry_id);
		while (((IntAvlEntry*)((uintptr_t)(min_entry)->left & ~3)) != (((void*)0))) {
			IntAvlBsStackVectorPushTail(stack, &min_entry_id);
			min_entry_parent_id = min_entry_id;
			min_entry_id = ((IntAvlEntry*)((uintptr_t)(min_entry)->left & ~3));
			;
			min_entry = ((IntAvlBsEntry*)min_entry_id);
		}
		*new_entry_id = min_entry_id;
		IntAvlBsEntry* min_entry_parent = ((IntAvlBsEntry*)min_entry_parent_id);
		((min_entry)->left = (uintptr_t)((IntAvlEntry*)((uintptr_t)(entry)->left & ~3)) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)min_entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)min_entry)->left) & 0x3))) & 0x3);
		;
		IntAvlEntry* old_right_id = ((min_entry)->right);
		if (((entry)->right) != min_entry_id) {
			((min_entry_parent)->left = (uintptr_t)((min_entry)->right) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)min_entry_parent)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)min_entry_parent)->left) & 0x3))) & 0x3);
			;
			((min_entry)->right = ((entry)->right));
			;
			if (is_parent_left) *is_parent_left = 1;
		}
		else {
			if (is_parent_left) *is_parent_left = 0;
		}
		;
		IntAvlBsTreeHitchEntry(tree, parent, entry_id, min_entry_id);
		entry_id = min_entry_id;
		((entry)->left = (uintptr_t)(((void*)0)) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3))) & 0x3);
		;
		((entry)->right = old_right_id);
		;
	}
	else {
		if (is_parent_left) {
			if (parent != ((void*)0)) {
				*is_parent_left = ((IntAvlEntry*)((uintptr_t)(parent)->left & ~3)) == entry_id; {
					if (!(*is_parent_left || *is_parent_left == 0 && ((parent)->right) == entry_id)) {
						*(int*)0 = 0;
					}
				}
				;
			}
			else {
				*is_parent_left = 0;
			}
		}
		if (((entry)->right) != (((void*)0))) {
			IntAvlBsTreeHitchEntry(tree, parent, entry_id, ((entry)->right));
		}
		else if (((IntAvlEntry*)((uintptr_t)(entry)->left & ~3)) != (((void*)0))) {
			IntAvlBsTreeHitchEntry(tree, parent, entry_id, ((IntAvlEntry*)((uintptr_t)(entry)->left & ~3)));
		}
		else {
			IntAvlBsTreeHitchEntry(tree, parent, entry_id, (((void*)0)));
		}
	}
	if (parent_id != ((void*)0)) {
		;
	}
	return entry_id;
}
uint32_t IntAvlBsTreeGetCount(IntAvlBsTree* tree, IntAvlBsStackVector* stack) {
	size_t count = 0;
	stack->count = 0;
	IntAvlEntry* cur_id = IntAvlBsTreeIteratorFirst(tree, stack);
	while (cur_id != (((void*)0))) {
		count++;
		cur_id = IntAvlBsTreeIteratorNext(tree, stack, cur_id);
	}
	return count;
}
IntAvlEntry* IntAvlBsTreeIteratorLocate(IntAvlBsTree* tree, IntAvlBsStackVector* stack, int64_t* key, int8_t* cmp_status) {
	IntAvlEntry* cur_id = tree->root;
	stack->count = 0;
	IntAvlEntry* perv_id = (((void*)0));
	while (cur_id != (((void*)0))) {
		perv_id = cur_id;
		IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
		int64_t* cur_key = (&((IntEntry_Avl*)cur)->key);
		if ((*(cur_key) < *(key))) {
			*cmp_status = 1;
			cur_id = ((cur)->right);
		}
		else if ((*(cur_key) > *(key))) {
			*cmp_status = -1;
			cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
		}
		else {
			;
			*cmp_status = 0;
			return cur_id;
		}
		IntAvlBsStackVectorPushTail(stack, &perv_id);
		;
	}
	return perv_id;
}
IntAvlEntry* IntAvlBsTreeIteratorFirst(IntAvlBsTree* tree, IntAvlBsStackVector* stack) {
	IntAvlEntry* cur_id = tree->root;
	if (cur_id == (((void*)0))) {
		return (((void*)0));
	}
	IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
	while (((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &cur_id);
		cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
		;
		cur = ((IntAvlBsEntry*)cur_id);
	}
	;
	return cur_id;
}
IntAvlEntry* IntAvlBsTreeIteratorLast(IntAvlBsTree* tree, IntAvlBsStackVector* stack) {
	IntAvlEntry* cur_id = tree->root;
	if (cur_id == (((void*)0))) {
		return (((void*)0));
	}
	IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
	while (((cur)->right) != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &cur_id);
		cur_id = ((cur)->right);
		;
		cur = ((IntAvlBsEntry*)cur_id);
	}
	;
	return cur_id;
}
IntAvlEntry* IntAvlBsTreeIteratorNext(IntAvlBsTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* cur_id) {
	IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
	if (((cur)->right) != (((void*)0))) {
		IntAvlBsStackVectorPushTail(stack, &cur_id);
		cur_id = ((cur)->right);
		cur = ((IntAvlBsEntry*)cur_id);
		while (((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) != (((void*)0))) {
			IntAvlBsStackVectorPushTail(stack, &cur_id);
			cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
			;
			cur = ((IntAvlBsEntry*)cur_id);
		}
		;
		return cur_id;
	}
	IntAvlEntry** parent_id = IntAvlBsStackVectorPopTail(stack);
	IntAvlBsEntry* parent = ((void*)0);
	while (parent_id != ((void*)0)) {
		parent = ((IntAvlBsEntry*)*parent_id);
		if (cur_id != ((parent)->right)) break;
		;
		cur = parent;
		cur_id = *parent_id;
		parent_id = IntAvlBsStackVectorPopTail(stack);
	}
	;
	if (parent);
	if (parent_id) return *parent_id;
	return (((void*)0));
}
IntAvlEntry* IntAvlBsTreeIteratorPrev(IntAvlBsTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* cur_id) {
	IntAvlBsEntry* cur = ((IntAvlBsEntry*)cur_id);
	if (((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) != (((void*)0))) {
		cur_id = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
		cur = ((IntAvlBsEntry*)cur_id);
		while (((cur)->right) != (((void*)0))) {
			cur_id = ((cur)->right);
			;
			cur = ((IntAvlBsEntry*)cur_id);
		}
		;
		return cur_id;
	}
	IntAvlEntry** parent_id = IntAvlBsStackVectorPopTail(stack);
	IntAvlBsEntry* parent = ((void*)0);
	while (parent_id != ((void*)0)) {
		parent = ((IntAvlBsEntry*)*parent_id);
		if (cur_id != ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3))) break;
		;
		cur = parent;
		cur_id = *parent_id;
		parent_id = IntAvlBsStackVectorPopTail(stack);
	}
	;
	if (parent);
	if (parent_id) return *parent_id;
	return (((void*)0));
}
static _Bool IntRotateByBalanceFactor(IntAvlTree* tree, IntAvlEntry* parent_id, IntAvlEntry** sub_root_id_io, IntAvlEntry** sub_root_io, IntAvlEntry* deep_child_id, IntAvlEntry* deep_child, int8_t cur_bf) {
	_Bool rotate = 0;
	_Bool height_update = 1;
	IntAvlEntry* new_sub_root_id = ((void*)0);
	IntAvlEntry* sub_root = *sub_root_io;
	IntAvlEntry* parent = ((IntAvlBsEntry*)parent_id);
	IntAvlEntry* sub_root_id = *sub_root_id_io; {
		if (!(cur_bf >= -2 || cur_bf <= 2)) {
			*(int*)0 = 0;
		}
	}
	;
	if (cur_bf == 2) {
		int8_t deep_child_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)deep_child)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)deep_child)->left) & 0x3));
		if (deep_child_bf == -1) {
			IntAvlEntry* deep_child_right = ((IntAvlBsEntry*)((deep_child)->right)); {
				if (!(deep_child_right)) {
					*(int*)0 = 0;
				}
			}
			;
			int8_t right_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)deep_child_right)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)deep_child_right)->left) & 0x3));
			(deep_child_right->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child_right)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			;
			IntAvlRotateLeft(tree, sub_root, deep_child_id, deep_child);
			new_sub_root_id = IntAvlRotateRight(tree, parent, sub_root_id, sub_root);
			if (right_bf == -1) {
				(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)1 & 0x3)));
				(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			}
			else {
				(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)0 & 0x3)));
				if (right_bf == 1) {
					(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)-1 & 0x3)));
				}
				else {
					(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
				}
			}
		}
		else if (deep_child_bf == 0) {
			height_update = 0;
			new_sub_root_id = IntAvlRotateRight(tree, parent, sub_root_id, sub_root);
			(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)-1 & 0x3)));
			(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)1 & 0x3)));
		}
		else {
			new_sub_root_id = IntAvlRotateRight(tree, parent, sub_root_id, sub_root);
			(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
		}
		rotate = 1;
	}
	else if (cur_bf == -2) {
		;
		int8_t deep_child_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)deep_child)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)deep_child)->left) & 0x3));
		if (deep_child_bf == 1) {
			IntAvlEntry* deep_child_left = ((IntAvlBsEntry*)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))); {
				if (!(deep_child_left)) {
					*(int*)0 = 0;
				}
			}
			;
			int8_t left_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)deep_child_left)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)deep_child_left)->left) & 0x3));
			(deep_child_left->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child_left)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			;
			IntAvlRotateRight(tree, sub_root, deep_child_id, deep_child);
			new_sub_root_id = IntAvlRotateLeft(tree, parent, sub_root_id, sub_root);
			if (left_bf == 1) {
				(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)-1 & 0x3)));
				(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			}
			else {
				(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)0 & 0x3)));
				if (left_bf == -1) {
					(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)1 & 0x3)));
				}
				else {
					(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
				}
			}
		}
		else if (deep_child_bf == 0) {
			height_update = 0;
			new_sub_root_id = IntAvlRotateLeft(tree, parent, sub_root_id, sub_root);
			(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)1 & 0x3)));
			(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)-1 & 0x3)));
		}
		else {
			new_sub_root_id = IntAvlRotateLeft(tree, parent, sub_root_id, sub_root);
			(deep_child->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(deep_child)->left & ~3))) | ((uintptr_t)0 & 0x3)));
			(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)0 & 0x3)));
		}
		rotate = 1;
	}
	else {
		height_update = 0;
		(sub_root->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(sub_root)->left & ~3))) | ((uintptr_t)cur_bf & 0x3)));
	}
	if (rotate) {
		if (tree->root == sub_root) {
			tree->root = new_sub_root_id;
		}
		*sub_root_id_io = new_sub_root_id;
		;
		*sub_root_io = ((IntAvlBsEntry*)new_sub_root_id);
	}
	;
	return height_update;
}
static void IntAvlTreeInsertFixup(IntAvlTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* ins_entry_id) {
	IntAvlEntry** cur_id_ptr = IntAvlBsStackVectorPopTail(stack);
	IntAvlEntry* cur_id = (((void*)0));
	if (cur_id_ptr) cur_id = *cur_id_ptr;
	IntAvlEntry* child_id = ins_entry_id;
	IntAvlEntry* child = ((void*)0), * cur = ((void*)0);
	while (cur_id != (((void*)0))) {
		cur = ((IntAvlBsEntry*)cur_id);
		child = ((IntAvlBsEntry*)child_id);
		int8_t cur_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3));
		if (child_id == ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3))) cur_bf++; else cur_bf--;
		IntAvlEntry** parent_id_ptr = IntAvlBsStackVectorPopTail(stack);
		IntAvlEntry* parent_id = (((void*)0));
		if (parent_id_ptr) parent_id = *parent_id_ptr;
		if (IntRotateByBalanceFactor(tree, parent_id, &cur_id, &cur, child_id, child, cur_bf) || cur_bf == 0) {
			break;
		}
		;
		child_id = cur_id;
		child = cur;
		cur = ((void*)0);
		cur_id = parent_id;
	}
	;
	;
}
static void IntAvlTreeDeleteFixup(IntAvlTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* del_entry_id, _Bool is_parent_left) {
	IntAvlEntry** cur_id_ptr = IntAvlBsStackVectorPopTail(stack);
	IntAvlEntry* cur_id = (((void*)0));
	if (cur_id_ptr) cur_id = *cur_id_ptr;
	IntAvlEntry* child_id = del_entry_id;
	IntAvlEntry* cur = ((void*)0);
	while (cur_id != (((void*)0))) {
		cur = ((IntAvlBsEntry*)cur_id);
		int8_t cur_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)cur)->left) & 0x3));
		if (is_parent_left) {
			cur_bf--;
		}
		else {
			cur_bf++;
		}
		IntAvlEntry** parent_id_ptr = IntAvlBsStackVectorPopTail(stack);
		IntAvlEntry* parent_id = (((void*)0));
		if (parent_id_ptr) parent_id = *parent_id_ptr;
		if (cur_bf != 0) {
			IntAvlEntry* deep_child_id = is_parent_left ? ((cur)->right) : ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3));
			IntAvlEntry* deep_child = ((IntAvlBsEntry*)deep_child_id);
			if (IntRotateByBalanceFactor(tree, parent_id, &cur_id, &cur, deep_child_id, deep_child, cur_bf) == 0) {
				;
				break;
			}
			;
		}
		else {
			(cur->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(cur)->left & ~3))) | ((uintptr_t)cur_bf & 0x3)));
		}
		child_id = cur_id;
		cur_id = parent_id;
		;
		cur = ((IntAvlBsEntry*)cur_id);
		if (cur) {
			is_parent_left = ((IntAvlEntry*)((uintptr_t)(cur)->left & ~3)) == child_id;
		}
	}
	;
}
void IntAvlTreeInit(IntAvlTree* tree) {
	IntAvlBsTreeInit(&tree->bs_tree);
}
static void IntAvlTreeEntryInit(IntAvlTree* tree, IntAvlEntry* entry) {
	(entry->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(entry)->left & ~3))) | ((uintptr_t)0 & 0x3)));
}
IntAvlEntry* IntAvlTreeFind(IntAvlTree* tree, IntAvlBsStackVector* stack, int64_t* key) {
	return IntAvlBsTreeFind(&tree->bs_tree, stack, key);
}
IntAvlEntry* IntAvlTreePut(IntAvlTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* put_entry_id) {
	IntAvlEntry* entry = ((IntAvlBsEntry*)put_entry_id);
	IntAvlTreeEntryInit(tree, entry);
	;
	IntAvlEntry* old_id = IntAvlBsTreePut(&tree->bs_tree, stack, put_entry_id);
	if (old_id == (((void*)0))) IntAvlTreeInsertFixup(tree, stack, put_entry_id);
	return old_id;
}
_Bool IntAvlTreeDelete(IntAvlTree* tree, IntAvlBsStackVector* stack, IntAvlEntry* del_entry_id) {
	_Bool is_parent_left;
	IntAvlEntry* del_min_entry_id = IntAvlBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left);
	if (del_min_entry_id == (((void*)0))) {
		return 0;
	}
	if (del_min_entry_id != del_entry_id) {
		IntAvlEntry* del_entry = ((IntAvlBsEntry*)del_entry_id);
		IntAvlEntry* del_min_entry = ((IntAvlBsEntry*)del_min_entry_id);
		;
		int8_t old_bf = ((int8_t)(((uintptr_t)((IntAvlEntry*)del_min_entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)del_min_entry)->left) & 0x3));
		(del_min_entry->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(del_min_entry)->left & ~3))) | ((uintptr_t)((int8_t)(((uintptr_t)((IntAvlEntry*)del_entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)del_entry)->left) & 0x3)) & 0x3)));
		(del_entry->left = (IntAvlEntry*)(((uintptr_t)((IntAvlEntry*)((uintptr_t)(del_entry)->left & ~3))) | ((uintptr_t)old_bf & 0x3)));
	}
	IntAvlTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left);
	return 1;
}
static _Bool IntAvlTreeCheckPath(IntAvlTree* tree, IntAvlEntry* entry_id, uint32_t* cur_height) {
	if (entry_id == (((void*)0))) {
		++*cur_height;
		return 1;
	}
	_Bool correct = 0;
	IntAvlEntry* entry = ((void*)0);
	IntAvlEntry* parent = ((void*)0);
	do {
		entry = ((IntAvlBsEntry*)entry_id);
		++*cur_height;
		uint32_t left_height = *cur_height, right_height = *cur_height;
		correct = IntAvlTreeCheckPath(tree, ((IntAvlEntry*)((uintptr_t)(entry)->left & ~3)), &left_height) && IntAvlTreeCheckPath(tree, ((entry)->right), &right_height);
		correct = correct && (left_height - right_height == ((int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)entry)->left) & 0x3)));
		*cur_height = max(left_height, right_height);
	} while (0);
	if (entry);
	if (parent);
	return correct;
}
_Bool IntAvlTreeVerify(IntAvlTree* tree, IntAvlBsStackVector* stack) {
	IntAvlEntry* entry = ((IntAvlBsEntry*)tree->root);
	if (!entry) return 1;
	uint32_t cur_height = 0;
	_Bool correct = IntAvlTreeCheckPath(tree, tree->root, &cur_height);
	;
	return correct;
}

LIBYUC_CONTAINER_HASH_TABLE_DEFINE(Int, uint32_t, int64_t, int64_t, LIBYUC_OBJECT_ALLOCATOR_DEFALUT, INT_HASHTABLE_ACCESSOR, LIBYUC_OBJECT_MOVER_DEFALUT, INT_HASHTABLE_HASHER_KEY, LIBYUC_OBJECT_COMPARER_DEFALUT)



    LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(Element, int16_t)
    LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(Element, int16_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_OBJECT_ALLOCATOR_DEFALUT)

