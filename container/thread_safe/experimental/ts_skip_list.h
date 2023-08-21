/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_

/*
* Lock-Free SkipList - 无锁跳表
* 参考：https://zhuanlan.zhihu.com/p/600729377
*/

/*
* 删除：
* 协同删除
    * 遇到cur被标记为删除时，协同删除
    * 遇到prev被标记为删除时，从头开始搜索
*/


#include <libyuc/basic.h>

#include <libyuc/concurrency/atomic.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL 32
const int LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P = (RAND_MAX / 2);

#define id_type AtomicPtr(struct _TsSkipListEntry)

#define IS_MARK(p) ((uintptr_t)(p) & 0x01)
#define MARK(p) ((TsSkipListEntry*)((uintptr_t)(p) | 0x01))
#define CLEAR_MARK(p) ((TsSkipListEntry*)((uintptr_t)(p) & ~0x01))



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
    int reference_count;
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
    number = rand();
    number &= RAND_MAX;
    while (LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P <= number && level < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL) {
        level++;
        rand_s(&number);
        number = rand();
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
    entry->reference_count = 1;
    return entry;
}






/*
* 根据prev和key定位单层铰接点
*/
static forceinline int TsSkipListLevelFindKey(TsSkipListEntry** prev, TsSkipListEntry** cur, int level_sub_1, int key) {
    do {
        *cur = CLEAR_MARK(AtomicPtrLoad(&(*prev)->upper[level_sub_1].next));
        if (!*cur) break;

        // 如果遇到cur是被标记的删除就顺带尝试CAS删除
        TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&(*cur)->upper[level_sub_1].next);
        if (IS_MARK(next)) {
            next = CLEAR_MARK(next);
            AtomicPtrCompareExchange(&(*prev)->upper[level_sub_1].next, next, *cur);
            // 查找过程不关心是否成功
            *cur = next;
        }
        if (!*cur) break;

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
        *cur = (TsSkipListEntry*)AtomicPtrLoad(&(*prev)->upper[level_sub_1].next);
        if (!*cur) break;

        // 如果遇到cur是被标记的删除就顺带尝试CAS删除
        TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&(*cur)->upper[level_sub_1].next);
        if (IS_MARK(next)) {
            next = CLEAR_MARK(next);
            AtomicPtrCompareExchange(&(*prev)->upper[level_sub_1].next, next, *cur);
            // 查找过程不关心是否成功
            *cur = next;
            if (!*cur) break;
        }
        
        if ((*cur)->key > node->key) {
            return 1;
        }
        if ((*cur) == node) {
            return 0;
        }

        (*prev) = (*cur);
    } while (true);
    return -1;
}

/*
* 对节点进行删除标记，必定完成，如果是当前线程对其标记返回true，其他线程对其标记返回false
*/
static forceinline bool TsSkipListNodeMarkDeleteing(TsSkipListEntry* node, int level) {
    do {
        // 为当前层的cur打上删除标记
        TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&node->upper[level].next);
        if (!AtomicPtrCompareExchange(&node->upper[level].next, MARK(next), CLEAR_MARK(next))) {
            // 标记失败的场景                                                    记
            if (IS_MARK(AtomicPtrLoad(&node->upper[level].next))) {
                // 如果要考虑可能存在相同key的情况，可以不直接返回失败，而是重新找相同的key的节点
                // 重新找会出现当前线程等待被标记的节点删除完成的情况吗？
                // goto _retry;

                // 已经有其他线程标记删除了，直接返回false就行
                return false;
            }
            // 2.next已被其他线程删除，尝试重新获取next
            // 3.cur和next中插入了其他节点，尝试重新获取next
            else {
                continue;
            }
        }
        break;
    } while (true);
    return true;
}

/*
* 按key构建铰接点
*/
static forceinline void TsSkipListBuildSpliceByKey(TsSkipList* list, TsSkipListSplice* splice, int max_level, int min_level, int key) {
    splice->cmp = -1;
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    
    TsSkipListEntry* prev = ObjectGetFromField(list->head, TsSkipListEntry, upper);
    TsSkipListEntry* cur = NULL;
    for (int i = max_level - 1; i >= min_level; i--) {
        // 当前索引层的链表查找
        splice->cmp = TsSkipListLevelFindKey(&prev, &cur, i, key);

        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
        splice->prev[i] = prev;
        if (IS_MARK(splice->prev[i]->upper[i].next) && splice->prev[i]->level_record > i + 1 && !IS_MARK(splice->prev[i]->upper[i+1].next)) {
            // 上层如果没有被标记但下层被标记/删除了，则说明是插删冲突导致上层节点遗留的场景，需要对其进行标记
            for (int j = i + 1; j < splice->prev[i]->level_record; j++) {
                 release_assert(!IS_MARK(splice->prev[i]->upper[j].next), "insertion and deletion conflict exception.");
                TsSkipListNodeMarkDeleteing(splice->prev[i], j);
            }
        }

        splice->cur[i] = cur;

        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }

    // 插入是自底向上的，但查找可能提前在低层索引找到了这个插入的节点，但是其他插入线程在高层索引还没有完成CAS插入这个节点
    // 策略1：将当前节点视为未插入(如果有重复key场景需求会导致后面存在的节点找不到)
    // 策略2：基于引用计数，在节点所有level都完全脱链后再回收
}



bool TsSkipListFind(TsSkipList* list, int key) {
    TsSkipListSplice splice;
    TsSkipListBuildSpliceByKey(list, &splice, AtomicInt32Load(&list->level), 0, key);
    return splice.cmp == 0;
}

bool TsSkipListPut(TsSkipList* list, int key, TsSkipListEntry** ptr) {
    TsSkipListSplice splice;

    // 准备创建随机高度索引层的节点
    int new_level = TsRandomLevel();
    TsSkipListEntry* new_entry = TsSkipListCreateEntry(new_level, key);

    *ptr = new_entry;


    int level = AtomicInt32Load(&list->level);
    TsSkipListBuildSpliceByKey(list, &splice, level, 0, key);

    if (splice.cmp == 0) {
        // value
        return true;
    }

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
            TsSkipListEntry* new_cur = (TsSkipListEntry*)AtomicPtrLoad(&splice.prev[i]->upper[i].next);
            // 1.prev被删除，被做了标记 
            if (IS_MARK(new_cur)) {
                // 重建铰接点
                // 简单策略，直接最上层重算，不会需要等待其他线程完成删除
                    // 删除是自顶向下的
                    // 要么上层prev被标记了，下层没标记
                    // 要么上层prev已被摘除，下层prev被标记
                    // 由于插删冲突，还是可能出现上层prev没标记，下层prev却被标记的情况，但构建过程中进行了处理
                // 优化则可以通过当前的铰接点来修正prev
                TsSkipListBuildSpliceByKey(list, &splice, level, 0, key);
                continue;
            }
            // 2.prev和cur之间插入了新节点，在当前层重新定位合适的插入点
            // 3.cur被删除，也重新定位
            if (TsSkipListLevelFindKey(&splice.prev[i], &splice.cur[i], i, key) == 0) {
                if (i != 0) {
                    continue;
                }
                // 在level0中触发相同key的插入冲突，即已经插入了一个相同的key，可以直接回收分配的节点
                // splice.cur[i].value = value;
                *ptr = NULL;
                ObjectRelease(new_entry);
                return true;
            }
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
    TsSkipListBuildSpliceByKey(list, &splice, level, 0, key);

    if (splice.cmp) {
        return false;
    }

    TsSkipListEntry* del = splice.cur[0];

    // 为何更新？因为提供的level可能比cur的level小(cur是新插入的)
    level = splice.cur[0]->level_record;

    // 自顶向下删除
    // 先标记
    for (int i = level - 1; i != -1; i--) {
        if (splice.cur[i] != del) {
            continue;
        }
        if (!TsSkipListNodeMarkDeleteing(splice.cur[i], i)) {
            return false;
        }
    }

    // 只有竞争成功的线程会进行删除
    for (int i = level - 1; i != -1; i--) {
        do {
            if (splice.cur[i] != del) {
                break;
            }
            // 尝试删除
            TsSkipListEntry* next = CLEAR_MARK(AtomicPtrLoad(&splice.cur[i]->upper[i].next));
            if (AtomicPtrCompareExchange(&splice.prev[i]->upper[i].next, next, splice.cur[i])) {
                // 删除成功
                break;
            }

            // 删除失败的场景
            // 1.prev将要被删除/已被删除，被打上标记
            TsSkipListEntry* new_cur = (TsSkipListEntry*)AtomicPtrLoad(&splice.prev[i]->upper[i].next);
            if (IS_MARK(new_cur)) {
                // 这里需要保证再次构建的splice还是原先的cur
                // 尝试重新构建铰接点
                TsSkipListBuildSpliceByKey(list, &splice, level, 0, key);
                // 重构建的过程可能会删除cur，随即会跳出循环返回true所以不影响
            }
            // 2.prev和cur之间插入了新节点，在当前层重新定位cur的prev
            else {
                int cmp = TsSkipListLevelFindNode(&splice.prev[i], &splice.cur[i], i, del);
                // 3.cur被删除，可能被其他线程辅助删除了，如果被删了就跳过
                if (splice.cur[i] != del) {
                    break;
                }
            }

        } while (true);
    }

    // 被删除的索引层可能是最高索引层，但不在这里调整

    // ObjectRelease(cur);      // 资源释放交给GC
    return true;
}



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_