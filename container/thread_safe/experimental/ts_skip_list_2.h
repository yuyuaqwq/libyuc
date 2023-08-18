/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_

/*
* Lock-Free SkipList - 无锁跳表
*/

/*
* 删除：
    * 协同删除
        * 即本跳表使用的方案
        * 遇到cur被标记为删除时，协同删除
        * 遇到prev被标记为删除时，从头开始搜索
    * 等待删除
        * 其他线程遇到删除标会通过被删除节点提供的回溯指针进行自旋，等待删除线程完成删除
*/


#include <libyuc/basic.h>

#include <libyuc/concurrency/atomic.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL 32
const int LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P = (RAND_MAX / 2);

#define id_type AtomicPtr(struct _TsSkipListEntry)

/*
* 跳表索引节点
*/
typedef struct _TsSkipListLevel {
    id_type next;        // 指向的同索引层的下一个SkipListEntry(不是直接指向逻辑上的同层链表)
} TsSkipListLevel;

/*
* 跳表节点，每个节点有随机的索引层数，所以用柔性数组动态分配
*/
typedef struct _TsSkipListEntry {
    int key;
    int level_record;
    TsSkipListLevel upper[];        // 节点的上层，是索引节点
} TsSkipListEntry;


typedef struct _TsSkipList {
    TsSkipListLevel head[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
    AtomicInt32 level;
} TsSkipList;


typedef struct _TsSkipListSplice {
    TsSkipListEntry* prev[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
    TsSkipListEntry* cur[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
    int level;
    int cmp;
} TsSkipListSplice;


static int TsRandomLevel() {
    int level = 1;
    unsigned int number;
    rand_s(&number);
    number &= RAND_MAX;
    while (LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P <= number && level < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL) {
        level++;
        rand_s(&number);
        number &= RAND_MAX;
    }
    return level;
}

void TsSkipListInit(TsSkipList* list) {
    AtomicInt32Store(&list->level, 1);
    for (int i = 0; i < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL; i++) {
        list->head[i].next = NULL;
    }
}

static TsSkipListEntry* TsSkipListCreateEntry(int level, int key) {
    TsSkipListEntry* entry = (TsSkipListEntry*)MemoryAlloc(sizeof(TsSkipListEntry) + level * sizeof(TsSkipListLevel));
    entry->key = key;
    entry->level_record = level;
    return entry;
}

#define IS_MARK(p) ((uintptr_t)(p) & 0x01)
#define MARK(p) ((TsSkipListEntry*)((uintptr_t)(p) | 0x01))
#define CLEAR_MARK(p) ((TsSkipListEntry*)((uintptr_t)(p) & ~0x01))






/*
* 根据prev和key定位单层铰接点
*/
static forceinline int TsSkipListLevelFindKey(TsSkipListEntry** prev, TsSkipListEntry** cur, int level_sub_1, int key) {
    do {
        *cur = (TsSkipListEntry*)AtomicPtrLoad(&(*prev)->upper[level_sub_1].next);
        if (!*cur) break;

        // 如果遇到cur是被标记的删除就顺带进行CAS删除
        TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&(*cur)->upper[level_sub_1].next);
        if (IS_MARK(next)) {
            next = CLEAR_MARK(next);
            AtomicPtrCompareExchange(&(*prev)->upper[level_sub_1].next, next, *cur);
            // 不必关心是否成功
            *cur = next;
        }
        if ((*cur)->key >= key) {
            return (*cur)->key == key ? 0 : 1;
        }
        (*prev) = (*cur);
    } while (true);
    return -1;
}

/*
* 根据prev和node定位单层铰接点
*/
static forceinline int TsSkipListLevelFindNode(TsSkipListEntry** prev, TsSkipListEntry** cur, int level_sub_1, TsSkipListEntry* node) {
    do {
        *cur = CLEAR_MARK((TsSkipListEntry*)AtomicPtrLoad(&(*prev)->upper[level_sub_1].next));
        if (!*cur) break;
        if ((*cur)->key > node->key) {
            return 1;
        }
        else if ((*cur) == node) {
            return 0;
        }
        (*prev) = (*cur);
    } while (true);
    return -1;
}

/*
* 按key构建铰接点
*/
static forceinline void TsSkipListBuildSpliceByKey(TsSkipList* list, TsSkipListSplice* splice, int max_level, int key) {
    splice->cmp = -1;
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    TsSkipListEntry* prev = ObjectGetFromField(list->head, TsSkipListEntry, upper);
    TsSkipListEntry* cur = NULL;
    for (int i = max_level - 1; i >= 0; i--) {
        // 当前索引层的链表查找
        splice->cmp = TsSkipListLevelFindKey(&prev, &cur, i, key);

        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
        splice->prev[i] = prev;
        splice->cur[i] = cur;

        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }

    if (splice->cmp == 0) {
        if (splice->cur[cur->level_record - 1] != cur) {
            // 插入是自底向上的，但查找可能提前在低层索引找到了这个插入的节点，但是其他插入线程在高层索引还没有插入这个节点，此时应将当前节点视为未插入
            // 查找/删除可以在循环里提前返回，插入需要完整的update数组
            splice->cmp = -1;
        }
    }
}



/*
* 按node构建铰接点
*/
static forceinline void TsSkipListSpliceFindNode(TsSkipList* list, TsSkipListSplice* splice, int max_level, int min_level, TsSkipListEntry* node) {
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    TsSkipListEntry* prev = ObjectGetFromField(list->head, TsSkipListEntry, upper);
    TsSkipListEntry* cur = NULL;
    for (int i = max_level - 1; i >= min_level; i--) {
        // 当前索引层的链表查找
        TsSkipListLevelFindNode(&prev, &cur, i, node);

        if (splice) {
            splice->prev[i] = prev;        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
            splice->cur[i] = cur;
        }

        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }
}


/*
* 根据cur定位其prev，只构建cur的prev铰接点，不考虑prev是否无效
*/
void TsSkipListSpliceRepairLevelPrev(TsSkipListSplice* splice, int level_sub_1) {
    for (int i = level_sub_1 + 1; i < splice->level; i++) {
        if (splice->prev[i] != splice->prev[level_sub_1]) {
            TsSkipListEntry* node = splice->prev[level_sub_1];
            splice->prev[level_sub_1] = splice->prev[i];
            // 自底向上定位到第一个高层索引不同的节点即可下降搜索
            TsSkipListLevelFindNode(&splice->prev[level_sub_1], &splice->cur[level_sub_1], level_sub_1, node);
            return;
        }
    }

    // 最顶层节点也相同，只能重建高层的铰接点
    
}

/*
* 删除splice指定层被标记的cur节点
*/
bool TsSkipListAssistDelete(TsSkipListSplice* splice, int level_sub_1) {
    TsSkipListEntry* del = splice->cur[level_sub_1];
    do {
        if (AtomicPtrCompareExchange(&splice->prev[level_sub_1]->upper[level_sub_1].next, (TsSkipListEntry*)CLEAR_MARK(splice->cur[level_sub_1]->upper[level_sub_1].next), splice->cur[level_sub_1])) {
            break;
        }

        // 删除失败的场景
        // 1.prev将要被删除/已被删除，被打上标记
        TsSkipListEntry* prev_next = (TsSkipListEntry*)AtomicPtrLoad(&splice->prev[level_sub_1]->upper[level_sub_1].next);
        if (IS_MARK(prev_next)) {
            TsSkipListSpliceRepairLevelPrev(splice, level_sub_1);
        }
        // 2.prev和cur之间插入了新节点
        // 3.cur被其他线程删除
    } while (true);
    return true;
}


bool TsSkipListFind(TsSkipList* list, int key) {
    TsSkipListSplice splice;
    TsSkipListBuildSpliceByKey(list, &splice, list->level, key);
    return splice.cmp == 0;
}

bool TsSkipListInsert(TsSkipList* list, int key, TsSkipListEntry** ptr) {
    TsSkipListSplice splice;

    // 准备创建随机高度索引层的节点
    int new_level = TsRandomLevel();
    TsSkipListEntry* new_entry = TsSkipListCreateEntry(new_level, key);

    *ptr = new_entry;

    int level = AtomicInt32Load(&list->level);
    TsSkipListBuildSpliceByKey(list, &splice, level, key);

    if (new_level > level) {
        // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
        for (int i = level; i < new_level; i++) {
            splice.prev[i] = ObjectGetFromField(list->head, TsSkipListEntry, upper);        // 头节点该层的索引需要指向新节点同层索引，待更新头节点
            splice.cur[i] = NULL;      // 头节点的下一节点此时应该是NULL
        }
    }

    // 自底向上插入
    for (int i = 0; i < new_level; i++) {
        do {
            new_entry->upper[i].next = splice.cur[i];
            if (AtomicPtrCompareExchange(&splice.prev[i]->upper[i].next, new_entry, splice.cur[i])) {
                break;
            }
            // 更新失败的场景，即prev的next不为cur
            TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&splice.prev[i]->upper[i].next);
            // 1.prev被删除，被做了标记 
            if (IS_MARK(next)) {
                // 重建铰接点
                // TsSkipListCollaborativeDelete(&splice, i);
            }
            // 2.prev和cur之间插入了新节点，在当前层重新定位合适的插入点
            TsSkipListLevelFindKey(&splice.prev[i], &splice.cur[i], i, key);
        } while (true);
    }

    if (new_level > level) {
        // 最后再更新高度，确保head是先写入的
        while (!AtomicInt32CompareExchange(&list->level, new_level, level)) {
            if (new_level <= list->level) break;        // 新高度更高也可以退出
            level = list->level;
        }
    }
    return true;
}

bool TsSkipListDelete(TsSkipList* list, int key) {
    TsSkipListSplice splice;

_retry:
    int level = AtomicInt32Load(&list->level);
    TsSkipListBuildSpliceByKey(list, &splice, level, key);

    if (splice.cmp) {
        return false;
    }
    bool success = false;

    // 为何更新？因为提供的level可能比cur的level小(cur是新插入的)
    level = splice.cur[0]->level_record;

    // 自顶向下删除
    for (int i = level - 1; i != -1; i--) {
        do {
            // 为当前层的cur打上删除标记
            TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&splice.cur[i]->upper[i].next);
            if (!AtomicPtrCompareExchange(&splice.cur[i]->upper[i].next, MARK(next), CLEAR_MARK(next))) {
                // 标记失败的场景
                // 1.cur被其他删除线程标记
                if (IS_MARK(AtomicPtrLoad(&splice.cur[i]->upper[i].next))) {
                    // goto _retry;        // 如果要考虑可能存在相同key的情况，可以不直接返回失败，而是重新找相同的key的节点
                    return false;
                    // 尝试协助删除
                }
                // 2.next已被其他线程删除，尝试重新获取next
                else {
                    continue;
                }
            }

            do {
                // 尝试删除
                if (AtomicPtrCompareExchange(&splice.prev[i]->upper[i].next, next, splice.cur[i])) {
                    // 删除成功
                    success = true;
                    break;
                }

                // 删除失败的场景
                // 1.prev将要被删除/已被删除，被打上标记
                TsSkipListEntry* new_cur = (TsSkipListEntry*)AtomicPtrLoad(&splice.prev[i]->upper[i].next);
                if (IS_MARK(new_cur)) {

                }
                // 2.prev和cur之间插入了新节点，在当前层重新定位cur的prev
                else {
                    bool success = TsSkipListLevelFindNode(&splice.prev[i], &splice.cur[i], i, splice.cur[i]);
                     release_assert(success, "find node failed.");
                }

            } while (true);
            break;
        } while (true);
    }

    // 被删除的索引层可能是最高索引层，但不在这里调整

    // ObjectRelease(cur);      // 资源释放交给GC
    return success;
}



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_