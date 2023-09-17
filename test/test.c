#define LIBYUC_TEMPLATE_DEFINE

#include "test.h"


//#define LIBYUC_TEMPLATE_DEFINE
//#include <libyuc/container/list.h>
//#include <libyuc/container/list.c>
//#include <libyuc/container/list.h>
//
//
//#define LIBYUC_TEMPLATE_DEFINE
//#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC
//#define CLASS_NAME HashBucket
//#include <libyuc/container/vector.h>
//#include <libyuc/container/vector.c>
//#include <libyuc/container/vector.h>



//LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(, int32_t, int32_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_BASIC_ALLOCATOR_DEFALUT)
//
//
////typedef struct _IntLru_Entry {
////	IntHashListEntry lru_entry;
////	int key;
////} IntLru_Entry;
////#define IntLruListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (&((IntLru_Entry*)(LRU_LIST_ENTRY))->key)
////#define IntLruListEntryAccessor IntLruListEntryAccessor
////LIBYUC_CONTAINER_LRU_LIST_DEFINE(Int, int, IntLruListEntryAccessor, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_BASIC_HASHER_DEFALUT, LIBYUC_BASIC_COMPARER_DEFALUT)
//
//
//#define TEST_COMPARER_DEFALUT_Equal(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) == 0)
//#define TEST_COMPARER_DEFALUT_Greater(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) > 0)
//#define TEST_COMPARER_DEFALUT_Less(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16) < 0)
//#define TEST_COMPARER_DEFALUT_Subrrac(MAIN_OBJ, OBJ1, OBJ2) (memcmp((OBJ1), (OBJ2), 16))
//#define TEST_COMPARER_DEFALUT TEST_COMPARER_DEFALUT
//LIBYUC_CONTAINER_RB_TREE_DEFINE(Int, IntRbEntry*, uint32_t, int64_t, INT_RB_TREE_REFERENCER, INT_RB_TREE_ACCESSOR, LIBYUC_BASIC_COMPARER_DEFALUT)//TEST_COMPARER_DEFALUT
//
//LIBYUC_CONTAINER_AVL_TREE_DEFINE(Int, IntAvlEntry*, uint32_t, int64_t, INT_AVL_TREE_REFERENCER, INT_AVL_TREE_ACCESSOR, LIBYUC_BASIC_COMPARER_DEFALUT)
//	
//
//LIBYUC_CONTAINER_HASH_TABLE_DEFINE(Int, uint32_t, int64_t, int64_t, LIBYUC_BASIC_ALLOCATOR_DEFALUT, INT_HASHTABLE_ACCESSOR, LIBYUC_BASIC_MOVER_DEFALUT, INT_HASHTABLE_HASHER_KEY, LIBYUC_BASIC_COMPARER_DEFALUT)
//
//
//
//LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(Element, int16_t, int16_t)
//LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(Element, int16_t, int16_t, LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER, LIBYUC_BASIC_ALLOCATOR_DEFALUT)
//
//
//
//
//
//
//#define element_max_count 128
//
//typedef struct _Int_BPlusElement {
//	union {
//		uint16_t next;
//		struct {
//			IntBPlusElement element;
//		};
//	};
//} Int_BPlusElement;
//LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(IntBPlus, uint16_t, Int_BPlusElement, 1, element_max_count)
//LIBYUC_CONTAINER_STATIC_LIST_DEFINE(IntBPlus, uint16_t, Int_BPlusElement, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, 1)
//
//
//
//typedef struct _Int_BPlusEntryExtend {
//	uint16_t count;
//	IntBPlusStaticList static_list;
//} Int_BPlusEntryExtend;
//
//#define TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY) ((Int_BPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(Int_BPlusEntryExtend)))
//
//
//
//struct {
//	Int_BPlusEntryExtend extend;
//	IntBPlusEntry entry;
//} temp_bplus_entry;
//
//
//
//#define TEST_BPLUS_ENTRY_ACCESSOR_GetTempCopyEntry(TREE, ENTRY) (memcpy(&temp_bplus_entry, TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY), sizeof(Int_BPlusEntryExtend) + ((ENTRY)->type == kBPlusEntryLeaf ? sizeof(IntBPlusLeafEntry):  sizeof(IntBPlusIndexEntry))), &temp_bplus_entry.entry)
//#define TEST_BPLUS_ENTRY_ACCESSOR_GetMergeThresholdRate(TREE, ENTRY) (element_max_count * 40 / 100)
//#define TEST_BPLUS_ENTRY_ACCESSOR_GetFreeRate(TREE, ENTRY) (element_max_count - TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
//#define TEST_BPLUS_ENTRY_ACCESSOR_GetFillRate(TREE, ENTRY) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count)
//#define TEST_BPLUS_ENTRY_ACCESSOR_GetMaxRate(TREE, ENTRY) (element_max_count)
//#define TEST_BPLUS_ENTRY_ACCESSOR_InitCallback(TREE, ENTRY) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count = 0, IntBPlusStaticListInit(&TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->static_list, element_max_count))
//#define TEST_BPLUS_ENTRY_ACCESSOR TEST_BPLUS_ENTRY_ACCESSOR
//
//forceinline IntBPlusEntry* TEST_BPLUS_ENTRY_ALLOCATOR_CreateBySize(IntBPlusTree* tree, size_t SIZE) {
//	Int_BPlusEntryExtend* exptend = (IntBPlusEntry*)MemoryAlloc(sizeof(Int_BPlusEntryExtend) + SIZE);
//	return (IntBPlusEntry*)((uintptr_t)exptend + sizeof(Int_BPlusEntryExtend));
//}
//forceinline void TEST_BPLUS_ENTRY_ALLOCATOR_Release(IntBPlusTree* tree, void* entry) {
//	MemoryFree((void*)((uintptr_t)entry - sizeof(Int_BPlusEntryExtend)));
//}
//#define TEST_BPLUS_ENTRY_ALLOCATOR TEST_BPLUS_ENTRY_ALLOCATOR
//
//
//
//
//
//#define TEST_BPLUS_ELEMENT_ACCESSOR_SetKey(DST_ENTRY, DST_ELEMENT, SRC_ENTRY, KEY) (DST_ELEMENT->key = *KEY)
//#define TEST_BPLUS_ELEMENT_ACCESSOR_SetValue(DST_ENTRY, DST_ELEMENT, SRC_ENTRY, VALUE) 
//#define TEST_BPLUS_ELEMENT_ACCESSOR_GetNeedRate(ENTRY, ELEMENT) 1
//#define TEST_BPLUS_ELEMENT_ACCESSOR TEST_BPLUS_ELEMENT_ACCESSOR
//
//#define TEST_BPLUS_ELEMENT_ALLOCATOR_CreateBySize(ENTRY, SIZE) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count++,  IntBPlusStaticListPop(&((Int_BPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(Int_BPlusEntryExtend)))->static_list, 0))
//#define TEST_BPLUS_ELEMENT_ALLOCATOR_Release(ENTRY, ELEMENT) (TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->count--, IntBPlusStaticListPush(&((Int_BPlusEntryExtend*)((uintptr_t)ENTRY - sizeof(Int_BPlusEntryExtend)))->static_list, 0, ELEMENT))
//#define TEST_BPLUS_ELEMENT_ALLOCATOR TEST_BPLUS_ELEMENT_ALLOCATOR
//
//
//
//
//#define TEST_BPLUS_ELEMENT_REFERENCER_Dereference(ENTRY, ELEMENT) 
//IntBPlusElement* TEST_BPLUS_ELEMENT_REFERENCER_Reference(IntBPlusEntry* ENTRY, int16_t ELEMENT_ID) {
//	return &TEST_BPLUS_ENTRY_TO_ENTRY_EXTEND(ENTRY)->static_list.obj_arr[ELEMENT_ID].element;
//}
//#define TEST_BPLUS_ELEMENT_REFERENCER_InvalidId (-1)
//#define TEST_BPLUS_ELEMENT_REFERENCER TEST_BPLUS_ELEMENT_REFERENCER
//LIBYUC_CONTAINER_BPLUS_TREE_DEFINE(Int, 
//	LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK, 
//	struct _IntBPlusEntry*, 
//	size_t, 
//	int16_t, 
//	int16_t, 
//	int64_t, 
//	int64_t, 
//	LIBYUC_BASIC_ALLOCATOR_DEFALUT,
//	TEST_BPLUS_ENTRY_ACCESSOR, 
//	TEST_BPLUS_ENTRY_ALLOCATOR,
//	LIBYUC_BASIC_REFERENCER_DEFALUT, 
//	TEST_BPLUS_ELEMENT_ACCESSOR, 
//	TEST_BPLUS_ELEMENT_ALLOCATOR, 
//	TEST_BPLUS_ELEMENT_REFERENCER, 
//	LIBYUC_BASIC_COMPARER_DEFALUT,
//	32)
//
//













//AvlTree gHead;
//struct QVQ {
//	int key;
//	AvlEntry entry;
//};
//



//forceinline BPlusEntryRbEntry* LIBYUC_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(BPlusEntryRbTree* tree, int16_t id) {
//	if (id == -1) {
//		return NULL;
//	}
//	BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
//	if (entry->type == kBPlusEntryIndex) {
//		return &entry->index.element_space.obj_arr[id].rb_entry;
//	}
//	else {
//		return &entry->leaf.element_space.obj_arr[id].rb_entry;
//	}
//}
//typedef struct {
//	int16_t color : 1;
//	int16_t parent : 15;
//} BPlusRbParentColor;
//forceinline Key LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(BPlusEntryRbTree* tree, BPlusEntryRbBsEntry* bs_entry) {
//	if (((BPlusEntry*)tree)->type == kBPlusEntryLeaf) {
//		return ((BPlusLeafElement*)bs_entry)->key;
//	}
//	else {
//		return ((BPlusIndexElement*)bs_entry)->key;
//	}
//}
//#define LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(TREE, ENTRY) (((BPlusRbParentColor*)&(((BPlusEntryRbEntry*)ENTRY)->parent_color))->parent)
//#define LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY) (((BPlusRbParentColor*)&(((BPlusEntryRbEntry*)ENTRY)->parent_color))->color == -1 ? 1 : 0)
//#define LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_SetParent(TREE, ENTRY, NEW_PARENT_ID) (((BPlusRbParentColor*)&(((BPlusEntryRbEntry*)ENTRY)->parent_color))->parent = NEW_PARENT_ID)
//#define LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_SetColor(TREE, ENTRY, COLOR) (((BPlusRbParentColor*)&(((BPlusEntryRbEntry*)ENTRY)->parent_color))->color = COLOR)
//void PrintRB(BPlusEntryRbTree* tree, int16_t entry_id, int Level, bool leaf) {
//	if (entry_id == -1) return;
//	BPlusEntryRbEntry* entry = LIBYUC_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, entry_id);
//	PrintRB(tree, entry->right, Level + 1, leaf);
//
//	//print
//	const char* str = "Not";
//	if (LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(tree, entry) != -1) {
//		str = (LIBYUC_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(tree, entry))->right == entry_id ? "Right" : "Left");
//	}
//	int aaa = LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetColor(tree, entry);
//	const char* color = aaa == 1 ? "Red" : "Black";
//
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//
//	int parentKey = 0;
//	if (LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(tree, entry) != -1) {
//		if (leaf) {
//			parentKey = ((BPlusLeafElement*)LIBYUC_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(tree, entry)))->key.data;
//		}
//		else {
//			parentKey = ((BPlusIndexElement*)LIBYUC_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(tree, entry)))->key.data;
//		}
//	}
//
//	printf("%skey:%d\n%sLevel:%d\n%sParent.%s:%x\n%scolor:%s\n\n", empty, leaf ? ((BPlusLeafElement*)entry)->key.data : ((BPlusIndexElement*)entry)->key.data, empty, Level, empty, str, parentKey, empty, color);
//
//	free(empty);
//
//	PrintRB(tree, entry->left, Level + 1, leaf);
//}
//
//
//
//extern "C" IntBPlusElement * TEST_BPLUS_ELEMENT_REFERENCER_Reference(IntBPlusEntry * ENTRY, int16_t ELEMENT_ID);
//void PrintBPlus(IntBPlusTree* tree, IntBPlusEntry* entry, int Level, int pos) {
//	if (!entry) return;
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//	//    assert(entry->element_count >= 1);
//	if (entry->type == 1) {
//		IntBPlusEntryRbTreeIterator iter;
//		int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);
//		//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, true);
//		for (int i = entry->element_count - 1; i >= 0; i--) {
//			printf("%sleaf:::key:%d\n%sLevel:%d\n%sId:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->leaf.key, empty, Level, empty, id, empty, NULL/*, pos ? ((BPlusEntry*)PageGet(tree, entry->, 1))->indexElement[pos].key : 0*/);
//			id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
//		}
//		free(empty);
//		return;
//	}
//
//	//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, false);
//	IntBPlusEntryRbTreeIterator iter;
//	int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);
//
//	for (int i = entry->element_count; i >= 0; i--) {
//		if (i == entry->element_count) {
//			PrintBPlus(tree, entry->index.tail_child_id, Level + 1, i - 1);
//			continue;
//		}
//		printf("%sindex:::key:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.key, empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
//		PrintBPlus(tree, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.child_id, Level + 1, i);
//		id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
//	}
//	free(empty);
//}
//
//void PrintBTree(BTree* tree, BTreeEntry* entry, int Level, int pos) {
//	if (!entry) return;
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//
//	for (int i = entry->count; i >= 0; i--) {
//		if (i == entry->count) {
//			if (!entry->is_leaf) {
//				PrintBTree(tree, entry->child[entry->count], Level + 1, i - 1);
//			}
//			continue;
//		}
//		printf("%skey:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, entry->element[i], empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
//		if (!entry->is_leaf) {
//			PrintBTree(tree, entry->child[i], Level + 1, i);
//		}
//	}
//	free(empty);
//}
//
//
//void PrintSkipList(TsSkipList* list) {
//	auto cur = list->head;
//	//while (cur) {
//	//	auto entry = ObjectGetFromField(cur, TsSkipListEntry, upper);
//	//	if (cur != list->head) {
//	//		for (int j = 0; j < entry->level_count; j++) {
//	//			printf("key:%d\t\t", entry->key);
//	//		}
//	//		printf("\n\n");
//	//	}
//	//	if (cur[0].next) {
//	//		cur = (TsSkipListLevel*)cur[0].next->upper;
//	//	}
//	//	else {
//	//		cur = NULL;
//	//	}
//	//}
//}
//

//
////struct QVQ2 {
////	int key;
////	ListEntry entry;
////};
////ListEntry gListHead;
////
////Queue que;
//
////int hashCmp(const void* i_, const void* j_, size_t) {
////	int i = *(int*)i_;
////	int j = *(int*)j_;
////	return i - j;
////}
//
//
////volatile int k = 0;
////SpinLock lock;
////MutexLock slock;
////std::map<HANDLE, int> threadcc;
////#include <mutex>
////std::mutex mutexaa;
////CRITICAL_SECTION cs;
////DWORD WINAPI testfunc(
////	LPVOID lpThreadParameter
////	) {
////	int num = (int)lpThreadParameter;
////	DWORD temp = GetTickCount();
////	for (int j = 0; j < 100000000; j++) {
////		/*SpinLockAcquire(&lock);
////		++k;
////		SpinLockRelease(&lock);*/
////
////		/*SleepLockAcquire(&slock);
////		++k;
////		SleepLockRelease(&slock);*/
////
////		/*
////		mutexaa.lock();
////		++k;
////		mutexaa.unlock();
////		*/
////
////		/*EnterCriticalSection(&cs);
////		++k;
////		LeaveCriticalSection(&cs);*/
////
////		// ++k;
////
////		//AtomicIncrement32(&k);
////	}
////	printf("num:%d, i:%d, %dms\n\n", num, k, GetTickCount() - temp);
////	return 0;
////}
//
//
//
//
//
//
//
//
////extern "C" {
////	IntRbEntry* INT_RB_TREE_ACCESSOR_GetParent(IntRbEntry* entry);
////	int INT_RB_TREE_ACCESSOR_GetColor(IntRbEntry* entry);
////}
//void PrintRB(IntRbTree* tree, IntRbEntry* entry, int Level) {
//	if (!entry) return;
//	PrintRB(tree, entry->right, Level + 1);
//
//	//print
//	const char* str = "Not";
//	const char* color = INT_RB_TREE_ACCESSOR_GetColor(tree, entry) == kRbRed ? "Red" : "Black";
//
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//
//	printf("%skey:%d\n%sLevel:%d\n%sParent.%s:%x\n%scolor:%s\n\n", empty, ((IntEntry_Rb*)entry)->key, empty, Level, empty, str, 0, empty, color);
//
//	free(empty);
//
//	PrintRB(tree, INT_RB_TREE_ACCESSOR_GetLeft(tree, entry), Level + 1);
//}
//
//#include <libyuc/container/lru_list.h>
//
//#include <thread>
//
//
//void PrintArt(ArTree* tree, ArNode* node, int Level) {
//	if (!node)return;
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//	if (ArNodeIsLeaf(node)) {
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//		printf("%sleaf:%s\n", empty, ArLeafGetElement(ArNodeGetLeaf(node))->buf);
//#else
//		printf("%sleaf:%llx\n", empty, ArLeafGetElement(ArNodeGetLeaf(node)));
//#endif
//	}
//	else {
//		if (node->head.node_type == kArNode4) {
//			const char* abc = "";
//			char prefix[prefix_max_len + 1];
//			prefix[0] = 0;
//			if (node->head.prefix_len > prefix_max_len) {
//				abc = "...";
//			}
//			else {
//				memcpy(prefix, node->head.prefix, node->head.prefix_len);
//				prefix[node->head.prefix_len] = 0;
//			}
//			printf("%snode4:\n%s        prefix:%d[%d] \"%s%s\"\n", empty, empty, node->head.prefix_len, node->head.prefix_len, prefix, abc);
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			if (node->head.eof) {
//				printf("%s        eof:\n", empty);
//
//				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
//			}
//#endif
//			for (int i = 0; i < node->head.child_count; i++) {
//				printf("%s        [%c]:\n", empty, node->node4.keys[i]);
//				PrintArt(tree, node->node4.child_arr[i], Level + 1);
//			}
//		}
//		else if (node->head.node_type == kArNode16) {
//			const char* abc = "";
//			char prefix[prefix_max_len + 1];
//			prefix[0] = 0;
//			if (node->head.prefix_len > prefix_max_len) {
//				abc = "...";
//			}
//			else {
//				memcpy(prefix, node->head.prefix, node->head.prefix_len);
//				prefix[node->head.prefix_len] = 0;
//			}
//			printf("%snode16:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			if (node->head.eof) {
//				printf("%s        eof:\n", empty);
//				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
//			}
//#endif
//			for (int i = 0; i < node->head.child_count; i++) {
//				printf("%s        [%c]:\n", empty, node->node16.keys[i]);
//				PrintArt(tree, node->node16.child_arr[i], Level + 1);
//			}
//		}
//		else if (node->head.node_type == kArNode48) {
//			const char* abc = "";
//			char prefix[prefix_max_len + 1];
//			prefix[0] = 0;
//			if (node->head.prefix_len > prefix_max_len) {
//				abc = "...";
//			}
//			else {
//				memcpy(prefix, node->head.prefix, node->head.prefix_len);
//				prefix[node->head.prefix_len] = 0;
//			}
//			printf("%snode48:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			if (node->head.eof) {
//				printf("%s        eof:\n", empty);
//				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
//			}
//#endif
//			for (int i = 0; i < 255; i++) {
//				if (node->node48.keys[i] != 0xff) {
//					printf("%s        [%c]:\n", empty, i);
//					PrintArt(tree, node->node48.child_arr.obj_arr[node->node48.keys[i]].child, Level + 1);
//				}
//			}
//		}
//		else {
//			const char* abc = "";
//			char prefix[prefix_max_len + 1];
//			prefix[0] = 0;
//			if (node->head.prefix_len > prefix_max_len) {
//				abc = "...";
//			}
//			else {
//				memcpy(prefix, node->head.prefix, node->head.prefix_len);
//				prefix[node->head.prefix_len] = 0;
//			}
//			printf("%snode256:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			if (node->head.eof) {
//				printf("%s        eof:\n", empty);
//				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
//			}
//#endif
//			for (int i = 0; i < 255; i++) {
//				if (node->node256.child_arr[i] != NULL) {
//					printf("%s        [%c]:\n", empty, i);
//					PrintArt(tree, node->node256.child_arr[i], Level + 1);
//				}
//			}
//		}
//	}
//	free(empty);
//}
//
//void PrintHashTable(IntHashTable* table) {
//	//IntHashTableIterator iter;
//	//for(uint32_t i = 0; i< table->bucket.capacity;i++){
//	//	if (table->bucket.obj_arr[i].dist != 0){
//	//		printf("[%d][%d]%llx\n", i, table->bucket.obj_arr[i].dist, table->bucket.obj_arr[i].obj);
//	//	}else{
//	//		printf("[%d]null\n", i);
//	//	}
//	//}
//}
//
//
//


//
//

//
//void TestSkipList() {
//	printf("\n������\n");
//	SkipList list;
//	SkipListInit(&list);
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		SkipListPut(&list, (&arr2[i]->key));
//		if (count < 20) {
//			//PrintSkipList(&list);
//			//printf("\n\n\n\n");
//		}
//	}
//	printf("�����ʱ��%dms\n", GetTickCount() - l);
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		if (!SkipListFind(&list, (&arr2[i]->key))) {
//			printf("�Ҳ���");
//		}
//	}
//	printf("���Һ�ʱ��%dms\n", GetTickCount() - l);
//
//	//int64_t old = 0;
//	//auto entry = list.head[0].next;
//	//while (entry) {
//	//	if (entry->element < old) {
//	//		printf("����");
//	//	}
//	//	printf("%llx ", old);
//	//	old = entry->element;
//	//	entry = entry->upper[0].next;
//	//}
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//
//		if (!SkipListDelete(&list, (&arr2[i]->key))) {
//			printf("�Ҳ���");
//		}
//		if (count < 20) {
//			//PrintSkipList(&list);
//			//printf("\n\n\n\n");
//		}
//	}
//	printf("ɾ����ʱ��%dms\n", GetTickCount() - l);
//
//}
//
//
//
//void TestTsSortSinglyListInsertThread(TsSortSinglyListHead* head, int j) {
//	for (int i = 0; i < count / thread_count; i++) {
//		TsSortSinglyListInsert(head, (TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right);
//	}
//}
//
//void TestTsSortSinglyListDeleteThread(TsSortSinglyListHead* head, int j) {
//	for (int i = 0; i < count / thread_count; i++) {
//		TsSortSinglyListDelete((TsSortSinglyListEntry*)head, TsSortSinglyListIteratorFirst(head), ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
//	}
//}
//
//void TestTsSortSinglyList() {
//	printf("�̰߳�ȫ����������%d���߳�\n", thread_count);
//
//	TsSortSinglyListHead head;
//	head.first = NULL;
//
//	std::vector<std::thread> t;
//	t.reserve(thread_count);
//	l = GetTickCount();
//
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread(TestTsSortSinglyListInsertThread, &head, i));
//		//t.push_back(std::thread(TestTsSortSinglyListDeleteThread, &head, i));
//	}
//
//	for (auto& thread : t) {
//		thread.join();
//	}
//
//	printf("%d���̣߳�����&ɾ���ܺ�ʱ��%dms    %d\n", thread_count, GetTickCount() - l, 0, 0);
//
//
//	TsSortSinglyListEntry* prev = NULL;
//	int i = 0;
//	TsSortSinglyListEntry* cur = TsSortSinglyListIteratorFirst(&head);
//	while (cur) {
//		if (prev != NULL) {
//			if (((TsSortSinglyListEntry*)prev)->key > ((TsSortSinglyListEntry*)cur)->key) {
//				printf("error");
//			}
//		}
//		i++;
//		prev = cur;
//		cur = TsSortSinglyListIteratorNext(cur);
//	}
//
//	printf("%d\n", i);
//
//	t.clear();
//	t.reserve(thread_count);
//	l = GetTickCount();
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread(TestTsSortSinglyListDeleteThread, &head, i));
//	}
//
//	for (auto& thread : t) {
//		thread.join();
//	}
//	printf("%d���̣߳�ɾ���ܺ�ʱ��%dms    %d\n", thread_count, GetTickCount() - l, 0, 0);
//
//	prev = NULL;
//	i = 0;
//	cur = TsSortSinglyListIteratorFirst(&head);
//	while (cur) {
//		if (prev != NULL) {
//			if (((TsSortSinglyListEntry*)prev)->key > ((TsSortSinglyListEntry*)cur)->key) {
//				printf("error");
//			}
//		}
//		i++;
//		prev = cur;
//		cur = TsSortSinglyListIteratorNext(cur);
//	}
//	printf("%d\n", i);
//}
//
//void TestTsSkipListInsertThread(TsSkipList* list, int j) {
//	for (int i = 0; i < count / thread_count; i++) {
//		TsSkipListPut(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key, &ptr_arr[j * (count / thread_count) + i]);
//
//		// PrintSkipList(list);
//		// printf("\n\n\n\n");
//	}
//}
//
//void TestTsSkipListFindThread(TsSkipList* list, int j) {
//	for (int i = 0; i < count / thread_count; i++) {
//		if (!TsSkipListFind(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key)) {
//			printf("�Ҳ���:%d", ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
//		}
//		// PrintSkipList(list);
//		// printf("\n\n\n\n");
//	}
//}
//
//void TestTsSkipListDeleteThread(TsSkipList* list, int j) {
//	for (int i = 0; i < count / thread_count; i++) {
//		if (!TsSkipListDelete(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key)) {
//			//printf("ɾ��ʧ�ܣ�%d", ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
//		}
//		// PrintSkipList(list);
//		// printf("\n\n\n\n");
//	}
//}
//
//
//void TestTsSkipList() {
//
//_re:
//
//	printf("�̰߳�ȫ������%d���߳�\n", thread_count);
//
//	TsSkipList list;
//	TsSkipListInit(&list);
//
//	ptr_arr.resize(count, nullptr);
//
//	std::vector<std::thread> t;
//	t.reserve(thread_count);
//	l = GetTickCount();
//
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread(TestTsSkipListInsertThread, &list, i));
//		//t.push_back(std::thread(TestTsSkipListDeleteThread, &list, i));
//	}
//
//	for (auto& thread : t) {
//		thread.join();
//	}
//
//	printf("����&ɾ���ܺ�ʱ��%dms    %d\n", GetTickCount() - l, 0, 0);
//
//	PrintSkipList(&list);
//
//	t.clear();
//	l = GetTickCount();
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread(TestTsSkipListFindThread, &list, i));
//	}
//
//	for (auto& thread : t) {
//		thread.join();
//	}
//	printf("�����ܺ�ʱ��%dms\n", GetTickCount() - l);
//
//	//for (int i = 0; i < ptr_arr.size(); i++) {
//	//	if (ptr_arr[i])
//	//	    release_assert(ptr_arr[i]->reference_count == ptr_arr[i]->level_record, "insert error");
//	//}
//
//	t.clear();
//	l = GetTickCount();
//
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread(TestTsSkipListDeleteThread, &list, i));
//	}
//
//	for (auto& thread : t) {
//		thread.join();
//	}
//
//	printf("ɾ���ܺ�ʱ��%dms    %d\n", GetTickCount() - l, 0, 0);
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		if (TsSkipListFind(&list, (arr2[i]->key))) {
//			printf("�ҵ��ˣ�%d", arr2[i]->key);
//		}
//	}
//	printf("���Һ�ʱ��%dms\n", GetTickCount() - l);
//
//	//
//
//
//	for (int i = 0; i < ptr_arr.size(); i++) {
//		if (ptr_arr[i]) {
//			release_assert(ptr_arr[i]->reference_count == 0, "reference count not 0.");
//		}
//		ObjectRelease(ptr_arr[i]);
//		ptr_arr[i] = NULL;
//	}
//
//	for (int i = 0; i < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL; i++) {
//		// release_assert(list.head[i].next == NULL, "delete head error");
//		list.head[i].next == NULL;
//	}
//
//	list.level = 0;
//
//
//	goto _re;
//}
//
//void TestEpoch() {
//	Epoch epoch;
//	std::vector<std::thread> t;
//	t.reserve(thread_count);
//	l = GetTickCount();
//
//	for (int i = 0; i < thread_count; i++) {
//		t.push_back(std::thread([&]() {
//			EpochEntry(&epoch);
//			EpochEntry(&epoch);
//			}
//		));
//	}
//	for (auto& thread : t) {
//		thread.join();
//	}
//
//}
