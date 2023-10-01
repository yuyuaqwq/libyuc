#define _CRT_RAND_S

#include <thread>

#include <gtest/gtest.h>

#include <libyuc/container/thread_safe/experimental/ts_skip_list.h>
#include <libyuc/test/rand_data_set.h>

static size_t test_count = 10000000;
static size_t test_thread_count = 1;
static TsSkipList test_skip_list;
static std::set<int64_t> test_set;
static std::vector<int64_t> test_res_data_set;

void PrintSkipList(TsSkipList* list) {
	auto cur = list->head;
	//while (cur) {
	//	auto entry = ObjectGetFromField(cur, TsSkipListEntry, upper);
	//	if (cur != list->head) {
	//		for (int j = 0; j < entry->level_count; j++) {
	//			printf("key:%d\t\t", entry->key);
	//		}
	//		printf("\n\n");
	//	}
	//	if (cur[0].next) {
	//		cur = (TsSkipListLevel*)cur[0].next->upper;
	//	}
	//	else {
	//		cur = NULL;
	//	}
	//}
}

TEST(TsSkipListTestEnvTestEnv, Start) {
	test_res_data_set = GenerateI64Vector(test_count);
	TsSkipListInit(&test_skip_list);
	test_thread_count = std::thread::hardware_concurrency();
	test_thread_count = 10;
}


TEST(TsSkipListTestEnvTest, Insert) {
	std::vector<std::thread> t;
	t.reserve(test_thread_count);
	for (int k = 0; k < test_thread_count; k++) {
		t.push_back(std::thread([](int k) {
			for (int i = k * (test_count / test_thread_count); i < (k + 1) * (test_count / test_thread_count); i++) {
				TsSkipListEntry* entry;
				TsSkipListPut(&test_skip_list, test_res_data_set[i], &entry);
			}
		}, k));
	}
	for (int k = 0; k < test_thread_count; k++) {
		t[k].join();
	}
}

TEST(TsSkipListTestEnvTest, Find) {
	std::vector<std::thread> t;
	t.reserve(test_thread_count);
	for (int k = 0; k < test_thread_count; k++) {
		t.push_back(std::thread([](int k) {
			for (int i = k * (test_count / test_thread_count); i < (k + 1) * (test_count / test_thread_count); i++) {
				if (TsSkipListFind(&test_skip_list, test_res_data_set[i]) == false) {
					ASSERT_TRUE(false);
				}
			}
			}, k));
	}
	for (int k = 0; k < test_thread_count; k++) {
		t[k].join();
	}
}

TEST(TsSkipListTestEnvTest, Delete) {
	std::vector<std::thread> t;
	t.reserve(test_thread_count);
	for (int k = 0; k < test_thread_count; k++) {
		t.push_back(std::thread([](int k) {
			for (int i = k * (test_count / test_thread_count); i < (k + 1) * (test_count / test_thread_count); i++) {
				if (!TsSkipListDelete(&test_skip_list, test_res_data_set[i])) {
					ASSERT_TRUE(false);
				}
			}
		}, k));
	}
	for (int k = 0; k < test_thread_count; k++) {
		t[k].join();
	}
}


TEST(TsSkipListTestEnvTest, Concurrency) {
	std::vector<std::thread> ins_t;
	ins_t.reserve(test_thread_count);
	for (int k = 0; k < test_thread_count; k++) {
		ins_t.push_back(std::thread([](int k) {
			for (int i = k * (test_count / test_thread_count); i < (k + 1) * (test_count / test_thread_count); i++) {
				TsSkipListEntry* entry;
				TsSkipListPut(&test_skip_list, test_res_data_set[i], &entry);
			}
		}, k));
	}
	std::vector<std::thread> del_t;
	del_t.reserve(test_thread_count);
	for (int k = 0; k < test_thread_count; k++) {
		del_t.push_back(std::thread([](int k) {
			for (int i = k * (test_count / test_thread_count); i < (k + 1) * (test_count / test_thread_count); i++) {
				if (!TsSkipListDelete(&test_skip_list, test_res_data_set[i])) {
					//ASSERT_TRUE(false);
				}
			}
			}, k));
	}
	for (int k = 0; k < test_thread_count; k++) {
		ins_t[k].join();
		del_t[k].join();
	}
}


TEST(TsSkipListTestEnvTest, End) {
	test_res_data_set.clear();
}
