
#include <chrono>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <libyuc/test/rand_data_set.h>

#define LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Key int64_t
#include <libyuc/container/experimental/avl_tree.h>

struct IntAvlEntry_User {
	IntAvlEntry avl_entry;
	int64_t key;
};
#define LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (&((IntAvlEntry_User*)OBJ)->key)
#include <libyuc/container/experimental/avl_tree.c>



static size_t test_count = 10000000;
static IntAvlTree test_avl_tree;
static std::set<int64_t> test_set;
static std::vector<int64_t> test_res_data_set;

void PrintAvl(IntAvlTree* tree, IntAvlEntry* parent, IntAvlEntry* entry_id, int Level) {
	if (entry_id == NULL) return;
	IntAvlEntry* entry = entry_id;
	PrintAvl(tree, entry, entry->right, Level + 1);


	int bf = (((uintptr_t)(entry)->left & 0x3) == 3 ? -1 : (uintptr_t)(entry)->left & 0x3);

	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;

	int64_t parentKey = 0;
	if (parent != NULL) {
		parentKey = *(&((IntAvlEntry_User*)entry)->key);
	}

	printf("%skey:%d\n%sLevel:%d\n%sParent:%x\n%sbf:%d\n\n", empty, *(&((IntAvlEntry_User*)entry)->key), empty, Level, empty, parentKey, empty, bf);

	free(empty);

	PrintAvl(tree, entry, ((IntAvlEntry*)((uintptr_t)(entry)->left & (~(uintptr_t)0x1))), Level + 1);
}

TEST(AvlTreeTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	IntAvlTreeInit(&test_avl_tree);
}


TEST(AvlTreeTest, Insert) {
	IntAvlBsTreeStackVector stack;
	for (int i = 0; i < test_count; i++) {
		IntAvlEntry_User* data = new IntAvlEntry_User;
		data->key = test_res_data_set[i];
		IntAvlTreePut(&test_avl_tree, &stack, &data->avl_entry);
		//PrintAvl(&test_avl_tree, NULL, test_avl_tree.root, 0);
		//printf("\n\n\n");
	}
}

TEST(AvlTreeTest, Verify_Insert) {
	ASSERT_EQ(IntAvlTreeVerify(&test_avl_tree), true);
}

TEST(AvlTreeTest, Find) {
	IntAvlBsTreeStackVector stack;
	for (int i = 0; i < test_count; i++) {
		if (IntAvlTreeFind(&test_avl_tree, &stack, &test_res_data_set[i]) == nullptr) {
			ASSERT_TRUE(false);
		}
	}
}

TEST(AvlTreeTest, Delete) {
	IntAvlBsTreeStackVector stack;
	for (int i = 0; i < test_count; i++) {
		IntAvlEntry* avl_entry = IntAvlTreeFind(&test_avl_tree, &stack, &test_res_data_set[i]);
		if (avl_entry == nullptr) {
			ASSERT_TRUE(false);
		}
		IntAvlTreeDelete(&test_avl_tree, &stack, avl_entry);
		delete (IntAvlEntry_User*)avl_entry;
	}

	ASSERT_EQ(test_avl_tree.root, nullptr);
}


TEST(AvlTreeTest, End) {
	test_res_data_set.clear();
}
