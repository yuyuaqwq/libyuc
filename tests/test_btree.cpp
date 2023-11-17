#include <gtest/gtest.h>

#include <libyuc/container/experimental/btree.h>

#include "rand_data_set.h"

static size_t test_count = 10000000;
static BTree test_btree;
static std::set<int64_t> test_set;
static std::vector<int64_t> test_res_data_set;


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

TEST(BTreeTestEnvTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	BTreeInit(&test_btree);
}


TEST(BTreeTestEnvTest, Insert) {
	for (int i = 0; i < test_count; i++) {
		BTreePut(&test_btree, &test_res_data_set[i]);
	}
}

TEST(BTreeTestEnvTest, Find) {
	for (int i = 0; i < test_count; i++) {
		if (BTreeFind(&test_btree, &test_res_data_set[i]) == false) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(BTreeTestEnvTest, Delete) {
	for (int i = 0; i < test_count; i++) {
		if (!BTreeDelete(&test_btree, &test_res_data_set[i])) {
			ASSERT_TRUE(false);
		}
	}
}


TEST(BTreeTestEnvTest, End) {
	test_res_data_set.clear();
}
