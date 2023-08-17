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


typedef struct _TsSplice {
    TsSkipListEntry* prev[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
    TsSkipListEntry* cur[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
    int level;
    int cmp;
} TsSplice;


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


void TsSkipListSpliceRebuildInvalidPrev(TsSplice* splice, int level_sub_1, int key) {
    for (int i = level_sub_1 + 1; i < splice->level; i++) {
        if (!IS_MARK(splice->prev[i]->upper[i].next)) {
            // 这一层的prev有效，开始根据key重建下层的铰接点
            splice->prev[level_sub_1] = splice->prev[i];
            
            break;
        }
    }

    // 所有铰接点的Prev都无效，整个重建
}

/*
* 删除splice指定层被标记的cur节点
*/
bool TsSkipListCollaborativeDelete(TsSplice* splice, int level_sub_1) {
    do {
        TsSkipListEntry* prev = splice->prev[level_sub_1];
        TsSkipListEntry* cur = splice->cur[level_sub_1];
        if (AtomicPtrCompareExchange(&prev->upper[level_sub_1].next, (TsSkipListEntry*)CLEAR_MARK(cur)->upper[level_sub_1].next, cur)) break;
        // 删除失败的场景
        // 1.prev将要被删除/已被删除，被打上标记
        TsSkipListEntry* prev_next = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[level_sub_1].next);
        if (IS_MARK(prev_next)) {
            // 这种情况，根据铰接点来向前定位有效prev
            

        }
        // 2.prev和cur之间插入了新节点
        // 3.cur被其他线程删除
        cur = prev_next;
    } while (true);
    return true;
}


/*
* 在指定层定位key
* cur最终指向第一个 >= key的节点
*/
static forceinline bool TsSkipListUpdateSpliceLevel(TsSplice* splice, int level_sub_1, int key) {
    TsSkipListEntry* cur = splice->cur[level_sub_1];
    TsSkipListEntry* prev = splice->prev[level_sub_1];
    TsSkipListEntry* prev_prev = NULL;
    // release_assert(!IS_MARK(prev->upper[level_sub_1].next), "prev is_mark!");
    if (cur != NULL) {
        do {
            if (IS_MARK(cur)) {
                // prev节点被标记了删除，协同删除
                TsSkipListCollaborativeDelete(splice, level_sub_1);
                prev = (TsSkipListEntry*)AtomicPtrLoad(&prev_prev->upper[level_sub_1].next);
            }
            if (cur->key >= key) {
                splice->cur[level_sub_1] = cur;
                splice->prev[level_sub_1] = prev;
                splice->cmp = cur->key == key ? 0 : 1;
                return true;
            }
            prev_prev = prev;
            prev = cur;
            cur = (TsSkipListEntry*)AtomicPtrLoad(&cur->upper[level_sub_1].next);
        } while (cur);
    }
    // 插入到末尾
    splice->prev[level_sub_1] = prev;
    splice->cur[level_sub_1] = NULL;
    splice->cmp = -1;
    return true;
}

static forceinline TsSkipListEntry* TsSkipListBuildSplice(TsSkipList* list, int max_level, int key, TsSplice* splice) {
    splice->cmp = -1;
    TsSkipListLevel* prev_level = list->head;
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    TsSkipListEntry* prev_entry = NULL;
    TsSkipListEntry* cur_entry = NULL;
    for (int i = max_level - 1; i >= 0; i--) {
        // 当前索引层的链表查找
        prev_entry = ObjectGetFromField(prev_level, TsSkipListEntry, upper);
        cur_entry = (TsSkipListEntry*)AtomicPtrLoad(&prev_level[i].next);
        if (!TsSkipListUpdateSpliceLevel(splice, i, key)) {
            
            i = max_level - 1;
        }
        if (splice) {
            splice->prev[i] = prev_entry->upper;        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
            splice->cur[i] = cur_entry;
        }
        prev_level = prev_entry->upper;
        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }
    if (splice->cmp == 0) {
        if (splice->cur[cur_entry->level_record - 1] != cur_entry) {
            // 插入是自底向上的，但查找可能提前在低层索引找到了这个插入的节点，但是其他插入线程在高层索引还没有插入这个节点，此时应将当前节点视为未插入
            // 查找/删除可以在循环里提前返回，插入需要完整的update数组
            splice->cmp = -1;
        }
    }
    return prev_entry;
}

bool TsSkipListFind(TsSkipList* list, int key) {
    TsSplice splice;
    TsSkipListEntry* prev = TsSkipListBuildSplice(list, list->level, key, &splice);
    return splice.cmp == 0;
}

bool TsSkipListInsert(TsSkipList* list, int key, TsSkipListEntry** ptr) {
    TsSplice splice;


    // 准备创建随机高度索引层的节点
    int new_level = TsRandomLevel();

    
    TsSkipListEntry* new_entry = TsSkipListCreateEntry(new_level, key);
    *ptr = new_entry;

    int level = list->level;
    TsSkipListEntry* prev = TsSkipListBuildSplice(list, level, key, &splice);

    // cur此时的next要么指向NULL，要么>=key
    TsSkipListEntry* cur = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[0].next);

    if (new_level > level) {
        // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
        for (int i = level; i < new_level; i++) {
            splice.prev[i] = ObjectGetFromField(list->head, TsSkipListEntry, upper);        // 头节点该层的索引需要指向新节点同层索引，待更新头节点
            splice.cur[i] = NULL;      // 头节点的下一节点此时应该是NULL
        }
    }


    // 自底向上插入

    for (int i = 0; i < new_level; i++) {
        prev = ObjectGetFromField(splice.prev[i], TsSkipListEntry, upper);
        cur = splice.cur[i];
        do {
            new_entry->upper[i].next = cur;
            if (AtomicPtrCompareExchange(&prev->upper[i].next, new_entry, cur)) {
                break;
            }
            // 更新失败的场景，即prev的next不为cur
            TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[i].next);
            // 1.prev被删除，被做了标记 
            if (IS_MARK(next)) {
                // 从最顶层开始重建铰接点
                TsSkipListCollaborativeDelete(&splice, i);
            }
            // 2.prev和cur之间插入了新节点，在当前层重新定位合适的插入点
            cur = next;

            if (!TsSkipListUpdateSpliceLevel(i, &splice, key)) {
                
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


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_