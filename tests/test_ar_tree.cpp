
#include <chrono>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <libyuc/tests/rand_data_set.h>

#define LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
#include <libyuc/container/experimental/ar_tree.h>


static int seed = 312962319;
static size_t test_count = 10000;
static ArTree test_ar_tree;
static std::set<int64_t> test_set;
#ifdef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
static std::vector<int64_t> test_res_data_set;
#else
static std::vector<std::string> test_res_data_set;
#endif

void PrintArt(ArTree* tree, ArNode* node, int Level) {
	if (!node) return;
	char* empty = (char*)malloc(Level * 8 + 1);
	memset(empty, ' ', Level * 8);
	empty[Level * 8] = 0;
	if (ArNodeIsLeaf(node)) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		printf("%sleaf:%s\n", empty, ArLeafGetElement(ArNodeGetLeaf(node))->buf);
#else
		printf("%sleaf:%llx\n", empty, *ArLeafGetElement(ArNodeGetLeaf(node)));
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
			printf("%snode4:\n%s        prefix:%d \"%s%s\"\n", empty, empty, node->head.prefix_len,  prefix, abc);
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


TEST(ArTreeTestEnv, Start) {
	srand(seed);

#ifdef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
	test_res_data_set = GenerateI64Vector(test_count, false);
#else
	test_res_data_set = GenerateStringVector(test_count, 64, 128);
#endif
	ArTreeInit(&test_ar_tree);
}

TEST(ArTreeTest, Insert) {
	for (auto& data : test_res_data_set) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		art_element_type* ele = new art_element_type;
		ele->buf = (uint8_t*)data.data();
		ele->size = data.size();
		ArTreePut(&test_ar_tree, ele);
#else
		ArTreePut(&test_ar_tree, &data);
#endif

		//PrintArt(&test_ar_tree, test_ar_tree.root, 0);
		//printf("\n\n\n");
	}
}

TEST(ArTreeTest, Find) {
	auto start_time = std::chrono::high_resolution_clock::now();
	for (auto& data : test_res_data_set) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		art_element_type ele;
		ele.buf = (uint8_t*)data.data();
		ele.size = data.size();
		auto data_ptr = ArTreeFind(&test_ar_tree, &ele);
#else
		auto data_ptr = ArTreeFind(&test_ar_tree, &data);
#endif
		if (data_ptr == nullptr) {
			ASSERT_TRUE(false);
		}
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	std::cout << "time: " << duration.count() << "ns" << std::endl;
}

TEST(ArTreeTest, Iterator) {
	size_t i = 0;
	ArTreeIterator iter;
	auto data = ArTreeIteratorFirst(&test_ar_tree, &iter);
	while (data) {
		++i;
		data = ArTreeIteratorNext(&iter);
	}
	ASSERT_EQ(i, test_res_data_set.size());
}

TEST(ArTreeTest, Delete) {
	for (auto& data : test_res_data_set) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
		art_element_type ele;
		ele.buf = (uint8_t*)data.data();
		ele.size = data.size();
		auto data_ptr = ArTreeDelete(&test_ar_tree, &ele);
		delete data_ptr;
#else
		auto data_ptr = ArTreeDelete(&test_ar_tree, &data);
#endif
		if (data_ptr == nullptr) {
			// 可能存在重复key
			// ASSERT_TRUE(false);
		}
	}

	ASSERT_EQ(test_ar_tree.root, nullptr);
}


TEST(ArTreeTest, End) {
	test_res_data_set.clear();
}