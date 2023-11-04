#include <chrono>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <libyuc/tests/rand_data_set.h>


#define LIBYUC_CONTAINER_RB_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key int64_t
#include <libyuc/container/rb_tree.h>

struct IntRbEntry_User {
	IntRbEntry rb_entry;
	int64_t key;
};

#define LIBYUC_CONTAINER_RB_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (&((IntRbEntry_User*)OBJ)->key)
#include <libyuc/container/rb_tree.c>

static int seed = 312962319;
static size_t test_count = 10000000;
static IntRbTree test_rb_tree;
static std::set<int64_t> test_set;
static std::vector<int64_t> test_res_data_set;

static void PrintRb(IntRbTree* tree, IntRbEntry* parent, IntRbEntry* entry_id, int Level) {
	if (entry_id == NULL) return;
	IntRbEntry* entry = entry_id;
	PrintRb(tree, entry, entry->right, Level + 1);

	//print
	const char* str = "Not";
	if (parent != NULL) {
		str = parent->right == entry_id ? "Right" : "Left";
	}

	int aaa = ((IntRbColor)((uintptr_t)(entry)->left & 0x1));
	const char* color = aaa == 1 ? "Red" : "Black";

	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;

	int64_t parentKey = 0;
	if (parent != NULL) {
		parentKey = *(&((IntRbEntry_User*)entry)->key);
	}

	printf("%skey:%d\n%sLevel:%d\n%sParent.%s:%x\n%scolor:%s\n\n", empty, *(&((IntRbEntry_User*)entry)->key), empty, Level, empty, str, parentKey, empty, color);

	free(empty);

	PrintRb(tree, entry, ((IntRbEntry*)((uintptr_t)(entry)->left & (~(uintptr_t)0x1))), Level + 1);
}

TEST(RbTreeTestEnv, Start) {
	srand(seed);

	test_res_data_set = GenerateI64Vector(test_count);
	IntRbTreeInit(&test_rb_tree);
}


TEST(RbTreeTest, Insert) {
	for (int i = 0; i < test_count; i++) {
		IntRbEntry_User* data = new IntRbEntry_User;
		data->key = test_res_data_set[i];
		IntRbTreePut(&test_rb_tree, &data->rb_entry);
		//PrintRb(&test_rb_tree, NULL, test_rb_tree.root, 0);
		//printf("\n\n\n");
	}
}

TEST(RbTreeTest, Verify_Insert) {
	ASSERT_EQ(IntRbTreeVerify(&test_rb_tree), true);
}

TEST(RbTreeTest, Find) {
	auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < test_count; i++) {
		if (IntRbTreeFind(&test_rb_tree, &test_res_data_set[i]) == nullptr) {
			ASSERT_TRUE(false);
		}
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	std::cout << "time: " << duration.count() << "ns" << std::endl;

}

TEST(RbTreeTest, Iterator) {
	size_t i = 0;
	IntRbTreeIterator iter;
	IntRbEntry* iter_key = IntRbTreeIteratorFirst(&test_rb_tree, &iter);
	while (iter_key) {
		++i;
		iter_key = IntRbTreeIteratorNext(&test_rb_tree, &iter);
		//printf("%d\n", ((IntRbEntry_User*)(iter_key))->key);
	}
	ASSERT_EQ(i, test_count);
}

TEST(RbTreeTest, Delete) {
	for (int i = 0; i < test_count; i++) {
		IntRbTreeIterator iter;
		ptrdiff_t cmp_diff;
		auto rb_entry = IntRbTreeIteratorLocate(&test_rb_tree, &iter, &test_res_data_set[i], &cmp_diff);
		if (rb_entry == nullptr || cmp_diff != 0) {
			ASSERT_TRUE(false);
		}
		if (IntRbTreeDeleteByIterator(&test_rb_tree, &iter) == false) {
			ASSERT_TRUE(false);
		}
		delete (IntRbEntry_User*)rb_entry;
	}

	ASSERT_EQ(test_rb_tree.root, nullptr);
}


TEST(RbTreeTest, End) {
	
}


TEST(STL_set, Insert) {
	for (int i = 0; i < test_count; i++) {
		test_set.insert(test_res_data_set[i]);
	}
}

TEST(STL_set, Find) {
	for (int i = 0; i < test_count; i++) {
		if (test_set.find(test_res_data_set[i]) == test_set.end()) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(STL_set, Iterator) {
	size_t i = 0;
	for (auto& key : test_set) {
		++i;
	}
	ASSERT_EQ(i, test_count);

}

TEST(STL_set, Delete) {
	for (int i = 0; i < test_count; i++) {
		if (test_set.erase(test_res_data_set[i]) == 0) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(STL_set, End) {
	test_res_data_set.clear();
	test_set.clear();
}