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


#include "C:\Users\yuyu\Desktop\unordered_dense.h"


#include <regex>
#include "test.h"
int randInt() {
	return (rand() << 16) | rand();
}


//#include <libyuc/container/experimental/ar_tree.h>
//#include <libyuc/container/experimental/btree.h>

#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))
//#include <libyuc/container/experimental/hash_table_list.h>


DWORD l;
int count = 10000000;
std::vector<QVQ*> arr2;
//int seed = GetTickCount() + rand();
int seed = 377884212;

// std::vector<TsSkipListEntry*> ptr_arr;

int section = 1;

int thread_count = 10;

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
		//PrintHashTable(&table);
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
			//PrintHashTable(&table);
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




	ankerl::unordered_dense::set<int64_t> aaa;

	printf("\nankerl::unordered_dense::set:\n");
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		aaa.insert(arr2[i]->key);
		//mapaa.insert(std::make_pair(arr2[i]->key, 0));
	}
	printf("插入耗时：%dms\n", GetTickCount() - l);

	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		if (aaa.find(arr2[i]->key) == aaa.end()) {
			printf("找不到");
		}
	}
	printf("查找耗时：%dms\n", GetTickCount() - l);
	l = GetTickCount();
	for (int i = 0; i < count; i++) {
		aaa.erase(arr2[i]->key);
	}
	printf("删除耗时：%dms\n", GetTickCount() - l);





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

				//PrintRB(&tree, tree.root, 0);
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
				//PrintRB(&tree, tree.root, 0);
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

	printf("\nstd::set:\n");
	l = GetTickCount();
	std::set<int64_t> mapaaaa;
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

//void PrintAvl(IntAvlTree* head, IntAvlEntry* entry, IntAvlEntry* parent, int Level) {
//	if (!entry) return;
//	PrintAvl(head, INT_AVL_TREE_ACCESSOR_GetRight(tree, entry), entry, Level + 1);
//
//	const char* str = "Not";
//	if (parent) {
//		str = (INT_AVL_TREE_ACCESSOR_GetRight(tree, parent) == entry ? "Right" : "Left");
//	}
//	char* empty = (char*)malloc(Level * 8 + 1);
//	memset(empty, ' ', Level * 8);
//	empty[Level * 8] = 0;
//
//
//	printf("%skey:%lld\n%sLevel:%d\n%sBalance factor:%d\n%sParent.%s:%lld\n\n", empty, *INT_AVL_TREE_ACCESSOR_GetKey(head, entry), empty, Level, empty, INT_AVL_TREE_ACCESSOR_GetBalanceFactor(head, entry), empty, str, parent ? *INT_AVL_TREE_ACCESSOR_GetKey(head, parent) : NULL);
//
//	free(empty);
//
//	PrintAvl(head, INT_AVL_TREE_ACCESSOR_GetLeft(tree, entry), entry, Level + 1);
//}

void TestAvl() {


	printf("\n无父结点Avl树：\n");
	IntAvlTree tree;

	l = GetTickCount();
	//void** buf = (void**)malloc((count / section) * sizeof(void*));

	IntAvlBsTreeStackVector stack;
	

	for (int j = 0; j < section; j++) {

		IntAvlTreeInit(&tree);
		l = GetTickCount();
		for (int i = 0; i < count / section; i++) {
			IntAvlBsTreeStackVectorInit(&stack);
			//if (!IntRbTreeInsert(&tree, (IntRbEntry*)&arr2[i]->entry)) {
			//	//printf("失败%d", i);
			//}
			//buf[i] = malloc(16);

			IntAvlTreePut(&tree, &stack, (IntAvlEntry*)&arr2[i]->entry);

			if (count <= 20) {

				//PrintAvl(&tree, tree.root, NULL, 0);
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


//void TestArt() {
//	printf("\n自适应基数树：\n");
//	ArTree artree;
//	ArTreeInit(&artree);
//
//	//l = GetTickCount();
//	//int j = 0;
//	//for (int jjj = 0; jjj < 10; jjj++) {
//	//	for (int i = 0; i < count; i++) {
//	//		__m128i results = _mm_cmpeq_epi8(_mm_set1_epi8(0x22), _mm_loadu_si128((__m128i*) & arr2[i]->key));
//	//		ptrdiff_t mask = (1 << 16) - 1;
//	//		ptrdiff_t j = _mm_movemask_epi8(results) & mask;
//	//		if (!j) {
//	//			j++;
//	//		}
//	//		j = _tzcnt_u32(j);
//	//		if (j == 16) {
//	//			printf("不存在");
//	//		}
//	//	}
//	//}
//	//l = GetTickCount() - l;
//	//printf("simd查找耗时：%dms%d\n", l, j);
//
//	//l = GetTickCount();
//	//uint8_t key = 0x22;
//	//j = 0;
//	//for (int jjj = 0; jjj < 10; jjj++) {
//	//	for (int i = 0; i < count; i++) {
//	//		ptrdiff_t j = ArNodeKeyBinarySearch((uint8_t*)&arr2[i]->key, 0, 15, &key);
//	//		if (j == 16) {
//	//			printf("不存在");
//	//		}
//	//	}
//	//}
//	//l = GetTickCount() - l;
//	//printf("bs查找耗时：%dms%d\n", l, j);
//
//	//l = GetTickCount();
//	//j = 0;
//	//for (int jjj = 0; jjj < 10; jjj++) {
//	//	for (int i = 0; i < count; i++) {
//	//		//ptrdiff_t j = ArNodeKeyBinarySearch((uint8_t*)&arr2[i]->key, 0, 15, &key);
//	//		ptrdiff_t j = ArNodeKeyArrayFind((uint8_t*)&arr2[i]->key, 16, &key);
//	//		if (j == 15) {
//	//			printf("不存在");
//	//		}
//	//	}
//	//}
//	//l = GetTickCount() - l;
//	//printf("直接查找耗时：%dms%d\n", l, j);
//
//	//
//
//
//	//element_type ele;
//	//ele.buf = (uint8_t*)"abcdefgh";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//ele.buf = (uint8_t*)"abcdefgi";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//ele.buf = (uint8_t*)"abcdefghi";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//ele.buf = (uint8_t*)"abcdefghij";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//ele.buf = (uint8_t*)"abcdefghijk";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//PrintArt(&artree, artree.root, 0);
//
//	//ele.buf = (uint8_t*)"abcde";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//ele.buf = (uint8_t*)"abcdf";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//	//PrintArt(&artree, artree.root, 0);
//	//ele.buf = (uint8_t*)"abc";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreeDelete(&artree, &ele);
//	//PrintArt(&artree, artree.root, 0);
//
//	//ele.buf = (uint8_t*)"abd";
//	//ele.size = strlen((char*)ele.buf);
//	//ArTreePut(&artree, &ele);
//
//	//PrintArt(&artree, artree.root, 0);
//
//	//ArTreeDelete(&artree, &ele);
//
//	l = GetTickCount();
//	int32_t ii = 0;
//
//	for (auto& it : arr2) {
//		ii++;
//		/*if (ii == 986) {
//			printf("??");
//		}*/
//
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//		element_type* ele = ObjectCreate(element_type);
//		ele->buf = (uint8_t*)&it->key;
//		ele->size = sizeof(it->key);
//		ArTreePut(&artree, ele);
//#else
//
//		ArTreePut(&artree, (art_element_type*)&it->key);
//		//ArTreePut(&artree, (art_element_type*)it->key << 1);
//#endif
//		//if (ii == 27) {
//		//	printf("??");
//		//}
//
//
//
//		//printf("插入:%s\n", ele->buf);
//		//ele.buf = (uint8_t*)"aacdeedb";
//		//ele.size = strlen((char*)ele.buf);
//		////
//		//PrintArt(&artree, artree.root, 0);
//		//printf("\n\n\n\n");
//		////
//		//if (!ArTreeFind(&artree, &ele)) {
//		//	printf("找不到");
//		//}
//		//else {
//		//	printf("找到了");
//		//}
//
//	}
//
//	l = GetTickCount() - l;
//	printf("插入耗时：%dms\n", l);
//	//PrintArt(&artree, artree.root, 0);
//	l = GetTickCount();
//	ii = 0;
//	for (auto& it : arr2) {
//		ii++;
//		art_element_type ele;
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//		ele.buf = (uint8_t*)&it->key;
//		ele.size = sizeof(it->key);
//		if (!ArTreeFind(&artree, &ele)) {
//			printf("找不到");
//		}
//#else
//		ele = it->key;
//		if (!ArTreeFind(&artree, &ele)) {
//			printf("找不到");
//		}
//		/*if (!ArTreeFind(&artree, (key_type*)(it->key << 1))) {
//			printf("找不到");
//		}*/
//#endif
//
//	}
//	//PrintArt(&artree, artree.root, 0);
//	l = GetTickCount() - l;
//	printf("查找耗时：%dms\n", l);
//	l = GetTickCount();
//	ii = 0;
//	for (auto& it : arr2) {
//		ii++;
//		art_element_type ele;
//#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
//		ele.buf = (uint8_t*)&it->key;
//		ele.size = sizeof(it->key);
//		if (!ArTreeDelete(&artree, &ele)) {
//			printf("找不到");
//			//PrintArt(&artree, artree.root, 0);
//		}
//#else
//		ele = it->key;
//		if (!ArTreeDelete(&artree, &ele)) {
//			printf("找不到");
//			//PrintArt(&artree, artree.root, 0);
//		}
//		/*if (!ArTreeDelete(&artree, (key_type*)(it->key << 1))) {
//			printf("找不到");
//		}*/
//#endif
//		//if (ii == 0x4390) {
//		//	printf("??");
//		//}
//
//
//
//		//printf("删除:%s\n", ele.buf);
//		//uint8_t buf[] = {05, 05, 05, 05, 05, 05, 05, 01};
//		//ele.buf = buf;
//		//ele.size = 8;
//		//PrintArt(&artree, artree.root, 0);
//
//		//ele = 0x658d2b070e2e2726;
//		//if (!ArTreeFind(&artree, &ele)) {
//		//	printf("找不到");
//		//	//PrintArt(&artree, artree.root, 0);
//		//}
//		//else {
//		//	//printf("找到了");
//		//}
//
//		//PrintArt(&artree, artree.root, 0);
//	}
//	printf("删除耗时：%dms\n", GetTickCount() - l);
//
//
//
//	//goto qqqqqq;
//
//
//
//	int qqc = 1;
//
//	l = GetTickCount();
//
//	//printf("插入耗时：%dms    %d\n", GetTickCount() - l, 0, 0/*BsTreeGetEntryCount(&gRb.bst)*/);
////	for (int j = 0; j < qqc; j++) {
////		ArTreeInit(&artree);
////		for (int i = 0; i < count / qqc; i++) {
////			element_type ele;
////#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
////			ele.buf = (uint8_t*)&arr2[i]->key;
////			ele.size = sizeof(arr2[i]->key);
////#else
////			ele = arr2[i]->key;
////#endif
////			//if (ii == 27) {
////			//	printf("??");
////			//}
////
////			ArTreePut(&artree, &ele);
////		}
////
////		for (int i = 0; i < count / qqc; i++) {
////			element_type ele;
////#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
////			ele.buf = (uint8_t*)&arr2[i]->key;
////			ele.size = sizeof(arr2[i]->key);
////#else
////			ele = arr2[i]->key;
////#endif
////			//if (ii == 27) {
////			//	printf("??");
////			//}
////
////			if (!ArTreeFind(&artree, &ele)) {
////				printf("找不到");
////			}
////		}
////		//printf("查找耗时：%dms\n", GetTickCount() - l);
////
////
////		//PrintRB(&gRb, gRb.root, 0);
////		 //for (int i = 10000000; i >= 1; i--) {
////		 //	int key = i;
////		 //	RBDeleteEntryByKey(&gRb, &key);
////		 //	// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
////		 //}
////
////		for (int i = 0; i < count / qqc; i++) {
////			element_type ele;
////#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
////			ele.buf = (uint8_t*)&arr2[i]->key;
////			ele.size = sizeof(arr2[i]->key);
////#else
////			ele = arr2[i]->key;
////#endif
////			ArTreeDelete(&artree, &ele);
////
////			// printf("\n\n\n\n"); PrintRB(&gRb, gRb.root, 0);
////		}
////		//printf("删除耗时：%dms    \n", GetTickCount() - l);
////
////	}
//	printf("总耗时：%dms    \n", GetTickCount() - l);
//}

//void TestBPlusTree() {
//	printf("\nB树：\n");
//	BTree btree;
//	BTreeInit(&btree);
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		BTreePut(&btree, &arr2[i]->key);
//		if (count < 31) {
//			//PrintBTree(&btree, btree.root, 0, 0);
//			printf("\n\n\n\n");
//		}
//	}
//	printf("插入耗时：%dms\n", GetTickCount() - l);
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		// int* qvq = CreateObject(int);
//		if (!BTreeFind(&btree, &arr2[i]->key)) {
//			printf("找不到, %d", arr2[i]->key);
//		}
//	}
//	printf("查找耗时：%dms\n", GetTickCount() - l);
//
//
//	l = GetTickCount();
//	for (int i = 0; i < count; i++) {
//		// int* qvq = CreateObject(int);
//		// printf("%d", i);
//		if (!BTreeDelete(&btree, &arr2[i]->key)) {
//			printf("找不到, %d", &arr2[i]->key);
//		}
//		if (count <= 30) {
//			printf("删除%d\n", arr2[i]->key);
//			//PrintBTree(&btree, btree.root, 0, 0);
//			printf("\n\n\n\n");
//		}
//
//	}
//	printf("删除耗时：%dms\n", GetTickCount() - l);
//
//	BTreeIterator iter;
//	element_type* ele = BTreeIteratorFirst(&btree, &iter);
//	while (ele) {
//		printf("%d  ", *ele);
//		ele = BTreeIteratorNext(&btree, &iter);
//	}
//
//	//printf("\nB+树：\n");
//	//IntBPlusTree bpTree;
//	//IntBPlusTreeInit(&bpTree);
//	//l = GetTickCount();
//	//IntBPlusLeafElement element;
//	//for (int i = 0; i < count; i++) {
//	//	// int* qvq = CreateObject(int);
//	//	element.key = arr2[i]->key;
//	//	IntBPlusTreeInsert(&bpTree, &element);
//	//	if (count < 31) {
//	//		PrintBPlus(&bpTree, bpTree.root_id, 0, 0);
//	//		printf("\n\n\n\n");
//	//	}
//	//}
//	//// PrintBPlus(&bpTree, bpTree.root, 0, 0);
//	//printf("插入耗时：%dms\n", GetTickCount() - l);
//	//l = GetTickCount();
//	//for (int i = 0; i < count; i++) {
//	//	// int* qvq = CreateObject(int);
//	//	if (!IntBPlusTreeFind(&bpTree, &arr2[i]->key)) {
//	//		printf("找不到, %d", arr2[i]->key);
//	//	}
//	//}
//	//printf("查找耗时：%dms\n", GetTickCount() - l);
//
//	//l = GetTickCount();
//	//for (int i = 0; i < count; i++) {
//	//	// int* qvq = CreateObject(int);
//	//	// printf("%d", i);
//	//	if (!IntBPlusTreeDelete(&bpTree, &arr2[i]->key)) {
//	//		printf("找不到, %d", &arr2[i]->key);
//	//	}
//	//	if (count <= 30) {
//	//		printf("删除%d\n", arr2[i]->key);
//	//		PrintBPlus(&bpTree, bpTree.root_id, 0, 0);
//	//		printf("\n\n\n\n");
//	//	}
//
//	//}
//	//printf("删除耗时：%dms\n", GetTickCount() - l);
//
//
//	//std::vector<std::string> str_arr(count);
//	//for (int i = 0; i < count; i++) {
//	//	str_arr[i] =
//	//		std::to_string(arr2[i]->key);
//	//}
//
//
//	//printf("\nbtree:\n");
//	//l = GetTickCount();
//	//btree::set<std::string> bset;
//	//for (int i = 0; i < count; i++) {
//	//	bset.insert(str_arr[i]);
//	//}
//	//printf("插入耗时：%dms\n", GetTickCount() - l);
//
//	//l = GetTickCount();
//	//for (int i = 0; i < count; i++) {
//	//	if (bset.find(str_arr[i]) == bset.end()) {
//	//		printf("找不到");
//	//	}
//	//}
//	//printf("查找耗时：%dms\n", GetTickCount() - l);
//
//	//l = GetTickCount();
//	//for (int i = 0; i < count; i++) {
//	//	bset.erase(str_arr[i]);
//	//}
//	//printf("删除耗时：%dms\n", GetTickCount() - l);
//
//}



//void TestTsSinglyList() {
//	TsSinglyListHead head;
//	TsSinglyListInit(head);
//	TsSinglyListPutFirst(&head, );
//}

#define ssize_t size_t
#undef max
#undef min
//#include <cpp-btree/set.h>


extern "C" {
}
struct bplus_tree_config {
	int order;
	int entries;
};


void ReverseOrder(void* buf, size_t size) {
	uint8_t* buf_ = (uint8_t*)buf;
	for (size_t i = 0; i < size / 2; i++) {
		uint8_t temp = buf_[i];
		buf_[i] = buf_[size - 1 - i];
		buf_[size - 1 - i] = temp;
	}
}


int main() {


	printf("seed：%d\n", seed);
	srand(seed);



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
		//qvq->key = ((int64_t)rand() << 48) + ((int64_t)rand() << 32) + ((int64_t)rand() << 16) + rand();
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
	//TestSkipList();
	//TestBPlusTree();

	TestAvl();
	//TestArt();
	TestRb();
	TestHashTable();

	
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