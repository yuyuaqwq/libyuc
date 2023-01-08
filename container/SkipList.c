#include "SkipList.h"

const int SKIPLIST_P = (RAND_MAX / 2);
#define SKIPLIST_MAX_LEVEL 64

static int RandomLevel() {
	int level = 1;

	while (SKIPLIST_P <= rand() && level < SKIPLIST_MAX_LEVEL)
		level++;

	return level;
}

static SkipListEntry* SkipListCreateEntry(int level, void* obj) {
	SkipListEntry* entry = (SkipListEntry*)MemoryAlloc(sizeof(SkipListEntry) + level * sizeof(SkipListLevel));
	entry->obj = obj;
	return entry;
}

static forceinline SkipListEntry* SkipListFind_(SkipList* list, void* key, int* cmpRes, SkipListEntry** update) {
	SkipListEntry* cur = list->head;
	// 从最顶层开始遍历，每趟循环都相当于下降一层索引
	for (int i = list->level - 1; i >= 0; i--) {
		// 当前索引层的查找
		while (cur->upper[i].next) {
			void* nextKey = GetFieldByFieldOffset(cur->upper[i].next->obj, list->keyFieldOffset, void);
			*cmpRes = list->cmpFunc(nextKey, key, list->keyFieldSize);
			if (*cmpRes >= 0) {
				break;
			}
			cur = cur->upper[i].next;
		}
		
		if (update) {
			update[i] = cur;		// 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
		}

		if (*cmpRes == 0) {
			// 不支持重复key，当前索引层找到了就不再需要下降了
			break;
		}

	}
	return cur;
}

void SkipListInit(SkipList* list, int keyFieldOffset, int keyFieldSize, CmpFunc cmpFunc) {
	list->level = 1;
	list->head = SkipListCreateEntry(SKIPLIST_MAX_LEVEL, 0);

	for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) {
		list->head->upper[i].next = NULL;
	}

	list->keyFieldOffset = keyFieldOffset;
	list->keyFieldSize = keyFieldSize;

	if (cmpFunc == NULL) {
		cmpFunc = MemoryCmpR;
	}
	list->cmpFunc = cmpFunc;
}

void SkipListRelease(SkipList* list, bool deleteObj) {
	SkipListEntry* cur = SkipListFirst(list);
	while (cur) {
		if (deleteObj) {
			DeleteObject_(cur->obj);
		}
		SkipListEntry* next = SkipListNext(list, cur);
		DeleteObject_(cur);
		cur = next;
	}
}


void* SkipListFind(SkipList* list, void* key) {
	int cmpRes = 0;
	SkipListEntry* cur = SkipListFind_(list, key, &cmpRes, NULL);

	cur = cur->upper[0].next;		// 刚出循环时cur还未更新
	// 查找出来，要么是所有索引层都找不到，要么是cur的key>=查找的key

	if (cur && cmpRes == 0) {
		return cur->obj;
	}
	return NULL;
}

bool SkipListInsert(SkipList* list, void* obj) {
	SkipListEntry* update[SKIPLIST_MAX_LEVEL];		// 对应每一层需要更新索引的节点，因为新节点可能会插入索引

	void* key = GetFieldByFieldOffset(obj, list->keyFieldOffset, void);

	int cmpRes = 0;
	SkipListEntry* cur = SkipListFind_(list, key, &cmpRes, update);

	// cur此时的next要么指向NULL，要么>=key
	if (cur->upper[0].next && cmpRes == 0) {
		// key相等则不插入
		return false;
	}

	// 准备创建随机高度索引层的节点
	int level = RandomLevel();
	if (level > list->level) {
		// 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
		for (int i = list->level; i < level; i++) {
			update[i] = list->head;		// 头节点该层的索引需要指向新节点同层索引
		}
		list->level = level;
	}

	// 创建节点
	SkipListEntry* newEntry = SkipListCreateEntry(level, obj);
	for (int i = 0; i < level; i++) {
		// 连接到同层索引链表中
		newEntry->upper[i].next = update[i]->upper[i].next;
		update[i]->upper[i].next = newEntry;
	}

	return true;
}

void* SkipListDelete(SkipList* list, void* key) {
	SkipListEntry* update[SKIPLIST_MAX_LEVEL];		// 对应每一层需要更新索引的节点，因为新节点可能会删除索引

	int cmpRes = 0;
	SkipListEntry* cur = SkipListFind_(list, key, &cmpRes, update);
	cur = cur->upper[0].next;

	if (!cur || cmpRes) {
		// 找不到该节点
		return NULL;
	}

	// 最底层索引开始向上更新
	for (int i = 0; i < list->level; i++) {
		if (update[i]->upper[i].next != cur) {		// 该层的索引不指向被删除的节点，停止更新
			break;
		}
		update[i]->upper[i].next = cur->upper[i].next;		// 删除索引，即指向被删除索引的下一索引
	}

	// 被删除的索引层可能是最高索引层，需要调整
	while (list->level > 1 && list->head->upper[list->level - 1].next == NULL) {
		list->level--;
	}

	void* obj = cur->obj;
	DeleteObject_(cur);
	return obj;
}


SkipListEntry* SkipListFirst(SkipList* list) {
	return list->head->upper[0].next;
}

SkipListEntry* SkipListNext(SkipList* list, SkipListEntry* cur) {
	return cur->upper[0].next;
}