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
	void* entry = MemoryAlloc(sizeof(Int_BPlusEntryExtend) + SIZE);
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
LIBYUC_CONTAINER_BPLUS_TREE_DEFINE(Int, LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK, struct _IntBPlusEntry*, size_t, int16_t, int16_t, int64_t, int64_t, LIBYUC_BASIC_ALLOCATOR_DEFALUT, TEST_BPLUS_ENTRY_ACCESSOR, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_BASIC_REFERENCER_DEFALUT, TEST_BPLUS_ELEMENT_ACCESSOR, TEST_BPLUS_ELEMENT_ALLOCATOR, TEST_BPLUS_ELEMENT_REFERENCER, LIBYUC_BASIC_COMPARER_DEFALUT, 32)