#define _CRT_RAND_S 

#include <stdio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <thread>


// #include "C:\Users\gt1\Desktop\unordered_dense.h"


#include <libyuc/container/experimental/ar_tree.h>
#include <libyuc/container/experimental/skip_list.h>
#include <libyuc/container/thread_safe/experimental/epoch.h>
#include <libyuc/container/thread_safe/experimental/ts_skip_list.h>
#include <libyuc/container/thread_safe/experimental/ts_sort_singly_list.h>
#include <libyuc/container/experimental/btree.h>

#include <regex>
#include "test.h"
int randInt() {
	return (rand() << 16) | rand();
}





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

extern "C" IntBPlusElement * TEST_BPLUS_ELEMENT_REFERENCER_Reference(IntBPlusEntry * ENTRY, int16_t ELEMENT_ID);
void PrintBPlus(IntBPlusTree* tree, IntBPlusEntry* entry, int Level, int pos) {
	if (!entry) return;
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;
	//    assert(entry->element_count >= 1);
	if (entry->type == 1) {
		IntBPlusEntryRbTreeIterator iter;
		int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);
		//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, true);
		for (int i = entry->element_count - 1; i >= 0; i--) {
			printf("%sleaf:::key:%d\n%sLevel:%d\n%sId:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->leaf.key, empty, Level, empty, id, empty, NULL/*, pos ? ((BPlusEntry*)PageGet(tree, entry->, 1))->indexElement[pos].key : 0*/);
			id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
		}
		free(empty);
		return;
	}

	//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, false);
	IntBPlusEntryRbTreeIterator iter;
	int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree, &iter);

	for (int i = entry->element_count; i >= 0; i--) {
		if (i == entry->element_count) {
			PrintBPlus(tree, entry->index.tail_child_id, Level + 1, i - 1);
			continue;
		}
		printf("%sindex:::key:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.key, empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
		PrintBPlus(tree, TEST_BPLUS_ELEMENT_REFERENCER_Reference(entry, id)->index.child_id, Level + 1, i);
		id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, &iter);
	}
	free(empty);
}

void PrintBTree(BTree* tree, BTreeEntry* entry, int Level, int pos) {
	if (!entry) return;
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;

	for (int i = entry->count; i >= 0; i--) {
		if (i == entry->count) {
			if (!entry->is_leaf) {
				PrintBTree(tree, entry->child[entry->count], Level + 1, i - 1);
			}
			continue;
		}
		printf("%skey:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, entry->element[i], empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
		if (!entry->is_leaf) {
			PrintBTree(tree, entry->child[i], Level + 1, i);
		}
	}
	free(empty);
}


void PrintSkipList(TsSkipList* list) {
	auto cur = list->head;
	//while (cur) {
	//	auto entry = ObjectGetFromField(cur, TsSkipListEntry, upper);
	//	if (cur != list->head) {
	//		for (int j = 0; j < entry->level_count; j++) {
	//			printf("key:%d\t\t", entry->key);
	//		}
	//		printf("\n\n");
	//	}
	//	if (cur[0].next) {
	//		cur = (TsSkipListLevel*)cur[0].next->upper;
	//	}
	//	else {
	//		cur = NULL;
	//	}
	//}
}

void PrintAvl(IntAvlTree* head, IntAvlEntry* entry, IntAvlEntry* parent, int Level) {
	if (!entry) return;
	PrintAvl(head, INT_AVL_TREE_ACCESSOR_GetRight(tree, entry), entry, Level + 1);

	const char* str = "Not";
	if (parent) {
		str = (INT_AVL_TREE_ACCESSOR_GetRight(tree, parent) == entry ? "Right" : "Left");
	}
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;


	printf("%skey:%lld\n%sLevel:%d\n%sBalance factor:%d\n%sParent.%s:%lld\n\n", empty, *INT_AVL_TREE_ACCESSOR_GetKey(head, entry), empty, Level, empty, INT_AVL_TREE_ACCESSOR_GetBalanceFactor(head, entry), empty, str, parent ? *INT_AVL_TREE_ACCESSOR_GetKey(head, parent) : NULL);

	free(empty);

	PrintAvl(head, INT_AVL_TREE_ACCESSOR_GetLeft(tree, entry), entry, Level + 1);
}

//struct QVQ2 {
//	int key;
//	ListEntry entry;
//};
//ListEntry gListHead;
//
//Queue que;

//int hashCmp(const void* i_, const void* j_, size_t) {
//	int i = *(int*)i_;
//	int j = *(int*)j_;
//	return i - j;
//}


//volatile int k = 0;
//SpinLock lock;
//MutexLock slock;
//std::map<HANDLE, int> threadcc;
//#include <mutex>
//std::mutex mutexaa;
//CRITICAL_SECTION cs;
//DWORD WINAPI testfunc(
//	LPVOID lpThreadParameter
//	) {
//	int num = (int)lpThreadParameter;
//	DWORD temp = GetTickCount();
//	for (int j = 0; j < 100000000; j++) {
//		/*SpinLockAcquire(&lock);
//		++k;
//		SpinLockRelease(&lock);*/
//
//		/*SleepLockAcquire(&slock);
//		++k;
//		SleepLockRelease(&slock);*/
//
//		/*
//		mutexaa.lock();
//		++k;
//		mutexaa.unlock();
//		*/
//
//		/*EnterCriticalSection(&cs);
//		++k;
//		LeaveCriticalSection(&cs);*/
//
//		// ++k;
//
//		//AtomicIncrement32(&k);
//	}
//	printf("num:%d, i:%d, %dms\n\n", num, k, GetTickCount() - temp);
//	return 0;
//}








//extern "C" {
//	IntRbEntry* INT_RB_TREE_ACCESSOR_GetParent(IntRbEntry* entry);
//	int INT_RB_TREE_ACCESSOR_GetColor(IntRbEntry* entry);
//}
void PrintRB(IntRbTree* tree, IntRbEntry* entry, int Level) {
	if (!entry) return;
	PrintRB(tree, entry->right, Level + 1);

	//print
	const char* str = "Not";
	const char* color = INT_RB_TREE_ACCESSOR_GetColor(tree, entry) == kRbRed ? "Red" : "Black";

	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;

	printf("%skey:%d\n%sLevel:%d\n%sParent.%s:%x\n%scolor:%s\n\n", empty, ((IntEntry_Rb*)entry)->key, empty, Level, empty, str, 0, empty, color);

	free(empty);

	PrintRB(tree, INT_RB_TREE_ACCESSOR_GetLeft(tree, entry), Level + 1);
}

#include <libyuc/container/lru_list.h>

#include <thread>


void PrintArt(ArTree* tree, ArNode* node, int Level) {
	if (!node)return;
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;
	if (ArNodeIsLeaf(node)) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		printf("%sleaf:%s\n", empty, ArLeafGetElement(ArNodeGetLeaf(node))->buf);
#else
		printf("%sleaf:%llx\n", empty, ArLeafGetElement(ArNodeGetLeaf(node)));
#endif
	}
	else {
		if (node->head.node_type == kArNode4) {
			const char* abc = "";
			char prefix[prefix_max_len + 1];
			prefix[0] = 0;
			if (node->head.prefix_len > prefix_max_len) {
				abc = "...";
			}
			else {
				memcpy(prefix, node->head.prefix, node->head.prefix_len);
				prefix[node->head.prefix_len] = 0;
			}
			printf("%snode4:\n%s        prefix:%d[%d] \"%s%s\"\n", empty, empty, node->head.prefix_len, node->head.prefix_len, prefix, abc);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			if (node->head.eof) {
				printf("%s        eof:\n", empty);
				
				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
			}
#endif
			for (int i = 0; i < node->head.child_count; i++) {
				printf("%s        [%c]:\n", empty, node->node4.keys[i]);
				PrintArt(tree, node->node4.child_arr[i], Level + 1);
			}
		}
		else if (node->head.node_type == kArNode16) {
			const char* abc = "";
			char prefix[prefix_max_len + 1];
			prefix[0] = 0;
			if (node->head.prefix_len > prefix_max_len) {
				abc = "...";
			}
			else {
				memcpy(prefix, node->head.prefix, node->head.prefix_len);
				prefix[node->head.prefix_len] = 0;
			}
			printf("%snode16:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			if (node->head.eof) {
				printf("%s        eof:\n", empty);
				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
			}
#endif
			for (int i = 0; i < node->head.child_count; i++) {
				printf("%s        [%c]:\n", empty, node->node16.keys[i]);
				PrintArt(tree, node->node16.child_arr[i], Level + 1);
			}
		}
		else if (node->head.node_type == kArNode48) {
			const char* abc = "";
			char prefix[prefix_max_len + 1];
			prefix[0] = 0;
			if (node->head.prefix_len > prefix_max_len) {
				abc = "...";
			}
			else {
				memcpy(prefix, node->head.prefix, node->head.prefix_len);
				prefix[node->head.prefix_len] = 0;
			}
			printf("%snode48:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			if (node->head.eof) {
				printf("%s        eof:\n", empty);
				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
			}
#endif
			for (int i = 0; i < 255; i++) {
				if (node->node48.keys[i] != 0xff) {
					printf("%s        [%c]:\n", empty, i);
					PrintArt(tree, node->node48.child_arr.obj_arr[node->node48.keys[i]].child, Level + 1);
				}
			}
		}
		else {
			const char* abc = "";
			char prefix[prefix_max_len + 1];
			prefix[0] = 0;
			if (node->head.prefix_len > prefix_max_len) {
				abc = "...";
			}
			else {
				memcpy(prefix, node->head.prefix, node->head.prefix_len);
				prefix[node->head.prefix_len] = 0;
			}
			printf("%snode256:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			if (node->head.eof) {
				printf("%s        eof:\n", empty);
				PrintArt(tree, (ArNode*)ArNodeGetEofChild(node), Level + 1);
			}
#endif
			for (int i = 0; i < 255; i++) {
				if (node->node256.child_arr[i] != NULL) {
					printf("%s        [%c]:\n", empty, i);
					PrintArt(tree, node->node256.child_arr[i], Level + 1);
				}
			}
		}
	}
	free(empty);
}

void PrintHashTable(IntHashTable* table) {
	//IntHashTableIterator iter;
	//for(uint32_t i = 0; i< table->bucket.capacity;i++){
	//	if (table->bucket.obj_arr[i].dist != 0){
	//		printf("[%d][%d]%llx\n", i, table->bucket.obj_arr[i].dist, table->bucket.obj_arr[i].obj);
	//	}else{
	//		printf("[%d]null\n", i);
	//	}
	//}
}



struct QVQ {
	IntRbEntry entry;
	int64_t key;
	//AvlEntry entry;
	//int height;
	//LruEntry lru_entry;
};

DWORD l;
int count = 200000;
std::vector<QVQ*> arr2;
//int seed = GetTickCount() + rand();
int seed = 377884212;

std::vector<TsSkipListEntry*> ptr_arr;

int section = 1;

int thread_count = 10;

void TestArt() {
	printf("\n自适应基数树：\n");
	ArTree artree;
	ArTreeInit(&artree);

	//l = GetTickCount();
	//int j = 0;
	//for (int jjj = 0; jjj < 10; jjj++) {
	//	for (int i = 0; i < count; i++) {
	//		__m128i results = _mm_cmpeq_epi8(_mm_set1_epi8(0x22), _mm_loadu_si128((__m128i*) & arr2[i]->key));
	//		ptrdiff_t mask = (1 << 16) - 1;
	//		ptrdiff_t j = _mm_movemask_epi8(results) & mask;
	//		if (!j) {
	//			j++;
	//		}
	//		j = _tzcnt_u32(j);
	//		if (j == 16) {
	//			printf("不存在");
	//		}
	//	}
	//}
	//l = GetTickCount() - l;
	//printf("simd查找耗时：%dms%d\n", l, j);

	//l = GetTickCount();
	//uint8_t key = 0x22;
	//j = 0;
	//for (int jjj = 0; jjj < 10; jjj++) {
	//	for (int i = 0; i < count; i++) {
	//		ptrdiff_t j = ArNodeKeyBinarySearch((uint8_t*)&arr2[i]->key, 0, 15, &key);
	//		if (j == 16) {
	//			printf("不存在");
	//		}
	//	}
	//}
	//l = GetTickCount() - l;
	//printf("bs查找耗时：%dms%d\n", l, j);

	//l = GetTickCount();
	//j = 0;
	//for (int jjj = 0; jjj < 10; jjj++) {
	//	for (int i = 0; i < count; i++) {
	//		//ptrdiff_t j = ArNodeKeyBinarySearch((uint8_t*)&arr2[i]->key, 0, 15, &key);
	//		ptrdiff_t j = ArNodeKeyArrayFind((uint8_t*)&arr2[i]->key, 16, &key);
	//		if (j == 15) {
	//			printf("不存在");
	//		}
	//	}
	//}
	//l = GetTickCount() - l;
	//printf("直接查找耗时：%dms%d\n", l, j);

	//


	//element_type ele;
	//ele.buf = (uint8_t*)"abcdefgh";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//ele.buf = (uint8_t*)"abcdefgi";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//ele.buf = (uint8_t*)"abcdefghi";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//ele.buf = (uint8_t*)"abcdefghij";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//ele.buf = (uint8_t*)"abcdefghijk";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//PrintArt(&artree, artree.root, 0);

	//ele.buf = (uint8_t*)"abcde";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//ele.buf = (uint8_t*)"abcdf";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);
	//PrintArt(&artree, artree.root, 0);
	//ele.buf = (uint8_t*)"abc";
	//ele.size = strlen((char*)ele.buf);
	//ArTreeDelete(&artree, &ele);
	//PrintArt(&artree, artree.root, 0);

	//ele.buf = (uint8_t*)"abd";
	//ele.size = strlen((char*)ele.buf);
	//ArTreePut(&artree, &ele);

	//PrintArt(&artree, artree.root, 0);

	//ArTreeDelete(&artree, &ele);

	l = GetTickCount();
	int32_t ii = 0;

	for (auto& it : arr2) {
		ii++;
		/*if (ii == 986) {
			printf("??");
		}*/
		
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		element_type* ele = ObjectCreate(element_type);
		ele->buf = (uint8_t*)&it->key;
		ele->size = sizeof(it->key);
		ArTreePut(&artree, ele);
#else

		ArTreePut(&artree, (art_element_type*)&it->key);
		//ArTreePut(&artree, (art_element_type*)it->key << 1);
#endif
		//if (ii == 27) {
		//	printf("??");
		//}

		

		//printf("插入:%s\n", ele->buf);
		//ele.buf = (uint8_t*)"aacdeedb";
		//ele.size = strlen((char*)ele.buf);
		////
		//PrintArt(&artree, artree.root, 0);
		//printf("\n\n\n\n");
		////
		//if (!ArTreeFind(&artree, &ele)) {
		//	printf("找不到");
		//}
		//else {
		//	printf("找到了");
		//}

	}

	l = GetTickCount() - l;
	printf("插入耗时：%dms\n", l);
	//PrintArt(&artree, artree.root, 0);
	l = GetTickCount();
	ii = 0;
	for (auto& it : arr2) {
		ii++;
		art_element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		ele.buf = (uint8_t*)&it->key;
		ele.size = sizeof(it->key);
		if (!ArTreeFind(&artree, &ele)) {
			printf("找不到");
		}
#else
		ele = it->key;
		if (!ArTreeFind(&artree, &ele)) {
			printf("找不到");
		}
		/*if (!ArTreeFind(&artree, (key_type*)(it->key << 1))) {
			printf("找不到");
		}*/
#endif
		
	}
	//PrintArt(&artree, artree.root, 0);
	l = GetTickCount() - l;
	printf("查找耗时：%dms\n", l);
	l = GetTickCount();
	ii = 0;
	for (auto& it : arr2) {
		ii++;
		art_element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		ele.buf = (uint8_t*)&it->key;
		ele.size = sizeof(it->key);
		if (!ArTreeDelete(&artree, &ele)) {
			printf("找不到");
			//PrintArt(&artree, artree.root, 0);
		}
#else
		ele = it->key;
		if (!ArTreeDelete(&artree, &ele)) {
			printf("找不到");
			//PrintArt(&artree, artree.root, 0);
		}
		/*if (!ArTreeDelete(&artree, (key_type*)(it->key << 1))) {
			printf("找不到");
		}*/
#endif
		//if (ii == 0x4390) {
		//	printf("??");
		//}

		

		//printf("删除:%s\n", ele.buf);
		//uint8_t buf[] = {05, 05, 05, 05, 05, 05, 05, 01};
		//ele.buf = buf;
		//ele.size = 8;
		//PrintArt(&artree, artree.root, 0);

		//ele = 0x658d2b070e2e2726;
		//if (!ArTreeFind(&artree, &ele)) {
		//	printf("找不到");
		//	//PrintArt(&artree, artree.root, 0);
		//}
		//else {
		//	//printf("找到了");
		//}

		//PrintArt(&artree, artree.root, 0);
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);



	//goto qqqqqq;



	int qqc = 1;

	l = GetTickCount();

	//printf("插入耗时：%dms    %d\n", GetTickCount() - l, 0, 0/*BsTreeGetEntryCount(&gRb.bst)*/);
//	for (int j = 0; j < qqc; j++) {
//		ArTreeInit(&artree);
//		for (int i = 0; i < count / qqc; i++) {
//			element_type ele;
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			ele.buf = (uint8_t*)&arr2[i]->key;
//			ele.size = sizeof(arr2[i]->key);
//#else
//			ele = arr2[i]->key;
//#endif
//			//if (ii == 27) {
//			//	printf("??");
//			//}
//
//			ArTreePut(&artree, &ele);
//		}
//
//		for (int i = 0; i < count / qqc; i++) {
//			element_type ele;
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			ele.buf = (uint8_t*)&arr2[i]->key;
//			ele.size = sizeof(arr2[i]->key);
//#else
//			ele = arr2[i]->key;
//#endif
//			//if (ii == 27) {
//			//	printf("??");
//			//}
//
//			if (!ArTreeFind(&artree, &ele)) {
//				printf("找不到");
//			}
//		}
//		//printf("查找耗时：%dms\n", GetTickCount() - l);
//
//
//		//PrintRB(&gRb, gRb.root, 0);
//		 //for (int i = 10000000; i >= 1; i--) {
//		 //	int key = i;
//		 //	RBDeleteEntryByKey(&gRb, &key);
//		 //	// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
//		 //}
//
//		for (int i = 0; i < count / qqc; i++) {
//			element_type ele;
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//			ele.buf = (uint8_t*)&arr2[i]->key;
//			ele.size = sizeof(arr2[i]->key);
//#else
//			ele = arr2[i]->key;
//#endif
//			ArTreeDelete(&artree, &ele);
//
//			// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
//		}
//		//printf("删除耗时：%dms    \n", GetTickCount() - l);
//
//	}
	printf("总耗时：%dms    \n", GetTickCount() - l);
}
 
void TestAvl() {
	printf("\n无父结点Avl树：\n");
	IntAvlTree tree;

	l = GetTickCount();
	//void** buf = (void**)malloc((count / section) * sizeof(void*));

	IntAvlBsStackVector stack;
	IntAvlBsStackVectorInit(&stack);

	for (int j = 0; j < section; j++) {

		IntAvlTreeInit(&tree);
		l = GetTickCount();
		for (int i = 0; i < count / section; i++) {
			//if (!IntRbTreeInsert(&tree, (IntRbEntry*)&arr2[i]->entry)) {
			//	//printf("失败%d", i);
			//}
			//buf[i] = malloc(16);

			IntAvlTreePut(&tree, &stack, (IntAvlEntry*)&arr2[i]->entry);

			if (count <= 20) {

				PrintAvl(&tree, tree.root, NULL, 0);
				printf("\n\n\n\n");
			}
		}



		//PrintAvl(&tree, tree.root, NULL, 0);
		//printf("\n\n\n\n");
		printf("插入耗时：%dms    %d\n", GetTickCount() - l, 0, 0/**/);

		if (!IntAvlTreeVerify(&tree)) {
			printf("不是正确的avl");
		}

		l = GetTickCount();

		for (int i = 0; i < count / section; i++) {
			if (!IntAvlTreeFind(&tree, &stack, &arr2[i]->key)) {
				printf("找不到");
			}
		}

		printf("查找耗时：%dms\n", GetTickCount() - l);
		l = GetTickCount();

		for (int i = 0; i < count / section; i++) {
			IntAvlEntry* entry = IntAvlTreeFind(&tree, &stack, &arr2[i]->key);
			if (!IntAvlTreeDelete(&tree, &stack, entry)) {
				printf("无法删除");
			}

		}
		printf("删除耗时：%dms    \n", GetTickCount() - l);

	}



	printf("总耗时：%dms    \n", GetTickCount() - l);

}


void TestRb() {
	printf("\n无父结点红黑树：\n");
	IntRbTree tree;

	l = GetTickCount();
	//void** buf = (void**)malloc((count / section) * sizeof(void*));

	for (int j = 0; j < section; j++) {

		IntRbTreeInit(&tree);
		l = GetTickCount();
		for (int i = 0; i < count / section; i++) {
			//if (!IntRbTreeInsert(&tree, (IntRbEntry*)&arr2[i]->entry)) {
			//	//printf("失败%d", i);
			//}
			//buf[i] = malloc(16);
			IntRbTreeInsert(&tree, (IntRbEntry*)&arr2[i]->entry);
			if (count < 20) {

				PrintRB(&tree, tree.root, 0);
				printf("\n\n\n\n");
			}
		}



		//PrintRB(&tree, tree.root, 0);
		//printf("\n\n\n\n");
		printf("插入耗时：%dms    %d\n", GetTickCount() - l, 0, 0/**/);


		l = GetTickCount();
		auto rbcount = IntRbTreeGetCount(&tree);
		printf("统计count:%d        %dms\n", rbcount, GetTickCount() - l);

		if (!IntRbTreeVerify(&tree)) {
			printf("不正确的红黑树");
		}

		l = GetTickCount();

		for (int i = 0; i < count / section; i++) {
			if (!IntRbTreeFind(&tree, &arr2[i]->key)) {
				printf("找不到");
			}
		}

		printf("查找耗时：%dms\n", GetTickCount() - l);

		l = GetTickCount();

		for (int i = 0; i < count / section; i++) {

			if (!IntRbTreeDelete(&tree, &arr2[i]->key)) {
				printf("无法删除");
			}

			if (count < 20) {
				PrintRB(&tree, tree.root, 0);
				printf("\n\n\n\n");
			}
			//if (!IntRbTreeVerify(&tree)) {
			//	printf("不正确的红黑树");
			//}
			// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
		}
		printf("删除耗时：%dms    \n", GetTickCount() - l);

	}
	printf("总耗时：%dms    \n", GetTickCount() - l);

}

void TestSkipList() {
	printf("\n跳表：\n");
	SkipList list;
	SkipListInit(&list);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		SkipListPut(&list, (&arr2[i]->key));
		if (count < 20) {
			//PrintSkipList(&list);
			//printf("\n\n\n\n");
		}
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);
	
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!SkipListFind(&list, (&arr2[i]->key))) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);

	//int64_t old = 0;
	//auto entry = list.head[0].next;
	//while (entry) {
	//	if (entry->element < old) {
	//		printf("错误");
	//	}
	//	printf("%llx ", old);
	//	old = entry->element;
	//	entry = entry->upper[0].next;
	//}

	l = GetTickCount();
	for (int i = 0; i < count; i++) {

		if (!SkipListDelete(&list, (&arr2[i]->key))) {
			printf("找不到");
		}
		if (count < 20) {
			//PrintSkipList(&list);
			//printf("\n\n\n\n");
		}
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);

}



void TestTsSortSinglyListInsertThread(TsSortSinglyListHead* head, int j) {
	for (int i = 0; i < count / thread_count; i++) {
		TsSortSinglyListInsert(head, (TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right);
	}
}

void TestTsSortSinglyListDeleteThread(TsSortSinglyListHead* head, int j) {
	for (int i = 0; i < count / thread_count; i++) {
		TsSortSinglyListDelete((TsSortSinglyListEntry*)head, TsSortSinglyListIteratorFirst(head), ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
	}
}

void TestTsSortSinglyList() {
	printf("线程安全有序链表：%d个线程\n", thread_count);
	
	TsSortSinglyListHead head;
	head.first = NULL;

	std::vector<std::thread> t;
	t.reserve(thread_count);
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSortSinglyListInsertThread, &head, i));
		//t.push_back(std::thread(TestTsSortSinglyListDeleteThread, &head, i));
	}

	for (auto& thread : t) {
		thread.join();
	}

	printf("%d个线程，插入&删除总耗时：%dms    %d\n", thread_count, GetTickCount() - l, 0, 0);


	TsSortSinglyListEntry* prev = NULL;
	int i = 0;
	TsSortSinglyListEntry* cur = TsSortSinglyListIteratorFirst(&head);
	while (cur) {
		if (prev != NULL) {
			if (((TsSortSinglyListEntry*)prev)->key > ((TsSortSinglyListEntry*)cur)->key) {
				printf("error");
			}
		}
		i++;
		prev = cur;
		cur = TsSortSinglyListIteratorNext(cur);
	}

	printf("%d\n", i);

	t.clear();
	t.reserve(thread_count);
	l = GetTickCount();
	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSortSinglyListDeleteThread, &head, i));
	}

	for (auto& thread : t) {
		thread.join();
	}
	printf("%d个线程，删除总耗时：%dms    %d\n", thread_count, GetTickCount() - l, 0, 0);

	prev = NULL;
	i = 0;
	cur = TsSortSinglyListIteratorFirst(&head);
	while (cur) {
		if (prev != NULL) {
			if (((TsSortSinglyListEntry*)prev)->key > ((TsSortSinglyListEntry*)cur)->key) {
				printf("error");
			}
		}
		i++;
		prev = cur;
		cur = TsSortSinglyListIteratorNext(cur);
	}
	printf("%d\n", i);
}

void TestTsSkipListInsertThread(TsSkipList* list, int j) {
	for (int i = 0; i < count / thread_count; i++) {
		TsSkipListPut(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key, &ptr_arr[j * (count / thread_count) + i]);

		// PrintSkipList(list);
		// printf("\n\n\n\n");
	}
}

void TestTsSkipListFindThread(TsSkipList* list, int j) {
	for (int i = 0; i < count / thread_count; i++) {
		if (!TsSkipListFind(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key)) {
			printf("找不到:%d", ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
		}
		// PrintSkipList(list);
		// printf("\n\n\n\n");
	}
}

void TestTsSkipListDeleteThread(TsSkipList* list, int j) {
	for (int i = 0; i < count / thread_count; i++) {
		if (!TsSkipListDelete(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key)) {
			//printf("删除失败：%d", ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
		}
		// PrintSkipList(list);
		// printf("\n\n\n\n");
	}
}


void TestTsSkipList() {

_re:

	printf("线程安全跳表：%d个线程\n", thread_count);

	TsSkipList list;
	TsSkipListInit(&list);

	ptr_arr.resize(count, nullptr);

	std::vector<std::thread> t;
	t.reserve(thread_count);
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSkipListInsertThread, &list, i));
		//t.push_back(std::thread(TestTsSkipListDeleteThread, &list, i));
	}

	for (auto& thread : t) {
		thread.join();
	}

	printf("插入&删除总耗时：%dms    %d\n", GetTickCount() - l, 0, 0);

	PrintSkipList(&list);

	t.clear();
	l = GetTickCount();
	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSkipListFindThread, &list, i));
	}

	for (auto& thread : t) {
		thread.join();
	}
	printf("查找总耗时：%dms\n", GetTickCount() - l);

	//for (int i = 0; i < ptr_arr.size(); i++) {
	//	if (ptr_arr[i])
	//	    release_assert(ptr_arr[i]->reference_count == ptr_arr[i]->level_record, "insert error");
	//}

	t.clear();
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSkipListDeleteThread, &list, i));
	}

	for (auto& thread : t) {
		thread.join();
	}

	printf("删除总耗时：%dms    %d\n", GetTickCount() - l, 0, 0);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (TsSkipListFind(&list, (arr2[i]->key))) {
			printf("找到了：%d", arr2[i]->key);
		}
	}	
	printf("查找耗时：%dms\n", GetTickCount() - l);

	//


	for (int i = 0; i < ptr_arr.size(); i++) {
		if (ptr_arr[i]) {
			 release_assert(ptr_arr[i]->reference_count == 0, "reference count not 0.");
		}
		ObjectRelease(ptr_arr[i]);
		ptr_arr[i] = NULL;
	}

	for (int i = 0; i < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL; i++) {
		// release_assert(list.head[i].next == NULL, "delete head error");
		list.head[i].next == NULL;
	}

	list.level = 0;


	goto _re;
}

void TestEpoch() {
	Epoch epoch;
	std::vector<std::thread> t;
	t.reserve(thread_count);
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread([&]() {
			EpochEntry(&epoch);
			EpochEntry(&epoch);
			}
		));
	}
	for (auto& thread : t) {
		thread.join();
	}
	
}

void TestHashTable() {
	printf("\n哈希表：\n");

	IntHashTable table;
	IntHashTableInit(&table, 8, 0);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		IntHashTablePut(&table, &arr2[i]->key);
		//printf("666");

		//HashTableIterator iter;
		//int* obj = (int*)HashTableFirst(&table, &iter);
		//while (obj) {
		//	printf("%d\n", *obj);
		//	obj = (int*)HashTableNext(&iter);
		//}
		//printf("\n\n\n");
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);

	//HashTableIterator iter;
	//int* obj = (int*)HashTableFirst(&table, &iter);
	//while (obj) {
	//	printf("%d\n", *obj);
	//	obj = (int*)HashTableNext(&iter);
	//}
	if (count <= 20) {
		PrintHashTable(&table);
		printf("\n\n\n\n");
	}
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		//if (Hashmap_murmurhash(&i, 4) % 100000000 == 0) {
		//	//printf("找不到");
		//	j += 1;
		//}
		if (!IntHashTableFind(&table, &arr2[i]->key)) {
			printf("找不到%llx", *&arr2[i]->key);
		}
	}
	l = GetTickCount() - l;
	printf("查找耗时：%dms\n", l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!IntHashTableDelete(&table, &arr2[i]->key)) {
			printf("删除找不到");
		}
		if(count <= 20) {
			printf("删除:%llx\n", arr2[i]->key);
			PrintHashTable(&table);
			printf("\n\n\n\n");
		}
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		//if (Hashmap_murmurhash(&i, 4) % 100000000 == 0) {
		//	//printf("找不到");
		//	j += 1;
		//}
		if (IntHashTableFind(&table, &arr2[i]->key)) {
			printf("找到了");
		}
	}
	l = GetTickCount() - l;
	printf("查找耗时：%dms\n", l);


	//struct hashfunc {
	//	size_t operator()(int64_t i) const {
	//		// return HashCode_murmur3_fmix64inline(i);
	//		return ankerl::unordered_dense::hash<uint64_t>()(i);
	//	}
	//};


	struct std_hash_func {
		size_t operator()(int64_t i) const {
			// return HashCode_murmur3_fmix64inline(i);
			return std::hash<uint64_t>()(i);
		}
	};




	//ankerl::unordered_dense::set<int64_t> aaa;

	//printf("\nankerl::unordered_dense::set:\n");
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	aaa.insert(arr2[i]->key);
	//	//mapaa.insert(std::make_pair(arr2[i]->key, 0));
	//}
	//printf("插入耗时：%dms\n", GetTickCount() - l);

	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	if (aaa.find(arr2[i]->key) == aaa.end()) {
	//		printf("找不到");
	//	}
	//}
	//printf("查找耗时：%dms\n", GetTickCount() - l);
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	aaa.erase(arr2[i]->key);
	//}
	//printf("删除耗时：%dms\n", GetTickCount() - l);





	printf("\nstd::unordered_set:\n");
	l = GetTickCount();
	std::unordered_set<int64_t, std_hash_func> mapaa;
	for (int i = 0; i < count; i++) {
		mapaa.insert(arr2[i]->key);
		//mapaa.insert(std::make_pair(arr2[i]->key, 0));
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (mapaa.find(arr2[i]->key) == mapaa.end()) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		mapaa.erase(arr2[i]->key);
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);



}

//void TestTsSinglyList() {
//	TsSinglyListHead head;
//	TsSinglyListInit(head);
//	TsSinglyListPutFirst(&head, );
//}

#define ssize_t size_t
#undef max
#undef min
#include <cpp-btree/set.h>


extern "C" {
}
struct bplus_tree_config {
	int order;
	int entries;
};

void TestBPlusTree() {
	printf("\nB树：\n");
	BTree btree;
	BTreeInit(&btree);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		BTreePut(&btree, &arr2[i]->key);
		if (count < 31) {
			PrintBTree(&btree, btree.root, 0, 0);
			printf("\n\n\n\n");
		}
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		// int* qvq = CreateObject(int);
		if (!BTreeFind(&btree, &arr2[i]->key)) {
			printf("找不到, %d", arr2[i]->key);
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);
	

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		// int* qvq = CreateObject(int);
		// printf("%d", i);
		if (!BTreeDelete(&btree, &arr2[i]->key)) {
			printf("找不到, %d", &arr2[i]->key);
		}
		if (count <= 30) {
			printf("删除%d\n", arr2[i]->key);
			PrintBTree(&btree, btree.root, 0, 0);
			printf("\n\n\n\n");
		}

	}
	printf("删除耗时：%dms\n", GetTickCount() - l);

	BTreeIterator iter;
	element_type* ele = BTreeIteratorFirst(&btree, &iter);
	while (ele) {
		printf("%d  ", *ele);
		ele = BTreeIteratorNext(&btree, &iter);
	}

	//printf("\nB+树：\n");
	//IntBPlusTree bpTree;
	//IntBPlusTreeInit(&bpTree);
	//l = GetTickCount();
	//IntBPlusLeafElement element;
	//for (int i = 0; i < count; i++) {
	//	// int* qvq = CreateObject(int);
	//	element.key = arr2[i]->key;
	//	IntBPlusTreeInsert(&bpTree, &element);
	//	if (count < 31) {
	//		PrintBPlus(&bpTree, bpTree.root_id, 0, 0);
	//		printf("\n\n\n\n");
	//	}
	//}
	//// PrintBPlus(&bpTree, bpTree.root, 0, 0);
	//printf("插入耗时：%dms\n", GetTickCount() - l);
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	// int* qvq = CreateObject(int);
	//	if (!IntBPlusTreeFind(&bpTree, &arr2[i]->key)) {
	//		printf("找不到, %d", arr2[i]->key);
	//	}
	//}
	//printf("查找耗时：%dms\n", GetTickCount() - l);

	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	// int* qvq = CreateObject(int);
	//	// printf("%d", i);
	//	if (!IntBPlusTreeDelete(&bpTree, &arr2[i]->key)) {
	//		printf("找不到, %d", &arr2[i]->key);
	//	}
	//	if (count <= 30) {
	//		printf("删除%d\n", arr2[i]->key);
	//		PrintBPlus(&bpTree, bpTree.root_id, 0, 0);
	//		printf("\n\n\n\n");
	//	}

	//}
	//printf("删除耗时：%dms\n", GetTickCount() - l);


	//std::vector<std::string> str_arr(count);
	//for (int i = 0; i < count; i++) {
	//	str_arr[i] =
	//		std::to_string(arr2[i]->key);
	//}


	//printf("\nbtree:\n");
	//l = GetTickCount();
	//btree::set<std::string> bset;
	//for (int i = 0; i < count; i++) {
	//	bset.insert(str_arr[i]);
	//}
	//printf("插入耗时：%dms\n", GetTickCount() - l);

	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	if (bset.find(str_arr[i]) == bset.end()) {
	//		printf("找不到");
	//	}
	//}
	//printf("查找耗时：%dms\n", GetTickCount() - l);

	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	bset.erase(str_arr[i]);
	//}
	//printf("删除耗时：%dms\n", GetTickCount() - l);


	printf("\nstl rb:\n");
	l = GetTickCount();
	std::set<int> mapaaaa;
	for (int i = 0; i < count; i++) {
		mapaaaa.insert(arr2[i]->key);
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (mapaaaa.find(arr2[i]->key) == mapaaaa.end()) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		mapaaaa.erase(arr2[i]->key);
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);
}

void ReverseOrder(void* buf, size_t size) {
	uint8_t* buf_ = (uint8_t*)buf;
	for (size_t i = 0; i < size / 2; i++) {
		uint8_t temp = buf_[i];
		buf_[i] = buf_[size - 1 - i];
		buf_[size - 1 - i] = temp;
	}
}



LIBYUC_CONTAINER_VECTOR_DECLARATION(Ac, LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC, uint32_t, uint32_t, uint32_t)

LIBYUC_CONTAINER_VECTOR_DEFINE(Ac, LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC, uint32_t, uint32_t, uint32_t, uint32_t, LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT, LIBYUC_BASIC_COMPARER_DEFALUT, LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT)

int main() {
	//IntLruList lru_list;
	//IntLru_Entry lru_entry;
	//IntLruListInit(&lru_list, 5);
	//IntLruListPut(&lru_list, &lru_entry.lru_entry);

	//InitializeCriticalSection(&cs);
	//




	printf("seed：%d\n", seed);
	srand(seed);


	//for (int i = 0; i < count; i++) {
	//	std::swap(arr2[i], arr2[randInt() % count]);
	//}


	const char* data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()<>?:{}";




	

	//std::set<std::string> arrarr;
	//for (int64_t i = 0; i < count; i++) {
	//	int size = 8;// rand() % 4 + 1;
	//	std::string qqqq;
	//	qqqq.resize(size);
	//	for (int j = 0; j < size; j++) {
	//		qqqq[j] = rand() % 255 + 1 /* + ('a' - 1)*/; // data[rand() % 77];// +'a';
	//	}
	//	arrarr.insert(qqqq);
	//}

	arr2.reserve(count);
	for (int64_t i = 0; i < count; i++) {
		QVQ* qvq = ObjectCreate(QVQ);
		//
		//for (uint32_t i = 0; i < sizeof(qvq->key); i++) {
		//	((uint8_t*)&qvq->key)[i] = rand() % 26 + 'a';
		//	if(i == sizeof(qvq->key) - 1)((uint8_t*)&qvq->key)[i] = '\0';
		//}
		 qvq->key = i ;// i;
		//ReverseOrder(&qvq->key, 8);
		qvq->key = ((int64_t)rand() << 48) + ((int64_t)rand() << 32) + ((int64_t)rand() << 16) + rand();
		arr2.push_back(qvq);
	}

	for (int64_t i = 0; i < count; i++) {
		std::swap(arr2[i]->key, arr2[randInt() % count]->key);
	}






	size_t len = 0;

	
	// TestEpoch();
	//TestTsSortSinglyList();
	//TestTsSkipList();
	//
	TestSkipList();
	//TestBPlusTree();

	//TestAvl();
	//TestArt();
	//TestRb();
	//TestHashTable();

	
	//goto qqqqqq;
	// printf("\n基数树：\n");
	/*RadixTree tree;
	RadixTreeInit(&tree);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!RadixTreeInsert(&tree, arr2[i]->key, arr2[i]->key)) {
			printf("失败");
		}
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!RadixTreeFind(&tree, arr2[i]->key)) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!RadixTreeDelete(&tree, arr2[i]->key)) {
			printf("找不到");
		}
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (RadixTreeFind(&tree, arr2[i]->key)) {
			printf("找到了");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);*/
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	if (!HashTableDelete(&table, &arr2[i]->key)) {
	//		printf("删除找不到");
	//	}
	//}
	//printf("删除耗时：%dms\n", GetTickCount() - l);

	//printf("\nLRU：\n");
	//l = GetTickCount();
	//LruList lru;
	//LruListInitByField(&lru, 16, QVQ, lru_entry, key);
	//for (int i = 0; i < count; i++) {
	//	if (!LruListPut(&lru, &arr2[i]->lru_entry)) {
	//		//printf("666");
	//	}
	//}
	//printf("插入耗时：%dms\n", GetTickCount() - l);

	//HashTableIterator iter;
	//	int* obj = (int*)HashTableFirst(&lru.hash_table, &iter);
	//while (obj) {
	//	printf("%d:%d\n", iter.curIndex-1, *obj);
	//	obj = (int*)HashTableNext(&iter);
	//}
	//printf("\n\n\n");



	//




}