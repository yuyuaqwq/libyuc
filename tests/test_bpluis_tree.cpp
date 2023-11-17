#include <chrono>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#define LIBYUC_CONTAINER_BPLUS_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id int16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset uint16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId -1
#include <libyuc/container/bplus_tree.h>

#define TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY) ((IntBPlusEntryExtend*)((uintptr_t)(ENTRY) - sizeof(IntBPlusEntryExtend)))

struct IntBPlusElement_User {
	union {
		uint16_t next;
		struct {
			IntBPlusElement element;
		};
	};
};

#define element_max_count 129

#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME IntBPlus
#define LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC
#define LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount element_max_count
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element IntBPlusElement_User
#include <libyuc/container/static_list.h>

#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME IntBPlus
#define LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC
#define LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount element_max_count
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element IntBPlusElement_User
#include <libyuc/container/static_list.c>


struct IntBPlusEntryExtend {
	uint16_t count;
	IntBPlusStaticList static_list;
};
struct {
	IntBPlusEntryExtend extend;
	IntBPlusEntry entry;
} temp_bplus_entry;



#define LIBYUC_CONTAINER_BPLUS_TREE_CLASS_NAME Int

#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key int64_t

#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_Const_ElementCount element_max_count
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetTempCopyEntry(TREE, ENTRY)  (memcpy(&temp_bplus_entry, TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY), sizeof(IntBPlusEntryExtend) + ((ENTRY)->type == kBPlusEntryLeaf ? sizeof(IntBPlusLeafEntry):  sizeof(IntBPlusIndexEntry))), &temp_bplus_entry.entry)
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetMergeThresholdRate(TREE, ENTRY) (element_max_count * 40 / 100)
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFreeRate(TREE, ENTRY) (element_max_count - TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(TREE, ENTRY) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_InitCallback(TREE, ENTRY) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count = 0, IntBPlusStaticListInit(&TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->static_list, element_max_count))

forceinline IntBPlusEntry* TEST_BPLUS_ENTRY_ALLOCATOR_CreateBySize(IntBPlusTree* tree, size_t SIZE) {
	IntBPlusEntryExtend* exptend = (IntBPlusEntryExtend*)MemoryAlloc(sizeof(IntBPlusEntryExtend) + SIZE);
	return (IntBPlusEntry*)((uintptr_t)exptend + sizeof(IntBPlusEntryExtend));
}
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_CreateBySize(TREE, SIZE) TEST_BPLUS_ENTRY_ALLOCATOR_CreateBySize(TREE, SIZE)
forceinline void TEST_BPLUS_ENTRY_ALLOCATOR_Release(IntBPlusTree* tree, void* entry) {
	MemoryFree((void*)((uintptr_t)entry - sizeof(IntBPlusEntryExtend)));
}
#define LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_Release(TREE, ENTRY) TEST_BPLUS_ENTRY_ALLOCATOR_Release(TREE, ENTRY)


#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(ENTRY, ELEMENT) 1
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_CreateBySize(ENTRY, SIZE) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count++,  IntBPlusStaticListPop(&((IntBPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(IntBPlusEntryExtend)))->static_list, 0))
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_Release(ENTRY, ELEMENT) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count--, IntBPlusStaticListPush(&((IntBPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(IntBPlusEntryExtend)))->static_list, 0, ELEMENT))

#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id int16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset uint16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId -1
IntBPlusElement* TEST_BPLUS_ELEMENT_REFERENCER_Reference(const IntBPlusEntry* ENTRY, int16_t ELEMENT_ID) {
	return &TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->static_list.obj_arr[ELEMENT_ID].element;
}
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(main_obj, reference) TEST_BPLUS_ELEMENT_REFERENCER_Reference(main_obj, reference)
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference

#include <libyuc/container/bplus_tree.c>


#include "rand_data_set.h"



static size_t test_count = 10000000;
static IntBPlusTree test_bp_tree;
static std::set<int64_t> test_set;
static std::vector<int64_t> test_res_data_set;

void PrintBPlusTree(IntBPlusTree* tree, IntBPlusEntry* entry, int Level, int pos) {
	if (!entry) return;
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;
	if (entry->type == 1) {
		IntBPlusEntryRbTreeIterator iter;
		int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);
		for (int i = entry->element_count - 1; i >= 0; i--) {
			printf("%sleaf:::key:%d\n%sLevel:%d\n%sId:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->leaf.key, empty, Level, empty, id, empty, NULL/*, pos ? ((BPlusEntry*)PageGet(tree, entry->, 1))->indexElement[pos].key : 0*/);
			id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
		}
		free(empty);
		return;
	}

	IntBPlusEntryRbTreeIterator iter;
	int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);

	for (int i = entry->element_count; i >= 0; i--) {
		if (i == entry->element_count) {
			PrintBPlusTree(tree, entry->index.tail_child_id, Level + 1, i - 1);
			continue;
		}
		printf("%sindex:::key:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.key, empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
		PrintBPlusTree(tree, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.child_id, Level + 1, i);
		id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
	}
	free(empty);
}

TEST(BPlusTreeTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	IntBPlusTreeInit(&test_bp_tree);
}


TEST(BPlusTreeTest, Insert) {
	IntBPlusLeafElement element;
	for (int i = 0; i < test_count; i++) {
		element.key = test_res_data_set[i];
		IntBPlusTreeInsert(&test_bp_tree, &element);
		//PrintBPlusTree(&test_bp_tree, test_bp_tree.root_id, 0, 0);
		//printf("\n\n\n");
	}
}

TEST(BPlusTreeTest, Find) {
	for (int i = 0; i < test_count; i++) {
		if (IntBPlusTreeFind(&test_bp_tree, &test_res_data_set[i]) == false) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(BPlusTreeTest, Delete) {
	for (int i = 0; i < test_count; i++) {
		if (!IntBPlusTreeDelete(&test_bp_tree, &test_res_data_set[i])) {
			ASSERT_TRUE(false);
		}
	}
}


TEST(BPlusTreeTest, End) {
	test_res_data_set.clear();
}
