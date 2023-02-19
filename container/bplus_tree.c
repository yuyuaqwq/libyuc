/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/array.h>
#include <CUtils/container/bplus_tree.h>

#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

const PageId kPageInvalidId = -1;

/*
* 分配页面
*/
PageId PageAlloc(void* tree, bool put_cache, size_t count) {
    return (PageId)MemoryAlloc(sizeof(BPlusEntry) + (((BPlusTree*)tree)->index_m - 1) * max(sizeof(BPlusIndexElement), sizeof(BPlusLeafElement)));
}

/*
* 释放分配的页面
* 如果引用计数未清零则挂到待释放链表中等待引用计数清空
*/
void PageFree(void* tree, PageId id, size_t count) {
    return MemoryFree((void*)id);
}

/*
* 获取页面缓冲区，递增页面引用计数
*/
void* PageGet(void* tree, PageId id, bool modify) {
    if (id == kPageInvalidId) {
        return NULL;
    }
    return (BPlusEntry*)id;
}

/*
* 解除对指定页面的引用
*/
void PageDereference(void* tree, PageId id) {
    return;
}


void SetElement(BPlusTree* tree, BPlusEntry* entry, int i, BPlusElement* element) {
    if (entry->type == kBPlusEntryLeaf) {
        entry->leaf_element[i] = element->leaf;

    }
    else if (entry->type == kBPlusEntryIndex) {
        entry->index_element[i] = element->index;
    }
}

int CmpEntryElement(BPlusTree* tree, BPlusEntry* entry, int i, Key* key) {
    size_t at_key_size;
    void* at_key;
    if (entry->type == kBPlusEntryLeaf) {
        at_key = entry->leaf_element[i].key.ptr;
        at_key_size = entry->leaf_element[i].key.size;
    }
    else {
        at_key = entry->index_element[i].key.ptr;
        at_key_size = entry->index_element[i].key.size;
    }
    int res = tree->cmpFunc(at_key, at_key_size, key->ptr, key->size);
    return res;
}

#endif // CUTILS_CONTAINER_BPLUS_TREE_DISK

static BPlusElement GetElement(BPlusTree* tree, BPlusEntry* entry, int i) {
    BPlusElement element;
    if (entry->type == kBPlusEntryLeaf) {
        element.leaf = entry->leaf_element[i];
    }
    else {
        element.index = entry->index_element[i];
    }
    return element;
}

static inline PageId GetIndexElementChildId(BPlusTree* tree, const BPlusEntry* index, int i) {
    if (i == index->element_count) {
        return index->tail_child_id;
    }
    return index->index_element[i].child_id;
}

static inline void SetIndexElementChildId(BPlusTree* tree, BPlusEntry* index, int i, PageId id) {
    if (i == index->element_count) {
        index->tail_child_id = id;
        return;
    }
    index->index_element[i].child_id = id;
}

static void CopyEntryElement(BPlusTree* tree, BPlusEntry* dst_entry, int dst, BPlusEntry* src_entry, int src) {
    if (dst_entry->type == kBPlusEntryLeaf) {
        dst_entry->leaf_element[dst] = src_entry->leaf_element[src];
    }
    else {
        dst_entry->index_element[dst] = src_entry->index_element[src];
    }
}




typedef struct {
    PageId pgid;
    int element_idx;
} Backtrack;


/*
* 二分查找
*/
static int BPlusBinarySearch(BPlusTree* tree, const BPlusEntry* entry, int first, int last, Key* key) {
    int mid;
    while (first <= last) {
        mid = first + (last - first) / 2;
        int res = CmpEntryElement(tree, entry, mid, key);
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

static int BPlusBinarySearch_Range(BPlusTree* tree, const BPlusEntry* entry, int first, int last, Key* key) {
    int mid = 0;
    while (first < last) {
        mid = first + (last - first) / 2;
        int res = CmpEntryElement(tree, entry, mid, key);
        if (res < 0) first = mid + 1;
        else last = mid;
    }
    return first;
}


/*
* 双向链表
*/
static void LeafListEntryInit(BPlusTree* tree, PageId entry_id) {
    BPlusEntry* first = (BPlusEntry*)PageGet(tree, entry_id, true);
    first->leaf_list_entry.next = entry_id;
    first->leaf_list_entry.prev = entry_id;
    PageDereference(tree, entry_id);
}

static void LeafListEntryInsertToNext(BPlusTree* tree, BPlusEntry* prev, PageId prev_id, BPlusEntry* entry, PageId entry_id) {
    PageId old_next_id = prev->leaf_list_entry.next;
    prev->leaf_list_entry.next = entry_id;
    entry->leaf_list_entry.next = old_next_id;
    entry->leaf_list_entry.prev = prev_id;
    BPlusEntry* old_next = (BPlusEntry*)PageGet(tree, old_next_id, true);
    old_next->leaf_list_entry.prev = entry_id;
    PageDereference(tree, old_next_id);
}

static void LeafListEntryRemoveFromPrev(BPlusTree* tree, BPlusEntry* prev, PageId prev_id, BPlusEntry* entry, PageId entry_id) {
    BPlusEntry* next = PageGet(tree, entry->leaf_list_entry.next, true);
    next->leaf_list_entry.prev = prev_id;
    prev->leaf_list_entry.next = entry->leaf_list_entry.next;
    PageDereference(tree, next);
}



static PageId BPlusCreateIndexEntry(BPlusTree* tree) {
    PageId entry_id = PageAlloc(tree, true, 1);
    BPlusEntry* entry = PageGet(tree, entry_id, true);
    entry->type = kBPlusEntryIndex;
    entry->element_count = 0;
    PageDereference(tree, entry_id);
    return entry_id;
}

static PageId BPlusCreateLeafEntry(BPlusTree* tree) {
    PageId entry_id = PageAlloc(tree, true, 1);
    BPlusEntry* entry = PageGet(tree, entry_id, true);
    entry->type = kBPlusEntryLeaf;
    entry->element_count = 0;
    PageDereference(tree, entry_id);
    return entry_id;
}

/*
* 源代码中没有明确标准的索引指的是key索引
*
* 孩子关系对应如下
*         3     |    6    |    8
*      /          /        /       \
*   1|2         4|5       7        10|11
* 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
*/

/*
* 获取左兄弟节点，没有则返回NULL
*/
static PageId BPlusGetLeftSiblingEntry(BPlusTree* tree, const BPlusEntry* entry, const BPlusEntry* parent, int index) {
    if (index == 0) {
        return kPageInvalidId;
    }
    PageId siblingId = GetIndexElementChildId(tree, parent, index - 1);
    return siblingId;
}

/*
* 获取右兄弟节点，没有则返回NULL
*/
static PageId BPlusGetRightSiblingEntry(BPlusTree* tree, const BPlusEntry* entry, const BPlusEntry* parent, int index) {
    if (index >= parent->element_count) {
        return kPageInvalidId;
    }
    PageId siblingId = GetIndexElementChildId(tree, parent, index + 1);
    return siblingId;
}

/*
* 向元素数组指定位置插入元素
* 不支持处理末尾子节点
*/
static void BPlusInsertElement(BPlusTree* tree, BPlusEntry* entry, int insert_index, BPlusElement* insert_element) {
    for (int j = entry->element_count - 1; j >= insert_index; j--) {
        CopyEntryElement(tree, entry, j + 1, entry, j);        // 向后挪动腾位置
    }
    SetElement(tree, entry, insert_index, insert_element);
    entry->element_count++;
}

/*
* 从元素数组中删除指定索引的元素
* 不支持处理末尾子节点
* 返回被删除的元素
*/
static BPlusElement BPlusDeleteElement(BPlusTree* tree, BPlusEntry* entry, int del_index) {
    BPlusElement del_element = GetElement(tree, entry, del_index);
    for (int i = del_index + 1; i < entry->element_count; i++) {
        CopyEntryElement(tree, entry, i - 1, entry, i);
    }
    entry->element_count--;
    return del_element;
}



/*
* 分裂节点
* 返回上升的索引元素
*
* 同一个叶节点多次分裂会导致重复的key上升吗？
* 如果叶节点不存在相同的key，那么是不会的
* 因为分裂后选择右节点的最左元素的key作为上升的key
* 这个时候无论怎么插入元素都不会插入到该节点最左元素的左侧(比它小的会被分到左侧节点，因为父元素key等于该元素)，该节点再分裂也就不存在最左元素再次上升的可能了
*/
static BPlusElement BPlusSplitEntry(BPlusTree* tree, BPlusEntry* left, PageId left_id, BPlusEntry* parent, int parent_index, int insertIndex, BPlusElement* element, PageId* out_right_id) {
    PageId right_id;
    BPlusEntry* right;
    int rightCount;
    BPlusElement up_element;
    if (left->type == kBPlusEntryLeaf) {
        right_id = BPlusCreateLeafEntry(tree);
        right = PageGet(tree, right_id, true);
        LeafListEntryInsertToNext(tree, left, left_id, right, right_id);
        // 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
        // 就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧
        int mid = tree->leaf_m / 2;
        rightCount = left->element_count + 1 - mid;        // +1是因为这个时候entry->count并没有把未插入元素也算上
        int i = rightCount - 1, j = left->element_count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                left->element_count++;
                SetElement(tree, right, i, element);
                j++;        // j不动
                insert = true;
                continue;
            }
            CopyEntryElement(tree, right, i, left, j);
        }
        left->element_count -= rightCount;
        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tree, left, insertIndex, element);
        }

        // 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点，
        up_element = GetElement(tree, right, 0);
        Key key = up_element.leaf.key;
        up_element.index.key = key;
    }
    else {
        right_id = BPlusCreateIndexEntry(tree);
        right = PageGet(tree, right_id, true);
        // 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了
        // 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
        int mid = (tree->index_m - 1) / 2;
        rightCount = left->element_count - mid;        // 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入
        int i = rightCount - 1, j = left->element_count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                left->element_count++;
                SetElement(tree, right, i, element);
                j++;        // j不动
                insert = true;
                continue;
            }
            CopyEntryElement(tree, right, i, left, j);
        }
        left->element_count -= rightCount;
        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tree, left, insertIndex, element);
        }

        // 假设如下节点需要分裂
        //   2   4   8   12
        //   |   |   |   |    \
        //   1   3   5   10   13   
        // ---------------------------
        //   2   4          8   12
        //   |   |   \      |   |   \
        //   1   3   13     5   10   
        // 此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下
        right->tail_child_id = left->tail_child_id;

        // 最后从左节点末尾拿到上升元素，将其摘除
        left->element_count--;
        up_element = GetElement(tree, left, left->element_count);
        left->tail_child_id = up_element.index.child_id;       // 3指定为2的右侧子节点
    }
    up_element.index.child_id = left_id;        // 上升的4的子节点为2
    SetIndexElementChildId(tree, parent, parent_index, right_id);      // 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点)

    right->element_count = rightCount;
    *out_right_id = right_id;
    PageDereference(tree, right_id);
    return up_element;
}

/*
* 合并节点
* 释放右节点，返回时右节点不再有效
*   2
* 1   3
*/
static void BPlusMergeEntry(BPlusTree* tree, BPlusEntry* left, PageId left_id, BPlusEntry* right, PageId right_id, BPlusEntry* parent, int parent_index) {
    if (left->type == kBPlusEntryLeaf) {
        LeafListEntryRemoveFromPrev(tree, left, left_id, right, right_id);
        // 是叶子节点，将right并入left中，并删除父元素
        for (int i = 0; i < right->element_count; i++) {
            CopyEntryElement(tree, left, left->element_count++, right, i);
        }
    }
    else {
        // 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素
        CopyEntryElement(tree, left, left->element_count++, parent, parent_index);
        SetIndexElementChildId(tree, left, left->element_count - 1, left->tail_child_id);       // left的末尾元素此时为下降的父元素，修改其子节点
        for (int i = 0; i < right->element_count; i++) {
            CopyEntryElement(tree, left, left->element_count++, right, i);
        }
        SetIndexElementChildId(tree, left, left->element_count, right->tail_child_id);
    }
    // 父元素会被删除，右兄弟的左侧子节点(或尾子节点)原先连接右节点，此时继承左节点
    SetIndexElementChildId(tree, parent, parent_index + 1, left_id);

    PageDereference(tree, right_id);
    PageFree(tree, right_id, 1);
    // 合并部分完成，删除部分交给调用者
}



/*
* 向节点插入元素
*/
static bool BPlusInsertEntry(BPlusTree* tree, PageId entry_id, BPlusElement* insert_element, Array* backtrack_stack) {
    PageId right_id;
    BPlusEntry* entry = PageGet(tree, entry_id, true);
    Backtrack* backtrack = ArrayPopTail(backtrack_stack);
    bool success = true, insertUp = false;
    BPlusElement up_element;
    do {
        if (entry->element_count == 0) {
            // 空树的叶子节点的插入
            BPlusInsertElement(tree, entry, 0, insert_element);
            break;
        }
        int insert_index;
        int res;
        if (entry->type == kBPlusEntryLeaf) {
            insert_index = BPlusBinarySearch_Range(tree, entry, 0, entry->element_count - 1, &insert_element->leaf.key);
            res = CmpEntryElement(tree, entry, insert_index, &insert_element->leaf.key);
        }
        else {
            insert_index = BPlusBinarySearch_Range(tree, entry, 0, entry->element_count - 1, &insert_element->index.key);
            res = CmpEntryElement(tree, entry, insert_index, &insert_element->index.key);
        }
        if (res < 0) {
            insert_index++;        // 如果所有元素都小于新元素，那就追加到最尾部
        }
        uint32_t m = entry->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (entry->element_count < m - 1) {
            // 有空余的位置插入
            BPlusInsertElement(tree, entry, insert_index, insert_element);
            break;
        }

        // 没有多余位置，需要分裂向上插入
        if (!backtrack) {
            // 没有父节点，创建
            PageId parent_id = BPlusCreateIndexEntry(tree);
            BPlusEntry* parent = PageGet(tree, parent_id, true);
            up_element = BPlusSplitEntry(tree, entry, entry_id, parent, 0, insert_index, insert_element, &right_id);
            BPlusInsertElement(tree, parent, 0, &up_element);
            tree->rootId = parent_id;
            PageDereference(tree, parent_id);
            break;
        }
        BPlusEntry* parent = PageGet(tree, backtrack->pgid, true);
        up_element = BPlusSplitEntry(tree, entry, entry_id, parent, backtrack->element_idx, insert_index, insert_element, &right_id);
        PageDereference(tree, backtrack->pgid);
        insertUp = true;
    } while (false);
    PageDereference(tree, entry_id);
    if (insertUp) {
        return BPlusInsertEntry(tree, backtrack->pgid, &up_element, backtrack_stack);
    }
    return success;
}

/*
* 删除指定节点的元素
*/
static bool BPlusDeleteEntry(BPlusTree* tree, PageId entryId, int del_index, Array* backtrack_stack) {
    BPlusEntry* entry = PageGet(tree, entryId, true);
    Backtrack* backtrack = NULL;
    PageId siblingId = 0;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, deleteUp = false;
    BPlusDeleteElement(tree, entry, del_index);      // 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了
    do {
        uint32_t m = entry->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (entry->element_count >= (m - 1) / 2) {
            break;
        }
        backtrack = ArrayPopTail(backtrack_stack);
        if (!backtrack) {
            // 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
            if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
                PageId childId = entry->tail_child_id;
                tree->rootId = childId;
                PageDereference(tree, entryId);
                PageFree(tree, entryId, 1);
                return true;
            }
            else {
                break;
            }
        }
        parent = PageGet(tree, backtrack->pgid, true);
        siblingId = BPlusGetLeftSiblingEntry(tree, entry, parent, backtrack->element_idx);
        bool leftSibling = true;
        if (siblingId == kPageInvalidId) {
            siblingId = BPlusGetRightSiblingEntry(tree, entry, parent, backtrack->element_idx);
            leftSibling = false;
        }
        sibling = PageGet(tree, siblingId, true);
        if (sibling->element_count > (m - 1) / 2) {
            // 向兄弟借节点
            if (entry->type == kBPlusEntryLeaf) {
                // 叶子节点处理较简单，可以直接移动
                if (leftSibling) {
                    backtrack->element_idx--;
                    // 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key
                    BPlusElement element = BPlusDeleteElement(tree, sibling, sibling->element_count - 1);
                    BPlusInsertElement(tree, entry, 0, &element);
                    parent->index_element[backtrack->element_idx].key = element.leaf.key;       // 更新索引
                }
                else {
                    // 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素
                    BPlusElement element = BPlusDeleteElement(tree, sibling, 0);
                    BPlusInsertElement(tree, entry, entry->element_count, &element);
                    // 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的
                    parent->index_element[backtrack->element_idx].key = sibling->leaf_element[0].key;       // 更新索引
                }
            }
            else {
                // 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换)
                if (leftSibling) {
                    backtrack->element_idx--;
                    // 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧
                    BPlusElement left_element = BPlusDeleteElement(tree, sibling, sibling->element_count - 1);
                    SwapObject(PageId, left_element.index.child_id, sibling->tail_child_id);        // 要拿的是末尾的子节点，处理一下
                    BPlusElement par_element = GetElement(tree, parent, backtrack->element_idx);
                    par_element.index.child_id = left_element.index.child_id;
                    BPlusInsertElement(tree, entry, 0, &par_element);
                    left_element.index.child_id = siblingId;
                    SetElement(tree, parent, backtrack->element_idx, &left_element);
                }
                else {
                    // 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧
                    BPlusElement right_element = BPlusDeleteElement(tree, sibling, 0);
                    BPlusElement par_element = GetElement(tree, parent, backtrack->element_idx);
                    par_element.index.child_id = right_element.index.child_id;
                    SwapObject(PageId, par_element.index.child_id, entry->tail_child_id);        // 要插入的是末尾的子节点，处理一下
                    BPlusInsertElement(tree, entry, entry->element_count, &par_element);
                    right_element.index.child_id = entryId;
                    SetElement(tree, parent, backtrack->element_idx, &right_element);
                }
            }
            
            break;
        }

        // 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素)
        if (leftSibling) {
            --backtrack->element_idx;
            BPlusMergeEntry(tree, sibling, siblingId, entry, entryId, parent, backtrack->element_idx);
            entry = NULL;
        }
        else {
            BPlusMergeEntry(tree, entry, entryId, sibling, siblingId, parent, backtrack->element_idx);
            sibling = NULL;
        }
        deleteUp = true;
    } while (false);
    if (parent) { PageDereference(tree, backtrack->pgid); }
    if (sibling) { PageDereference(tree, siblingId); }
    if (entry) { PageDereference(tree, entryId); }

    if (deleteUp) {
        return BPlusDeleteEntry(tree, backtrack->pgid, backtrack->element_idx, backtrack_stack);     // 尾递归
    }
    return success;
}

/*
* 根据key查找到指定的叶子节点
* stack返回每一层节点的父(左)元素索引
*/
static PageId BPlusTreeFindLeaf(BPlusTree* tree, Key* key, BPlusEntry** leaf, Array* backtrack_stack) {
    Backtrack backtrack;
    backtrack.pgid = tree->rootId;
    const BPlusEntry* cur = PageGet(tree, backtrack.pgid, false);
    while (cur && cur->type == kBPlusEntryIndex) {
        //       4        8         12
        //    /       |        |       \
        // 3  4      6  7      10        15
        backtrack.element_idx = BPlusBinarySearch_Range(tree, cur, 0, cur->element_count - 1, key);
        int res = CmpEntryElement(tree, cur, backtrack.element_idx, key);
        if (res <= 0) {
            // 查找key较大或相等，找右孩子
            ++backtrack.element_idx;
        }
        else {} // 查找key较小，向左找
        PageId oldId = backtrack.pgid;
        PageDereference(tree, oldId);
        if (backtrack_stack) {
            ArrayPushTail(backtrack_stack, &backtrack);
        }
        backtrack.pgid = GetIndexElementChildId(tree, cur, backtrack.element_idx);
        cur = PageGet(tree, backtrack.pgid, true);
    }
    if (leaf) {
        *leaf = cur;
    }
    else {
        PageDereference(tree, backtrack.pgid);
    }
    return backtrack.pgid;
}


/*
* 初始化B+树
*/
void BPlusTreeInit(BPlusTree* tree, uint32_t index_m, uint32_t leaf_m, CmpFunc2 cmpFunc) {
    if (index_m < 3) {
        index_m = 3;      // 最少3阶，否则索引节点分裂会出现一侧没有节点的情况
    }
    if (leaf_m < 2) {
        leaf_m = 2;
    }
    tree->index_m = index_m;
    tree->leaf_m = leaf_m;
    tree->rootId = BPlusCreateLeafEntry(tree);
    tree->leaf_list_first = tree->rootId;
    LeafListEntryInit(tree, tree->leaf_list_first);
    if (cmpFunc == NULL) {
        cmpFunc = MemoryCmpR2;
    }
    tree->cmpFunc = cmpFunc;
}

/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(BPlusTree* tree, Key* key) {
    BPlusEntry* leaf;
    PageId leaf_id = BPlusTreeFindLeaf(tree, key, &leaf, NULL);
    int index = BPlusBinarySearch(tree, leaf, 0, leaf->element_count - 1, key);
    PageDereference(tree, leaf_id);
    if (index == -1) {
        return false;
    }
    return true;
}

/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element) {
    Array stack;
    Backtrack buf[8];
    ArrayInit(&stack, 8, sizeof(Backtrack));
    PageId leafId = BPlusTreeFindLeaf(tree, &element->key, NULL, &stack);
    bool success = BPlusInsertEntry(tree, leafId, (BPlusLeafElement*)element, &stack);
    ArrayRelease(&stack);
    return success;
}

/*
* 从B+树中删除指定key
*/
bool BPlusTreeDelete(BPlusTree* tree, Key* key) {
    Array stack;
    ArrayInit(&stack, 8, sizeof(Backtrack));
    bool success = false;
    BPlusEntry* leaf;
    PageId leaf_id = BPlusTreeFindLeaf(tree, key, &leaf, &stack);
    int deleteIndex = BPlusBinarySearch(tree, leaf, 0, leaf->element_count - 1, key);
    PageDereference(tree, leaf_id);
    do {
        if (deleteIndex == -1) {
            break;
        }
        success = BPlusDeleteEntry(tree, leaf_id, deleteIndex, &stack);
    } while (false);
    ArrayRelease(&stack);
    return success;
}