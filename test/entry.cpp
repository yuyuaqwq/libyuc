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


#include "C:\Users\gt1\Desktop\unordered_dense.h"


#include <regex>
#include "test.h"
int randInt() {
	return (rand() << 16) | rand();
}

struct QVQ {
	//IntRbEntry entry;
	int64_t key;
	//AvlEntry entry;
	//int height;
	//LruEntry lru_entry;
};

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

	//TestAvl();
	//TestArt();
	//TestRb();
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