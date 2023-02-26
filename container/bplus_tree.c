/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/array.h>
#include <CUtils/container/bplus_tree.h>

#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK
typedef struct _Tx {
    BPlusTree tree;
} Tx;

const PageId kPageInvalidId = -1;


inline BPlusTree* BPlusTreeGet(Tx* tx) {
    return &tx->tree;
}


BPlusEntry* BPlusEntryGet(Tx* tx, PageId pgid) {
    if (pgid == kPageInvalidId) {
        return NULL;
    }
    return (BPlusEntry*)pgid;
}

void BPlusEntryDereference(Tx* tx, PageId pgid) {

}

PageId BPlusEntryCreate(Tx* tx, BPlusEntryType type) {
    BPlusTree* tree = BPlusTreeGet(tx);
    size_t size;
    if (type == kBPlusEntryIndex) {
        size = (((BPlusTree*)tree)->index_m - 1) * sizeof(BPlusIndexElement);
    }
    else {
        size = (((BPlusTree*)tree)->leaf_m - 1) * sizeof(BPlusLeafElement);
    }
    PageId entry_id = MemoryAlloc(sizeof(BPlusEntry) + size);
    BPlusEntry* entry = BPlusEntryGet(tx, entry_id, true);
    entry->type = type;
    entry->element_count = 0;
    BPlusEntryDereference(tx, entry_id);
    return entry_id;
}

void BPlusEntryDelete(Tx* tx, PageId pgid) {
    MemoryFree((void*)pgid);
}


void BPlusElementSet(Tx* tx, BPlusEntry* entry, int i, BPlusElement* element) {
    if (entry->type == kBPlusEntryLeaf) {
        entry->leaf.element[i] = element->leaf;
    }
    else if (entry->type == kBPlusEntryIndex) {
        entry->index.element[i] = element->index;
    }
}


ptrdiff_t BPlusKeyCmp(Tx* tx, const Key* key1, const Key* key2) {
    return MemoryCmpR2(key1->ptr, key1->size, key2->ptr, key2->size);
}

#endif // CUTILS_CONTAINER_BPLUS_TREE_DISK


static BPlusElement BPlusElementGet(Tx* tx, BPlusEntry* entry, int i) {
    BPlusElement element;
    if (entry->type == kBPlusEntryLeaf) {
        element.leaf = entry->leaf.element[i];
    }
    else {
        element.index = entry->index.element[i];
    }
    return element;
}

static inline PageId BPlusElementGetChildId(Tx* tx, const BPlusEntry* index, int i) {
    if (i == index->element_count) {
        return index->index.tail_child_id;
    }
    return index->index.element[i].child_id;
}

static inline void BPlusElementSetChildId(Tx* tx, BPlusEntry* index, int i, PageId id) {
    if (i == index->element_count) {
        index->index.tail_child_id = id;
        return;
    }
    index->index.element[i].child_id = id;
}

static void BPlusElementCopy(Tx* tx, BPlusEntry* dst_entry, int dst, BPlusEntry* src_entry, int src) {
    if (dst_entry->type == kBPlusEntryLeaf) {
        dst_entry->leaf.element[dst] = src_entry->leaf.element[src];
    }
    else {
        dst_entry->index.element[dst] = src_entry->index.element[src];
    }
}

ptrdiff_t BPlusElementCmp(Tx* tx, BPlusEntry* entry, int i, Key* key2) {
    Key* key1;
    if (entry->type == kBPlusEntryLeaf) {
        key1 = &entry->leaf.element[i].key;
    }
    else {
        key1 = &entry->index.element[i].key;
    }
    ptrdiff_t res = BPlusKeyCmp(tx, key1, key2);
    return res;
}





BPlusElementPos* BPlusCursorCur(Tx* tx, BPlusCursor* cursor) {
    if (cursor->level < 0) {
        return NULL;
    }
    return ArrayAt(&cursor->stack, cursor->level, BPlusElementPos);
}

BPlusElementPos* BPlusCursorUp(Tx* tx, BPlusCursor* cursor) {
    if (cursor->level <= 0) {
        return NULL;
    }
    return ArrayAt(&cursor->stack, --cursor->level, BPlusElementPos);
}

BPlusElementPos* BPlusCursorDown(Tx* tx, BPlusCursor* cursor) {
    if (cursor->level + 1 >= ArrayGetCount(&cursor->stack)) {
        return NULL;
    }
    return ArrayAt(&cursor->stack, ++cursor->level, BPlusElementPos);
}

BPlusCursorStatus BPlusCursorFirst(Tx* tx, BPlusCursor* cursor, Key* key) {
    ArrayInit(&cursor->stack, 8, sizeof(BPlusElementPos));
    cursor->level = -1;
    return BPlusCursorNext(tx, cursor, key);
}

void BPlusCursorRelease(Tx* tx, BPlusCursor* cursor) {
    if (cursor->stack.capacity != 0) {
        ArrayRelease(&cursor->stack);
    }
}

BPlusCursorStatus BPlusCursorNext(Tx* tx, BPlusCursor* cursor, Key* key) {
    BPlusTree* tree = BPlusTreeGet(tx);
    BPlusElementPos cur;
    BPlusElementPos* parent = BPlusCursorCur(tx, cursor);
    if (parent) {
        BPlusEntry* parent_entry = BPlusEntryGet(tx, parent->entry_id);
        if (parent_entry->type == kBPlusEntryLeaf) {
            return kBPlusCursorEnd;
        }
        cur.entry_id = BPlusElementGetChildId(tx, parent_entry, parent->element_idx);
        BPlusEntryDereference(tx, parent->entry_id);
    }
    else {
        cur.entry_id = tree->root_id;
    }

    BPlusEntry* cur_entry = BPlusEntryGet(tx, cur.entry_id);
    int res;
    if (cur_entry->element_count > 0) {
        cur.element_idx = BPlusBinarySearch_Range(tx, cur_entry, 0, cur_entry->element_count - 1, key);
        res = BPlusElementCmp(tx, cur_entry, cur.element_idx, key);
        if (cur_entry->type == kBPlusEntryIndex) {
            if (res == 0) {
                // 索引节点，查找key相等，取右兄弟元素
                ++cur.element_idx;
            }
        }
        if (res < 0) {
            // 查找key较大，取右兄弟元素/向右找
            ++cur.element_idx;
        }
        else {} // 查找key较小，向左找
    }
    else {
        res = -1;
        cur.element_idx = 0;
    }
    ArrayPushTail(&cursor->stack, &cur);
    BPlusCursorStatus status = kBPlusCursorNext;
    if (cur_entry->type == kBPlusEntryLeaf) {
        if (res != 0) {
            status = kBPlusCursorNe;
        }
        else {
            status = kBPlusCursorEq;
        }
        cursor->leaf_status = status;
    }
    ++cursor->level;
    BPlusEntryDereference(tx, cur.entry_id);
    return status;
}






/*
* 二分查找
*/
static int BPlusBinarySearch(Tx* tx, const BPlusEntry* entry, int first, int last, Key* key) {
    BPlusTree* tree = BPlusTreeGet(tx);
    int mid;
    while (first <= last) {
        mid = first + (last - first) / 2;
        int res = BPlusElementCmp(tree, entry, mid, key);
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

static int BPlusBinarySearch_Range(Tx* tx, const BPlusEntry* entry, int first, int last, Key* key) {
    BPlusTree* tree = BPlusTreeGet(tx);
    int mid = 0;
    while (first < last) {
        mid = first + (last - first) / 2;
        int res = BPlusElementCmp(tree, entry, mid, key);
        if (res < 0) first = mid + 1;
        else last = mid;
    }
    return first;
}


/*
* 双向链表
*/
static void LeafListEntryInit(Tx* tx, PageId entry_id) {
    BPlusEntry* first = (BPlusEntry*)BPlusEntryGet(tx, entry_id, true);
    first->leaf.list_entry.next = entry_id;
    first->leaf.list_entry.prev = entry_id;
    BPlusEntryDereference(tx, entry_id);
}

static void LeafListEntryInsertToNext(Tx* tx, BPlusEntry* prev, PageId prev_id, BPlusEntry* entry, PageId entry_id) {
    PageId old_next_id = prev->leaf.list_entry.next;
    prev->leaf.list_entry.next = entry_id;
    entry->leaf.list_entry.next = old_next_id;
    entry->leaf.list_entry.prev = prev_id;
    BPlusEntry* old_next = (BPlusEntry*)BPlusEntryGet(tx, old_next_id, true);
    old_next->leaf.list_entry.prev = entry_id;
    BPlusEntryDereference(tx, old_next_id);
}

static void LeafListEntryRemoveFromPrev(Tx* tx, BPlusEntry* prev, PageId prev_id, BPlusEntry* entry, PageId entry_id) {
    BPlusEntry* next = BPlusEntryGet(tx, entry->leaf.list_entry.next, true);
    next->leaf.list_entry.prev = prev_id;
    prev->leaf.list_entry.next = entry->leaf.list_entry.next;
    BPlusEntryDereference(tx, next);
}




/*
* 孩子关系对应如下
*         3     |    6    |    8
*      /          /        /       \
*   1|2         4|5       7        10|11
* 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
*/

/*
* 获取左兄弟节点，没有则返回NULL
*/
static PageId BPlusGetLeftSiblingEntry(Tx* tx, const BPlusEntry* entry, const BPlusEntry* parent, int index) {
    if (index == 0) {
        return kPageInvalidId;
    }
    PageId siblingId = BPlusElementGetChildId(tx, parent, index - 1);
    return siblingId;
}

/*
* 获取右兄弟节点，没有则返回NULL
*/
static PageId BPlusGetRightSiblingEntry(Tx* tx, const BPlusEntry* entry, const BPlusEntry* parent, int index) {
    if (index >= parent->element_count) {
        return kPageInvalidId;
    }
    PageId siblingId = BPlusElementGetChildId(tx, parent, index + 1);
    return siblingId;
}

/*
* 向元素数组指定位置插入元素
* 不支持处理末尾子节点
*/
static void BPlusInsertElement(Tx* tx, BPlusEntry* entry, int insert_index, BPlusElement* insert_element) {
    for (int j = entry->element_count - 1; j >= insert_index; j--) {
        BPlusElementCopy(tx, entry, j + 1, entry, j);        // 向后挪动腾位置
    }
    BPlusElementSet(tx, entry, insert_index, insert_element);
    entry->element_count++;
}

/*
* 从元素数组中删除指定索引的元素
* 不支持处理末尾子节点
* 返回被删除的元素
*/
static BPlusElement BPlusDeleteElement(Tx* tx, BPlusEntry* entry, int del_index) {
    BPlusElement del_element = BPlusElementGet(tx, entry, del_index);
    for (int i = del_index + 1; i < entry->element_count; i++) {
        BPlusElementCopy(tx, entry, i - 1, entry, i);
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
static BPlusElement BPlusSplitEntry(Tx* tx, BPlusEntry* left, PageId left_id, BPlusEntry* parent, int parent_index, int insertIndex, BPlusElement* element, PageId* out_right_id) {
    BPlusTree* tree = BPlusTreeGet(tx);
    PageId right_id;
    BPlusEntry* right;
    int rightCount;
    BPlusElement up_element;
    if (left->type == kBPlusEntryLeaf) {
        right_id = BPlusEntryCreate(tx, kBPlusEntryLeaf);
        right = BPlusEntryGet(tx, right_id, true);
        LeafListEntryInsertToNext(tx, left, left_id, right, right_id);
        // 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
        // 就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧
        int mid = tree->leaf_m / 2;
        rightCount = left->element_count + 1 - mid;        // +1是因为这个时候entry->count并没有把未插入元素也算上
        int i = rightCount - 1, j = left->element_count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                left->element_count++;
                BPlusElementSet(tx, right, i, element);
                j++;        // j不动
                insert = true;
                continue;
            }
            BPlusElementCopy(tx, right, i, left, j);
        }
        left->element_count -= rightCount;
        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tx, left, insertIndex, element);
        }

        // 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点，
        up_element = BPlusElementGet(tx, right, 0);
        Key key = up_element.leaf.key;
        up_element.index.key = key;
    }
    else {
        right_id = BPlusEntryCreate(tx, kBPlusEntryIndex);
        right = BPlusEntryGet(tx, right_id, true);
        // 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了
        // 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
        int mid = (tree->index_m - 1) / 2;
        rightCount = left->element_count - mid;        // 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入
        int i = rightCount - 1, j = left->element_count - 1;
        bool insert = false;
        for (; i >= 0; i--, j--) {
            if (!insert && j + 1 == insertIndex) {        // 这里j+1是因为，循环的时候j并没有把未插入元素也算上
                left->element_count++;
                BPlusElementSet(tx, right, i, element);
                j++;        // j不动
                insert = true;
                continue;
            }
            BPlusElementCopy(tx, right, i, left, j);
        }
        left->element_count -= rightCount;
        if (!insert) {
            // 新元素还没有插入，将其插入
            BPlusInsertElement(tx, left, insertIndex, element);
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
        right->index.tail_child_id = left->index.tail_child_id;

        // 最后从左节点末尾拿到上升元素，将其摘除
        left->element_count--;
        up_element = BPlusElementGet(tx, left, left->element_count);
        left->index.tail_child_id = up_element.index.child_id;       // 3指定为2的右侧子节点
    }
    up_element.index.child_id = left_id;        // 上升的4的子节点为2
    BPlusElementSetChildId(tx, parent, parent_index, right_id);      // 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点)

    right->element_count = rightCount;
    *out_right_id = right_id;
    BPlusEntryDereference(tx, right_id);
    return up_element;
}

/*
* 合并节点
* 释放右节点，返回时右节点不再有效
*   2
* 1   3
*/
static void BPlusMergeEntry(Tx* tx, BPlusEntry* left, PageId left_id, BPlusEntry* right, PageId right_id, BPlusEntry* parent, int parent_index) {
    if (left->type == kBPlusEntryLeaf) {
        LeafListEntryRemoveFromPrev(tx, left, left_id, right, right_id);
        // 是叶子节点，将right并入left中，并删除父元素
        for (int i = 0; i < right->element_count; i++) {
            BPlusElementCopy(tx, left, left->element_count++, right, i);
        }
    }
    else {
        // 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素
        BPlusElementCopy(tx, left, left->element_count++, parent, parent_index);
        BPlusElementSetChildId(tx, left, left->element_count - 1, left->index.tail_child_id);       // left的末尾元素此时为下降的父元素，修改其子节点
        for (int i = 0; i < right->element_count; i++) {
            BPlusElementCopy(tx, left, left->element_count++, right, i);
        }
        BPlusElementSetChildId(tx, left, left->element_count, right->index.tail_child_id);
    }
    // 父元素会被删除，右兄弟的左侧子节点(或尾子节点)原先连接右节点，此时继承左节点
    BPlusElementSetChildId(tx, parent, parent_index + 1, left_id);

    BPlusEntryDereference(tx, right_id);
    BPlusEntryDelete(tx, right_id);
    // 合并部分完成，删除部分交给调用者
}



/*
* 向节点插入元素
*/
bool BPlusInsertEntry(Tx* tx, BPlusCursor* cursor, BPlusElement* insert_element) {
    BPlusTree* tree = BPlusTreeGet(tx);
    BPlusElementPos* cur_pos = BPlusCursorCur(tx, cursor);
    BPlusElementPos* parent_pos = BPlusCursorUp(tx, cursor);
    PageId right_id;
    BPlusEntry* cur = BPlusEntryGet(tx, cur_pos->entry_id, true);
    
    bool success = true, insertUp = false;
    BPlusElement up_element;
    do {
        if (cur->element_count == 0) {
            // 空树的叶子节点的插入
            BPlusInsertElement(tx, cur, 0, insert_element);
            break;
        }
        if (cursor->leaf_status == kBPlusCursorEq) {
            BPlusElementSet(tx, cur, cur_pos->element_idx, insert_element);
            break;
        }
        uint32_t m = cur->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (cur->element_count < m - 1) {
            // 有空余的位置插入
            BPlusInsertElement(tx, cur, cur_pos->element_idx, insert_element);
            break;
        }

        // 没有多余位置，需要分裂向上插入
        if (!parent_pos) {
            // 没有父节点，创建
            PageId parent_id = BPlusEntryCreate(tx, kBPlusEntryIndex);
            BPlusEntry* parent = BPlusEntryGet(tx, parent_id, true);
            up_element = BPlusSplitEntry(tx, cur, cur_pos->entry_id, parent, 0, cur_pos->element_idx, insert_element, &right_id);
            BPlusInsertElement(tx, parent, 0, &up_element);
            tree->root_id = parent_id;
            BPlusEntryDereference(tx, parent_id);
            break;
        }
        BPlusEntry* parent = BPlusEntryGet(tx, parent_pos->entry_id, true);
        up_element = BPlusSplitEntry(tx, cur, cur_pos->entry_id, parent, parent_pos->element_idx, cur_pos->element_idx, insert_element, &right_id);
        BPlusEntryDereference(tx, parent_pos->entry_id);
        insertUp = true;
    } while (false);
    BPlusEntryDereference(tx, cur_pos->entry_id);
    if (insertUp) {
        return BPlusInsertEntry(tx, cursor, &up_element);
    }
    return success;
}

/*
* 删除指定节点的元素
*/
bool BPlusDeleteEntry(Tx* tx, BPlusCursor* cursor) {
    BPlusTree* tree = BPlusTreeGet(tx);
    BPlusElementPos* cur_pos = BPlusCursorCur(tx, cursor);
    BPlusElementPos* parent_pos = BPlusCursorUp(tx, cursor);
    BPlusEntry* entry = BPlusEntryGet(tx, cur_pos->entry_id, true);
    PageId siblingId = 0;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, deleteUp = false;
    BPlusDeleteElement(tx, entry, cur_pos->element_idx);      // 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了
    do {
        uint32_t m = entry->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (entry->element_count >= (m - 1) / 2) {
            break;
        }
        if (!parent_pos) {
            // 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
            if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
                PageId childId = entry->index.tail_child_id;
                tree->root_id = childId;
                BPlusEntryDereference(tx, cur_pos->entry_id);
                BPlusEntryDelete(tx, cur_pos->entry_id);
                return true;
            }
            else {
                break;
            }
        }
        parent = BPlusEntryGet(tx, parent_pos->entry_id, true);
        siblingId = BPlusGetLeftSiblingEntry(tx, entry, parent, parent_pos->element_idx);
        bool leftSibling = true;
        if (siblingId == kPageInvalidId) {
            siblingId = BPlusGetRightSiblingEntry(tx, entry, parent, parent_pos->element_idx);
            leftSibling = false;
        }
        sibling = BPlusEntryGet(tx, siblingId, true);
        if (sibling->element_count > (m - 1) / 2) {
            // 向兄弟借节点
            if (entry->type == kBPlusEntryLeaf) {
                // 叶子节点处理较简单，可以直接移动
                if (leftSibling) {
                    parent_pos->element_idx--;
                    // 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key
                    BPlusElement element = BPlusDeleteElement(tx, sibling, sibling->element_count - 1);
                    BPlusInsertElement(tx, entry, 0, &element);
                    parent->index.element[parent_pos->element_idx].key = element.leaf.key;       // 更新索引
                }
                else {
                    // 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素
                    BPlusElement element = BPlusDeleteElement(tx, sibling, 0);
                    BPlusInsertElement(tx, entry, entry->element_count, &element);
                    // 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的
                    parent->index.element[parent_pos->element_idx].key = sibling->leaf.element[0].key;       // 更新索引
                }
            }
            else {
                // 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换)
                if (leftSibling) {
                    parent_pos->element_idx--;
                    // 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧
                    BPlusElement left_element = BPlusDeleteElement(tx, sibling, sibling->element_count - 1);
                    SwapObject(PageId, left_element.index.child_id, sibling->index.tail_child_id);        // 要拿的是末尾的子节点，处理一下
                    BPlusElement par_element = BPlusElementGet(tx, parent, parent_pos->element_idx);
                    par_element.index.child_id = left_element.index.child_id;
                    BPlusInsertElement(tx, entry, 0, &par_element);
                    left_element.index.child_id = siblingId;
                    BPlusElementSet(tx, parent, parent_pos->element_idx, &left_element);
                }
                else {
                    // 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧
                    BPlusElement right_element = BPlusDeleteElement(tx, sibling, 0);
                    BPlusElement par_element = BPlusElementGet(tx, parent, parent_pos->element_idx);
                    par_element.index.child_id = right_element.index.child_id;
                    SwapObject(PageId, par_element.index.child_id, entry->index.tail_child_id);        // 要插入的是末尾的子节点，处理一下
                    BPlusInsertElement(tx, entry, entry->element_count, &par_element);
                    right_element.index.child_id = cur_pos->entry_id;
                    BPlusElementSet(tx, parent, parent_pos->element_idx, &right_element);
                }
            }
            
            break;
        }

        // 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素)
        if (leftSibling) {
            --parent_pos->element_idx;
            BPlusMergeEntry(tx, sibling, siblingId, entry, cur_pos->entry_id, parent, parent_pos->element_idx);
            entry = NULL;
        }
        else {
            BPlusMergeEntry(tx, entry, cur_pos->entry_id, sibling, siblingId, parent, parent_pos->element_idx);
            sibling = NULL;
        }
        deleteUp = true;
    } while (false);
    if (parent) { BPlusEntryDereference(tx, parent_pos->entry_id); }
    if (sibling) { BPlusEntryDereference(tx, siblingId); }
    if (entry) { BPlusEntryDereference(tx, cur_pos->entry_id); }

    if (deleteUp) {
        return BPlusDeleteEntry(tx, cursor);     // 尾递归
    }
    return success;
}


/*
* 初始化B+树
*/
void BPlusTreeInit(Tx* tx, uint32_t index_m, uint32_t leaf_m) {
    BPlusTree* tree = BPlusTreeGet(tx);
    if (index_m < 3) {
        index_m = 3;      // 最少3阶，否则索引节点分裂会出现一侧没有节点的情况
    }
    if (leaf_m < 3) {       //2
        leaf_m = 3;     // 可以2阶，但是删除有地方需要加强判断
    }
    tree->index_m = index_m;
    tree->leaf_m = leaf_m;
    tree->root_id = BPlusEntryCreate(tx, kBPlusEntryLeaf);
    tree->leaf_list_first = tree->root_id;
    LeafListEntryInit(tx, tree->leaf_list_first);
}

/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(Tx* tx, Key* key) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tx, &cursor, key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tx, &cursor, key);
    }
    return status == kBPlusCursorEq;
}

/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(Tx* tx, BPlusLeafElement* element) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tx, &cursor, &element->key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tx, &cursor, &element->key);
    }
    bool success = BPlusInsertEntry(tx, &cursor, (BPlusLeafElement*)element);
    BPlusCursorRelease(tx, &cursor);
    return success;
}

/*
* 从B+树中删除指定key
*/
bool BPlusTreeDelete(Tx* tx, Key* key) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tx, &cursor, key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tx, &cursor, key);
    }
    if (status == kBPlusCursorNe) {
        return false;
    }
    bool success = BPlusDeleteEntry(tx, &cursor);
    return success;
}