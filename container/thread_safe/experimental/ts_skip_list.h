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
        * 遇到cur被标记为删除时，协同删除
        * 遇到prev被标记为删除时，从头开始搜索
    * 等待删除
        * 即本跳表使用的方案
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
* 在指定层定位key
* cur最终指向第一个 >= key的节点
*/
static forceinline int TsSkipListLevelLocate(TsSkipListEntry** prev_ptr, TsSkipListEntry** cur_ptr, int level_sub_1, int key) {
    TsSkipListEntry* cur = *cur_ptr;
    TsSkipListEntry* prev = *prev_ptr;
    // release_assert(!IS_MARK(prev->upper[level_sub_1].next), "prev is_mark!");
    if (cur != NULL) {
        do {
            if (IS_MARK(cur)) {
                // 当前节点已经标记了删除，通过MARK标记的prev重试
                prev = CLEAR_MARK(cur);
                cur = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[level_sub_1].next);
                continue;
            }
            if (cur->key >= key) {
                *cur_ptr = cur;
                *prev_ptr = prev;
                return cur->key == key ? 0 : 1;
            }
            prev = cur;
            cur = (TsSkipListEntry*)AtomicPtrLoad(&cur->upper[level_sub_1].next);
        } while (cur);
    }
    // 插入到末尾
    *prev_ptr = prev;
    *cur_ptr = NULL;
    return -1;
}

static forceinline TsSkipListEntry* TsSkipListLocate(TsSkipList* list, int level, int key, int* cmp, TsSkipListLevel** update, TsSkipListEntry** update_next) {
    *cmp = -1;
    TsSkipListLevel* cur_level = list->head;
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    for (int i = level - 1; i >= 0; i--) {
        // 当前索引层的链表查找
        TsSkipListEntry* cur_entry = ObjectGetFromField(cur_level, TsSkipListEntry, upper);
        TsSkipListEntry* next_entry = (TsSkipListEntry*)AtomicPtrLoad(&cur_level[i].next);
        *cmp = TsSkipListLevelLocate(&cur_entry, &next_entry, i, key);
        if (update) {
            update[i] = cur_entry->upper;        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
            update_next[i] = next_entry;
        }
        cur_level = cur_entry->upper;
        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }
    return ObjectGetFromField(cur_level, TsSkipListEntry, upper);
}

bool TsSkipListFind(TsSkipList* list, int key) {
    int cmp;
    TsSkipListEntry* prev = TsSkipListLocate(list, list->level, key, &cmp, NULL, NULL);
    return cmp == 0;
}

bool TsSkipListInsert(TsSkipList* list, int key, TsSkipListEntry** ptr) {
    TsSkipListLevel* update[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];        // 对应每一层需要更新索引的节点，因为新节点可能会插入索引
    TsSkipListEntry* update_next[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];

    int cmp;
    int level = list->level;
    TsSkipListEntry* prev = TsSkipListLocate(list, level, key, &cmp, update, update_next);

    // cur此时的next要么指向NULL，要么>=key
    TsSkipListEntry* cur = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[0].next);

    // 准备创建随机高度索引层的节点
    int new_level = TsRandomLevel();
    if (new_level > level) {
        // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
        for (int i = level; i < new_level; i++) {
            update[i] = list->head;        // 头节点该层的索引需要指向新节点同层索引，待更新头节点
            update_next[i] = NULL;      // 头节点的下一节点此时应该是NULL
        }
    }

    // 自底向上插入
    TsSkipListEntry* new_entry = TsSkipListCreateEntry(new_level, key);

    *ptr = new_entry;
    for (int i = 0; i < new_level; i++) {
        prev = ObjectGetFromField(update[i], TsSkipListEntry, upper);
        cur = update_next[i];
        do {
            new_entry->upper[i].next = cur;
            if (AtomicPtrCompareExchange(&prev->upper[i].next, new_entry, cur)) {
                break;
            }
            // 更新失败的场景，即prev的next不为cur
            TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[i].next);
            // 1.prev被删除，被做了标记 
            if (IS_MARK(next)) {
                prev = CLEAR_MARK(next);        // 定位到标记时写入的上一节点，进行重试
                next = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[i].next);
            }
            // 2.prev和cur之间插入了新节点，在当前层重新定位合适的插入点
            cur = next;

            TsSkipListLevelLocate(&prev, &cur, i, key);
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
    TsSkipListLevel* update[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];        // 对应每一层需要更新索引的节点，因为新节点可能会插入索引
    TsSkipListEntry* update_next[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];


_retry:
    int cmp;
    int level = AtomicInt32Load(&list->level);
    TsSkipListEntry* prev = TsSkipListLocate(list, level, key, &cmp, update, update_next);

    if (cmp) {
        // 找不到该节点
        return false;
    }
    bool success = false;

    TsSkipListEntry* cur = update_next[0];      // 拿最低层的节点
    // 为何更新？因为提供的level可能比cur的level小(cur是新插入的)
    level = cur->level_record;

    if (update_next[level-1] != cur) {
        // 插入是自底向上的，但删除可能提前在低层索引找到了这个插入的节点，但是插入在高层索引还没有插入这个节点，此时将当前节点视为未插入
        return false;
    }

    // 自顶向下删除
    for (int i = level - 1; i != -1; i--) {
        
        prev = ObjectGetFromField(update[i], TsSkipListEntry, upper);
        do {
            // 为当前层的cur打上删除标记
            TsSkipListEntry* next = (TsSkipListEntry*)AtomicPtrLoad(&cur->upper[i].next);
            if (AtomicPtrCompareExchange(&cur->upper[i].next, MARK(prev), CLEAR_MARK(next))) {
                // 只有一个线程能标记成功

                // 尝试删除
                do {
                    if (AtomicPtrCompareExchange(&prev->upper[i].next, next, cur)) {
                        // release_assert(list->head[i].next != NULL, "delete head");
                        success = true;
                        break;
                    }
                    // 删除失败的场景
                    // 1.prev将要被删除/已被删除，被打上标记
                    TsSkipListEntry* temp_cur = (TsSkipListEntry*)AtomicPtrLoad(&prev->upper[i].next);
                    if (IS_MARK(temp_cur)) {
                        prev = CLEAR_MARK(temp_cur);        // 由此重试
                    }
                    // 2.prev和cur之间插入了新节点
                    else {
                        prev = temp_cur;
                    }

                    // 尝试删除失败，允许在当前层重新查找next为cur的prev
                    TsSkipListEntry* temp = prev;
                    int cmp;
                     release_assert(prev->key < cur->key, "refine key ordered error");
                    do {
                        temp = (TsSkipListEntry*)AtomicPtrLoad(&temp->upper[i].next);
                        cmp = TsSkipListLevelLocate(&prev, &temp, i, key);
                         release_assert(cmp <= 0, "refine key ordered error");
                    } while (temp != cur);      // 可能有相同的key被插入，所以需要继续向下
                    
                    // 这里与无锁有序链表采用不同的策略，不释放cur的标记，而是直接重试
                }  while (true);
                break;
            }
            // 标记失败的场景
            // 1.cur被其他删除线程标记
            if (IS_MARK(AtomicPtrLoad(&cur->upper[i].next))) {
                prev = CLEAR_MARK(AtomicPtrLoad(&cur->upper[i].next));

                // 删除抢占失败，没办法直接在最低层重新定位删除的节点(没有索引节点的update/update_next环境)，考虑到可能存在相同key的节点，不直接返回而是重新搜索
                goto _retry;
                // TsSkipListLevelLocate(&prev, &cur, 0, key);
            }
            // 2.next已被其他线程删除，尝试重新获取next
        }  while (true);
    }

    // 被删除的索引层可能是最高索引层，但不在这里调整
    //while (list->head[level-1].next == NULL && list->level == level) {
    //    // 只会出现删插冲突(空头节点不可能出现删除)
    //    
    //    if (AtomicInt32CompareExchange(&list->level, level-1, level)) {
    //        // 此时可能出现当前节点又插入了新节点，但CAS修改成功
    //        printf("%d -> %d\t", level + 1, level);
    //        if (list->head[level-1].next != NULL) {
    //            // 出现高度丢失，尝试修正
    //            if (!AtomicInt32CompareExchange(&list->level, level, level-1)) {
    //                // 修正失败就说明高度有变，直接结束
    //                break;
    //            }
    //            // 但是此时可能又出现节点被删除，开始重试
    //            --level;
    //            continue;
    //        }
    //        break;
    //    }
    //    --level;
    //}

    // ObjectRelease(cur);      // 资源释放交给GC
    return success;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_