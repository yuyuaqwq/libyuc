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

#include <libyuc/container/experimental/ar_tree.h>
#include <libyuc/container/experimental/skip_list.h>
#include <libyuc/container/thread_safe/experimental/epoch.h>
#include <libyuc/container/thread_safe/experimental/ts_skip_list.h>
#include <libyuc/container/thread_safe/experimental/ts_sort_singly_list.h>

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

//extern "C" IntBPlusElement * TEST_ELEMENT_REFERENCER_Reference(IntBPlusEntry * entry, int16_t element_id);
//void PrintBPlus(IntBPlusTree* tree, IntBPlusEntry* entry, int Level, int pos) {
//	if (!entry) return;
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//	//  assert(entry->element_count >= 1);
//	if (entry->type == 1) {
//		int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree);
//		//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, true);
//		for (int i = entry->element_count - 1; i >= 0; i--) {
//			printf("%sleaf:::key:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, TEST_ELEMENT_REFERENCER_Reference(entry, id)->leaf.key, empty, Level, empty/*, pos ? ((BPlusEntry*)PageGet(tree, entry->, 1))->indexElement[pos].key : 0*/);
//			id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, id);
//		}
//		free(empty);
//		return;
//	}
//
//	//PrintRB(&entry->rb_tree, entry->rb_tree.root, 0, false);
//	int16_t id = IntBPlusEntryRbTreeIteratorLast(&entry->rb_tree);
//	for (int i = entry->element_count; i >= 0; i--) {
//		if (i == entry->element_count) {
//			PrintBPlus(tree, entry->index.tail_child_id, Level + 1, i - 1);
//			continue;
//		}
//		printf("%sindex:::key:%d\n%sLevel:%d\n%sParent:%d\n\n", empty, TEST_ELEMENT_REFERENCER_Reference(entry, id)->index.key, empty, Level, empty/*, entry->parentId != kPageInvalidId ? ((BPlusEntry*)PageGet(tree, entry->parentId))->indexElement[pos].key: 0*/);
//		PrintBPlus(tree, TEST_ELEMENT_REFERENCER_Reference(entry, id)->index.child_id, Level + 1, i);
//		id = IntBPlusEntryRbTreeIteratorPrev(&entry->rb_tree, id);
//	}
//	free(empty);
//}
//

void PrintSkipList(TsSkipList* list) {
	auto cur = list->head;
	while (cur) {
		auto entry = ObjectGetFromField(cur, TsSkipListEntry, upper);
		if (cur != list->head) {
			for (int j = 0; j < entry->level_count; j++) {
				printf("key:%d\t\t", entry->key);
			}
			printf("\n\n");
		}
		if (cur[0].next) {
			cur = (TsSkipListLevel*)cur[0].next->upper;
		}
		else {
			cur = NULL;
		}
	}
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

#include <libyuc/container/hash_list.h>

#include <thread>

//
//void PrintArt(ArTree* tree, ArNode* node, int Level) {
//	if (!node)return;
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//	if (node->head.node_type == kArLeaf) {
//		printf("%sleaf:%s\n", empty, node->leaf.element.buf);
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
//			printf("%snode4:\n%s    prefix:%d[%d] \"%s%s\"\n", empty, empty, node->head.prefix_len, node->head.prefix_len, prefix, abc);
//			if (node->head.eof_child) {
//				printf("%s    eof:\n", empty);
//				PrintArt(tree, (ArNode*)node->head.eof_child, Level + 1);
//			}
//			for (int i = 0; i < node->head.child_count; i++) {
//				printf("%s    [%c]:\n", empty, node->node4.keys[i]);
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
//			printf("%snode16:\n%s    prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//			if (node->head.eof_child) {
//				printf("%s    eof:\n", empty);
//				PrintArt(tree, (ArNode*)node->head.eof_child, Level + 1);
//			}
//			for (int i = 0; i < node->head.child_count; i++) {
//				printf("%s    [%c]:\n", empty, node->node16.keys[i]);
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
//			printf("%snode48:\n%s    prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//			if (node->head.eof_child) {
//				printf("%s    eof:\n", empty);
//				PrintArt(tree, (ArNode*)node->head.eof_child, Level + 1);
//			}
//			for (int i = 0; i < 255; i++) {
//				if (node->node48.keys[i] != 0xff) {
//					printf("%s    [%c]:\n", empty, i);
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
//			printf("%snode256:\n%s    prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len, prefix, abc);
//			if (node->head.eof_child) {
//				printf("%s    eof:\n", empty);
//				PrintArt(tree, (ArNode*)node->head.eof_child, Level + 1);
//			}
//			for (int i = 0; i < 255; i++) {
//				if (node->node256.child_arr[i] != NULL) {
//					printf("%s    [%c]:\n", empty, i);
//					PrintArt(tree, node->node256.child_arr[i], Level + 1);
//				}
//			}
//		}
//	}
//	free(empty);
//}
//



struct QVQ {
	IntRbEntry entry;
	int64_t key;
	//AvlEntry entry;
	//int height;
	//LruEntry lru_entry;
};

DWORD l;
int count = 10000000;
std::vector<QVQ*> arr2;
// int seed = GetTickCount() + rand();
int seed = 377884212;


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
		element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		ele.buf = (uint8_t*)&it->key;
		ele.size = sizeof(it->key);
#else
		ele = it->key;
#endif
		//if (ii == 27) {
		//	printf("??");
		//}

		ArTreePut(&artree, &ele);

		//printf("插入:%s\n", ele.buf);
		//ele.buf = (uint8_t*)"aacdeedb";
		//ele.size = strlen((char*)ele.buf);
		////
		//PrintArt(&artree, artree.root, 0);
		////
		//if (!ArTreeFind(&artree, &ele)) {
		//	printf("找不到");
		//}
		//else {
		//	printf("找到了");
		//}

	}
	printf("插入耗时：%dms\n", GetTickCount() - l);
	//PrintArt(&artree, artree.root, 0);
	l = GetTickCount();
	ii = 0;
	for (auto& it : arr2) {
		ii++;
		element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		ele.buf = (uint8_t*)&it->key;
		ele.size = sizeof(it->key);
#else
		ele = it->key;
#endif
		if (!ArTreeFind(&artree, &ele)) {
			printf("找不到");
		}
	}
	//PrintArt(&artree, artree.root, 0);
	printf("查找耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	ii = 0;
	for (auto& it : arr2) {
		ii++;
		element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		ele.buf = (uint8_t*)&it->key;
		ele.size = sizeof(it->key);
#else
		ele = it->key;
#endif
		//if (ii == 0x4390) {
		//	printf("??");
		//}

		if (!ArTreeDelete(&artree, &ele)) {
			printf("找不到");
			//PrintArt(&artree, artree.root, 0);
		}

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

	//printf("插入耗时：%dms  %d\n", GetTickCount() - l, 0, 0/*BsTreeGetEntryCount(&gRb.bst)*/);
	for (int j = 0; j < qqc; j++) {
		ArTreeInit(&artree);
		for (int i = 0; i < count / qqc; i++) {
			element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			ele.buf = (uint8_t*)&arr2[i]->key;
			ele.size = sizeof(arr2[i]->key);
#else
			ele = arr2[i]->key;
#endif
			//if (ii == 27) {
			//	printf("??");
			//}

			ArTreePut(&artree, &ele);
		}

		for (int i = 0; i < count / qqc; i++) {
			element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			ele.buf = (uint8_t*)&arr2[i]->key;
			ele.size = sizeof(arr2[i]->key);
#else
			ele = arr2[i]->key;
#endif
			//if (ii == 27) {
			//	printf("??");
			//}

			if (!ArTreeFind(&artree, &ele)) {
				printf("找不到");
			}
		}
		//printf("查找耗时：%dms\n", GetTickCount() - l);


		//PrintRB(&gRb, gRb.root, 0);
		 //for (int i = 10000000; i >= 1; i--) {
		 //	int key = i;
		 //	RBDeleteEntryByKey(&gRb, &key);
		 //	// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
		 //}

		for (int i = 0; i < count / qqc; i++) {
			element_type ele;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
			ele.buf = (uint8_t*)&arr2[i]->key;
			ele.size = sizeof(arr2[i]->key);
#else
			ele = arr2[i]->key;
#endif
			ArTreeDelete(&artree, &ele);

			// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
		}
		//printf("删除耗时：%dms  \n", GetTickCount() - l);

	}
	printf("总耗时：%dms  \n", GetTickCount() - l);
}
 
void TestAvl() {
	printf("\n无父结点Avl树：\n");
	IntAvlTree tree;

	l = GetTickCount();
	//void** buf = (void**)malloc((count / section) * sizeof(void*));

	IntAvlBsStackVector stack;
	IntAvlEntry* stack_arr[48];
	IntAvlBsStackVectorInit(&stack, 64, stack_arr);

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
		printf("插入耗时：%dms  %d\n", GetTickCount() - l, 0, 0/**/);

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
		printf("删除耗时：%dms  \n", GetTickCount() - l);

	}



	printf("总耗时：%dms  \n", GetTickCount() - l);

}


void TestRb() {
#ifndef NP
	printf("\n红黑树：\n");
#else
	printf("\n无父结点红黑树：\n");
#endif
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

				//PrintRB(&tree, tree.root, 0);
				//printf("\n\n\n\n");
			}
		}



		//PrintRB(&tree, tree.root, 0);
		//printf("\n\n\n\n");
		printf("插入耗时：%dms  %d\n", GetTickCount() - l, 0, 0/**/);


		l = GetTickCount();
		printf("统计count:%d    %dms\n", IntRbTreeGetCount(&tree), GetTickCount() - l);

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
		printf("删除耗时：%dms  \n", GetTickCount() - l);

	}
	printf("总耗时：%dms  \n", GetTickCount() - l);

}

void TestSkipList() {
	printf("\n跳表：\n");
	SkipList list;
	SkipListInit(&list);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		SkipListInsert(&list, (arr2[i]->key));
		if (count < 20) {
			//PrintSkipList(&list);
			//printf("\n\n\n\n");
		}
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);
	
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!SkipListFind(&list, (arr2[i]->key))) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!SkipListDelete(&list, (arr2[i]->key))) {
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
	TsSortSinglyListHead head;
	head.first = NULL;

	std::vector<std::thread> t;
	t.reserve(thread_count);
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSortSinglyListInsertThread, &head, i));
		t.push_back(std::thread(TestTsSortSinglyListDeleteThread, &head, i));
	}

	for (auto& thread : t) {
		thread.join();
	}

	printf("%d个线程，插入总耗时：%dms  %d\n", thread_count, GetTickCount() - l, 0, 0);


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
	printf("%d个线程，删除总耗时：%dms  %d\n", thread_count, GetTickCount() - l, 0, 0);

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
		TsSkipListInsert(list, ((TsSortSinglyListEntry*)&arr2[j * (count / thread_count) + i]->entry.right)->key);
		// PrintSkipList(list);
		// printf("\n\n\n\n");
	}
}

void TestTsSkipList() {
	TsSkipList list;
	TsSkipListInit(&list);

	std::vector<std::thread> t;
	t.reserve(thread_count);
	l = GetTickCount();

	for (int i = 0; i < thread_count; i++) {
		t.push_back(std::thread(TestTsSkipListInsertThread, &list, i));
	}

	for (auto& thread : t) {
		thread.join();
	}

	printf("%d个线程，插入总耗时：%dms  %d\n", thread_count, GetTickCount() - l, 0, 0);


	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (!TsSkipListFind(&list, (arr2[i]->key))) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);

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


//void TestTsSinglyList() {
//	TsSinglyListHead head;
//	TsSinglyListInit(head);
//	TsSinglyListPutFirst(&head, );
//}

int main() {
	//IntLruList lru_list;
	//IntLru_Entry lru_entry;
	//IntLruListInit(&lru_list, 5);
	//IntLruListPut(&lru_list, &lru_entry.lru_entry);

	//InitializeCriticalSection(&cs);
	//

	//SpinLockInit(&lock);
	//SleepLockInit(&slock);
	//
	//HANDLE hand[10];		//
	//for (int i = 0; i < sizeof(hand)/sizeof(HANDLE); i++) {
	//	hand[i] = CreateThread(NULL, NULL, testfunc, (LPVOID)i, NULL, NULL);
	//};
	//
	//l = GetTickCount();
	//
	//DWORD pp = WaitForMultipleObjects(sizeof(hand) / sizeof(HANDLE), hand, TRUE, -1);
	//printf("%d   i:%d, %dms\n\n", GetLastError(), k, GetTickCount() - l);
	//while (true) {
	//	Sleep(1000);
	//}

	//std::vector<uint8_t> bitarr(10);
	//Bitmap bitmap;
	//bitmap.arr.objArr = bitarr.data();
	//bitmap.arr.count = 10;
	//bitmap.arr.objSize = 1;
	//BitmapAlloc(&bitmap, 5);
	//BitmapAlloc(&bitmap, 3);
	//BitmapFree(&bitmap, 2, 3);
	//BitmapAlloc(&bitmap, 4);
	//BitmapAlloc(&bitmap, 3);




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
		qvq->key = i;// i;
		//qvq->key = ((int64_t)rand() << 48) + ((int64_t)rand() << 32) + ((int64_t)rand() << 16) + rand();
		arr2.push_back(qvq);
	}

	for (int64_t i = 0; i < count; i++) {
		std::swap(arr2[i]->key, arr2[randInt() % count]->key);
	}


	size_t len = 0;

	// TestEpoch();
	// TestTsSkipList();
	//
	//TestSkipList();

	//TestAvl();
	//TestArt();
	TestRb();
	//TestTsSortSinglyList();


	
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



	printf("\n哈希表：\n");

	l = GetTickCount();
	IntHashTable table;
	int64_t empty = -1;
	IntHashTableInit(&table, 8, 0, &empty);
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
			//printf("删除找不到");
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


	struct hashfunc {
		size_t operator()(int64_t i) const {
			return HashCode_murmur3_fmix64inline(i);
		}
	};

	printf("\nSTL ht:\n");
	l = GetTickCount();
	std::unordered_set<int64_t, hashfunc> mapaa;
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
	//int key;
	//printf("插入耗时：%dms\n", GetTickCount() - l);
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	// int* qvq = CreateObject(int);
	//	key = arr2[i]->key;
	//	if (!IntBPlusTreeFind(&bpTree, &key)) {
	//		printf("找不到, %d", arr2[i]->key);
	//	}
	//}
	//printf("查找耗时：%dms\n", GetTickCount() - l);
	//l = GetTickCount();
	//for (int i = 0; i < count; i++) {
	//	// int* qvq = CreateObject(int);
	//	// printf("%d", i);
	//	key = arr2[i]->key;
	//	if (!IntBPlusTreeDelete(&bpTree, &key)) {
	//		printf("找不到, %d", &arr2[i]->key);
	//	}
	//	if (count < 31) {
	//		printf("删除%d\n", arr2[i]->key);
	//		PrintBPlus(&bpTree, bpTree.root_id, 0, 0);
	//		printf("\n\n\n\n");
	//	}

	//}
	//printf("删除耗时：%dms\n", GetTickCount() - l);

	printf("\nSTL rb:\n");
	l = GetTickCount();
	std::map<int, int> mapaaaa;
	for (int i = 0; i < count; i++) {
		mapaaaa.insert(std::make_pair(arr2[i]->key, 0));
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