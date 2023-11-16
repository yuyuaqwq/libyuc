#include <chrono>
#include <vector>
#include <unordered_set>

#include <gtest/gtest.h>

#include <libyuc/tests/rand_data_set.h>

#include "C:/Users/yuyu/Desktop/src/wyhash.h"
#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME Int
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int64_t
#include <libyuc/container/hash_table.h>

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME Int
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int64_t
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))
#include <libyuc/container/hash_table.c>

static size_t test_count = 10000000;
static IntHashTable test_hash_table;
static std::unordered_set<int64_t> test_unordered_set;
static std::vector<int64_t> test_res_data_set;

TEST(HashTableTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	IntHashTableInit(&test_hash_table, 8, 0);
}


TEST(HashTableTest, Insert) {
	for (int i = 0; i < test_count; i++) {
		IntHashTablePut(&test_hash_table, &test_res_data_set[i]);
	}
}

TEST(HashTableTest, Find) {
	for (int i = 0; i < test_count; i++) {
		if (IntHashTableFind(&test_hash_table, &test_res_data_set[i]) == nullptr) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(HashTableTest, Iterator) {
	size_t i = 0;
	IntHashTableIterator iter;
	int64_t* iter_key = IntHashTableIteratorFirst(&test_hash_table, &iter);
	while (iter_key) {
		++i;
		iter_key = IntHashTableIteratorNext(&test_hash_table, &iter);
	}
	ASSERT_EQ(i, test_count);
}

TEST(HashTableTest, Delete) {
	for (int i = 0; i < test_count; i++) {
		if (IntHashTableDelete(&test_hash_table, &test_res_data_set[i]) != true) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(HashTableTest, End) {
	IntHashTableRelease(&test_hash_table);
}


TEST(STL_unordered_set, Insert) {
	for (int i = 0; i < test_count; i++) {
		test_unordered_set.insert(test_res_data_set[i]);
	}
}

TEST(STL_unordered_set, Find) {
	for (int i = 0; i < test_count; i++) {
		if (test_unordered_set.find(test_res_data_set[i]) == test_unordered_set.end()) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(STL_unordered_set, Iterator) {
	size_t i = 0;
	for (auto& key : test_unordered_set) {
		++i;
	}
	ASSERT_EQ(i, test_count);

}

TEST(STL_unordered_set, Delete) {
	for (int i = 0; i < test_count; i++) {
		if (test_unordered_set.erase(test_res_data_set[i]) == 0) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(STL_unordered_set, End) {
	test_res_data_set.clear();
	test_unordered_set.clear();
}