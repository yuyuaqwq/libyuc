/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/array.h>
#include <CUtils/container/bplus_tree.h>

#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

const kPageInvalidId = -1;

/*
* 分配页面
*/
PageId PageAlloc(void* tree) {
    // child 应当是m个，所以最后多申请一个
    return (PageId)MemoryAlloc(sizeof(BPlusEntry) - sizeof(BPlusLeafListEntry) + (((BPlusTree*)tree)->m - 1) * sizeof(BPlusIndexBlockInfo) + sizeof(struct _BPlusIndexEntry*));
}

/*
* 释放分配的页面
* 如果引用计数未清零则挂到待释放链表中等待引用计数清空
*/
void PageFree(void* tree, PageId id) {
    return MemoryFree((void*)id);
}

/*
* 获取页面缓冲区，递增页面引用计数
*/
void* PageGet(void* tree, PageId id) {
    return (BPlusEntry*)id;
}

/*
* 解除对指定页面的引用
*/
void PageDereference(void* tree, PageId id) {
    return;
}

/*
* 标记指定页面为脏页
*/
void PageMarkDirty(void* tree, PageId id) {
    return;
}


inline void* GetLeafElementKey(BPlusTree* tree, const BPlusEntry* leaf, int i, size_t* key_size) {
    *key_size = sizeof(Key);
    return &leaf->leafElement[i].key;
}

inline void SetLeafElementKey(BPlusTree* tree, BPlusEntry* leaf, int i, void* key, size_t key_size) {
    leaf->leafElement[i].key = *(Key*)key;
}

inline void SwapLeafElementKey(BPlusTree* tree, BPlusEntry* leaf, int i, int j) {
    size_t size1, size2;
    MemorySwap(GetLeafElementKey(tree, leaf, i, &size1), GetLeafElementKey(tree, leaf, j, &size2), sizeof(Key));
}

inline void CopyLeafElement(BPlusTree* tree, BPlusEntry* leaf, int dst, int src) {

}

inline PageId GetIndexElementChildId(BPlusTree* tree, const BPlusEntry* index, int i) {
    return index->indexElement[i].leftChildId;
}

inline void SetIndexElementChildId(BPlusTree* tree, BPlusEntry* index, int i, PageId id) {
    index->indexElement[i].leftChildId = id;
}

inline void* GetIndexElementKey(BPlusTree* tree, const BPlusEntry* index, int i, size_t* key_size) {
    *key_size = sizeof(Key);
    return &index->indexElement[i].key;
}

inline void SetIndexElementKey(BPlusTree* tree, BPlusEntry* index, int i, void* key, size_t key_size) {
    index->indexElement[i].key = *(Key*)key;
}

inline void SwapIndexElementKey(BPlusTree* tree, BPlusEntry* index, int i, int j) {
    size_t size1, size2;
    MemorySwap(GetIndexElementKey(tree, index, i, &size1), GetIndexElementKey(tree, index, j, &size2), sizeof(Key));
}

inline void CopyIndexElement(BPlusTree* tree, BPlusEntry* index, int dst, int src) {

}

#endif // CUTILS_CONTAINER_BPLUS_TREE_DISK


/*
* 二分查找
*/
static int BPlusEntryBinarySearch(BPlusTree* tree, const BPlusEntry* entry, int first, int last, void* key, size_t key_size, CmpFunc2 cmpFunc) {
    int mid;
    while (first <= last) {
        mid = first + (last - first) / 2;
        void* cur_key;
        size_t cur_key_size;
        if (entry->type == kBPlusEntryLeaf) {
            cur_key = GetLeafElementKey(tree, entry, mid, &cur_key_size);
        }
        else {
            cur_key = GetIndexElementKey(tree, entry, mid, &cur_key_size);
        }
        int res = cmpFunc(cur_key, cur_key_size, key, key_size);
        if (res > 0) {
            last = mid - 1;
        }
        else if (res < 0) {
            first = mid + 1;
        }
        else {
            return mid;
        }
    }
    return -1;
}

static int BPlusEntryBinarySearch_Range(BPlusTree* tree, const BPlusEntry* entry, int first, int last, void* key, size_t key_size, CmpFunc2 cmpFunc) {
    int mid = 0;
    while (first < last) {
        mid = first + (last - first) / 2;
        void* cur_key;
        size_t cur_key_size;
        if (entry->type == kBPlusEntryLeaf) {
            cur_key = GetLeafElementKey(tree, entry, mid, &cur_key_size);
        }
        else {
            cur_key = GetIndexElementKey(tree, entry, mid, &cur_key_size);
        }
        int res = cmpFunc(cur_key, cur_key_size, key, key_size);
        if (res < 0) first = mid + 1;
        else last = mid;
    }
    return first;
}


/*
* 双向链表
*/
static void LeafListEntryInit(BPlusTree* tree, PageId entryId) {
    BPlusEntry* first = PageGet(tree, entryId);
    first->leafListEntry.next = entryId;
    first->leafListEntry.prev = entryId;
}

static void LeafListEntryInsertTail(BPlusTree* tree, PageId leftId, PageId rightId) {
    BPlusEntry* left = PageGet(tree, leftId);
    BPlusEntry* right = PageGet(tree, rightId);

    PageId temp = left->leafListEntry.next;
    left->leafListEntry.next = rightId;
    right->leafListEntry.next = temp;
    right->leafListEntry.prev = leftId;
    ((BPlusEntry*)PageGet(tree, temp))->leafListEntry.prev = rightId;
}

static void LeafListEntryRemoveTail(BPlusTree* tree, PageId entryId) {
    BPlusEntry* entry = PageGet(tree, entryId);
    BPlusEntry* next = PageGet(tree, entry->leafListEntry.next);
    entry->leafListEntry.next = next->leafListEntry.next;
    ((BPlusEntry*)PageGet(tree, next->leafListEntry.next))->leafListEntry.prev = entryId;
}



static PageId BPlusCreateIndexEntry(BPlusTree* tree) {
    PageId entryId = PageAlloc(tree);
    BPlusEntry* entry = PageGet(tree, entryId);
    entry->type = kBPlusEntryIndex;
    entry->parentId = kPageInvalidId;
    for (int i = 0; i < tree->m; i++) {
        SetIndexElementChildId(tree, entry, i, NULL);
    }
    entry->count = 0;
    PageDereference(tree, entryId);
    return entryId;
}

static PageId BPlusCreateLeafEntry(BPlusTree* tree) {
    PageId entryId = PageAlloc(tree);
    BPlusEntry* entry = PageGet(tree, entryId);
    entry->type = kBPlusEntryLeaf;
    entry->parentId = kPageInvalidId;
    for (int i = 0; i < tree->m - 1; i++) {

    }
    entry->count = 0;
    PageDereference(tree, entryId);
    return entryId;
}

/*
* 源代码中没有明确标准的索引指的是key索引
*
* 通常情况下，父节点元素是左父索引，子节点是右子节点
* 左父索引从-1开始
* 即默认是key[0] - child[1]、key[1] - child[2]...
*         3     |    6    |    8
*      /        \         \        \
*   1|2         4|5        7        10|11
* 如上面画的示例，默认是3对应4|5，6对应7、8对应10|11
*/

/*
* 获取左兄弟节点，没有则返回NULL
*/
static PageId BPlusGetLeftSiblingEntry(BPlusTree* tree, const BPlusEntry* entry, int leftParentIndex) {
    PageId parentId = entry->parentId;
    if (parentId == kPageInvalidId) {
        return kPageInvalidId;
    }
    BPlusEntry* parent = PageGet(tree, parentId);
    PageId siblingId = GetIndexElementChildId(tree, parent, leftParentIndex);
    PageDereference(tree, parentId);
    if (leftParentIndex > -1) {
        return siblingId;
    }
    return kPageInvalidId;
}

/*
* 获取右兄弟节点，没有则返回NULL
*/
static PageId BPlusGetRightSiblingEntry(BPlusTree* tree, const BPlusEntry* entryId, int leftParentIndex) {
    PageId parentId = entryId->parentId;
    if (parentId == kPageInvalidId) {
        return kPageInvalidId;
    }
    BPlusEntry* parent = PageGet(tree, parentId);
    int parentCount = parent->count;
    PageId siblingId = GetIndexElementChildId(tree, parent, leftParentIndex + 2);
    PageDereference(tree, parentId);
    if (leftParentIndex < parentCount) {
        return siblingId;
    }
    return kPageInvalidId;
}

/*
* 向元素数组指定位置插入元素(是索引节点时)
*/
static void BPlusInsertElement(BPlusTree* tree, BPlusEntry* entry, PageId entryId, int keyIndex, void* key, size_t key_size, BPlusEntry* child, PageId childId, bool rightChild) {
    if (entry->type == kBPlusEntryLeaf) {
        BPlusEntry* leaf = entry;
        for (int j = leaf->count - 1; j >= keyIndex; j--) {
            size_t size_;
            void* key_ = GetLeafElementKey(tree, leaf, j, &size_);
            SetLeafElementKey(tree, leaf, j + 1, key_, size_);
        }
        SetLeafElementKey(tree, leaf, keyIndex, key, key_size);
        leaf->count++;
    }
    else {
        BPlusEntry* index = entry;
        for (int j = index->count - 1; j >= keyIndex; j--) {
            size_t size_;
            void* key_ = GetIndexElementKey(tree, index, j, &size_);
            SetIndexElementKey(tree, index, j + 1, key_, size_);
            SetIndexElementChildId(tree, index, j + 2, GetIndexElementChildId(tree, index, j + 1));
        }
        SetIndexElementKey(tree, index, keyIndex, key, key_size);
        child->parentId = entryId;
        if (!rightChild) {
            // 不是右子节点，多挪一个，并 keyIndex-- 使其指向右子节点
            SetIndexElementChildId(tree, index, keyIndex + 1, GetIndexElementChildId(tree, index, keyIndex));
            keyIndex--;
        }
        SetIndexElementChildId(tree, index, keyIndex + 1, childId);
        index->count++;
    }
}

/*
* 从元素数组中删除指定索引的元素
* 返回被删除元素的子节点
*/
static PageId BPlusDeleteElement(BPlusTree* tree, BPlusEntry* entry, PageId entryId, int keyIndex, void** key, size_t* key_size, bool rightChild) {
    if (entry->type == kBPlusEntryLeaf) {
        BPlusEntry* leaf = entry;
        for (int i = keyIndex + 1; i < leaf->count; i++) {
            if (key) {
                SwapLeafElementKey(tree, leaf, i - 1, i);        // 该key暂时不能被覆盖
            }
            else {
                size_t size_;
                void* key_ = GetLeafElementKey(tree, leaf, i, &size_);
                SetLeafElementKey(tree, leaf, i - 1, key_, size_);
            }
        }
        leaf->count--;
        if (key) {
            *key = GetLeafElementKey(tree, leaf, leaf->count, key_size);
        }
        return kPageInvalidId;
    }
    else {
        BPlusEntry* index = entry;
        PageId deleteEntryId;
        if (rightChild) {
            deleteEntryId = GetIndexElementChildId(tree, index, keyIndex + 1);
            for (int i = keyIndex + 1; i < index->count; i++) {
                if (key) {
                    SwapIndexElementKey(tree, index, i - 1, i);        // 该key暂时不能被覆盖
                }
                else {
                    size_t size_;
                    void* key_ = GetIndexElementKey(tree, index, i, &size_);
                    SetIndexElementKey(tree, index, i - 1, key_, size_);
                }
                SetIndexElementChildId(tree, index, i, GetIndexElementChildId(tree, index, i + 1));
            }
        }
        else {
            deleteEntryId = GetIndexElementChildId(tree, index, keyIndex);
            for (int i = keyIndex + 1; i < index->count; i++) {
                if (key) {
                    SwapIndexElementKey(tree, index, i - 1, i);        // 该key暂时不能被覆盖
                }
                else {
                    size_t size_;
                    void* key_ = GetIndexElementKey(tree, index, i, &size_);
                    SetIndexElementKey(tree, index, i - 1, key_, size_);
                }
                SetIndexElementChildId(tree, index, i - 1, GetIndexElementChildId(tree, index, i));
            }
            SetIndexElementChildId(tree, index, index->count - 1, GetIndexElementChildId(tree, index, index->count));
        }
        index->count--;
        if (key) {
            *key = GetIndexElementKey(tree, index, index->count, key_size);
        }
        return deleteEntryId;
    }
}



/*
* 分裂节点
* 返回分裂后右侧新节点PageId，key会返回上升的key
*
* 同一个叶节点多次分裂会导致重复的key上升吗？
* 如果叶节点不存在相同的key，那么是不会的
* 因为分裂后选择右节点的最左元素的key作为上升的key
* 这个时候无论怎么插入元素都不会插入到该节点最左元素的左侧(比它小的会被分到左侧节点，因为父元素key等于该元素)，该节点再分裂也就不存在最左元素再次上升的可能了
*/
static PageId BPlusSplitEntry(BPlusTree* tree, BPlusEntry* entry, PageId entryId, int insertIndex, void** key_midKey, size_t* key_size_or_mid_key_size, BPlusEntry* rightChild, PageId rightChildId, BPlusEntry** retRight) {
    PageId rightId;
    BPlusEntry* right;
    int rightCount;
    if (entry->type == kBPlusEntryLeaf) {
        rightId = BPlusCreateLeafEntry(tree);
        right = PageGet(tree, rightId);
        LeafListEntryInsertTail(tree, entryId, rightId);
        // 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
        // 就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧
        int mid = tree->m / 2;
        rightCount = entry->count + 1 - mid;        // +1是因为这个时候entry->count并没有把未插入元素也算上
        int i = rightCount - 1, j = entry->count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                entry->count++;
                SetLeafElementKey(tree, right, i, *key_midKey, *key_size_or_mid_key_size);
                j++;        // j不动
                insert = true;
                continue;
            }
            size_t size_;
            void* key_ = GetLeafElementKey(tree, entry, j, &size_);
            SetLeafElementKey(tree, right, i, key_, size_);
        }

        entry->count -= rightCount;

        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tree, entry, entryId, insertIndex, *key_midKey, *key_size_or_mid_key_size, rightChild, rightChildId, true);
        }

        // 从mid拿到上升元素
        *key_midKey = GetLeafElementKey(tree, right, 0, key_size_or_mid_key_size);
    }
    else {
        rightId = BPlusCreateIndexEntry(tree);
        right = PageGet(tree, rightId);
        // 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了

        // 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
        //  1 2       3 4
        // / |  |      |  \    
        // 此时新节点缺少了一条链接，我们最终选用旧节点的末尾元素作为上升元素，故旧节点的左侧的末尾元素的右子树就可以挂接到新节点上

        int mid = (tree->m - 1) / 2;
        rightCount = entry->count - mid;        // 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入
        int i = rightCount - 1, j = entry->count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                entry->count++;
                SetIndexElementKey(tree, right, i, *key_midKey, *key_size_or_mid_key_size);
                SetIndexElementChildId(tree, right, i + 1, rightChildId);
                rightChild->parentId = rightId;
                j++;        // j不动
                insert = true;
                continue;
            }
            size_t size_;
            void* key_ = GetIndexElementKey(tree, entry, j, &size_);
            SetIndexElementKey(tree, right, i, key_, size_);
            SetIndexElementChildId(tree, right, i + 1, GetIndexElementChildId(tree, entry, j + 1));
            BPlusEntry* temp = PageGet(tree, GetIndexElementChildId(tree, right, i + 1));
            temp->parentId = rightId;
        }
        entry->count -= rightCount;

        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tree, entry, entryId, insertIndex, *key_midKey, *key_size_or_mid_key_size, rightChild, rightChildId, true);
        }

        // 最后从entry->indexData末尾拿到上升元素，将其摘除
        entry->count--;
        *key_midKey = GetIndexElementKey(tree, entry, entry->count, key_size_or_mid_key_size);

        // entry最右的链接交给newEntry(right)
        SetIndexElementChildId(tree, right, 0, GetIndexElementChildId(tree, entry, entry->count + 1));
        BPlusEntry* head = PageGet(tree, GetIndexElementChildId(tree, right, 0));
        head->parentId = rightId;
    }
    right->count = rightCount;
    if (retRight) {
        *retRight = right;
    }
    else {
        PageDereference(tree, rightId);
    }
    return rightId;
}

/*
* 合并节点
* 释放右节点，返回时右节点不再有效
*   2
* 1   3
*/
static void BPlusMergeEntry(BPlusTree* tree, BPlusEntry* left, PageId leftId, BPlusEntry* right, PageId rightId, int commonParentIndex) {
    if (left->type == kBPlusEntryLeaf) {
        LeafListEntryRemoveTail(tree, leftId);
        // 是叶子节点，将right并入left中，并删除right的父索引元素
        for (int i = 0; i < right->count; i++) {
            size_t size_;
            void* key_ = GetLeafElementKey(tree, right, i, &size_);
            SetLeafElementKey(tree, left, left->count++, key_, size_);
        }
    }
    else {
        // 是索引节点，将即将被删除的父索引元素(子节点丢弃，因为父索引子节点就指向左和右)和right都并入到left中，删除right的父索引元素
        size_t size_;
        void* key_ = GetIndexElementKey(tree, PageGet(tree, right->parentId), commonParentIndex, &size_);
        SetIndexElementKey(tree, left, left->count++, key_, size_);
        // 右节点搬迁后会多出一个头部子节点，因为父节点不需要带子节点下降，所以可以直接插入到左节点尾部
        PageId rightChildId = GetIndexElementChildId(tree, right, 0);
        SetIndexElementChildId(tree, left, left->count, rightChildId);
        BPlusEntry* rightChild = PageGet(tree, rightChildId);
        rightChild->parentId = leftId;
        for (int i = 0; i < right->count; i++) {
            size_t size_;
            void* key_ = GetIndexElementKey(tree, right, i, &size_);
            SetIndexElementKey(tree, left, left->count++, key_, size_);
            rightChildId = GetIndexElementChildId(tree, right, i + 1);
            SetIndexElementChildId(tree, left, left->count, rightChildId);
            rightChild = PageGet(tree, rightChildId);
            rightChild->parentId = leftId;
        }
    }
    PageDereference(tree, rightId);
    PageFree(tree, rightId);
    // 合并部分完成，删除部分交给调用者
}


/*
* 向节点插入块
*/
static bool BPlusInsertEntry(BPlusTree* tree, PageId entryId, void* key, size_t key_size, PageId rightChildId, void* value, size_t value_size) {
    PageId rightId;
    BPlusEntry* entry = PageGet(tree, entryId);
    PageId parentId = entry->parentId;
    BPlusEntry* rightChild = PageGet(tree, rightChildId);
    bool success = true, insertUp = false;
    do {
        if (entry->count == 0) {
            BPlusInsertElement(tree, entry, entryId, 0, key, key_size, rightChild, rightChildId, true);
            break;
        }
        int insertIndex = BPlusEntryBinarySearch_Range(tree, entry, 0, entry->count - 1, key, key_size, tree->cmpFunc);
        size_t size_;
        void* key_;
        if (entry->type == kBPlusEntryLeaf) {
            key_ = GetLeafElementKey(tree, entry, insertIndex, &size_);
        }
        else {
            key_ = GetIndexElementKey(tree, entry, insertIndex, &size_);
        }
        int res = tree->cmpFunc(key_, size_, key, key_size);
        if (res < 0) {
            insertIndex++;        // 如果所有节点都小于key，那就追加到最尾部
        }
        if (entry->count < tree->m - 1) {
            // 有空余的位置插入
            BPlusInsertElement(tree, entry, entryId, insertIndex, key, key_size, rightChild, rightChildId, true);
            break;
        }

        // 没有多余位置，需要分裂向上插入
        BPlusEntry* right;
        rightId = BPlusSplitEntry(tree, entry, entryId, insertIndex, &key, &key_size, rightChild, rightChildId, &right);
        // 分裂出的元素向上传递
        if (parentId == kPageInvalidId) {
            PageId newIndexId = BPlusCreateIndexEntry(tree);
            entry->parentId = newIndexId;
            right->parentId = newIndexId;
            BPlusEntry* newIndex = PageGet(tree, newIndexId);
            SetIndexElementChildId(tree, newIndex, 0, entryId);
            BPlusInsertElement(tree, newIndex, newIndexId, 0, key, key_size, right, rightId, true);
            tree->rootId = newIndexId;
            PageDereference(tree, newIndexId);
        }
        else {
            insertUp = true;
        }
        PageDereference(tree, rightId);
    } while (false);
    PageDereference(tree, rightChildId);
    PageDereference(tree, entryId);
    if (insertUp) {
        return BPlusInsertEntry(tree, parentId, key, key_size, rightId, value, value_size);
    }
    else {
        return success;
    }
}

/*
* 删除指定节点的块
*/
static bool BPlusDeleteEntry(BPlusTree* tree, PageId entryId, int deleteIndex, Array* stack) {
    BPlusEntry* entry = PageGet(tree, entryId);
    PageId siblingId = 0;
    PageId parentId = entry->parentId;
    int leftParentIndex;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, deleteUp = false;
    BPlusDeleteElement(tree, entry, entryId, deleteIndex, NULL, NULL, true);       // 直接删除，不考虑返回的子节点，因为索引节点的删除是从底下传递上来的，底下已经处理好了被删除元素的子节点关系了，而叶子节点又没有子节点。
    do {
        if (entry->count >= (tree->m - 1) / 2) {
            break;
        }
        if (parentId == kPageInvalidId) {
            // 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
            if (entry->type == kBPlusEntryIndex && entry->count == 0) {
                PageId childId = GetIndexElementChildId(tree, entry, 0);
                BPlusEntry* child = PageGet(tree, childId);
                child->parentId = kPageInvalidId;
                tree->rootId = childId;
                PageDereference(tree, childId);
                PageDereference(tree, entryId);
                PageFree(tree, entryId);
                return true;
            }
            else {
                break;
            }
        }
        leftParentIndex = *(int*)ArrayPopTail(stack);
        siblingId = BPlusGetLeftSiblingEntry(tree, entry, leftParentIndex);
        bool leftSibling = true;
        if (siblingId == kPageInvalidId) {
            siblingId = BPlusGetRightSiblingEntry(tree, entry, leftParentIndex);
            leftSibling = false;
        }
        sibling = PageGet(tree, siblingId);
        parent = PageGet(tree, parentId);
        if (sibling->count > (tree->m - 1) / 2) {
            // 向兄弟借节点
            void* newKey;
            size_t new_key_size;
            if (entry->type == kBPlusEntryLeaf) {
                // 叶子节点处理较简单，可以直接移动
                if (leftSibling) {
                    // 从左兄弟节点的末尾的元素插入到当前节点的头部并更新父元素key为借来的key
                    PageId childId = BPlusDeleteElement(tree, sibling, siblingId, sibling->count - 1, &newKey, &new_key_size, true);
                    BPlusEntry* child = PageGet(tree, childId);
                    BPlusInsertElement(tree, entry, entryId, 0, newKey, new_key_size, child, childId, true);
                    PageDereference(tree, childId);
                }
                else {
                    // 从右兄弟节点的头部的元素插入到当前节点的尾部并更新父元素key为右兄弟的新首元素
                    PageId childId = BPlusDeleteElement(tree, sibling, siblingId, 0, &newKey, &new_key_size, true);
                    BPlusEntry* child = PageGet(tree, childId);
                    BPlusInsertElement(tree, entry, entryId, entry->count, newKey, new_key_size, child, childId, true);
                    PageDereference(tree, childId);
                    newKey = GetLeafElementKey(tree, sibling, 0, &new_key_size);         // 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的
                    ++leftParentIndex;        // 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
                }
            }
            else {
                // 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换)
                if (leftSibling) {
                    // 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧
                    PageId rightChildId = BPlusDeleteElement(tree, sibling, siblingId, sibling->count - 1, &newKey, &new_key_size, true);
                    BPlusEntry* rightChild = PageGet(tree, rightChildId);
                    size_t size_;
                    void* key_ = GetIndexElementKey(tree, parent, leftParentIndex, &size_);
                    BPlusInsertElement(tree, entry, entryId, 0, key_, size_, rightChild, rightChildId, false);
                    PageDereference(tree, rightChildId);
                }
                else {
                    // 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧
                    PageId leftChildId = BPlusDeleteElement(tree, sibling, siblingId, 0, &newKey, &new_key_size, false);
                    leftParentIndex++;        // 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
                    BPlusEntry* leftChild = PageGet(tree, leftChildId);
                    size_t size_;
                    void* key_ = GetIndexElementKey(tree, parent, leftParentIndex, &size_);
                    BPlusInsertElement(tree, entry, entryId, entry->count, key_, size_, leftChild, leftChildId, true);
                    PageDereference(tree, leftChildId);
                }
            }
            SetIndexElementKey(tree, parent, leftParentIndex, newKey, new_key_size);
            break;
        }

        // 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点合并也足够下降一个父元素)
        BPlusEntry* left, * right;
        PageId leftId, rightId;
        if (leftSibling) {
            left = sibling;
            leftId = siblingId;
            right = entry;
            entry = NULL;
            rightId = entryId;
        }
        else {
            ++leftParentIndex;        // 要求共同父索引
            left = entry;
            leftId = entryId;
            right = sibling;
            sibling = NULL;
            rightId = siblingId;
        }
        BPlusMergeEntry(tree, left, leftId, right, rightId, leftParentIndex);
        deleteUp = true;
    } while (false);

    if (parent) {
        PageDereference(tree, parentId);
    }
    if (sibling) {
        PageDereference(tree, siblingId);
    }
    if (entry) {
        PageDereference(tree, entryId);
    }
    if (deleteUp) {
        return BPlusDeleteEntry(tree, parentId, leftParentIndex, stack);     // 尾递归
    }
    return success;
}

/*
* 根据key查找到指定的叶子节点
* stack返回每一层节点的父节点的块(左父)索引
*/
static PageId BPlusTreeFindLeaf(BPlusTree* tree, void* key, size_t key_size, BPlusEntry** leaf, Array* stack) {
    PageId id = tree->rootId;
    BPlusEntry* cur = PageGet(tree, id);
    while (cur && cur->type == kBPlusEntryIndex) {
        //       4        8         12
        //    /       |        |       \
        // 3  4      6  7      10        15
        int i = BPlusEntryBinarySearch_Range(tree, cur, 0, cur->count - 1, key, key_size, tree->cmpFunc);
        size_t size_;
        void* key_ = GetIndexElementKey(tree, cur, i, &size_);
        int res = tree->cmpFunc(key, key_size, key_, size_);
        if (res >= 0) {
            // 查找key较大，向右找，cur是i的右子节点，i是cur的左父索引
            // 相等的key也向右边找，因为分裂的索引是右叶子的第一个节点上升的
            ++i;
        }
        else {} // 查找key较小，向左找，cur是i的左子节点，i是cur的右父索引
        PageId oldId = id;
        id = GetIndexElementChildId(tree, cur, i);
        PageDereference(tree, oldId);
        if (stack) {
            --i;
            ArrayPushTail(stack, &i);
        }
        cur = PageGet(tree, id);
    }
    if (leaf) {
        *leaf = cur;
    }
    else {
        PageDereference(tree, id);
    }
    return id;
}


/*
* 初始化B+树
*/
void BPlusTreeInit(BPlusTree* tree, int m, CmpFunc cmpFunc) {
    if (m < 3) {
        m = 3;      // 最少3阶，否则索引节点分裂会出现一侧没有节点的情况
    }
    tree->m = m;
    tree->rootId = BPlusCreateLeafEntry(tree);
    tree->leafListFirst = tree->rootId;
    LeafListEntryInit(tree, tree->leafListFirst);
    if (cmpFunc == NULL) {
        cmpFunc = MemoryCmpR2;
    }
    tree->cmpFunc = cmpFunc;
}

/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(BPlusTree* tree, void* key, size_t key_size) {
    BPlusEntry* leaf;
    PageId leafId = BPlusTreeFindLeaf(tree, key, key_size, &leaf, NULL);
    int index = BPlusEntryBinarySearch(tree, leaf, 0, leaf->count - 1, key, key_size, tree->cmpFunc);
    PageDereference(tree, leafId);
    if (index == -1) {
        return false;
    }
    return true;
}

/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(BPlusTree* tree, void* key, size_t key_size, void* value, size_t value_size) {
    PageId leafId = BPlusTreeFindLeaf(tree, key, key_size, NULL, NULL);
    bool success = BPlusInsertEntry(tree, leafId, key, key_size, NULL, value, value_size);
    return success;
}

/*
* 从B+树中删除指定key
*/
bool BPlusTreeDelete(BPlusTree* tree, void* key, size_t key_size) {
    Array stack;
    ArrayInit(&stack, 16, sizeof(int));
    /*stack.capacity = 16;
    int stackBuf[16] = { 0 };
    stack.objArr = stackBuf;*/
    BPlusEntry* leaf;
    PageId leafId = BPlusTreeFindLeaf(tree, key, key_size, &leaf, &stack);
    int deleteIndex = BPlusEntryBinarySearch(tree, leaf, 0, leaf->count - 1, key, key_size, tree->cmpFunc);
    PageDereference(tree, leafId);
    if (deleteIndex == -1) {
        return false;
    }
    bool success = BPlusDeleteEntry(tree, leafId, deleteIndex, &stack);
    return success;
}