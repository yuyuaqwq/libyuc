/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/hash_table.h>

typedef enum _HashEntryType {
    kFree,
    kObj,
    kList,
} HashEntryType;

static inline uint32_t GetIndex(HashTable* table, void* key) {
    return table->hashFunc(key, table->keyFieldSize) % table->bucket.capacity;
}

static inline int GetCurrentLoadFator(HashTable* table) {
    return table->bucket.count * 100 / table->bucket.capacity;
}

static void Resize(HashTable* table, size_t newCapacity) {
    HashTable temp;
    HashTableInit(&temp, newCapacity, table->loadFator, table->keyFieldOffset, table->keyFieldSize, table->hashFunc, table->cmpFunc);
    
    // 重映射
    
    // 优化点之一，遍历的时候可以顺带删除节点，可以节省一次查找
    HashTableIterator iter;
    void* obj = HashTableFirst(table, &iter);
    while (obj) {
        void* key = GetFieldByFieldOffset(obj, table->keyFieldOffset, void);
        HashTableDelete(table, key);
        HashTableInsert(&temp, obj);
        obj = HashTableNext(&iter);
    }

    ArrayRelease(&table->bucket);
    MemoryCopy(table, &temp, sizeof(temp));
}


void HashTableInit(HashTable* table, size_t capacity, uint32_t loadFator, int keyFieldOffset, int keySize, HashU32Func hashFunc, CmpFunc cmpFunc) {
    if (capacity == 0) {
        capacity = HASHTABLE_DEFAULT_BUCKETS_SIZE;
    }
    ArrayInit(&table->bucket, capacity, sizeof(HashEntry));
    // ArrayInit(&table->tempBucket, 0, sizeof(HashEntry));

    for (int i = 0; i < table->bucket.capacity; i++) {
        HashEntryInit(ArrayAt(&table->bucket, i, HashEntry));
    }
    if (loadFator == 0) {
        loadFator = HASHTABLE_DEFAULT_LOAD_FACTOR;
    }
    table->loadFator = loadFator;
    table->keyFieldOffset = keyFieldOffset;
    table->keyFieldSize = keySize;
    if (hashFunc == NULL) {
        hashFunc = Hashmap_jenkins_hash;
    }
    table->hashFunc = hashFunc;
    if (cmpFunc == NULL) {
        cmpFunc = MemoryCmp;
    }
    table->cmpFunc = cmpFunc;
}

void HashEntryInit(HashEntry* entry) {
    entry->type = kFree;
}

void HashTableRelease(HashTable* table, bool deleteObj) {
    HashTableIterator iter;
    void* obj = HashTableFirst(table, &iter);
    while (obj) {
        void* key = GetFieldByFieldOffset(obj, table->keyFieldOffset, void);
        HashTableDelete(table, key);
        if (deleteObj) {
            DeleteObject_(obj);
        }
        obj = HashTableNext(&iter);
    }
    ArrayRelease(&table->bucket);
}

size_t HashTableGetCount(HashTable* table) {
    return ArrayGetCount(&table->bucket);
}

size_t HashTableGetCapacity(HashTable* table) {
    return ArrayGetCapacity(&table->bucket);
}


void* HashTableFind(HashTable* table, void* key) {
    int index = GetIndex(table, key);
    HashEntry* entry = ArrayAt(&table->bucket, index, HashEntry);
    if (entry->type == kObj) {
        void* obj = entry->obj;
        int res = table->cmpFunc(GetFieldByFieldOffset(obj, table->keyFieldOffset, void), key, table->keyFieldSize);
        if (res == 0) {
            return obj;
        }
    }
    else if (entry->type == kList) {
        SinglyListEntry* cur = SinglyListFirst(&entry->listHead);
        while (cur) {
            void* obj = ((HashDataList*)cur)->obj;
            int res = table->cmpFunc(GetFieldByFieldOffset(obj, table->keyFieldOffset, void), key, table->keyFieldSize);
            if (res == 0) {
                return obj;
            }
            cur = SinglyListNext(cur);
        }
    }
    return NULL;
}

bool HashTableInsert(HashTable* table, void* obj) {
    void* key = GetFieldByFieldOffset(obj, table->keyFieldOffset, void);
    int index = GetIndex(table, key);
    HashEntry* entry = ArrayAt(&table->bucket, index, HashEntry);
    if (entry->type == kFree) {
        entry->obj = obj;
        entry->type = kObj;
    }
    else if (entry->type == kObj) {
        int res = table->cmpFunc(GetFieldByFieldOffset(entry->obj, table->keyFieldOffset, void), key, table->keyFieldSize);
        if (res == 0) {
            return obj == entry->obj;
        }
        entry->type = kList;
        void* oldObj = entry->obj;
        SinglyListHeadInit(&entry->listHead);
        HashDataList* listEntry = CreateObject(HashDataList);
        listEntry->obj = oldObj;
        SinglyListInsertHead(&entry->listHead, &listEntry->listEntry);
        listEntry = CreateObject(HashDataList);
        listEntry->obj = obj;
        SinglyListInsertHead(&entry->listHead, &listEntry->listEntry);
    }
    else if (entry->type == kList) {
        HashDataList* listEntry = CreateObject(HashDataList);
        listEntry->obj = obj;
        SinglyListInsertHead(&entry->listHead, &listEntry->listEntry);
    }
    table->bucket.count++;
    if (GetCurrentLoadFator(table) >= table->loadFator) {
        // 触发扩容
        Resize(table, table->bucket.capacity * HASHTABLE_DEFAULT_EXPANSION_FACTOR);
    }
    return true;
}

void* HashTableDelete(HashTable* table, void* key) {
    int index = GetIndex(table, key);
    HashEntry* entry = ArrayAt(&table->bucket, index, HashEntry);
    void* obj = NULL;
    if (entry->type == kFree) {
        return NULL;
    }
    else if (entry->type == kObj) {
        obj = entry->obj;
        int res = table->cmpFunc(GetFieldByFieldOffset(obj, table->keyFieldOffset, void), key, table->keyFieldSize);
        if (res != 0) {
            return NULL;
        }
        entry->type = kFree;
    }
    else if (entry->type == kList) {
        SinglyListEntry* prev = NULL;
        SinglyListEntry* cur = SinglyListFirst(&entry->listHead);
        while (cur) {
            void* curObj = ((HashDataList*)cur)->obj;
            int res = table->cmpFunc(GetFieldByFieldOffset(curObj, table->keyFieldOffset, void), key, table->keyFieldSize);
            if (res != 0) {
                prev = cur;
                cur = SinglyListNext(cur);
                continue;
            }
            obj = curObj;
            if (prev) {
                SinglyListRemoveEntry(prev, cur);
            }
            else {
                SinglyListRemoveHead(&entry->listHead);
                if (SinglyListIsEmpty(&entry->listHead)) {
                    entry->type = kObj;
                }
            }
            DeleteObject_(cur);
            break;
        }
    }
    table->bucket.count--;
    return obj;
}


/*
* 现在的迭代思路是遍历空间所有节点，另外可以用静态链表连接所有已映射的节点，但需要额外空间
*/
void* HashTableFirst(HashTable* table, HashTableIterator* iter) {
    iter->table = table;
    iter->curListEntry = NULL;
    iter->curIndex = 0;
#ifdef HASHTABLE_DATA_STATISTICS
    iter->objCount = 0;
    iter->freeCount = 0;
    iter->listEntryCount = 0;
    iter->listHeadCount = 0;
    iter->maxListCount = 0;
    iter->curListCount = 0;
#endif // HASHTABLE_DATA_STATISTICS
    return HashTableNext(iter);
}

void* HashTableNext(HashTableIterator* iter) {
    if (iter->curListEntry) {
        HashDataList* cur = iter->curListEntry;
        iter->curListEntry = (HashDataList*)iter->curListEntry->listEntry.next;
        if (iter->curListEntry == NULL) {
            iter->curIndex++;
        }
        if (cur) {
#ifdef HASHTABLE_DATA_STATISTICS
            iter->curListCount++;
            if (iter->curListCount > iter->maxListCount) {
                iter->maxListCount = iter->curListCount;
            }
            iter->listEntryCount++;
#endif // HASHTABLE_DATA_STATISTICS
            return cur->obj;
        }
    }
    HashTable* table = iter->table;
    for (; iter->curIndex < table->bucket.capacity; iter->curIndex++) {
        HashEntry* entry = ArrayAt(&table->bucket, iter->curIndex, HashEntry);
        if (entry->type == kFree) {
#ifdef HASHTABLE_DATA_STATISTICS
            iter->freeCount++;
#endif // HASHTABLE_DATA_STATISTICS
            continue;
        }
        if (entry->type == kObj) {
#ifdef HASHTABLE_DATA_STATISTICS
            iter->objCount++;
#endif // HASHTABLE_DATA_STATISTICS
            iter->curIndex++;
            return entry->obj;
        }
        if (entry->type == kList) {
#ifdef HASHTABLE_DATA_STATISTICS
            iter->curListCount = 0;
            iter->listHeadCount++;
#endif // HASHTABLE_DATA_STATISTICS
            iter->curListEntry = (HashDataList*)entry->listHead.next;
            return HashTableNext(iter);
        }
    }
    return NULL;
}