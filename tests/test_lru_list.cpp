#include <chrono>
#include <vector>
#include <unordered_set>

#include <gtest/gtest.h>



#include "C:/Users/yuyu/Desktop/src/wyhash.h"
#define LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME Int
#define LIBYUC_CONTAINER_LRU_LIST_Type_Key int64_t
#include <libyuc/container/lru_list.h>

struct IntLruEntry_User {
	IntLruListEntry lru_entry;
	int64_t key;
};

#define LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME Int
#define LIBYUC_CONTAINER_LRU_LIST_Type_Key int64_t
#define LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(LRU_LIST, ENTRY) (&(ObjectGetFromField(ENTRY, IntLruEntry_User, lru_entry))->key)
#include <libyuc/container/lru_list.c>

//#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME Int
//#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int64_t
//#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))


#include "rand_data_set.h"

static size_t test_count = 1000000;
static IntLruList test_hash_table;
static std::unordered_set<int64_t> test_unordered_set;
static std::vector<int64_t> test_res_data_set;

TEST(LruListTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count, false);
	IntLruListInit(&test_hash_table, 8);
}


TEST(LruListTest, Insert) {
	for (int i = 0; i < test_count; i++) {
		IntLruEntry_User* data = new IntLruEntry_User;
		data->key = test_res_data_set[i];
		IntLruListPut(&test_hash_table, &data->lru_entry);
	}
}

TEST(LruListTest, Find) {
	for (int i = 0; i < test_count; i++) {
		if (IntLruListGetByKey(&test_hash_table, &test_res_data_set[i], false) == nullptr) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(LruListTest, Iterator) {
	//size_t i = 0;
	//IntHashTableIterator iter;
	//int64_t* iter_key = IntHashTableIteratorFirst(&test_hash_table, &iter);
	//while (iter_key) {
	//	++i;
	//	iter_key = IntHashTableIteratorNext(&test_hash_table, &iter);
	//}
	//ASSERT_EQ(i, test_count);
}

TEST(LruListTest, Delete) {
	//for (int i = 0; i < test_count; i++) {
	//	if (IntHashTableDelete(&test_hash_table, &test_res_data_set[i]) != true) {
	//		ASSERT_TRUE(false);
	//	}
	//}
}

TEST(LruListTest, End) {
	//IntHashTableRelease(&test_hash_table);
}

