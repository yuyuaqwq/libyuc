/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/array.h>
#include <CUtils/container/bplus_tree.h>

typedef enum _BPlusEntryType {
    kIndex,
    kLeaf,
} BPlusEntryType;


#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

PageId PageAlloc(BPlusTree* tree) {
    // child 应当是m个，所以最后多申请一个
    return (PageId)MemoryAlloc(sizeof(BPlusEntry) - sizeof(BPlusLeafListEntry) + (tree->m - 1) * sizeof(BPlusIndexInternalEntry) + sizeof(struct _BPlusIndexEntry*));
}

void PageFree(BPlusTree* tree, PageId id) {
    return MemoryFree((void*)id);
}

struct _BPlusEntry* PageGet(BPlusTree* tree, PageId id) {
    return (BPlusEntry*)id;
}


inline void* GetLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i) {
    return &leaf->leafInternalEntry[i].key;
}

inline void SetLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i, void* key) {
    leaf->leafInternalEntry[i].key = *(Key*)key;
}

inline void SwapLeafInternalKey(BPlusTree* tree, BPlusEntry* leaf, int i, int j) {
    MemorySwap(GetLeafInternalKey(tree, leaf, i), GetLeafInternalKey(tree, leaf, j), sizeof(Key));
}

inline PageId GetIndexInternalChildId(BPlusTree* tree, BPlusEntry* index, int i) {
    return index->indexInternalEntry[i].childId;
}

inline void SetIndexInternalChildId(BPlusTree* tree, BPlusEntry* index, int i, PageId id) {
    index->indexInternalEntry[i].childId = id;
}

inline void* GetIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i) {
    return &index->indexInternalEntry[i].key;
}

inline void SetIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i, void* key) {
    index->indexInternalEntry[i].key = *(Key*)key;
}

inline void SwapIndexInternalKey(BPlusTree* tree, BPlusEntry* index, int i, int j) {
    MemorySwap(GetIndexInternalKey(tree, index, i), GetIndexInternalKey(tree, index, j), sizeof(Key));
}

#endif // CUTILS_CONTAINER_BPLUS_TREE_DISK



static int BPlusEntryBinarySearch(BPlusTree* tree, BPlusEntry* entry, int first, int last, void* key, int keySize, CmpFunc cmpFunc) {
    int mid;
    while (first <= last) {
        mid = first + (last - first) / 2;
        void* curKey;
        if (entry->type == kLeaf) {
            curKey = GetLeafInternalKey(tree, entry, mid);
        }
        else {
            curKey = GetIndexInternalKey(tree, entry, mid);
        }
        int res = cmpFunc(curKey, key, keySize);
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

static int BPlusEntryBinarySearch_Range(BPlusTree* tree, BPlusEntry* entry, int first, int last, void* key, int keySize, CmpFunc cmpFunc) {
    int mid = 0;
    while (first < last) {
        mid = first + (last - first) / 2;
        void* curKey;
        if (entry->type == kLeaf) {
            curKey = GetLeafInternalKey(tree, entry, mid);
        }
        else {
            curKey = GetIndexInternalKey(tree, entry, mid);
        }
        int res = cmpFunc(curKey, key, keySize);
        if (res < 0) first = mid + 1;
        else last = mid;
    }
    return first;
}



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
    PageGet(tree, temp)->leafListEntry.prev = rightId;
}

static void LeafListEntryRemoveTail(BPlusTree* tree, PageId entryId) {
    BPlusEntry* entry = PageGet(tree, entryId);
    BPlusEntry* next = PageGet(tree, entry->leafListEntry.next);
    entry->leafListEntry.next = next->leafListEntry.next;
    PageGet(tree, next->leafListEntry.next)->leafListEntry.prev = entryId;
}



static PageId BPlusCreateIndexEntry(BPlusTree* tree) {  
    PageId entryId = PageAlloc(tree);
    BPlusEntry* entry = PageGet(tree, entryId);
    entry->type = kIndex;
    entry->parentId = kBPlusInvalidPageId;
    entry->leftParentIndex = 0;
    for (int i = 0; i < tree->m; i++) {
        SetIndexInternalChildId(tree, entry, i, NULL);
    }
    entry->count = 0;
    return entryId;
}

static PageId BPlusCreateLeafEntry(BPlusTree* tree) {
    PageId entryId = PageAlloc(tree);
    BPlusEntry* entry = PageGet(tree, entryId);
    entry->type = kLeaf;
    entry->parentId = kBPlusInvalidPageId;
    entry->leftParentIndex = 0;
    for (int i = 0; i < tree->m - 1; i++) {
        
    }
    entry->count = 0;
    return entryId;
}

/*
* 源代码中没有明确标准的索引指的是key索引
* 
* 通常情况下，父节点内部节点是左父索引，子节点是右子节点
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
static PageId BPlusGetLeftSiblingEntry(BPlusTree* tree, PageId entryId, int leftParentIndex) {
    PageId parentId = PageGet(tree, entryId)->parentId;
    if (parentId == kBPlusInvalidPageId) {
        return kBPlusInvalidPageId;
    }
    BPlusEntry* parent = PageGet(tree, parentId);
    if (leftParentIndex > -1) {
        return GetIndexInternalChildId(tree, parent, leftParentIndex);
    }
    return kBPlusInvalidPageId;
}

/*
* 获取右兄弟节点，没有则返回NULL
*/
static PageId BPlusGetRightSiblingEntry(BPlusTree* tree, PageId entryId, int leftParentIndex) {
    PageId parentId = PageGet(tree, entryId)->parentId;
    if (parentId == kBPlusInvalidPageId) {
        return kBPlusInvalidPageId;
    }
    BPlusEntry* parent = PageGet(tree, parentId);
    if (leftParentIndex < parent->count) {
        return GetIndexInternalChildId(tree, parent, leftParentIndex + 2);
    }
    return kBPlusInvalidPageId;
}

/*
* 向内部节点数组指定位置插入key及子节点(是索引节点时)
*/
static void BPlusInsertInternalEntry(BPlusTree* tree, PageId entryId, int keyIndex, void* key, PageId childId, bool rightChild) {
    if (PageGet(tree, entryId)->type == kLeaf) {
        BPlusEntry* leaf = PageGet(tree, entryId);
        for (int j = leaf->count - 1; j >= keyIndex; j--) {
            SetLeafInternalKey(tree, leaf, j + 1, GetLeafInternalKey(tree, leaf, j));
        }
        SetLeafInternalKey(tree, leaf, keyIndex, key);
        leaf->count++;
    }
    else {
        BPlusEntry* index = PageGet(tree, entryId);
        for (int j = index->count - 1; j >= keyIndex; j--) {
            SetIndexInternalKey(tree, index, j + 1, GetIndexInternalKey(tree, index, j));
            PageId childId = GetIndexInternalChildId(tree, index, j + 1);
            BPlusEntry* child = PageGet(tree, childId);
            SetIndexInternalChildId(tree, index, j + 2, childId);
            child->leftParentIndex++;
        }
        SetIndexInternalKey(tree, index, keyIndex, key);
        BPlusEntry* child = PageGet(tree, childId);
        child->parentId = entryId;
        if (!rightChild) {
            // 不是右子节点，多挪一个，并 keyIndex-- 使其指向右子节点
            PageId childId = GetIndexInternalChildId(tree, index, keyIndex);
            SetIndexInternalChildId(tree, index, keyIndex + 1, childId);
            BPlusEntry* child = PageGet(tree, childId);
            child->leftParentIndex++;
            keyIndex--;
        }
        child->leftParentIndex = keyIndex;
        SetIndexInternalChildId(tree, index, keyIndex + 1, childId);
        index->count++;
    }
}

/*
* 从内部节点数组中删除指定索引的内部节点
* 返回被删除内部节点的子节点
*/
static PageId BPlusDeleteInternalEntry(BPlusTree* tree, PageId entryId, int keyIndex, void** key, bool rightChild) {
    BPlusEntry* entry = PageGet(tree, entryId);
    if (entry->type == kLeaf) {
        BPlusEntry* leaf = entry;
        for (int i = keyIndex + 1; i < leaf->count; i++) {
            if (key) {
                SwapLeafInternalKey(tree, leaf, i - 1, i);        // 该key暂时不能被覆盖
            } else {
                SetLeafInternalKey(tree, leaf, i - 1, GetLeafInternalKey(tree, leaf, i));
            }
        }
        leaf->count--;
        if (key) {
            *key = GetLeafInternalKey(tree, leaf, leaf->count);
        }
        return kBPlusInvalidPageId;
    }
    else {
        BPlusEntry* index = entry;
        
        PageId deleteEntryId;
        if (rightChild) {
            deleteEntryId = GetIndexInternalChildId(tree, index, keyIndex + 1);
            for (int i = keyIndex + 1; i < index->count; i++) {
                if (key) {
                    SwapIndexInternalKey(tree, index, i - 1, i);        // 该key暂时不能被覆盖
                } else {
                    SetIndexInternalKey(tree, index, i - 1, GetIndexInternalKey(tree, index, i));
                }
                PageId childId = GetIndexInternalChildId(tree, index, i + 1);
                BPlusEntry* child = PageGet(tree, childId);
                SetIndexInternalChildId(tree, index, i, childId);
                child->leftParentIndex--;
            }
        } else {
            deleteEntryId = GetIndexInternalChildId(tree, index, keyIndex);
            for (int i = keyIndex + 1; i < index->count; i++) {
                if (key) {
                    SwapIndexInternalKey(tree, index, i - 1, i);        // 该key暂时不能被覆盖
                } else {
                    SetIndexInternalKey(tree, index, i - 1, GetIndexInternalKey(tree, index, i));
                }
                PageId childId = GetIndexInternalChildId(tree, index, i);
                BPlusEntry* child = PageGet(tree, childId);
                SetIndexInternalChildId(tree, index, i - 1, childId);
                child->leftParentIndex--;
            }
            PageId childId = GetIndexInternalChildId(tree, index, index->count);
            BPlusEntry* child = PageGet(tree, childId);
            SetIndexInternalChildId(tree, index, index->count - 1, childId);
            child->leftParentIndex--;
        }
        index->count--;
        if (key) {
            *key = GetIndexInternalKey(tree, index, index->count);
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
* 因为分裂后选择右节点的最左内部节点的key作为上升的key
* 这个时候无论怎么插入内部节点都不会插入到该节点最左内部节点的左侧(比它小的会被分到左侧节点，因为父索引内部节点key等于该内部节点)，该节点再分裂也就不存在最左内部节点再次上升的可能了
*/
static PageId BPlusSplitEntry(BPlusTree* tree, PageId entryId, int insertIndex, void** key_midKey, PageId rightChildId) {
    BPlusEntry* entry = PageGet(tree, entryId);
    BPlusEntry* rightChild = PageGet(tree, rightChildId);

    PageId rightId;
    BPlusEntry* right;
    int rightCount;
    if (entry->type == kLeaf) {
        rightId = BPlusCreateLeafEntry(tree);
        right = PageGet(tree, rightId);
        LeafListEntryInsertTail(tree, entryId, rightId);
        // 原地分裂思路：mid将未插入的内部节点也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
        // 就是提前算好右侧应当有多少个内部节点，拷贝过去，中间遇到新内部节点插入就代替这一次的拷贝，没插入再插入到左侧
        int mid = tree->m / 2;
        rightCount = entry->count + 1 - mid;        // +1是因为这个时候entry->count并没有把未插入内部节点也算上
        int i = rightCount - 1, j = entry->count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j+1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入内部节点也算上
                entry->count++;
                SetLeafInternalKey(tree, right, i, *key_midKey);
                j++;        // j不动
                insert = true;
                continue;
            }
            SetLeafInternalKey(tree, right, i, GetLeafInternalKey(tree, entry, j));
        }

        entry->count -= rightCount;

        if (!insert) {
            // 新内部节点还没有插入，将其插入
            BPlusInsertInternalEntry(tree, entryId, insertIndex, *key_midKey, rightChildId, true);
        }
        
        // 从mid拿到上升内部节点
        *key_midKey = GetLeafInternalKey(tree, right, 0);
    }
    else {
        rightId = BPlusCreateIndexEntry(tree);
        right = PageGet(tree, rightId);
        // 原地分裂思路：mid将未插入的内部节点和即将上升的内部节点都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了

        // 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
        //  1 2       3 4
        // / |  |      |  \    
        // 此时新节点缺少了一条链接，我们最终选用旧节点的末尾内部节点作为上升内部节点，故旧节点的左侧的末尾内部节点的右子树就可以挂接到新节点上

        int mid = (tree->m-1) / 2;
        rightCount = entry->count - mid;        // 这个时候entry->count并没有把未插入内部节点也算上，但是会上升一个内部节点，抵消故不计入
        int i = rightCount - 1, j = entry->count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j+1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入内部节点也算上
                entry->count++;
                SetIndexInternalKey(tree, right, i, *key_midKey);
                SetIndexInternalChildId(tree, right, i+1, rightChildId);
                rightChild->parentId = rightId;
                rightChild->leftParentIndex = i;
                j++;        // j不动
                insert = true;
                continue;
            }
            SetIndexInternalKey(tree, right, i, GetIndexInternalKey(tree, entry, j));
            SetIndexInternalChildId(tree, right, i+1, GetIndexInternalChildId(tree, entry, j+1));
            BPlusEntry* temp = PageGet(tree, GetIndexInternalChildId(tree, right, i + 1));
            temp->parentId = rightId;
            temp->leftParentIndex = i;
        }
        entry->count -= rightCount;

        if (!insert) {
            // 新内部节点还没有插入，将其插入
            BPlusInsertInternalEntry(tree, entryId, insertIndex, *key_midKey, rightChildId, true);
        }

        // 最后从entry->indexData末尾拿到上升内部节点，将其摘除
        entry->count--;
        *key_midKey = GetIndexInternalKey(tree, entry, entry->count);

        // entry最右的链接交给newEntry(right)
        SetIndexInternalChildId(tree, right, 0, GetIndexInternalChildId(tree, entry, entry->count + 1));
        BPlusEntry* head = PageGet(tree, GetIndexInternalChildId(tree, right, 0));
        head->parentId = rightId;
        head->leftParentIndex = -1;
    }
    right->count = rightCount;
    return rightId;
}

/*
* 合并节点
*   2
* 1   3
* 父、左、右的关系如下
*/
static void BPlusMergeEntry(BPlusTree* tree, PageId leftId, PageId rightId, int commonParentIndex) {
    BPlusEntry* left = PageGet(tree, leftId);
    BPlusEntry* right = PageGet(tree, rightId);

    if (left->type == kLeaf) {
        LeafListEntryRemoveTail(tree, leftId);
        // 是叶子节点，将right并入left中，并删除right的父索引内部节点
        for (int i = 0; i < right->count; i++) {
            SetLeafInternalKey(tree, left, left->count++, GetLeafInternalKey(tree, right, i));
        }
    }
    else {
        // 是索引节点，将即将被删除的父索引内部节点(子节点丢弃，因为父索引子节点就指向左和右)和right都并入到left中，删除right的父索引内部节点
        SetIndexInternalKey(tree, left, left->count++, GetIndexInternalKey(tree, PageGet(tree, right->parentId), commonParentIndex));
        // 右节点搬迁后会多出一个头部子节点，因为父节点不需要带子节点下降，所以可以直接插入到左节点尾部
        PageId rightChildId = GetIndexInternalChildId(tree, right, 0);
        SetIndexInternalChildId(tree, left, left->count, rightChildId);
        BPlusEntry* rightChild = PageGet(tree, rightChildId);
        rightChild->parentId = leftId;
        rightChild->leftParentIndex = left->count-1;
        for (int i = 0; i < right->count; i++) {
            SetIndexInternalKey(tree, left, left->count++, GetIndexInternalKey(tree, right, i));
            rightChildId = GetIndexInternalChildId(tree, right, i + 1);
            SetIndexInternalChildId(tree, left, left->count, rightChildId);
            rightChild = PageGet(tree, rightChildId);
            rightChild->parentId = leftId;
            rightChild->leftParentIndex = left->count-1;
        }
    }
    PageFree(tree, rightId);
    // 合并部分完成，删除部分交给调用者
}


/*
* 向节点插入内部节点
*/
static bool BPlusInsertEntry(BPlusTree* tree, PageId entryId, void* key, PageId rightChildId) {
    BPlusEntry* entry = PageGet(tree, entryId);
    if (entry->count == 0) {
        BPlusInsertInternalEntry(tree, entryId, 0, key, rightChildId, true);
        return true;
    }
    int insertIndex = BPlusEntryBinarySearch_Range(tree, entry, 0, entry->count - 1, key, tree->keySize, tree->cmpFunc);
    int res = tree->cmpFunc(GetIndexInternalKey(tree, entry, insertIndex), key, tree->keySize);
    //if (res == 0) {        // 允许插入相同的节点
    //    return false;
    //}
    if (res < 0) {
        insertIndex++;        // 如果所有节点都小于key，那就追加到最尾部
    }

    if (entry->count < tree->m - 1) {
        // 有空余的位置插入
        BPlusInsertInternalEntry(tree, entryId, insertIndex, key, rightChildId, true);
        return true;
    }

    // 没有多余位置，需要分裂向上插入
    // BPlusSplitEntry(tree, entryId, insertIndex, key, rightChildId);

    PageId rightId = BPlusSplitEntry(tree, entryId, insertIndex, &key, rightChildId);
    // 分裂出的内部节点向上传递
    if (entry->parentId == kBPlusInvalidPageId) {
        PageId newIndexId = BPlusCreateIndexEntry(tree);
        entry->parentId = newIndexId;
        entry->leftParentIndex = -1;
        BPlusEntry* right = PageGet(tree, rightId);
        right->parentId = newIndexId;
        right->leftParentIndex = 0;
        BPlusEntry* newIndex = PageGet(tree, newIndexId);
        SetIndexInternalChildId(tree, newIndex, 0, entryId);
        BPlusInsertInternalEntry(tree, newIndexId, 0, key, rightId, true);
        tree->rootId = newIndexId;
    }
    else {
        return BPlusInsertEntry(tree, entry->parentId, key, rightId);
    }
}

/*
* 删除指定节点的内部节点
*/
static bool BPlusDeleteEntry(BPlusTree* tree, PageId entryId, int deleteIndex) {
    BPlusEntry* entry = PageGet(tree, entryId);

    BPlusDeleteInternalEntry(tree, entryId, deleteIndex, NULL, true);       // 直接删除，不考虑子节点，因为索引节点的删除是从底下传递上来的，底下已经处理好了被删除内部节点的子节点关系了，而叶子节点又没有子节点。
    if (entry->count >= (tree->m-1) / 2) {
        return true;
    }

    if (entry->parentId == kBPlusInvalidPageId) {
        // 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
        if (entry->type == kIndex && entry->count == 0) {
            PageId childId = GetIndexInternalChildId(tree, entry, 0);
            BPlusEntry* child = PageGet(tree, childId);
            child->parentId = kBPlusInvalidPageId;
            child->leftParentIndex = 0;
            tree->rootId = childId;
            PageFree(tree, entryId);
        }
        return true;
    }

    int leftParentIndex = entry->leftParentIndex;
    PageId siblingId = BPlusGetLeftSiblingEntry(tree, entryId, leftParentIndex);
    bool leftSibling = true;
    if (siblingId == kBPlusInvalidPageId) {
        siblingId = BPlusGetRightSiblingEntry(tree, entryId, leftParentIndex);
        leftSibling = false;
    }
    BPlusEntry* sibling = PageGet(tree, siblingId);
    if (sibling->count > (tree->m-1) / 2) {
        // 向兄弟借节点
        void* newKey;
        if (entry->type == kLeaf) {
            // 叶子节点处理较简单，可以直接移动
            if (leftSibling) {
                // 从左兄弟节点的末尾的内部节点插入到当前节点的头部并更新父内部节点key为借来的key
                PageId childId = BPlusDeleteInternalEntry(tree, siblingId, sibling->count - 1, &newKey, true);
                BPlusInsertInternalEntry(tree, entryId, 0, newKey, childId, true);
            }
            else {
                // 从右兄弟节点的头部的内部节点插入到当前节点的尾部并更新父内部节点key为右兄弟的新首内部节点
                PageId childId = BPlusDeleteInternalEntry(tree, siblingId, 0, &newKey, true);
                BPlusInsertInternalEntry(tree, entryId, entry->count, newKey, childId, true);
                newKey = GetLeafInternalKey(tree, sibling, 0);         // 右节点的头内部节点key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首内部节点是最好的
                ++leftParentIndex;        // 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
            }
        }
        else {
            // 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父内部节点，上升兄弟的内部节点到共同父内部节点(即交换)
            if (leftSibling) {
                // 左兄弟节点的末尾内部节点上升到父节点的头部，父节点的对应内部节点下降到当前节点的头部，上升内部节点其右子节点挂在下降的父节点内部节点的左侧
                PageId rightChildId = BPlusDeleteInternalEntry(tree, siblingId, sibling->count - 1, &newKey, true);
                BPlusInsertInternalEntry(tree, entryId, 0, GetIndexInternalKey(tree, PageGet(tree, entry->parentId), leftParentIndex), rightChildId, false);
            } else {
                // 右兄弟节点的头内部节点上升到父节点的头部，父节点的对应内部节点下降到当前节点的尾部，上升内部节点其左子节点挂在下降的父节点内部节点的右侧
                PageId leftChildId = BPlusDeleteInternalEntry(tree, siblingId, 0, &newKey, false);
                leftParentIndex++;        // 当前节点是左节点，要找与兄弟节点的共同父节点来更新，所以++
                BPlusInsertInternalEntry(tree, entryId, entry->count, GetIndexInternalKey(tree, PageGet(tree, entry->parentId), leftParentIndex), leftChildId, true);
            }
        }
        SetIndexInternalKey(tree, PageGet(tree, entry->parentId), leftParentIndex, newKey);
        return true;
    }

    // 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点合并也足够下降一个父内部节点)
    if (leftSibling) {
        BPlusMergeEntry(tree, siblingId, entryId, leftParentIndex);
        // 合并需要从父亲节点中拿一个内部节点(即两个子节点的公共父内部节点)，已经拿了，要把删除也处理一下
        return BPlusDeleteEntry(tree, sibling->parentId, leftParentIndex);     // 尾递归优化
    } else {
        BPlusMergeEntry(tree, entryId, siblingId, leftParentIndex + 1);        // 要求共同父索引
        return BPlusDeleteEntry(tree, entry->parentId, leftParentIndex + 1);     // 尾递归优化
    }
}

/*
* 根据key查找到指定的叶子节点
* stack返回每一层节点的父节点内部节点(左父)索引
*/
static PageId BPlusTreeFindLeaf(BPlusTree* tree, void* key) {
    PageId id = tree->rootId;
    BPlusEntry* cur = PageGet(tree, id);
    while (cur && cur->type == kIndex) {
        //       4        8         12
        //    /       |        |       \
        // 3  4      6  7      10        15
        int i = BPlusEntryBinarySearch_Range(tree, cur, 0, cur->count - 1, key, tree->keySize, tree->cmpFunc);
        int res = tree->cmpFunc(key, GetIndexInternalKey(tree, cur, i), tree->keySize);
        if (res >= 0) {
            // 查找key较大，向右找，cur是i的右子节点，i是cur的左父索引
            // 相等的key也向右边找，因为分裂的索引是右叶子的第一个节点上升的
            ++i;
        } else { } // 查找key较小，向左找，cur是i的左子节点，i是cur的右父索引
        id = GetIndexInternalChildId(tree, cur, i);
        // --i;     // cur的左父索引
        cur = PageGet(tree, id);
    }
    return id;
}


/*
* 初始化B+树
*/
void BPlusTreeInit(BPlusTree* tree, int m, int keySize, CmpFunc cmpFunc) {
    if (m < 3) {
        m = 3;      // 最少3阶，否则索引节点分裂会出现一侧没有节点的情况
    }
    tree->m = m;
    tree->keySize = keySize;
    tree->rootId = BPlusCreateLeafEntry(tree);
    tree->leafListFirst = tree->rootId;
    LeafListEntryInit(tree, tree->leafListFirst);
    if (cmpFunc == NULL) {
        cmpFunc = MemoryCmpR;
    }
    tree->cmpFunc = cmpFunc;
}

/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(BPlusTree* tree, void* key) {
    BPlusEntry* leaf = PageGet(tree, BPlusTreeFindLeaf(tree, key));
    int index = BPlusEntryBinarySearch(tree, leaf, 0, leaf->count - 1, key, tree->keySize, tree->cmpFunc);
    if (index == -1) {
        return false;
    }
    return true;
}

/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(BPlusTree* tree, void* key) {
    return BPlusInsertEntry(tree, BPlusTreeFindLeaf(tree, key), key, NULL);
}

/*
* 从B+树中删除指定key
*/
bool BPlusTreeDelete(BPlusTree* tree, void* key) {
    PageId leafId = BPlusTreeFindLeaf(tree, key);
    BPlusEntry* leaf = PageGet(tree, leafId);
    int deleteIndex = BPlusEntryBinarySearch(tree, leaf, 0, leaf->count - 1, key, tree->keySize, tree->cmpFunc);
    if (deleteIndex == -1) {
        return false;
    }
    bool success = BPlusDeleteEntry(tree, leafId, deleteIndex);
    return success;
}
