#include <chrono>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <libyuc/tests/rand_data_set.h>


#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME Int
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element union IntEntry_User
#include <libyuc/space_manager/object_pool.h>

union IntEntry_User {
	int64_t ele;
};


#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME Int
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element IntEntry_User
#include <libyuc/space_manager/object_pool.c>


static size_t test_count = 100000000;
static IntObjectPool test_pool;
static std::vector<IntObjectPoolSlotPos> test_res;
static std::vector<int64_t*> test_res_stl;


TEST(ObjectPoolTestEnv, Start) {
	IntObjectPoolInit(&test_pool);
	test_res.resize(test_count);
	test_res_stl.resize(test_count);
}


TEST(ObjectPoolTest, Alloc) {
	for (int i = 0; i < test_count; i++) {
		test_res[i] = IntObjectPoolAlloc(&test_pool);
		//auto& obj = ObjectPoolGetPtr(&test_pool, &test_res[i])->element;
		//obj.ele = i;
	}
}


TEST(ObjectPoolTest, Free) {
	for (int i = 0; i < test_count; i++) {
		//auto obj = ObjectPoolGetPtr(&test_pool, &test_res[i])->element;
		//ASSERT_EQ(obj.ele, i);
		IntObjectPoolFree(&test_pool, &test_res[i]);
	}

}


TEST(ObjectPoolTest, Alloc_Stl) {
	for (int i = 0; i < test_count; i++) {
		test_res_stl[i] = new int64_t;
	}
}

TEST(ObjectPoolTest, Alloc_Free) {
	for (int i = 0; i < test_count; i++) {
		delete test_res_stl[i];
	}
}

TEST(ObjectPoolTest, End) {
	test_res.clear();
	test_res_stl.clear();
	IntObjectPoolRelease(&test_pool);
}
