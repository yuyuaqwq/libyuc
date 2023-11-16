#include <chrono>
#include <vector>
#include <unordered_set>

#include <gtest/gtest.h>

#include <libyuc/tests/rand_data_set.h>

#include "C:/Users/yuyu/Desktop/src/wyhash.h"
#define LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME Int
#define LIBYUC_CONTAINER_LRU_LIST_Type_Key int64_t
#include <libyuc/container/lru_list.h>


#define LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME Int
#define LIBYUC_CONTAINER_LRU_LIST_Type_Key int64_t
#include <libyuc/container/lru_list.c>

//#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME Int
//#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int64_t
//#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))


static size_t test_count = 10000000;
static IntLruList test_hash_table;
static std::unordered_set<int64_t> test_unordered_set;
static std::vector<int64_t> test_res_data_set;

TEST(HashTableTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	IntLruListInit(&test_hash_table, 8);
}


TEST(HashTableTest, Insert) {
	for (int i = 0; i < test_count; i++) {
		//IntLruListPut(&test_hash_table, &test_res_data_set[i]);
	}
}

TEST(HashTableTest, Find) {
	for (int i = 0; i < test_count; i++) {
		if (IntLruListGet(&test_hash_table, &test_res_data_set[i], false) == nullptr) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(HashTableTest, Iterator) {
	//size_t i = 0;
	//IntHashTableIterator iter;
	//int64_t* iter_key = IntHashTableIteratorFirst(&test_hash_table, &iter);
	//while (iter_key) {
	//	++i;
	//	iter_key = IntHashTableIteratorNext(&test_hash_table, &iter);
	//}
	//ASSERT_EQ(i, test_count);
}

TEST(HashTableTest, Delete) {
	//for (int i = 0; i < test_count; i++) {
	//	if (IntHashTableDelete(&test_hash_table, &test_res_data_set[i]) != true) {
	//		ASSERT_TRUE(false);
	//	}
	//}
}

TEST(HashTableTest, End) {
	//IntHashTableRelease(&test_hash_table);
}

