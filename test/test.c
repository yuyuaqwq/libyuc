#include "test.h"


#include <libyuc/basic.c>


LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(, int32_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_BASIC_ALLOCATOR_DEFALUT)


//typedef struct _IntLru_Entry {
//	IntHashListEntry lru_entry;
//	int key;
//} IntLru_Entry;
//#define IntLruListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (&((IntLru_Entry*)(LRU_LIST_ENTRY))->key)
//#define IntLruListEntryAccessor IntLruListEntryAccessor
//LIBYUC_CONTAINER_LRU_LIST_DEFINE(Int, int, IntLruListEntryAccessor, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_BASIC_HASHER_DEFALUT, LIBYUC_BASIC_COMPARER_DEFALUT)


#define TEST_COMPARER_DEFALUT_Equal(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) == 0)
#define TEST_COMPARER_DEFALUT_Greater(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) > 0)
#define TEST_COMPARER_DEFALUT_Less(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) < 0)
#define TEST_COMPARER_DEFALUT_Subrrac(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16))
#define TEST_COMPARER_DEFALUT TEST_COMPARER_DEFALUT
LIBYUC_CONTAINER_RB_TREE_DEFINE(Int, IntRbEntry*, uint32_t, int64_t, INT_RB_TREE_REFERENCER, INT_RB_TREE_ACCESSOR, LIBYUC_BASIC_COMPARER_DEFALUT)//TEST_COMPARER_DEFALUT

LIBYUC_CONTAINER_AVL_TREE_DEFINE(Int, IntAvlEntry*, uint32_t, int64_t, INT_AVL_TREE_REFERENCER, INT_AVL_TREE_ACCESSOR, LIBYUC_BASIC_COMPARER_DEFALUT)
	

LIBYUC_CONTAINER_HASH_TABLE_DEFINE(Int, uint32_t, int64_t, int64_t, LIBYUC_BASIC_ALLOCATOR_DEFALUT, INT_HASHTABLE_ACCESSOR, LIBYUC_BASIC_MOVER_DEFALUT, INT_HASHTABLE_HASHER_KEY, LIBYUC_BASIC_COMPARER_DEFALUT)



LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(Element, int16_t)
LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(Element, int16_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_BASIC_ALLOCATOR_DEFALUT)






#define element_max_count 100

typedef struct _Int_BPlusElement {
	union {
		uint16_t next;
		struct {
			IntBPlusElement element;
		};
	};
} Int_BPlusElement;
LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(IntBPlus, uint16_t, Int_BPlusElement, 1, element_max_count)
LIBYUC_CONTAINER_STATIC_LIST_DEFINE(IntBPlus, uint16_t, Int_BPlusElement, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, 1)



typedef struct _Int_BPlusEntryExtend {
	uint16_t count;
	IntBPlusStaticList static_list;
} Int_BPlusEntryExtend;

#define TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY) ((Int_BPlusEntryExtend*)((ENTRY)->type == kBPlusEntryLeaf ? (uintptr_t)(ENTRY)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(ENTRY)+sizeof(IntBPlusIndexEntry)))



struct {
	Int_BPlusEntryExtend extend;
	IntBPlusEntry entry;
} temp_bplus_entry;



#define TEST_BPLUS_ENTRY_ACCESSOR_GetTempCopyEntry(TREE, ENTRY) (memcpy(&temp_bplus_entry, (ENTRY), sizeof(Int_BPlusEntryExtend) + ((ENTRY)->type == kBPlusEntryLeaf ? sizeof(IntBPlusLeafEntry):  sizeof(IntBPlusIndexEntry))), &temp_bplus_entry.entry)
#define TEST_BPLUS_ENTRY_ACCESSOR_GetMergeThresholdRate(TREE, ENTRY) (element_max_count * 40 / 100)
#define TEST_BPLUS_ENTRY_ACCESSOR_GetFreeRate(TREE, ENTRY) (element_max_count - TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
#define TEST_BPLUS_ENTRY_ACCESSOR_GetFillRate(TREE, ENTRY) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
#define TEST_BPLUS_ENTRY_ACCESSOR_GetMaxRate(TREE, ENTRY) (element_max_count)
#define TEST_BPLUS_ENTRY_ACCESSOR TEST_BPLUS_ENTRY_ACCESSOR

forceinline IntBPlusEntry* TEST_BPLUS_ENTRY_ALLOCATOR_CreateBySize(IntBPlusTree* tree, size_t SIZE) {
	IntBPlusEntry* entry = (IntBPlusEntry*)MemoryAlloc(sizeof(Int_BPlusEntryExtend) + SIZE);
	Int_BPlusEntryExtend* exptend = TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(entry);
	exptend->count = 0;
	IntBPlusStaticListInit(&exptend->static_list, element_max_count);
	return (IntBPlusEntry*)((uintptr_t)entry + sizeof(Int_BPlusEntryExtend));
}
#define TEST_BPLUS_ENTRY_ALLOCATOR TEST_BPLUS_ENTRY_ALLOCATOR





#define TEST_BPLUS_ELEMENT_ACCESSOR_SetKey(ENTRY, ELEMENT)
#define TEST_BPLUS_ELEMENT_ACCESSOR_SetValue(ENTRY, ELEMENT)
#define TEST_BPLUS_ELEMENT_ACCESSOR_GetNeedRate(ENTRY, ELEMENT) 1
#define TEST_BPLUS_ELEMENT_ACCESSOR TEST_BPLUS_ELEMENT_ACCESSOR

#define TEST_BPLUS_ELEMENT_ALLOCATOR_CreateBySize(ENTRY, SIZE) (IntBPlusStaticListPop(&((Int_BPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(Int_BPlusEntryExtend)))->static_list, 0))
#define TEST_BPLUS_ELEMENT_ALLOCATOR_Release(ENTRY, ELEMENT) (IntBPlusStaticListPush(&((Int_BPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(Int_BPlusEntryExtend)))->static_list, 0, ELEMENT))
#define TEST_BPLUS_ELEMENT_ALLOCATOR TEST_BPLUS_ELEMENT_ALLOCATOR




#define TEST_BPLUS_ELEMENT_REFERENCER_Dereference(ENTRY, ELEMENT) 
IntBPlusElement* TEST_BPLUS_ELEMENT_REFERENCER_Reference(IntBPlusEntry* ENTRY, int16_t ELEMENT_ID) {
	return &TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->static_list.obj_arr[ELEMENT_ID].element;
}
#define TEST_BPLUS_ELEMENT_REFERENCER_InvalidId (-1)
#define TEST_BPLUS_ELEMENT_REFERENCER TEST_BPLUS_ELEMENT_REFERENCER
//LIBYUC_CONTAINER_BPLUS_TREE_DEFINE(Int, 
//	LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK, 
//	struct _IntBPlusEntry*, 
//	size_t, 
//	int16_t, 
//	int16_t, 
//	int64_t, 
//	int64_t, 
//	TEST_BPLUS_ENTRY_ALLOCATOR,
//	TEST_BPLUS_ENTRY_ACCESSOR, 
//	LIBYUC_BASIC_ALLOCATOR_DEFALUT, 
//	LIBYUC_BASIC_REFERENCER_DEFALUT, 
//	TEST_BPLUS_ELEMENT_ACCESSOR, 
//	TEST_BPLUS_ELEMENT_ALLOCATOR, 
//	TEST_BPLUS_ELEMENT_REFERENCER, 
//	LIBYUC_BASIC_COMPARER_DEFALUT, 
//	32)
//
static const struct _IntBPlusEntry* IntBPlusLeafEntryReferencer_InvalidId = (((void*)0));
_Bool IntBPlusIteratorStackVectorIsEmpty(IntBPlusIteratorStackVector* vector) {
	return vector->count == 0;
}
size_t IntBPlusIteratorStackVectorGetCapacity(IntBPlusIteratorStackVector* vector) {
	return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]);
}
size_t IntBPlusIteratorStackVectorGetCount(IntBPlusIteratorStackVector* vector) {
	return vector->count;
}
void IntBPlusIteratorStackVectorSetCount(IntBPlusIteratorStackVector* vector, size_t count) {
	vector->count = count;
}
IntBPlusElementPos* IntBPlusIteratorStackVectorGetArray(IntBPlusIteratorStackVector* vector) {
	return vector->obj_arr;
}
IntBPlusElementPos* IntBPlusIteratorStackVectorIndex(IntBPlusIteratorStackVector* vector, size_t index) {
	return &vector->obj_arr[index];
}
void IntBPlusIteratorStackVectorInit(IntBPlusIteratorStackVector* vector) {
	IntBPlusIteratorStackVectorSetCount(vector, 0);
}
void IntBPlusIteratorStackVectorRelease(IntBPlusIteratorStackVector* vector) {
}
IntBPlusElementPos* IntBPlusIteratorStackVectorPushTail(IntBPlusIteratorStackVector* vector, const IntBPlusElementPos* obj) {
	if (IntBPlusIteratorStackVectorGetCapacity(vector) <= IntBPlusIteratorStackVectorGetCount(vector)) {
		return ((void*)0);
	}
	memcpy((void*)(IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector))), (const void*)(obj), (sizeof(IntBPlusElementPos)));
	IntBPlusIteratorStackVectorSetCount(vector, IntBPlusIteratorStackVectorGetCount(vector) + 1);
	return IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector) - 1);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorPushMultipleTail(IntBPlusIteratorStackVector* vector, const IntBPlusElementPos* obj, size_t count) {
	if (IntBPlusIteratorStackVectorGetCapacity(vector) <= IntBPlusIteratorStackVectorGetCount(vector) + count) {
		return ((void*)0);
	}
	memcpy((void*)(IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector))), (const void*)(obj), (sizeof(IntBPlusElementPos) * count));
	IntBPlusIteratorStackVectorSetCount(vector, IntBPlusIteratorStackVectorGetCount(vector) + count);
	return IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector) - count);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorGetTail(IntBPlusIteratorStackVector* vector) {
	if (IntBPlusIteratorStackVectorGetCount(vector) == 0) {
		return ((void*)0);
	}
	return IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector) - 1);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorPopTail(IntBPlusIteratorStackVector* vector) {
	if (IntBPlusIteratorStackVectorGetCount(vector) == 0) {
		return ((void*)0);
	}
	IntBPlusIteratorStackVectorSetCount(vector, IntBPlusIteratorStackVectorGetCount(vector) - 1);
	return IntBPlusIteratorStackVectorIndex(vector, IntBPlusIteratorStackVectorGetCount(vector));
}
size_t IntBPlusIteratorStackVectorArrayFind(IntBPlusElementPos* array, size_t count, const IntBPlusElementPos* key) {
	for (size_t i = 0; i < count; i++) {
		if ((0)) return (size_t)i;
	}
	return (-1);
}
void IntBPlusIteratorStackVectorArrayInsert(IntBPlusElementPos* array, size_t count, size_t index, IntBPlusElementPos* element) {
	for (size_t i = count; i > index; i--) {
		array[i] = array[i - 1];
	}
	array[index] = *element;
}
void IntBPlusIteratorStackVectorArrayDelete(IntBPlusElementPos* array, size_t count, size_t index) {
	for (size_t i = index; i < count - 1; i++) {
		array[i] = array[i + 1];
	}
}
IntBPlusElementPos* IntBPlusIteratorStackVectorPut(IntBPlusIteratorStackVector* vector, const IntBPlusElementPos* obj) {
	const IntBPlusElementPos* key = (obj);
	size_t index = IntBPlusIteratorStackVectorArrayFind(IntBPlusIteratorStackVectorGetArray(vector), IntBPlusIteratorStackVectorGetCount(vector), key);
	if (index == (-1)) {
		return IntBPlusIteratorStackVectorPushTail(vector, obj);
	}
	else {
		IntBPlusElementPos* ele = IntBPlusIteratorStackVectorIndex(vector, index);
		memcpy((void*)(ele), (const void*)(obj), (sizeof(IntBPlusElementPos)));
		return ele;
	}
}
IntBPlusElementPos* IntBPlusIteratorStackVectorIteratorLocate(IntBPlusIteratorStackVector* vector, IntBPlusIteratorStackVectorIterator* iter, const IntBPlusElementPos* key) {
	iter->cur_index = IntBPlusIteratorStackVectorArrayFind(IntBPlusIteratorStackVectorGetArray(vector), IntBPlusIteratorStackVectorGetCount(vector), key);
	return IntBPlusIteratorStackVectorIteratorIndex(vector, iter);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorIteratorIndex(IntBPlusIteratorStackVector* vector, IntBPlusIteratorStackVectorIterator* iter) {
	if (iter->cur_index == (-1) || iter->cur_index >= IntBPlusIteratorStackVectorGetCount(vector)) return ((void*)0);
	return IntBPlusIteratorStackVectorIndex(vector, iter->cur_index);
}
void IntBPlusIteratorStackVectorIteratorDelete(IntBPlusIteratorStackVector* vector, IntBPlusIteratorStackVectorIterator* iter) {
	{
		if (!(IntBPlusIteratorStackVectorGetCount(vector) > 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	; {
		if (!(iter->cur_index < IntBPlusIteratorStackVectorGetCount(vector))) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusIteratorStackVectorArrayDelete(IntBPlusIteratorStackVectorGetArray(vector), IntBPlusIteratorStackVectorGetCount(vector), iter->cur_index);
	IntBPlusIteratorStackVectorSetCount(vector, IntBPlusIteratorStackVectorGetCount(vector) - 1);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorIteratorFirst(IntBPlusIteratorStackVector* vector, IntBPlusIteratorStackVectorIterator* iter) {
	iter->cur_index = 0;
	return IntBPlusIteratorStackVectorGetCount(vector) > 0 ? IntBPlusIteratorStackVectorIndex(vector, 0) : ((void*)0);
}
IntBPlusElementPos* IntBPlusIteratorStackVectorIteratorNext(IntBPlusIteratorStackVector* vector, IntBPlusIteratorStackVectorIterator* iter) {
	if (iter->cur_index == (-1) || iter->cur_index >= IntBPlusIteratorStackVectorGetCount(vector)) return ((void*)0);
	++iter->cur_index;
	return IntBPlusIteratorStackVectorIndex(vector, iter->cur_index);
}
static const int16_t IntBPlusEntryRbReferencer_InvalidId = (-1);
__forceinline IntBPlusEntryRbEntry* IntBPlusEntryRbReferencer_Reference(IntBPlusEntryRbTree* tree, int16_t element_id) {
	if (element_id == IntBPlusEntryRbReferencer_InvalidId) {
		return ((void*)0);
	}
	IntBPlusEntry* entry = ((IntBPlusEntry*)((uintptr_t)(tree)-((uintptr_t) & (((IntBPlusEntry*)0)->rb_tree))));
	return &(TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, element_id)->rb_entry);
}
__forceinline void IntBPlusEntryRbReferencer_Dereference(IntBPlusEntryRbTree* tree, IntBPlusEntryRbEntry* rb_entry) {
	IntBPlusEntry* entry = ((IntBPlusEntry*)((uintptr_t)(tree)-((uintptr_t) & (((IntBPlusEntry*)0)->rb_tree))));
	IntBPlusElement* element = ((IntBPlusElement*)((uintptr_t)(rb_entry)-((uintptr_t) & (((IntBPlusElement*)0)->rb_entry))));
	;
}
typedef struct {
	int16_t color : 1;
	int16_t left : sizeof(int16_t) * 8 - 1;
}
IntBPlusEntryRbParentColor;
__forceinline int16_t IntBPlusEntryRbAccessor_GetLeft(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
	return (((IntBPlusEntryRbParentColor*)&(((IntBPlusEntryRbEntry*)bs_entry)->left))->left);
}
__forceinline RbColor IntBPlusEntryRbAccessor_GetColor(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
	return (((IntBPlusEntryRbParentColor*)&(((IntBPlusEntryRbEntry*)bs_entry)->left))->color == -1 ? 1 : 0);
}
__forceinline void IntBPlusEntryRbAccessor_SetLeft(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry, int16_t new_id) {
	((IntBPlusEntryRbParentColor*)&(((IntBPlusEntryRbEntry*)bs_entry)->left))->left = new_id;
}
__forceinline void IntBPlusEntryRbAccessor_SetColor(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry, RbColor new_color) {
	return ((IntBPlusEntryRbParentColor*)&(((IntBPlusEntryRbEntry*)bs_entry)->left))->color = new_color;
}
__forceinline int16_t IntBPlusEntryRbAccessor_GetRight(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
	return bs_entry->right;
}
__forceinline void IntBPlusEntryRbAccessor_SetRight(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry, int16_t new_id) {
	bs_entry->right = new_id;
}
__forceinline int64_t* IntBPlusEntryRbAccessor_GetKey(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
	IntBPlusElement* element = ((IntBPlusElement*)((uintptr_t)(bs_entry)-((uintptr_t) & (((IntBPlusElement*)0)->rb_entry))));
	return &element->key;
}
_Bool IntBPlusEntryRbBsStackVectorIsEmpty(IntBPlusEntryRbBsStackVector* vector) {
	return vector->count == 0;
}
int16_t IntBPlusEntryRbBsStackVectorGetCapacity(IntBPlusEntryRbBsStackVector* vector) {
	return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]);
}
int16_t IntBPlusEntryRbBsStackVectorGetCount(IntBPlusEntryRbBsStackVector* vector) {
	return vector->count;
}
void IntBPlusEntryRbBsStackVectorSetCount(IntBPlusEntryRbBsStackVector* vector, int16_t count) {
	vector->count = count;
}
int16_t* IntBPlusEntryRbBsStackVectorGetArray(IntBPlusEntryRbBsStackVector* vector) {
	return vector->obj_arr;
}
int16_t* IntBPlusEntryRbBsStackVectorIndex(IntBPlusEntryRbBsStackVector* vector, int16_t index) {
	return &vector->obj_arr[index];
}
void IntBPlusEntryRbBsStackVectorInit(IntBPlusEntryRbBsStackVector* vector) {
	IntBPlusEntryRbBsStackVectorSetCount(vector, 0);
}
void IntBPlusEntryRbBsStackVectorRelease(IntBPlusEntryRbBsStackVector* vector) {
}
int16_t* IntBPlusEntryRbBsStackVectorPushTail(IntBPlusEntryRbBsStackVector* vector, const int16_t* obj) {
	if (IntBPlusEntryRbBsStackVectorGetCapacity(vector) <= IntBPlusEntryRbBsStackVectorGetCount(vector)) {
		return ((void*)0);
	}
	memcpy((void*)(IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector))), (const void*)(obj), (sizeof(int16_t)));
	IntBPlusEntryRbBsStackVectorSetCount(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) + 1);
	return IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) - 1);
}
int16_t* IntBPlusEntryRbBsStackVectorPushMultipleTail(IntBPlusEntryRbBsStackVector* vector, const int16_t* obj, int16_t count) {
	if (IntBPlusEntryRbBsStackVectorGetCapacity(vector) <= IntBPlusEntryRbBsStackVectorGetCount(vector) + count) {
		return ((void*)0);
	}
	memcpy((void*)(IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector))), (const void*)(obj), (sizeof(int16_t) * count));
	IntBPlusEntryRbBsStackVectorSetCount(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) + count);
	return IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) - count);
}
int16_t* IntBPlusEntryRbBsStackVectorGetTail(IntBPlusEntryRbBsStackVector* vector) {
	if (IntBPlusEntryRbBsStackVectorGetCount(vector) == 0) {
		return ((void*)0);
	}
	return IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) - 1);
}
int16_t* IntBPlusEntryRbBsStackVectorPopTail(IntBPlusEntryRbBsStackVector* vector) {
	if (IntBPlusEntryRbBsStackVectorGetCount(vector) == 0) {
		return ((void*)0);
	}
	IntBPlusEntryRbBsStackVectorSetCount(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) - 1);
	return IntBPlusEntryRbBsStackVectorIndex(vector, IntBPlusEntryRbBsStackVectorGetCount(vector));
}
int16_t IntBPlusEntryRbBsStackVectorArrayFind(int16_t* array, int16_t count, const int16_t* key) {
	for (int16_t i = 0; i < count; i++) {
		if ((0)) return (int16_t)i;
	}
	return (-1);
}
void IntBPlusEntryRbBsStackVectorArrayInsert(int16_t* array, int16_t count, int16_t index, int16_t* element) {
	for (int16_t i = count; i > index; i--) {
		array[i] = array[i - 1];
	}
	array[index] = *element;
}
void IntBPlusEntryRbBsStackVectorArrayDelete(int16_t* array, int16_t count, int16_t index) {
	for (int16_t i = index; i < count - 1; i++) {
		array[i] = array[i + 1];
	}
}
int16_t* IntBPlusEntryRbBsStackVectorPut(IntBPlusEntryRbBsStackVector* vector, const int16_t* obj) {
	const int16_t* key = (obj);
	int16_t index = IntBPlusEntryRbBsStackVectorArrayFind(IntBPlusEntryRbBsStackVectorGetArray(vector), IntBPlusEntryRbBsStackVectorGetCount(vector), key);
	if (index == (-1)) {
		return IntBPlusEntryRbBsStackVectorPushTail(vector, obj);
	}
	else {
		int16_t* ele = IntBPlusEntryRbBsStackVectorIndex(vector, index);
		memcpy((void*)(ele), (const void*)(obj), (sizeof(int16_t)));
		return ele;
	}
}
int16_t* IntBPlusEntryRbBsStackVectorIteratorLocate(IntBPlusEntryRbBsStackVector* vector, IntBPlusEntryRbBsStackVectorIterator* iter, const int16_t* key) {
	iter->cur_index = IntBPlusEntryRbBsStackVectorArrayFind(IntBPlusEntryRbBsStackVectorGetArray(vector), IntBPlusEntryRbBsStackVectorGetCount(vector), key);
	return IntBPlusEntryRbBsStackVectorIteratorIndex(vector, iter);
}
int16_t* IntBPlusEntryRbBsStackVectorIteratorIndex(IntBPlusEntryRbBsStackVector* vector, IntBPlusEntryRbBsStackVectorIterator* iter) {
	if (iter->cur_index == (-1) || iter->cur_index >= IntBPlusEntryRbBsStackVectorGetCount(vector)) return ((void*)0);
	return IntBPlusEntryRbBsStackVectorIndex(vector, iter->cur_index);
}
void IntBPlusEntryRbBsStackVectorIteratorDelete(IntBPlusEntryRbBsStackVector* vector, IntBPlusEntryRbBsStackVectorIterator* iter) {
	{
		if (!(IntBPlusEntryRbBsStackVectorGetCount(vector) > 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	; {
		if (!(iter->cur_index < IntBPlusEntryRbBsStackVectorGetCount(vector))) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusEntryRbBsStackVectorArrayDelete(IntBPlusEntryRbBsStackVectorGetArray(vector), IntBPlusEntryRbBsStackVectorGetCount(vector), iter->cur_index);
	IntBPlusEntryRbBsStackVectorSetCount(vector, IntBPlusEntryRbBsStackVectorGetCount(vector) - 1);
}
int16_t* IntBPlusEntryRbBsStackVectorIteratorFirst(IntBPlusEntryRbBsStackVector* vector, IntBPlusEntryRbBsStackVectorIterator* iter) {
	iter->cur_index = 0;
	return IntBPlusEntryRbBsStackVectorGetCount(vector) > 0 ? IntBPlusEntryRbBsStackVectorIndex(vector, 0) : ((void*)0);
}
int16_t* IntBPlusEntryRbBsStackVectorIteratorNext(IntBPlusEntryRbBsStackVector* vector, IntBPlusEntryRbBsStackVectorIterator* iter) {
	if (iter->cur_index == (-1) || iter->cur_index >= IntBPlusEntryRbBsStackVectorGetCount(vector)) return ((void*)0);
	++iter->cur_index;
	return IntBPlusEntryRbBsStackVectorIndex(vector, iter->cur_index);
}
static void IntBPlusEntryRbBsTreeSetNewChild(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsEntry* entry_parent, int16_t entry_id, int16_t new_entry_id) {
	if (IntBPlusEntryRbAccessor_GetLeft(tree, entry_parent) == entry_id) {
		IntBPlusEntryRbAccessor_SetLeft(tree, entry_parent, new_entry_id);
	}
	else {
		{
			if (!(IntBPlusEntryRbAccessor_GetRight(tree, entry_parent) == entry_id)) {
				{
					printf("");
					__debugbreak();
				}
			}
		}
		;
		IntBPlusEntryRbAccessor_SetRight(tree, entry_parent, new_entry_id);
	}
}
static void IntBPlusEntryRbBsTreeHitchEntry(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsEntry* entry_parent, int16_t entry_id, int16_t new_entry_id) {
	IntBPlusEntryRbBsEntry* entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
	IntBPlusEntryRbBsEntry* new_entry = IntBPlusEntryRbReferencer_Reference(tree, new_entry_id);
	if (entry_parent != ((void*)0)) {
		IntBPlusEntryRbBsTreeSetNewChild(tree, entry_parent, entry_id, new_entry_id);
	}
	if (tree->root == entry_id) {
		tree->root = new_entry_id;
	}
	IntBPlusEntryRbReferencer_Dereference(tree, entry);
	IntBPlusEntryRbReferencer_Dereference(tree, new_entry);
}
static int16_t IntBPlusEntryRbRotateLeft(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsEntry* sub_root_parent, int16_t sub_root_id, IntBPlusEntryRbBsEntry* sub_root) {
	int16_t new_sub_root_id = IntBPlusEntryRbAccessor_GetRight(tree, sub_root);
	if (new_sub_root_id == IntBPlusEntryRbReferencer_InvalidId) {
		return sub_root_id;
	}
	IntBPlusEntryRbBsEntry* new_sub_root = IntBPlusEntryRbReferencer_Reference(tree, new_sub_root_id);
	if (sub_root_parent != ((void*)0)) {
		IntBPlusEntryRbBsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
	}
	IntBPlusEntryRbAccessor_SetRight(tree, sub_root, IntBPlusEntryRbAccessor_GetLeft(tree, new_sub_root));
	IntBPlusEntryRbAccessor_SetLeft(tree, new_sub_root, sub_root_id);
	IntBPlusEntryRbReferencer_Dereference(tree, new_sub_root);
	return new_sub_root_id;
}
static int16_t IntBPlusEntryRbRotateRight(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsEntry* sub_root_parent, int16_t sub_root_id, IntBPlusEntryRbBsEntry* sub_root) {
	int16_t new_sub_root_id = IntBPlusEntryRbAccessor_GetLeft(tree, sub_root);
	if (new_sub_root_id == IntBPlusEntryRbReferencer_InvalidId) {
		return sub_root_id;
	}
	IntBPlusEntryRbBsEntry* new_sub_root = IntBPlusEntryRbReferencer_Reference(tree, new_sub_root_id);
	if (sub_root_parent != ((void*)0)) {
		IntBPlusEntryRbBsTreeSetNewChild(tree, sub_root_parent, sub_root_id, new_sub_root_id);
	}
	IntBPlusEntryRbAccessor_SetLeft(tree, sub_root, IntBPlusEntryRbAccessor_GetRight(tree, new_sub_root));
	IntBPlusEntryRbAccessor_SetRight(tree, new_sub_root, sub_root_id);
	IntBPlusEntryRbReferencer_Dereference(tree, new_sub_root);
	return new_sub_root_id;
}
static void IntBPlusEntryRbBsEntryInit(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsEntry* entry) {
	IntBPlusEntryRbAccessor_SetLeft(tree, entry, IntBPlusEntryRbReferencer_InvalidId);
	IntBPlusEntryRbAccessor_SetRight(tree, entry, IntBPlusEntryRbReferencer_InvalidId);
}
void IntBPlusEntryRbBsTreeInit(IntBPlusEntryRbBsTree* tree) {
	tree->root = IntBPlusEntryRbReferencer_InvalidId;
}
int16_t IntBPlusEntryRbBsTreeFind(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int64_t* key) {
	int8_t status;
	int16_t id = IntBPlusEntryRbBsTreeIteratorLocate(tree, stack, key, &status);
	return status == 0 ? id : IntBPlusEntryRbReferencer_InvalidId;
}
_Bool IntBPlusEntryRbBsTreeInsert(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t entry_id) {
	IntBPlusEntryRbBsEntry* entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
	stack->count = 0;
	if (tree->root == IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsEntryInit(tree, entry);
		tree->root = entry_id;
		return 1;
	}
	int16_t cur_id = tree->root;
	IntBPlusEntryRbBsEntry* cur = ((void*)0);
	_Bool success = 1;
	while (cur_id != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
		if (cur_id == entry_id) {
			success = 0;
			break;
		}
		IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		int64_t* cur_key = IntBPlusEntryRbAccessor_GetKey(tree, cur);
		int64_t* entry_key = IntBPlusEntryRbAccessor_GetKey(tree, entry);
		if ((*(cur_key) < *(entry_key))) {
			if (IntBPlusEntryRbAccessor_GetRight(tree, cur) == IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbAccessor_SetRight(tree, cur, entry_id);
				break;
			}
			cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
		}
		else {
			if (cur_id == entry_id) break;
			if (IntBPlusEntryRbAccessor_GetLeft(tree, cur) == IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbAccessor_SetLeft(tree, cur, entry_id);
				break;
			}
			cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
		}
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
	}
	if (cur) IntBPlusEntryRbReferencer_Dereference(tree, cur);
	if (cur_id != entry_id) {
		IntBPlusEntryRbBsEntryInit(tree, entry);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, entry);
	return success;
}
int16_t IntBPlusEntryRbBsTreePut(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t entry_id) {
	IntBPlusEntryRbBsEntry* entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
	stack->count = 0;
	if (tree->root == IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsEntryInit(tree, entry);
		tree->root = entry_id;
		return IntBPlusEntryRbReferencer_InvalidId;
	}
	int16_t cur_id = tree->root;
	IntBPlusEntryRbBsEntry* cur = ((void*)0);
	int16_t old_id = IntBPlusEntryRbReferencer_InvalidId;
	int16_t parent_id = IntBPlusEntryRbReferencer_InvalidId;
	while (cur_id != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
		cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		int64_t* cur_key = IntBPlusEntryRbAccessor_GetKey(tree, cur);
		int64_t* entry_key = IntBPlusEntryRbAccessor_GetKey(tree, entry);
		if ((*(cur_key) < *(entry_key))) {
			if (IntBPlusEntryRbAccessor_GetRight(tree, cur) == IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbBsEntryInit(tree, entry);
				IntBPlusEntryRbAccessor_SetRight(tree, cur, entry_id);
				break;
			}
			parent_id = cur_id;
			cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
		}
		else if ((*(cur_key) > *(entry_key))) {
			if (IntBPlusEntryRbAccessor_GetLeft(tree, cur) == IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbBsEntryInit(tree, entry);
				IntBPlusEntryRbAccessor_SetLeft(tree, cur, entry_id);
				break;
			}
			parent_id = cur_id;
			cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
		}
		else {
			IntBPlusEntryRbBsStackVectorPopTail(stack);
			old_id = cur_id;
			if (cur_id == entry_id) break;
			IntBPlusEntryRbBsEntryInit(tree, entry);
			if (parent_id != IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbBsEntry* parent = IntBPlusEntryRbReferencer_Reference(tree, parent_id);
				IntBPlusEntryRbBsTreeSetNewChild(tree, parent, cur_id, entry_id);
				IntBPlusEntryRbReferencer_Dereference(tree, parent);
			}
			else {
				tree->root = entry_id;
			}
			*entry = *cur;
			break;
		}
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
	}
	if (cur) IntBPlusEntryRbReferencer_Dereference(tree, cur);
	IntBPlusEntryRbReferencer_Dereference(tree, entry);
	return old_id;
}
int16_t IntBPlusEntryRbBsTreeDelete(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t entry_id, _Bool* is_parent_left) {
	{
		if (!(entry_id != IntBPlusEntryRbReferencer_InvalidId)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	int16_t backtrack_id;
	IntBPlusEntryRbBsEntry* entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
	int16_t* parent_id = IntBPlusEntryRbBsStackVectorGetTail(stack);
	IntBPlusEntryRbBsEntry* parent = ((void*)0);
	if (parent_id != IntBPlusEntryRbReferencer_InvalidId) {
		parent = IntBPlusEntryRbReferencer_Reference(tree, *parent_id);
	}
	if (IntBPlusEntryRbAccessor_GetLeft(tree, entry) != IntBPlusEntryRbReferencer_InvalidId && IntBPlusEntryRbAccessor_GetRight(tree, entry) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &entry_id);
		int16_t* new_entry_id = IntBPlusEntryRbBsStackVectorGetTail(stack);
		int16_t min_entry_id = IntBPlusEntryRbAccessor_GetRight(tree, entry);
		int16_t min_entry_parent_id = IntBPlusEntryRbReferencer_InvalidId;
		IntBPlusEntryRbBsEntry* min_entry = IntBPlusEntryRbReferencer_Reference(tree, min_entry_id);
		while (IntBPlusEntryRbAccessor_GetLeft(tree, min_entry) != IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusEntryRbBsStackVectorPushTail(stack, &min_entry_id);
			min_entry_parent_id = min_entry_id;
			min_entry_id = IntBPlusEntryRbAccessor_GetLeft(tree, min_entry);
			IntBPlusEntryRbReferencer_Dereference(tree, min_entry);
			min_entry = IntBPlusEntryRbReferencer_Reference(tree, min_entry_id);
		}
		*new_entry_id = min_entry_id;
		IntBPlusEntryRbBsEntry* min_entry_parent = IntBPlusEntryRbReferencer_Reference(tree, min_entry_parent_id);
		IntBPlusEntryRbAccessor_SetLeft(tree, min_entry, IntBPlusEntryRbAccessor_GetLeft(tree, entry));
		int16_t old_right_id = IntBPlusEntryRbAccessor_GetRight(tree, min_entry);
		if (IntBPlusEntryRbAccessor_GetRight(tree, entry) != min_entry_id) {
			IntBPlusEntryRbAccessor_SetLeft(tree, min_entry_parent, IntBPlusEntryRbAccessor_GetRight(tree, min_entry));
			IntBPlusEntryRbAccessor_SetRight(tree, min_entry, IntBPlusEntryRbAccessor_GetRight(tree, entry));
			if (is_parent_left) *is_parent_left = 1;
		}
		else {
			if (is_parent_left) *is_parent_left = 0;
		}
		IntBPlusEntryRbReferencer_Dereference(tree, min_entry_parent);
		IntBPlusEntryRbBsTreeHitchEntry(tree, parent, entry_id, min_entry_id);
		entry_id = min_entry_id;
		IntBPlusEntryRbAccessor_SetLeft(tree, entry, IntBPlusEntryRbReferencer_InvalidId);
		IntBPlusEntryRbAccessor_SetRight(tree, entry, old_right_id);
	}
	else {
		if (is_parent_left) {
			if (parent != ((void*)0)) {
				*is_parent_left = IntBPlusEntryRbAccessor_GetLeft(tree, parent) == entry_id; {
					if (!(*is_parent_left || *is_parent_left == 0 && IntBPlusEntryRbAccessor_GetRight(tree, parent) == entry_id)) {
						{
							printf("");
							__debugbreak();
						}
					}
				}
				;
			}
			else {
				*is_parent_left = 0;
			}
		}
		if (IntBPlusEntryRbAccessor_GetRight(tree, entry) != IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusEntryRbBsTreeHitchEntry(tree, parent, entry_id, IntBPlusEntryRbAccessor_GetRight(tree, entry));
		}
		else if (IntBPlusEntryRbAccessor_GetLeft(tree, entry) != IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusEntryRbBsTreeHitchEntry(tree, parent, entry_id, IntBPlusEntryRbAccessor_GetLeft(tree, entry));
		}
		else {
			IntBPlusEntryRbBsTreeHitchEntry(tree, parent, entry_id, IntBPlusEntryRbReferencer_InvalidId);
		}
	}
	if (parent) {
		IntBPlusEntryRbReferencer_Dereference(tree, parent);
	}
	return entry_id;
}
int16_t IntBPlusEntryRbBsTreeGetCount(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack) {
	size_t count = 0;
	stack->count = 0;
	int16_t cur_id = IntBPlusEntryRbBsTreeIteratorFirst(tree, stack);
	while (cur_id != IntBPlusEntryRbReferencer_InvalidId) {
		count++;
		cur_id = IntBPlusEntryRbBsTreeIteratorNext(tree, stack, cur_id);
	}
	return count;
}
int16_t IntBPlusEntryRbBsTreeIteratorLocate(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int64_t* key, int8_t* cmp_status) {
	int16_t cur_id = tree->root;
	stack->count = 0;
	int16_t perv_id = IntBPlusEntryRbReferencer_InvalidId;
	while (cur_id != IntBPlusEntryRbReferencer_InvalidId) {
		perv_id = cur_id;
		IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		int64_t* cur_key = IntBPlusEntryRbAccessor_GetKey(tree, cur);
		if ((*(cur_key) < *(key))) {
			*cmp_status = 1;
			cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
		}
		else if ((*(cur_key) > *(key))) {
			*cmp_status = -1;
			cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
		}
		else {
			IntBPlusEntryRbReferencer_Dereference(tree, cur);
			*cmp_status = 0;
			return cur_id;
		}
		IntBPlusEntryRbBsStackVectorPushTail(stack, &perv_id);
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
	}
	return perv_id;
}
int16_t IntBPlusEntryRbBsTreeIteratorFirst(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack) {
	int16_t cur_id = tree->root;
	if (cur_id == IntBPlusEntryRbReferencer_InvalidId) {
		return IntBPlusEntryRbReferencer_InvalidId;
	}
	IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	while (IntBPlusEntryRbAccessor_GetLeft(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
		cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, cur);
	return cur_id;
}
int16_t IntBPlusEntryRbBsTreeIteratorLast(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack) {
	int16_t cur_id = tree->root;
	if (cur_id == IntBPlusEntryRbReferencer_InvalidId) {
		return IntBPlusEntryRbReferencer_InvalidId;
	}
	IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	while (IntBPlusEntryRbAccessor_GetRight(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
		cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, cur);
	return cur_id;
}
int16_t IntBPlusEntryRbBsTreeIteratorNext(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t cur_id) {
	IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	if (IntBPlusEntryRbAccessor_GetRight(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
		cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
		cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		while (IntBPlusEntryRbAccessor_GetLeft(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusEntryRbBsStackVectorPushTail(stack, &cur_id);
			cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
			IntBPlusEntryRbReferencer_Dereference(tree, cur);
			cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		}
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		return cur_id;
	}
	int16_t* parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	IntBPlusEntryRbBsEntry* parent = ((void*)0);
	while (parent_id != ((void*)0)) {
		parent = IntBPlusEntryRbReferencer_Reference(tree, *parent_id);
		if (cur_id != IntBPlusEntryRbAccessor_GetRight(tree, parent)) break;
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		cur = parent;
		cur_id = *parent_id;
		parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, cur);
	if (parent) IntBPlusEntryRbReferencer_Dereference(tree, parent);
	if (parent_id) return *parent_id;
	return IntBPlusEntryRbReferencer_InvalidId;
}
int16_t IntBPlusEntryRbBsTreeIteratorPrev(IntBPlusEntryRbBsTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t cur_id) {
	IntBPlusEntryRbBsEntry* cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
	if (IntBPlusEntryRbAccessor_GetLeft(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
		cur_id = IntBPlusEntryRbAccessor_GetLeft(tree, cur);
		cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		while (IntBPlusEntryRbAccessor_GetRight(tree, cur) != IntBPlusEntryRbReferencer_InvalidId) {
			cur_id = IntBPlusEntryRbAccessor_GetRight(tree, cur);
			IntBPlusEntryRbReferencer_Dereference(tree, cur);
			cur = IntBPlusEntryRbReferencer_Reference(tree, cur_id);
		}
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		return cur_id;
	}
	int16_t* parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	IntBPlusEntryRbBsEntry* parent = ((void*)0);
	while (parent_id != ((void*)0)) {
		parent = IntBPlusEntryRbReferencer_Reference(tree, *parent_id);
		if (cur_id != IntBPlusEntryRbAccessor_GetLeft(tree, cur)) break;
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
		cur = parent;
		cur_id = *parent_id;
		parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, cur);
	if (parent) IntBPlusEntryRbReferencer_Dereference(tree, parent);
	if (parent_id) return *parent_id;
	return IntBPlusEntryRbReferencer_InvalidId;
}
static int16_t IntBPlusEntryGetSiblingEntry(IntBPlusEntryRbTree* tree, IntBPlusEntryRbEntry* entry_parent, int16_t entry_id, IntBPlusEntryRbEntry* entry) {
	int16_t ret;
	if (IntBPlusEntryRbAccessor_GetLeft(tree, entry_parent) == entry_id) {
		ret = IntBPlusEntryRbAccessor_GetRight(tree, entry_parent);
	}
	else {
		{
			if (!(IntBPlusEntryRbAccessor_GetRight(tree, entry_parent) == entry_id)) {
				{
					printf("");
					__debugbreak();
				}
			}
		}
		;
		ret = IntBPlusEntryRbAccessor_GetLeft(tree, entry_parent);
	}
	return ret;
}
static void IntBPlusEntryRbTreeInsertFixup(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t ins_entry_id) {
	IntBPlusEntryRbEntry* ins_entry = IntBPlusEntryRbReferencer_Reference(tree, ins_entry_id);
	IntBPlusEntryRbAccessor_SetColor(tree, ins_entry, kRbBlack);
	int16_t* cur_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	if (cur_id == ((void*)0)) {
		IntBPlusEntryRbAccessor_SetColor(tree, ins_entry, kRbBlack);
		IntBPlusEntryRbReferencer_Dereference(tree, ins_entry);
		return;
	}
	IntBPlusEntryRbAccessor_SetColor(tree, ins_entry, kRbRed);
	IntBPlusEntryRbReferencer_Dereference(tree, ins_entry);
	IntBPlusEntryRbEntry* cur = ((void*)0);
	while (cur_id != ((void*)0)) {
		cur = IntBPlusEntryRbReferencer_Reference(tree, *cur_id);
		if (IntBPlusEntryRbAccessor_GetColor(tree, cur) == kRbBlack) {
			break;
		}
		int16_t* parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
		if (parent_id == ((void*)0)) {
			IntBPlusEntryRbAccessor_SetColor(tree, cur, kRbBlack);
			break;
		}
		IntBPlusEntryRbEntry* parent = IntBPlusEntryRbReferencer_Reference(tree, *parent_id);
		int16_t sibling_id = IntBPlusEntryGetSiblingEntry(tree, parent, *cur_id, cur);
		IntBPlusEntryRbReferencer_Dereference(tree, parent);
		IntBPlusEntryRbEntry* sibling = IntBPlusEntryRbReferencer_Reference(tree, sibling_id);
		if (sibling_id != IntBPlusEntryRbReferencer_InvalidId && IntBPlusEntryRbAccessor_GetColor(tree, sibling) == kRbRed) {
			IntBPlusEntryRbAccessor_SetColor(tree, cur, kRbBlack);
			IntBPlusEntryRbAccessor_SetColor(tree, sibling, kRbBlack);
			ins_entry_id = *parent_id;
			ins_entry = IntBPlusEntryRbReferencer_Reference(tree, ins_entry_id);
			if (IntBPlusEntryRbBsStackVectorGetTail(stack) == ((void*)0)) {
				IntBPlusEntryRbAccessor_SetColor(tree, ins_entry, kRbBlack);
				break;
			}
			IntBPlusEntryRbAccessor_SetColor(tree, ins_entry, kRbRed);
			parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
		}
		else {
			{
				if (!(sibling_id == IntBPlusEntryRbReferencer_InvalidId || IntBPlusEntryRbAccessor_GetColor(tree, sibling) == kRbBlack)) {
					{
						printf("");
						__debugbreak();
					}
				}
			}
			;
			int16_t new_sub_root_id;
			int16_t old_sub_root_id = *parent_id;
			IntBPlusEntryRbEntry* old_sub_root = IntBPlusEntryRbReferencer_Reference(tree, old_sub_root_id);
			IntBPlusEntryRbEntry* new_sub_root_parent = ((void*)0);
			int16_t* new_sub_root_parent_id = IntBPlusEntryRbBsStackVectorGetTail(stack);
			if (new_sub_root_parent_id) new_sub_root_parent = IntBPlusEntryRbReferencer_Reference(tree, *new_sub_root_parent_id);
			if (IntBPlusEntryRbAccessor_GetLeft(tree, old_sub_root) == *cur_id) {
				if (IntBPlusEntryRbAccessor_GetRight(tree, cur) == ins_entry_id) {
					IntBPlusEntryRbRotateLeft(tree, old_sub_root, *cur_id, cur);
				}
				new_sub_root_id = IntBPlusEntryRbRotateRight(tree, new_sub_root_parent, old_sub_root_id, old_sub_root);
			}
			else {
				if (IntBPlusEntryRbAccessor_GetLeft(tree, cur) == ins_entry_id) {
					IntBPlusEntryRbRotateRight(tree, old_sub_root, *cur_id, cur);
				}
				new_sub_root_id = IntBPlusEntryRbRotateLeft(tree, new_sub_root_parent, old_sub_root_id, old_sub_root);
			}
			if (new_sub_root_parent_id) IntBPlusEntryRbReferencer_Dereference(tree, new_sub_root_parent);
			IntBPlusEntryRbEntry* new_sub_root = IntBPlusEntryRbReferencer_Reference(tree, new_sub_root_id);
			IntBPlusEntryRbAccessor_SetColor(tree, new_sub_root, kRbBlack);
			IntBPlusEntryRbAccessor_SetColor(tree, old_sub_root, kRbRed);
			IntBPlusEntryRbReferencer_Dereference(tree, new_sub_root);
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			break;
		}
		cur_id = parent_id;
		IntBPlusEntryRbReferencer_Dereference(tree, cur);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, cur);
}
static void IntBPlusEntryRbTreeDeleteFixup(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t del_entry_id, _Bool is_parent_left) {
	IntBPlusEntryRbEntry* del_entry = IntBPlusEntryRbReferencer_Reference(tree, del_entry_id);
	int16_t* parent_id = ((void*)0);
	RbColor del_color = IntBPlusEntryRbAccessor_GetColor(tree, del_entry);
	if (del_color == kRbRed) {
	}
	else if (IntBPlusEntryRbAccessor_GetLeft(tree, del_entry) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbEntry* del_entry_left = IntBPlusEntryRbReferencer_Reference(tree, IntBPlusEntryRbAccessor_GetLeft(tree, del_entry));
		IntBPlusEntryRbAccessor_SetColor(tree, del_entry_left, kRbBlack);
		IntBPlusEntryRbReferencer_Dereference(tree, del_entry_left);
	}
	else if (IntBPlusEntryRbAccessor_GetRight(tree, del_entry) != IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbEntry* del_entry_right = IntBPlusEntryRbReferencer_Reference(tree, IntBPlusEntryRbAccessor_GetRight(tree, del_entry));
		IntBPlusEntryRbAccessor_SetColor(tree, del_entry_right, kRbBlack);
		IntBPlusEntryRbReferencer_Dereference(tree, del_entry_right);
	}
	else {
		parent_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
	}
	int16_t new_sub_root_id;
	int16_t* grandpa_id = ((void*)0);
	IntBPlusEntryRbEntry* parent = ((void*)0), * sibling = ((void*)0), * grandpa = ((void*)0);
	if (parent_id) parent = IntBPlusEntryRbReferencer_Reference(tree, *parent_id);
	while (parent_id != ((void*)0)) {
		int16_t sibling_id = is_parent_left ? IntBPlusEntryRbAccessor_GetRight(tree, parent) : IntBPlusEntryRbAccessor_GetLeft(tree, parent);
		sibling = IntBPlusEntryRbReferencer_Reference(tree, sibling_id);
		grandpa_id = IntBPlusEntryRbBsStackVectorPopTail(stack);
		if (grandpa_id) grandpa = IntBPlusEntryRbReferencer_Reference(tree, *grandpa_id); else grandpa = ((void*)0);
		if (IntBPlusEntryRbAccessor_GetColor(tree, sibling) == kRbRed) {
			int16_t old_sub_root_id = *parent_id;
			IntBPlusEntryRbEntry* old_sub_root = IntBPlusEntryRbReferencer_Reference(tree, old_sub_root_id);
			IntBPlusEntryRbAccessor_SetColor(tree, old_sub_root, kRbRed);
			IntBPlusEntryRbAccessor_SetColor(tree, sibling, kRbBlack);
			if (IntBPlusEntryRbAccessor_GetLeft(tree, old_sub_root) == sibling_id) {
				new_sub_root_id = IntBPlusEntryRbRotateRight(tree, grandpa, old_sub_root_id, old_sub_root);
				sibling_id = IntBPlusEntryRbAccessor_GetLeft(tree, old_sub_root);
				IntBPlusEntryRbReferencer_Dereference(tree, sibling);
				sibling = IntBPlusEntryRbReferencer_Reference(tree, sibling_id);
			}
			else {
				new_sub_root_id = IntBPlusEntryRbRotateLeft(tree, grandpa, old_sub_root_id, old_sub_root);
				sibling_id = IntBPlusEntryRbAccessor_GetRight(tree, old_sub_root);
				IntBPlusEntryRbReferencer_Dereference(tree, sibling);
				sibling = IntBPlusEntryRbReferencer_Reference(tree, sibling_id);
			}
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			IntBPlusEntryRbReferencer_Dereference(tree, old_sub_root);
			IntBPlusEntryRbReferencer_Dereference(tree, grandpa);
			grandpa_id = new_sub_root_id;
			grandpa = IntBPlusEntryRbReferencer_Reference(tree, grandpa_id);
		}
		IntBPlusEntryRbEntry* sibling_right = IntBPlusEntryRbReferencer_Reference(tree, IntBPlusEntryRbAccessor_GetRight(tree, sibling));
		IntBPlusEntryRbEntry* sibling_left = IntBPlusEntryRbReferencer_Reference(tree, IntBPlusEntryRbAccessor_GetLeft(tree, sibling));
		if (IntBPlusEntryRbAccessor_GetRight(tree, sibling) != IntBPlusEntryRbReferencer_InvalidId && IntBPlusEntryRbAccessor_GetColor(tree, sibling_right) == kRbRed || IntBPlusEntryRbAccessor_GetLeft(tree, sibling) != IntBPlusEntryRbReferencer_InvalidId && IntBPlusEntryRbAccessor_GetColor(tree, sibling_left) == kRbRed) {
			RbColor parent_color = IntBPlusEntryRbAccessor_GetColor(tree, parent);
			IntBPlusEntryRbAccessor_SetColor(tree, parent, kRbBlack);
			int16_t old_sub_root_id = *parent_id;
			IntBPlusEntryRbEntry* new_sub_root_parent = ((void*)0);
			if (IntBPlusEntryRbAccessor_GetLeft(tree, parent) == sibling_id) {
				if (IntBPlusEntryRbAccessor_GetLeft(tree, sibling) == IntBPlusEntryRbReferencer_InvalidId || IntBPlusEntryRbAccessor_GetColor(tree, sibling_left) == kRbBlack) {
					IntBPlusEntryRbAccessor_SetColor(tree, sibling_right, kRbBlack);
					sibling_id = IntBPlusEntryRbRotateLeft(tree, parent, sibling_id, sibling);
				}
				else {
					IntBPlusEntryRbAccessor_SetColor(tree, sibling_left, kRbBlack);
				}
				new_sub_root_id = IntBPlusEntryRbRotateRight(tree, grandpa, *parent_id, parent);
			}
			else {
				if (IntBPlusEntryRbAccessor_GetRight(tree, sibling) == IntBPlusEntryRbReferencer_InvalidId || IntBPlusEntryRbAccessor_GetColor(tree, sibling_right) == kRbBlack) {
					IntBPlusEntryRbAccessor_SetColor(tree, sibling_left, kRbBlack);
					sibling_id = IntBPlusEntryRbRotateRight(tree, parent, sibling_id, sibling);
				}
				else {
					IntBPlusEntryRbAccessor_SetColor(tree, sibling_right, kRbBlack);
				}
				new_sub_root_id = IntBPlusEntryRbRotateLeft(tree, grandpa, *parent_id, parent);
			}
			IntBPlusEntryRbReferencer_Dereference(tree, sibling);
			sibling = IntBPlusEntryRbReferencer_Reference(tree, sibling_id);
			IntBPlusEntryRbAccessor_SetColor(tree, sibling, parent_color);
			if (tree->root == old_sub_root_id) {
				tree->root = new_sub_root_id;
			}
			IntBPlusEntryRbReferencer_Dereference(tree, sibling_right);
			IntBPlusEntryRbReferencer_Dereference(tree, sibling_left);
			break;
		}
		IntBPlusEntryRbReferencer_Dereference(tree, sibling_right);
		IntBPlusEntryRbReferencer_Dereference(tree, sibling_left);
		if (IntBPlusEntryRbAccessor_GetColor(tree, parent) == kRbRed) {
			IntBPlusEntryRbAccessor_SetColor(tree, sibling, kRbRed);
			IntBPlusEntryRbAccessor_SetColor(tree, parent, kRbBlack);
			break;
		}
		else {
			IntBPlusEntryRbAccessor_SetColor(tree, sibling, kRbRed);
		}
		int16_t child_id = *parent_id;
		parent_id = grandpa_id;
		IntBPlusEntryRbReferencer_Dereference(tree, parent);
		parent = grandpa;
		if (parent != ((void*)0)) {
			is_parent_left = IntBPlusEntryRbAccessor_GetLeft(tree, parent) == child_id;
		}
	}
	IntBPlusEntryRbReferencer_Dereference(tree, sibling);
	IntBPlusEntryRbReferencer_Dereference(tree, parent);
	IntBPlusEntryRbEntry* root = IntBPlusEntryRbReferencer_Reference(tree, tree->root);
	if (root && IntBPlusEntryRbAccessor_GetColor(tree, root) == kRbRed) {
		IntBPlusEntryRbAccessor_SetColor(tree, root, kRbBlack);
	}
	IntBPlusEntryRbReferencer_Dereference(tree, root);
}
void IntBPlusEntryRbTreeInit(IntBPlusEntryRbTree* tree) {
	IntBPlusEntryRbBsTreeInit(&tree->bs_tree);
}
static int16_t IntBPlusEntryRbTreeFindInternal(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsStackVector* stack, int64_t* key) {
	return IntBPlusEntryRbBsTreeFind(&tree->bs_tree, stack, key);
}
int16_t IntBPlusEntryRbTreeFind(IntBPlusEntryRbTree* tree, int64_t* key) {
	IntBPlusEntryRbBsStackVector stack;
	IntBPlusEntryRbBsStackVectorInit(&stack);
	;
	return IntBPlusEntryRbTreeFindInternal(&tree->bs_tree, &stack, key);
}
_Bool IntBPlusEntryRbTreeInsert(IntBPlusEntryRbTree* tree, int16_t insert_entry_id) {
	IntBPlusEntryRbBsStackVector stack;
	IntBPlusEntryRbBsStackVectorInit(&stack);
	;
	if (!IntBPlusEntryRbBsTreeInsert(&tree->bs_tree, &stack, insert_entry_id)) return 0;
	IntBPlusEntryRbTreeInsertFixup(tree, &stack, insert_entry_id);
	return 1;
}
int16_t IntBPlusEntryRbTreePut(IntBPlusEntryRbTree* tree, int16_t put_entry_id) {
	IntBPlusEntryRbBsStackVector stack;
	IntBPlusEntryRbBsStackVectorInit(&stack);
	;
	int16_t old_id = IntBPlusEntryRbBsTreePut(&tree->bs_tree, &stack, put_entry_id);
	if (old_id == IntBPlusEntryRbReferencer_InvalidId) IntBPlusEntryRbTreeInsertFixup(tree, &stack, put_entry_id);
	return old_id;
}
static _Bool IntBPlusEntryRbTreeDeleteInternal(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsStackVector* stack, int16_t del_entry_id) {
	{
		if (!(del_entry_id != IntBPlusEntryRbReferencer_InvalidId)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	_Bool is_parent_left;
	int16_t del_min_entry_id = IntBPlusEntryRbBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left);
	if (del_min_entry_id == IntBPlusEntryRbReferencer_InvalidId) {
		return 0;
	}
	if (del_min_entry_id != del_entry_id) {
		IntBPlusEntryRbEntry* del_entry = IntBPlusEntryRbReferencer_Reference(tree, del_entry_id);
		IntBPlusEntryRbEntry* del_min_entry = IntBPlusEntryRbReferencer_Reference(tree, del_min_entry_id);
		;
		RbColor old_color = IntBPlusEntryRbAccessor_GetColor(tree, del_min_entry);
		IntBPlusEntryRbAccessor_SetColor(tree, del_min_entry, IntBPlusEntryRbAccessor_GetColor(tree, del_entry));
		IntBPlusEntryRbAccessor_SetColor(tree, del_entry, old_color);
		IntBPlusEntryRbReferencer_Dereference(tree, del_min_entry);
		IntBPlusEntryRbReferencer_Dereference(tree, del_entry);
	}
	IntBPlusEntryRbTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left);
	return 1;
}
_Bool IntBPlusEntryRbTreeDelete(IntBPlusEntryRbTree* tree, int64_t* key) {
	IntBPlusEntryRbBsStackVector stack;
	IntBPlusEntryRbBsStackVectorInit(&stack);
	;
	int16_t del_entry_id = IntBPlusEntryRbTreeFindInternal(&tree->bs_tree, &stack, key);
	if (del_entry_id == IntBPlusEntryRbReferencer_InvalidId) return 0;
	return IntBPlusEntryRbTreeDeleteInternal(tree, &stack, del_entry_id);
}
_Bool IntBPlusEntryRbTreeDeleteByIterator(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator) {
	if (iterator->cur_id == IntBPlusEntryRbReferencer_InvalidId) return 0;
	return IntBPlusEntryRbTreeDeleteInternal(tree, &iterator->stack, iterator->cur_id);
}
int16_t IntBPlusEntryRbTreeGetCount(IntBPlusEntryRbTree* tree) {
	IntBPlusEntryRbBsStackVector stack;
	IntBPlusEntryRbBsStackVectorInit(&stack);
	;
	return IntBPlusEntryRbBsTreeGetCount((IntBPlusEntryRbBsTree*)tree, &stack);
}
int16_t IntBPlusEntryRbTreeIteratorLocate(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator, int64_t* key, int8_t* cmp_status) {
	IntBPlusEntryRbBsStackVectorInit(&iterator->stack);
	return IntBPlusEntryRbBsTreeIteratorLocate((IntBPlusEntryRbBsTree*)tree, &iterator->stack, key, cmp_status);
}
int16_t IntBPlusEntryRbTreeIteratorFirst(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator) {
	IntBPlusEntryRbBsStackVectorInit(&iterator->stack);
	return IntBPlusEntryRbBsTreeIteratorFirst((IntBPlusEntryRbBsTree*)tree, &iterator->stack);
}
int16_t IntBPlusEntryRbTreeIteratorLast(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator) {
	IntBPlusEntryRbBsStackVectorInit(&iterator->stack);
	return IntBPlusEntryRbBsTreeIteratorLast((IntBPlusEntryRbBsTree*)tree, &iterator->stack);
}
int16_t IntBPlusEntryRbTreeIteratorNext(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator) {
	if (iterator->cur_id == IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbTreeIteratorFirst(tree, iterator);
		return iterator->cur_id;
	}
	return IntBPlusEntryRbBsTreeIteratorNext((IntBPlusEntryRbBsTree*)tree, &iterator->stack, iterator->cur_id);
}
int16_t IntBPlusEntryRbTreeIteratorPrev(IntBPlusEntryRbTree* tree, IntBPlusEntryRbTreeIterator* iterator) {
	if (iterator->cur_id == IntBPlusEntryRbReferencer_InvalidId) {
		IntBPlusEntryRbTreeIteratorLast(tree, iterator);
		return iterator->cur_id;
	}
	return IntBPlusEntryRbBsTreeIteratorPrev((IntBPlusEntryRbBsTree*)tree, &iterator->stack, iterator->cur_id);
}
void IntBPlusEntryRbTreeIteratorCopy(IntBPlusEntryRbTreeIterator* dst_iterator, IntBPlusEntryRbTreeIterator* src_iterator) {
	memcpy(dst_iterator, src_iterator, sizeof(*dst_iterator));
}
static _Bool IntBPlusEntryRbTreeCheckPath(IntBPlusEntryRbTree* tree, int16_t parent_id, int16_t entry_id, int16_t cur_high, int16_t max_high) {
	if (entry_id == IntBPlusEntryRbReferencer_InvalidId) {
		return cur_high == max_high;
	}
	_Bool correct = 0;
	IntBPlusEntryRbEntry* entry = ((void*)0);
	IntBPlusEntryRbEntry* parent = ((void*)0);
	do {
		entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
		if (parent_id != IntBPlusEntryRbReferencer_InvalidId) {
			parent = IntBPlusEntryRbReferencer_Reference(tree, parent_id);
			if (IntBPlusEntryRbAccessor_GetColor(tree, entry) == kRbRed && IntBPlusEntryRbAccessor_GetColor(tree, parent) == kRbRed) {
				break;
			}
		}
		if (IntBPlusEntryRbAccessor_GetColor(tree, entry) == kRbBlack) {
			cur_high++;
		}
		correct = IntBPlusEntryRbTreeCheckPath(tree, entry_id, IntBPlusEntryRbAccessor_GetLeft(tree, entry), cur_high, max_high) && IntBPlusEntryRbTreeCheckPath(tree, entry_id, IntBPlusEntryRbAccessor_GetRight(tree, entry), cur_high, max_high);
	} while (0);
	if (entry) IntBPlusEntryRbReferencer_Dereference(tree, entry);
	if (parent) IntBPlusEntryRbReferencer_Dereference(tree, parent);
	return correct;
}
_Bool IntBPlusEntryRbTreeVerify(IntBPlusEntryRbTree* tree) {
	IntBPlusEntryRbEntry* entry = IntBPlusEntryRbReferencer_Reference(tree, tree->root);
	if (!entry) return 1;
	_Bool correct = 0;
	do {
		if (IntBPlusEntryRbAccessor_GetColor(tree, entry) != kRbBlack) break;
		int16_t high = 1;
		while (IntBPlusEntryRbAccessor_GetLeft(tree, entry) != IntBPlusEntryRbReferencer_InvalidId) {
			int16_t entry_id = IntBPlusEntryRbAccessor_GetLeft(tree, entry);
			IntBPlusEntryRbReferencer_Dereference(tree, entry);
			entry = IntBPlusEntryRbReferencer_Reference(tree, entry_id);
			if (IntBPlusEntryRbAccessor_GetColor(tree, entry) == kRbBlack) {
				high++;
			}
		}
		correct = IntBPlusEntryRbTreeCheckPath(tree, IntBPlusEntryRbReferencer_InvalidId, tree->root, 0, high);
	} while (0);
	IntBPlusEntryRbReferencer_Dereference(tree, entry);
	return correct;
}
static void IntBPlusElementSet(IntBPlusEntry* dst_entry, int16_t element_id, IntBPlusEntry* src_entry, IntBPlusElement* element, struct _IntBPlusEntry* element_child_id) {
	{
		if (!(element_id >= 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusElement* dst_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(dst_entry, element_id);
	if (dst_entry->type == kBPlusEntryLeaf) {
		;
		;
	}
	else {
		if (src_entry->type == kBPlusEntryLeaf) {
			; {
				if (!(element_child_id != (((void*)0)))) {
					{
						printf("");
						__debugbreak();
					}
				}
			}
			;
			dst_element->index.child_id = element_child_id;
		}
		else {
			;
			if (element_child_id != (((void*)0))) {
				dst_element->index.child_id = element_child_id;
			}
			else {
				dst_element->index.child_id = element->index.child_id;
			}
		}
	}
	;
}
static struct _IntBPlusEntry* IntBPlusElementGetChildId(const IntBPlusEntry* index, int16_t element_id) {
	if (element_id == IntBPlusEntryRbReferencer_InvalidId) {
		return index->index.tail_child_id;
	}
	IntBPlusElement* element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(index, element_id);
	struct _IntBPlusEntry* child_id = element->index.child_id;
	;
	return child_id;
}
static void IntBPlusElementSetChildId(IntBPlusEntry* index, int16_t element_id, struct _IntBPlusEntry* entry_id) {
	if (element_id == IntBPlusEntryRbReferencer_InvalidId) {
		index->index.tail_child_id = entry_id;
		return;
	}
	IntBPlusElement* element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(index, element_id);
	element->index.child_id = entry_id;
	;
}
static int16_t IntBPlusElementCreate(IntBPlusEntry* entry) {
	int16_t element_id = (IntBPlusStaticListPop(&((Int_BPlusEntryExtend*)((uintptr_t)entry - sizeof(Int_BPlusEntryExtend)))->static_list, 0)); {
		if (!(element_id >= 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	return element_id;
}
static IntBPlusElement* IntBPlusElementRelease(IntBPlusEntry* entry, int16_t element_id) {
	{
		if (!(element_id >= 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusElement* element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, element_id);
	;
	(IntBPlusStaticListPush(&((Int_BPlusEntryExtend*)((uintptr_t)entry - sizeof(Int_BPlusEntryExtend)))->static_list, 0, element_id));
	return element;
}
BPlusIteratorStatus IntBPlusIteratorFirst(IntBPlusTree* tree, IntBPlusIterator* iterator, int64_t* key) {
	IntBPlusIteratorStackVectorInit(&iterator->stack, 32, iterator->default_stack);
	iterator->stack.count = 0;
	iterator->level = -1;
	return IntBPlusIteratorDown(tree, iterator, key);
}
void IntBPlusIteratorRelease(IntBPlusTree* tree, IntBPlusIterator* iterator) {
	IntBPlusIteratorStackVectorRelease(&iterator->stack);
}
IntBPlusElementPos* IntBPlusIteratorCur(IntBPlusTree* tree, IntBPlusIterator* iterator) {
	if (iterator->level < 0) {
		return ((void*)0);
	}
	return &iterator->stack.obj_arr[iterator->level];
}
IntBPlusElementPos* IntBPlusIteratorUp(IntBPlusTree* tree, IntBPlusIterator* iterator) {
	if (iterator->level <= 0) {
		return ((void*)0);
	}
	return &iterator->stack.obj_arr[--iterator->level];
}
BPlusIteratorStatus IntBPlusIteratorDown(IntBPlusTree* tree, IntBPlusIterator* iterator, int64_t* key) {
	IntBPlusElementPos cur;
	IntBPlusElementPos* parent = IntBPlusIteratorCur(tree, iterator);
	if (parent) {
		IntBPlusEntry* parent_entry = (parent->entry_id);
		if (parent_entry->type == kBPlusEntryLeaf) {
			return kBPlusIteratorEnd;
		}
		cur.entry_id = IntBPlusElementGetChildId(parent_entry, parent->element_iterator.cur_id);
		;
	}
	else {
		cur.entry_id = tree->root_id;
	}
	IntBPlusEntry* cur_entry = (cur.entry_id);
	int8_t cmp_status = -1;
	if (cur_entry->element_count > 0) {
		IntBPlusEntryRbTreeIteratorLocate(&cur_entry->rb_tree, &cur.element_iterator, key, &cmp_status);
		if (cmp_status == -1) {
		}
		else {
			if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) {
				IntBPlusEntryRbTreeIteratorNext(&cur_entry->rb_tree, &cur.element_iterator);
			}
		}
	}
	else {
		cur.element_iterator.cur_id = IntBPlusEntryRbReferencer_InvalidId;
	}
	IntBPlusIteratorStackVectorPushTail(&iterator->stack, &cur);
	BPlusIteratorStatus status = kBPlusIteratorDown;
	if (cur_entry->type == kBPlusEntryLeaf) {
		if (cmp_status != 0) {
			status = kBPlusIteratorNe;
		}
		else {
			status = kBPlusIteratorEq;
		}
		iterator->leaf_status = status;
	}
	++iterator->level;
	;
	return status;
}
static int16_t IntBPlusEntryInsertElement(IntBPlusEntry* dst_entry, IntBPlusEntry* src_entry, IntBPlusElement* insert_element, struct _IntBPlusEntry* element_child_id) {
	int16_t element_id = IntBPlusElementCreate(dst_entry); {
		if (!(element_id != IntBPlusEntryRbReferencer_InvalidId)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusElementSet(dst_entry, element_id, src_entry, insert_element, element_child_id);
	int16_t old_element_id = IntBPlusEntryRbTreePut(&dst_entry->rb_tree, element_id);
	if (old_element_id != IntBPlusEntryRbReferencer_InvalidId && old_element_id != element_id) IntBPlusElementRelease(dst_entry, old_element_id);
	dst_entry->element_count++;
	return element_id;
}
static void IntBPlusEntryDeleteElement(IntBPlusEntry* entry, IntBPlusEntryRbTreeIterator* element_iterator) {
	{
		if (!(element_iterator->cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusEntryRbTreeDeleteByIterator(&entry->rb_tree, element_iterator);
	entry->element_count--;
	IntBPlusElementRelease(entry, element_iterator->cur_id);
}
void IntBPlusEntryInit(IntBPlusTree* tree, IntBPlusEntry* entry, BPlusEntryType type) {
	entry->type = type;
	entry->element_count = 0;
	IntBPlusEntryRbTreeInit(&entry->rb_tree);
}
struct _IntBPlusEntry* IntBPlusEntryCreate(IntBPlusTree* tree, BPlusEntryType type) {
	size_t size;
	struct _IntBPlusEntry* entry_id;
	if (type == kBPlusEntryIndex) {
		entry_id = (MemoryAlloc((sizeof(IntBPlusEntry) - sizeof(IntBPlusLeafEntry) + sizeof(IntBPlusIndexEntry)) * (1)));
	}
	else {
		entry_id = (MemoryAlloc((sizeof(IntBPlusEntry) - sizeof(IntBPlusIndexEntry) + sizeof(IntBPlusLeafEntry)) * (1)));
	}
	IntBPlusEntry* entry = (entry_id);
	IntBPlusEntryInit(tree, entry, type);
	;
	return entry_id;
}
void IntBPlusEntryRelease(IntBPlusTree* tree, struct _IntBPlusEntry* entry_id) {
	(MemoryFree(entry_id));
}
static int16_t IntBuildRbTree(IntBPlusEntry* src_entry, IntBPlusEntryRbTreeIterator* element_iterator, IntBPlusEntry* dst_entry, int16_t left, int16_t right, int16_t parent_id, int16_t max_level, int16_t level) {
	if (left > right) return (-1);
	int16_t mid = (left + right + 1) / 2;
	int16_t new_element_id = IntBPlusElementCreate(dst_entry);
	IntBPlusElement* new_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(dst_entry, new_element_id);
	IntBPlusEntryRbEntry* rb_entry = &new_element->rb_entry;
	IntBPlusEntryRbAccessor_SetColor(&dst_entry->rb_tree, rb_entry, level == max_level && max_level > 1 ? kRbRed : kRbBlack);
	IntBPlusEntryRbAccessor_SetLeft(&dst_entry->rb_tree, rb_entry, IntBuildRbTree(src_entry, element_iterator, dst_entry, left, mid - 1, new_element_id, max_level, level + 1)); {
		if (!(element_iterator->cur_id != (-1))) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusElement* src_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(src_entry, element_iterator->cur_id);
	IntBPlusElementSet(dst_entry, new_element_id, src_entry, src_element, (((void*)0)));
	;
	IntBPlusEntryRbTreeIteratorNext(&src_entry->rb_tree, element_iterator);
	IntBPlusEntryRbAccessor_SetRight(&dst_entry->rb_tree, rb_entry, IntBuildRbTree(src_entry, element_iterator, dst_entry, mid + 1, right, new_element_id, max_level, level + 1));
	;
	return new_element_id;
}
static int16_t IntBPlusEntrySplit(IntBPlusTree* tree, IntBPlusEntry* left, struct _IntBPlusEntry* left_id, IntBPlusEntry* parent, int16_t parent_element_id, IntBPlusEntry** src_entry, IntBPlusElement* insert_element, int16_t insert_id, struct _IntBPlusEntry* insert_element_child_id, _Bool dereference_src_entry, struct _IntBPlusEntry** out_right_id, IntBPlusEntryRbTreeIterator* up_element_iterator) {
	struct _IntBPlusEntry* right_id = IntBPlusEntryCreate(tree, left->type);
	IntBPlusEntry* right = (right_id);
	int16_t up_element_id;
	if (left->type == kBPlusEntryLeaf) {
	}
	IntBPlusEntryRbTreeIterator left_element_iterator;
	IntBPlusEntryRbTreeIteratorLast(&left->rb_tree, &left_element_iterator);
	_Bool insert_right = 0;
	int16_t fill_rate = ((((Int_BPlusEntryExtend*)((left)->type == kBPlusEntryLeaf ? (uintptr_t)(left)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(left)+sizeof(IntBPlusIndexEntry)))->count) + 1) / 2;
	int16_t right_count = 1, left_count = 0;
	int16_t left_fill_rate = (((Int_BPlusEntryExtend*)((left)->type == kBPlusEntryLeaf ? (uintptr_t)(left)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(left)+sizeof(IntBPlusIndexEntry)))->count);
	while (1) {
		if (!insert_right && left_element_iterator.cur_id == insert_id) {
			insert_right = 1;
		}
		if (left_fill_rate <= fill_rate || left->element_count <= 2) {
			break;
		}
		left_fill_rate -= 1;
		IntBPlusEntryRbTreeIteratorPrev(&left->rb_tree, &left_element_iterator);
		++right_count;
	} {
		if (!(left_element_iterator.cur_id != (-1))) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	; {
		if (!(right_count > 0)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	IntBPlusEntryRbTreeIterator mid_element_iterator;
	IntBPlusEntryRbTreeIteratorCopy(&mid_element_iterator, &left_element_iterator);
	int16_t logn = right_count == 1 ? -1 : 0;
	for (int16_t i = right_count; i > 0; i /= 2) ++logn;
	right->rb_tree.root = IntBuildRbTree(left, &left_element_iterator, right, 0, right_count - 1, (-1), logn, 0);
	IntBPlusEntry* temp_entry = (memcpy(&temp_bplus_entry, (left), sizeof(Int_BPlusEntryExtend) + ((left)->type == kBPlusEntryLeaf ? sizeof(IntBPlusLeafEntry) : sizeof(IntBPlusIndexEntry))), &temp_bplus_entry.entry);
	IntBPlusEntryInit(tree, left, right->type);
	do {
		IntBPlusEntryRbTreeIteratorPrev(&temp_entry->rb_tree, &mid_element_iterator);
		++left_count;
	} while (mid_element_iterator.cur_id != (-1));
	--left_count;
	IntBPlusEntryRbTreeIteratorFirst(&temp_entry->rb_tree, &left_element_iterator);
	logn = left_count == 1 ? -1 : 0;
	for (int16_t i = left_count; i > 0; i /= 2) ++logn;
	left->rb_tree.root = IntBuildRbTree(temp_entry, &left_element_iterator, left, 0, left_count - 1, (-1), logn, 0);
	if (insert_right) {
		IntBPlusEntryInsertElement(right, *src_entry, insert_element, insert_element_child_id);
		++right_count;
	}
	else {
		IntBPlusEntryInsertElement(left, *src_entry, insert_element, insert_element_child_id);
		++left_count;
	}
	left->element_count = left_count;
	right->element_count = right_count;
	if (dereference_src_entry && *src_entry);
	IntBPlusElement* up_element;
	if (left->type == kBPlusEntryLeaf) {
		*src_entry = right;
		(right_id);
		up_element_id = IntBPlusEntryRbTreeIteratorFirst(&right->rb_tree, up_element_iterator);
		up_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
	}
	else {
		right->index.tail_child_id = left->index.tail_child_id;
		up_element_id = IntBPlusEntryRbTreeIteratorLast(&left->rb_tree, up_element_iterator);
		*src_entry = left;
		(left_id);
		up_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
		left->index.tail_child_id = up_element->index.child_id;
	}
	IntBPlusElementSetChildId(parent, parent_element_id, right_id);
	*out_right_id = right_id;
	; {
		if (!(left->element_count >= 1)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	; {
		if (!(right->element_count >= 1)) {
			{
				printf("");
				__debugbreak();
			}
		}
	}
	;
	return up_element_id;
}
static void IntBPlusEntryMerge(IntBPlusTree* tree, IntBPlusEntry* left, struct _IntBPlusEntry* left_id, IntBPlusEntry* right, struct _IntBPlusEntry* right_id, IntBPlusEntry* parent, int16_t parent_index) {
	IntBPlusEntryRbTreeIterator element_iterator;
	int16_t right_element_id = IntBPlusEntryRbTreeIteratorLast(&right->rb_tree, &element_iterator);
	for (int16_t i = 0; i < right->element_count; i++) {
		{
			if (!(right_element_id != IntBPlusEntryRbReferencer_InvalidId)) {
				{
					printf("");
					__debugbreak();
				}
			}
		}
		;
		IntBPlusElement* right_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(right, right_element_id);
		IntBPlusEntryInsertElement(left, ((void*)0), right_element, (((void*)0)));
		;
		right_element_id = IntBPlusEntryRbTreeIteratorPrev(&right->rb_tree, &element_iterator);
	}
	if (left->type == kBPlusEntryLeaf) {
	}
	else {
		IntBPlusElement* parent_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(parent, parent_index);
		int16_t left_element_id = IntBPlusEntryInsertElement(left, ((void*)0), parent_element, (((void*)0)));
		;
		IntBPlusElementSetChildId(left, left_element_id, left->index.tail_child_id);
		IntBPlusElementSetChildId(left, -1, right->index.tail_child_id);
	}
	IntBPlusElementSetChildId(parent, IntBPlusEntryRbTreeIteratorNext(&parent->rb_tree, parent_index), left_id);
	;
	IntBPlusEntryRelease(tree, right_id);
}
static _Bool IntBPlusTreeInsertElement(IntBPlusTree* tree, IntBPlusIterator* iterator, IntBPlusEntry* src_entry, IntBPlusElement* insert_element, int16_t insert_element_id, struct _IntBPlusEntry* insert_element_child_id) {
	IntBPlusElementPos* cur_pos = IntBPlusIteratorCur(tree, iterator);
	IntBPlusElementPos* parent_pos = IntBPlusIteratorUp(tree, iterator);
	struct _IntBPlusEntry* right_id;
	IntBPlusEntry* cur = (cur_pos->entry_id);
	_Bool success = 1, insert_up = 0;
	int16_t up_element_id = (-1);
	IntBPlusEntryRbTreeIterator up_element_iterator;
	do {
		int16_t free_rate = (100 - ((Int_BPlusEntryExtend*)((cur)->type == kBPlusEntryLeaf ? (uintptr_t)(cur)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(cur)+sizeof(IntBPlusIndexEntry)))->count);
		int16_t need_rate = 1;
		if (iterator->leaf_status == kBPlusIteratorEq) {
			IntBPlusElement* raw = TEST_BPLUS_ELEMENT_REFERENCER_Reference(cur, cur_pos->element_iterator.cur_id);
			int16_t raw_rate = 1;
			;
			if (free_rate + need_rate >= raw_rate) {
				;
				break;
			}
			else {
				{
					if (!(0)) {
						{
							printf("");
							__debugbreak();
						}
					}
				}
				;
			}
		}
		else if ((100 - ((Int_BPlusEntryExtend*)((cur)->type == kBPlusEntryLeaf ? (uintptr_t)(cur)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(cur)+sizeof(IntBPlusIndexEntry)))->count) >= 1) {
			IntBPlusEntryInsertElement(cur, src_entry, insert_element, insert_element_child_id);
			break;
		}
		else {
			if (cur_pos->element_iterator.cur_id == IntBPlusEntryRbReferencer_InvalidId) {
				IntBPlusEntryRbTreeIteratorLast(&cur->rb_tree, &cur_pos->element_iterator);
			}
			else {
				IntBPlusEntryRbTreeIteratorPrev(&cur->rb_tree, &cur_pos->element_iterator);
			}
		}
		if (!parent_pos) {
			struct _IntBPlusEntry* parent_id = IntBPlusEntryCreate(tree, kBPlusEntryIndex);
			IntBPlusEntry* parent = (parent_id);
			IntBPlusEntry* new_src_entry = src_entry;
			up_element_id = IntBPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, -1, &new_src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, 0, &right_id, &up_element_iterator);
			IntBPlusElement* up_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(new_src_entry, up_element_id);
			IntBPlusEntryInsertElement(parent, new_src_entry, up_element, cur_pos->entry_id);
			;
			;
			tree->root_id = parent_id;
			;
			break;
		}
		IntBPlusEntry* parent = (parent_pos->entry_id);
		up_element_id = IntBPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, parent_pos->element_iterator.cur_id, &src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, 1, &right_id, &up_element_iterator);
		;
		insert_up = 1;
	} while (0);
	;
	if (insert_up) {
		IntBPlusElement* up_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(src_entry, up_element_id);
		return IntBPlusTreeInsertElement(tree, iterator, src_entry, up_element, up_element_id, cur_pos->entry_id);
	}
	if (src_entry) {
		;
		if (src_entry->type == kBPlusEntryIndex) IntBPlusEntryDeleteElement(src_entry, insert_element_id);
		;
	}
	return success;
}
static _Bool IntBPlusTreeDeleteElement(IntBPlusTree* tree, IntBPlusIterator* iterator) {
	IntBPlusElementPos* cur_pos = IntBPlusIteratorCur(tree, iterator);
	IntBPlusElementPos* parent_pos = IntBPlusIteratorUp(tree, iterator);
	IntBPlusEntry* entry = (cur_pos->entry_id);
	struct _IntBPlusEntry* sibling_entry_id = 0;
	IntBPlusEntry* sibling = ((void*)0);
	IntBPlusEntry* parent = ((void*)0);
	_Bool success = 1, delete_up = 0;
	IntBPlusEntryDeleteElement(entry, &cur_pos->element_iterator);
	do {
		if ((((Int_BPlusEntryExtend*)((entry)->type == kBPlusEntryLeaf ? (uintptr_t)(entry)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(entry)+sizeof(IntBPlusIndexEntry)))->count) >= (100 * 40 / 100)) {
			break;
		}
		if (!parent_pos) {
			if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
				struct _IntBPlusEntry* childId = entry->index.tail_child_id;
				tree->root_id = childId;
				;
				IntBPlusEntryRelease(tree, cur_pos->entry_id);
				return 1;
			}
			else {
				break;
			}
		}
		parent = (parent_pos->entry_id);
		_Bool left_sibling = 1;
		IntBPlusEntryRbTreeIterator common_parent_element_iterator;
		IntBPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &parent_pos->element_iterator);
		IntBPlusEntryRbTreeIterator sibling_element_iterator;
		if (common_parent_element_iterator.cur_id == IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusEntryRbTreeIteratorLast(&parent->rb_tree, &sibling_element_iterator);
		}
		else {
			IntBPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
			IntBPlusEntryRbTreeIteratorPrev(&parent->rb_tree, &sibling_element_iterator);
			if (sibling_element_iterator.cur_id == IntBPlusEntryRbReferencer_InvalidId) {
				left_sibling = 0;
				IntBPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
				IntBPlusEntryRbTreeIteratorNext(&parent->rb_tree, &sibling_element_iterator);
				if (sibling_element_iterator.cur_id == IntBPlusEntryRbReferencer_InvalidId) {
					sibling_entry_id = parent->index.tail_child_id;
				}
			}
		}
		if (sibling_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId) {
			IntBPlusElement* sibling_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(parent, sibling_element_iterator.cur_id);
			sibling_entry_id = sibling_element->index.child_id;
			;
		}
		if (left_sibling) {
			IntBPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &sibling_element_iterator);
			IntBPlusEntryRbTreeIteratorCopy(&parent_pos->element_iterator, &sibling_element_iterator);
		} {
			if (!(common_parent_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
				{
					printf("");
					__debugbreak();
				}
			}
		}
		; {
			if (!(sibling_element_iterator.cur_id != (((void*)0)))) {
				{
					printf("");
					__debugbreak();
				}
			}
		}
		;
		sibling = (sibling_entry_id);
		if ((((Int_BPlusEntryExtend*)((sibling)->type == kBPlusEntryLeaf ? (uintptr_t)(sibling)+sizeof(IntBPlusLeafEntry) : (uintptr_t)(sibling)+sizeof(IntBPlusIndexEntry)))->count) > (100 * 40 / 100)) {
			{
				if (!(sibling->element_count >= 2)) {
					{
						printf("");
						__debugbreak();
					}
				}
			}
			;
			if (entry->type == kBPlusEntryLeaf) {
				if (left_sibling) {
					IntBPlusEntryRbTreeIterator last_element_iterator;
					IntBPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator); {
						if (!(last_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
							{
								printf("");
								__debugbreak();
							}
						}
					}
					;
					IntBPlusElement* element = (last_element_iterator.cur_id);
					IntBPlusEntryInsertElement(entry, sibling, element, (((void*)0)));
					;
					IntBPlusEntryDeleteElement(sibling, &last_element_iterator);
					IntBPlusElement* common_parent_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
					common_parent_element->index.key = element->leaf.key;
					;
				}
				else {
					IntBPlusEntryRbTreeIterator first_element_iterator;
					IntBPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator);
					IntBPlusEntryRbTreeIterator new_element_iterator;
					IntBPlusEntryRbTreeIteratorCopy(&new_element_iterator, &first_element_iterator);
					IntBPlusEntryRbTreeIteratorNext(&sibling->rb_tree, &new_element_iterator); {
						if (!(first_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
							{
								printf("");
								__debugbreak();
							}
						}
					}
					; {
						if (!(new_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
							{
								printf("");
								__debugbreak();
							}
						}
					}
					;
					IntBPlusElement* element = (first_element_iterator.cur_id);
					IntBPlusEntryInsertElement(entry, sibling, element, (((void*)0)));
					;
					IntBPlusEntryDeleteElement(sibling, &first_element_iterator);
					IntBPlusElement* common_parent_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
					IntBPlusElement* sibling_element = TEST_BPLUS_ELEMENT_REFERENCER_Reference(sibling, new_element_iterator.cur_id);
					common_parent_element->index.key = sibling_element->leaf.key;
					;
					;
				}
			}
			else {
				if (left_sibling) {
					IntBPlusEntryRbTreeIterator last_element_iterator;
					IntBPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator); {
						if (!(last_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
							{
								printf("");
								__debugbreak();
							}
						}
					}
					;
					IntBPlusElement* left_element = (last_element_iterator.cur_id); {
						struct _IntBPlusEntry* temp = left_element->index.child_id;
						left_element->index.child_id = sibling->index.tail_child_id;
						sibling->index.tail_child_id = temp;
					}
					;
					IntBPlusElement* par_element = (common_parent_element_iterator.cur_id);
					par_element->index.child_id = left_element->index.child_id;
					IntBPlusEntryInsertElement(entry, parent, par_element, (((void*)0)));
					left_element->index.child_id = sibling_entry_id;
					IntBPlusEntryInsertElement(parent, sibling, left_element, (((void*)0)));
					;
					IntBPlusEntryDeleteElement(sibling, &last_element_iterator);
					;
					IntBPlusEntryDeleteElement(parent, &common_parent_element_iterator);
				}
				else {
					IntBPlusEntryRbTreeIterator first_element_iterator;
					IntBPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator); {
						if (!(first_element_iterator.cur_id != IntBPlusEntryRbReferencer_InvalidId)) {
							{
								printf("");
								__debugbreak();
							}
						}
					}
					;
					IntBPlusElement* right_element = (first_element_iterator.cur_id);
					IntBPlusElement* par_element = (common_parent_element_iterator.cur_id);
					par_element->index.child_id = right_element->index.child_id; {
						struct _IntBPlusEntry* temp = par_element->index.child_id;
						par_element->index.child_id = entry->index.tail_child_id;
						entry->index.tail_child_id = temp;
					}
					;
					IntBPlusEntryInsertElement(entry, parent, par_element, (((void*)0)));
					right_element->index.child_id = cur_pos->entry_id;
					IntBPlusEntryInsertElement(parent, sibling, right_element, (((void*)0)));
					;
					IntBPlusEntryDeleteElement(sibling, &first_element_iterator);
					;
					IntBPlusEntryDeleteElement(parent, &common_parent_element_iterator);
				}
			}
			break;
		}
		if (left_sibling) {
			IntBPlusEntryMerge(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, common_parent_element_iterator.cur_id);
			entry = ((void*)0);
		}
		else {
			IntBPlusEntryMerge(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, common_parent_element_iterator.cur_id);
			sibling = ((void*)0);
		}
		delete_up = 1;
	} while (0);
	if (parent) {
		;
	}
	if (sibling) {
		;
	}
	if (entry) {
		;
	}
	if (delete_up) {
		return IntBPlusTreeDeleteElement(tree, iterator);
	}
	return success;
}
void IntBPlusTreeInit(IntBPlusTree* tree) {
	tree->root_id = IntBPlusEntryCreate(tree, kBPlusEntryLeaf);
}
_Bool IntBPlusTreeFind(IntBPlusTree* tree, int64_t* key) {
	IntBPlusIterator iterator;
	BPlusIteratorStatus status = IntBPlusIteratorFirst(tree, &iterator, key);
	while (status == kBPlusIteratorDown) {
		status = IntBPlusIteratorDown(tree, &iterator, key);
	}
	return status == kBPlusIteratorEq;
}
_Bool IntBPlusTreeInsert(IntBPlusTree* tree, IntBPlusLeafElement* element) {
	IntBPlusIterator iterator;
	BPlusIteratorStatus status = IntBPlusIteratorFirst(tree, &iterator, &element->key);
	while (status == kBPlusIteratorDown) {
		status = IntBPlusIteratorDown(tree, &iterator, &element->key);
	}
	_Bool success = IntBPlusTreeInsertElement(tree, &iterator, ((void*)0), (IntBPlusElement*)element, (-1), (((void*)0)));
	IntBPlusIteratorRelease(tree, &iterator);
	return success;
}
_Bool IntBPlusTreeDelete(IntBPlusTree* tree, int64_t* key) {
	IntBPlusIterator iterator;
	BPlusIteratorStatus status = IntBPlusIteratorFirst(tree, &iterator, key);
	while (status == kBPlusIteratorDown) {
		status = IntBPlusIteratorDown(tree, &iterator, key);
	}
	if (status == kBPlusIteratorNe) {
		return 0;
	}
	_Bool success = IntBPlusTreeDeleteElement(tree, &iterator);
	return success;
}