/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_SKIP_LIST_H_
#define LIBYUC_CONTAINER_SKIP_LIST_H_

#include <libyuc/basic.h>


#define LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL 32
const int LIBYUC_CONTAINER_SKIP_LIST_SKIPLIST_P = (RAND_MAX / 2);

#define element_type int64_t
#define key_type element_type

/*
* 2:    5    
* 1:    5     8                         30
* 0:    5     8     11     14     30
*/


/*
* 跳表索引节点
*/
typedef struct _SkipListLevel {
    struct _SkipListEntry* next;        // 指向的同索引层的下一个SkipListEntry(不是直接指向逻辑上的同层链表)
} SkipListLevel;

/*
* 跳表节点，每个节点有随机的索引层数，所以用柔性数组动态分配
*/
typedef struct _SkipListEntry {
    element_type element;
    //int level_count;
    SkipListLevel upper[];        // 节点的上层，是索引节点
} SkipListEntry;

typedef struct _SkipList {
    SkipListLevel head[LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL];
    int level_count;
} SkipList;


void SkipListInit(SkipList* list);
void SkipListRelease(SkipList* list);
bool SkipListFind(SkipList* list, key_type* key);
bool SkipListPut(SkipList* list, element_type* element);
bool SkipListDelete(SkipList* list, key_type* key);
SkipListEntry* SkipListFirst(SkipList* list);
SkipListEntry* SkipListNext(SkipList* list, SkipListEntry* cur);



static int RandomLevel() {
    int level = 1;

    while (LIBYUC_CONTAINER_SKIP_LIST_SKIPLIST_P <= rand() && level < LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL)
        level++;

    return level;
}

static SkipListEntry* SkipListCreateEntry(int level_count, element_type* element) {
    SkipListEntry* entry = (SkipListEntry*)MemoryAlloc(sizeof(SkipListEntry) + level_count * sizeof(SkipListLevel));
    entry->element = *element;
    //entry->level_count = level_count;
    return entry;
}

static forceinline SkipListEntry* SkipListLocate(SkipList* list, key_type* key, key_type* cmp, SkipListEntry** prev_level) {
    *cmp = 0;
    SkipListLevel* cur = list->head;
    // 从最顶层开始遍历，每趟循环都相当于下降一层索引
    for (int i = list->level_count - 1; i >= 0; i--) {
        // 当前索引层的链表查找
        SkipListEntry* next = cur[i].next;
        while (next) {
            *cmp = next->element - *key;
            if (*cmp >= 0) {
                break;
            }
            cur = next->upper;     // 转到当前层的下一链表节点
            next = cur[i].next;
        }
        if (prev_level) {
            prev_level[i] = ObjectGetFromField(cur, SkipListEntry, upper);;        // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
        }
        // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
    }
    return ObjectGetFromField(cur, SkipListEntry, upper);
}

void SkipListInit(SkipList* list) {
    list->level_count = 1;
    for (int i = 0; i < LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL; i++) {
        list->head[i].next = NULL;
    }
}

void SkipListRelease(SkipList* list) {
    SkipListEntry* cur = SkipListFirst(list);
    while (cur) {
        SkipListEntry* next = SkipListNext(list, cur);
        ObjectRelease(cur);
        cur = next;
    }
}

bool SkipListFind(SkipList* list, key_type* key) {
    key_type cmp;
    SkipListEntry* prev = SkipListLocate(list, key, &cmp, NULL);

    // 查找出来，要么是所有索引层都找不到，要么是cur的key>=查找的key
    if (prev->upper[0].next && cmp == 0) {
        return true;
    }
    return false;
}

bool SkipListPut(SkipList* list, element_type* element) {
    SkipListEntry* prev_level[LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL];        // 对应每一层需要更新索引的节点，因为新节点可能会插入索引

    key_type* key = element;

    key_type cmp;
    SkipListEntry* prev = SkipListLocate(list, key, &cmp, prev_level);

    // cur此时的next要么指向NULL，要么>=key
    if (prev->upper[0].next && cmp == 0) {
        // value
        return true;
    }

    // 准备创建随机高度索引层的节点
    int level = RandomLevel();
    if (level > list->level_count) {
        // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
        for (int i = list->level_count; i < level; i++) {
            prev_level[i] = ObjectGetFromField(list->head, SkipListEntry, upper);
        }
        list->level_count = level;
    }

    // 创建节点
    SkipListEntry* new_entry = SkipListCreateEntry(level, element);
    for (int i = 0; i < level; i++) {
        // 更新各层的Level数组，即连接到同层索引链表中
        new_entry->upper[i].next = prev_level[i]->upper[i].next;
        prev_level[i]->upper[i].next = new_entry;        // 直接指向新节点，而不是各层相互连接
    }

    return true;
}

bool SkipListDelete(SkipList* list, key_type* key) {
    SkipListEntry* prev_level[LIBYUC_CONTAINER_SKIP_LIST_MAX_LEVEL];        // 对应每一层需要更新索引的节点，因为新节点可能会删除索引

    key_type cmp;
    SkipListEntry* prev = SkipListLocate(list, key, &cmp, prev_level);
    SkipListEntry* cur = prev->upper[0].next;

    if (!cur || cmp) {
        // 找不到该节点
        return false;
    }

    // 最底层索引开始向上更新
    for (int i = 0; i < list->level_count; i++) {
        if (prev_level[i]->upper[i].next != cur) {        // 该层的索引不指向被删除的节点，停止更新
            break;
        }
        prev_level[i]->upper[i].next = cur->upper[i].next;        // 删除索引，即指向被删除索引的下一索引
    }

    // 被删除的索引层可能是最高索引层，需要调整
    while (list->level_count > 1 && list->head[list->level_count - 1].next == NULL) {
        list->level_count--;
    }

    ObjectRelease(cur);
    return true;
}


SkipListEntry* SkipListFirst(SkipList* list) {
    return list->head[0].next;
}

SkipListEntry* SkipListNext(SkipList* list, SkipListEntry* cur) {
    return cur->upper[0].next;
}

#endif // LIBYUC_CONTAINER_STATIC_LIST_H_