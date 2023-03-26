/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/vector.h>
#include <CUtils/container/bplus_tree.h>

const BPlusEntryId kBPlusEntryInvalidId = (BPlusEntryId)-1;

CUTILS_CONTAINER_VECTOR_DEFINE(BPlusCursorStack, BPlusElementPos, CUTILS_OBJECT_ALLOCATOR_DEFALUT)

forceinline BPlusLeafListEntry* CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Reference(BPlusLeafListHead* head, BPlusEntryId entry_id) {
    BPlusTree* tree = ObjectGetFromField(head, BPlusTree, leaf_list);
    BPlusEntry* entry = BPlusEntryReference(tree, entry_id);
    return &entry->leaf.list_entry;
}
forceinline void CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Dereference(BPlusLeafListHead* head, BPlusLeafListEntry* list_entry) {
    BPlusTree* tree = ObjectGetFromField(head, BPlusTree, leaf_list);
    BPlusLeafEntry* entry = ObjectGetFromField(list_entry, BPlusLeafEntry, list_entry);
    BPlusEntryDereference(tree, (BPlusEntry*)entry);
}
#define CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_Dereference(TREE, ENTRY_ID)
#define CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT_InvalidId kBPlusEntryInvalidId
#define CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT
CUTILS_CONTAINER_LIST_DEFINE(BPlusLeaf, BPlusEntryId, CUTILS_CONTAINER_BPLUS_ENTRY_REFERENCER_DEFALUT)

typedef struct {
    int16_t color:1;
    int16_t parent:15;
} BPlusRbParentColor;
forceinline Key CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(BPlusRbTree* tree, BPlusRbBsEntry* bs_entry) {
    if (((BPlusEntry*)tree)->type == kBPlusEntryLeaf) {
        return ((BPlusLeafElement*)bs_entry)->key;
    }
    else {
        return ((BPlusIndexElement*)bs_entry)->key;
    }
}
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetParent(TREE, ENTRY) (((BPlusRbParentColor*)&(((BPlusRbEntry*)ENTRY)->parent_color))->parent)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY) (((BPlusRbParentColor*)&(((BPlusRbEntry*)ENTRY)->parent_color))->color == -1 ? 1 : 0)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_SetParent(TREE, ENTRY, NEW_PARENT_ID) (((BPlusRbParentColor*)&(((BPlusRbEntry*)ENTRY)->parent_color))->parent = NEW_PARENT_ID)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_SetColor(TREE, ENTRY, COLOR) (((BPlusRbParentColor*)&(((BPlusRbEntry*)ENTRY)->parent_color))->color = COLOR)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR

#define CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_InvalidId (-1)
forceinline BPlusRbEntry* CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Reference(BPlusRbTree* tree, int16_t id) {
    if (id == CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_InvalidId) {
        return NULL;
    }
    BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
    if (entry->type == kBPlusEntryIndex) {
        return &entry->index.element_space.obj_arr[id].rb_entry;
    }
    else {
        return &entry->leaf.element_space.obj_arr[id].rb_entry;
    }
}
#define CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_Dereference(TREE, OBJ)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER

#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Equal(TREE, OBJ1, OBJ2) ((OBJ1).data == (OBJ2).data)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Greater(TREE, OBJ1, OBJ2) ((OBJ1).data > (OBJ2).data)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE_Less(TREE, OBJ1, OBJ2) ((OBJ1).data < (OBJ2).data)
#define CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE
CUTILS_CONTAINER_RB_TREE_DEFINE(BPlus, int16_t, Key, CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER, CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR, CUTILS_CONTAINER_BPLUS_RB_TREE_COMPARE)

#define CUTILS_CONTAINER_BPLUS_STATIC_LIST_REFERENCER_InvalidId (-1)
#define CUTILS_CONTAINER_BPLUS_STATIC_LIST_REFERENCER CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER
CUTILS_CONTAINER_STATIC_LIST_DEFINE(BPlusIndex, int16_t, BPlusIndexElement, CUTILS_CONTAINER_BPLUS_STATIC_LIST_REFERENCER, 1)
CUTILS_CONTAINER_STATIC_LIST_DEFINE(BPlusLeaf, int16_t, BPlusLeafElement, CUTILS_CONTAINER_BPLUS_STATIC_LIST_REFERENCER, 1)


#ifndef CUTILS_CONTAINER_BPLUS_TREE_MODE_DISK

BPlusEntry* BPlusEntryReference(BPlusTree* tree, BPlusEntryId entry_id) {
    if (entry_id == kBPlusEntryInvalidId) {
        return NULL;
    }
    return (BPlusEntry*)entry_id;
}

void BPlusEntryDereference(BPlusTree* tree, BPlusEntry* entry) {

}

BPlusEntryId BPlusEntryCreate(BPlusTree* tree, BPlusEntryType type) {
    size_t size;
    if (type == kBPlusEntryIndex) {
        size = (tree->index_m - 1) * sizeof(BPlusIndexElement);
    }
    else {
        size = (tree->leaf_m - 1) * sizeof(BPlusLeafElement);
    }
    BPlusEntryId entry_id = (BPlusEntryId)MemoryAlloc(sizeof(BPlusEntry) + size);
    BPlusEntry* entry = BPlusEntryReference(tree, entry_id);
    entry->type = type;
    entry->element_count = 0;
    BPlusEntryDereference(tree, entry);
	BPlusRbTreeInit(&entry->rb_tree);
    if (type == kBPlusEntryIndex) {
        BPlusIndexStaticListInit(&entry->index.element_space, tree->index_m - 1);
    }
    else {
        BPlusLeafStaticListInit(&entry->leaf.element_space, tree->leaf_m - 1);
    }
    return entry_id;
}

void BPlusEntryDelete(BPlusTree* tree, BPlusEntryId entry_id) {
    MemoryFree((void*)entry_id);
}

void BPlusElementSet(BPlusTree* tree, BPlusEntry* entry, int16_t element_id, BPlusElement* element) {
    if (entry->type == kBPlusEntryLeaf) {
        entry->leaf.element_space.obj_arr[element_id].key = element->leaf.key;
        entry->leaf.element_space.obj_arr[element_id].value = element->leaf.value;
    }
    else if (entry->type == kBPlusEntryIndex) {
        entry->index.element_space.obj_arr[element_id].key = element->index.key;
        entry->index.element_space.obj_arr[element_id].child_id = element->index.child_id;
    }
}

#endif // CUTILS_CONTAINER_BPLUS_TREE_DISK


BPlusElement* BPlusElementGet(BPlusTree* tree, BPlusEntry* entry, int16_t element_id) {
      assert(element_id >= 0);
    if (entry->type == kBPlusEntryLeaf) {
        return (BPlusElement*)&entry->leaf.element_space.obj_arr[element_id];
    }
    else {
        return (BPlusElement*)&entry->index.element_space.obj_arr[element_id];
    }
}

BPlusEntryId BPlusElementGetChildId(BPlusTree* tree, const BPlusEntry* index, int16_t element_id) {
    if (element_id == -1) {
        return index->index.tail_child_id;
    }
    return index->index.element_space.obj_arr[element_id].child_id;
}

void BPlusElementSetChildId(BPlusTree* tree, BPlusEntry* index, int16_t element_id, BPlusEntryId entry_id) {
    if (element_id == -1) {
        index->index.tail_child_id = entry_id;
        return;
    }
    index->index.element_space.obj_arr[element_id].child_id = entry_id;
}





BPlusElementPos* BPlusCursorCur(BPlusTree* tree, BPlusCursor* cursor) {
    if (cursor->level < 0) {
        return NULL;
    }
    return &cursor->stack.obj_arr[cursor->level];
}

BPlusElementPos* BPlusCursorUp(BPlusTree* tree, BPlusCursor* cursor) {
    if (cursor->level <= 0) {
        return NULL;
    }
    return &cursor->stack.obj_arr[--cursor->level];;
}

BPlusElementPos* BPlusCursorDown(BPlusTree* tree, BPlusCursor* cursor) {
    if (cursor->level + 1 >= cursor->stack.count) {
        return NULL;
    }
    return &cursor->stack.obj_arr[++cursor->level];
}

BPlusCursorStatus BPlusCursorFirst(BPlusTree* tree, BPlusCursor* cursor, Key* key) {
    BPlusCursorStackVectorInit(&cursor->stack, 8, false);
    cursor->stack.count = 0;
    cursor->stack.obj_arr = cursor->pos;
    cursor->level = -1;
    return BPlusCursorNext(tree, cursor, key);
}

void BPlusCursorRelease(BPlusTree* tree, BPlusCursor* cursor) {
    if (cursor->stack.capacity != 0) {
        //BPlusCursorStackVectorRelease(&cursor->stack);
    }
}

BPlusCursorStatus BPlusCursorNext(BPlusTree* tree, BPlusCursor* cursor, Key* key) {
    BPlusElementPos cur;
    BPlusElementPos* parent = BPlusCursorCur(tree, cursor);
    if (parent) {
        BPlusEntry* parent_entry = BPlusEntryReference(tree, parent->entry_id);
        if (parent_entry->type == kBPlusEntryLeaf) {
            return kBPlusCursorEnd;
        }
        cur.entry_id = BPlusElementGetChildId(tree, parent_entry, parent->element_idx);
        BPlusEntryDereference(tree, parent_entry);
    }
    else {
        cur.entry_id = tree->root_id;
    }

    BPlusEntry* cur_entry = BPlusEntryReference(tree, cur.entry_id);
    int8_t cmp_status = -1;
    if (cur_entry->element_count > 0) {
        cur.element_idx = BPlusRbTreeIteratorLocate(&cur_entry->rb_tree, key, &cmp_status);
        if (cmp_status == -1) { /* key小于当前定位元素 */ }
        else { /* key大于等于当前定位元素 */
            if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) {
                cur.element_idx = BPlusRbTreeIteratorNext(&cur_entry->rb_tree, cur.element_idx);
            }
        } 
    }
    else {
        cur.element_idx = -1;
    }
    BPlusCursorStackVectorPushTail(&cursor->stack, &cur);
    BPlusCursorStatus status = kBPlusCursorNext;
    if (cur_entry->type == kBPlusEntryLeaf) {
        if (cmp_status != 0) {
            status = kBPlusCursorNe;
        }
        else {
            status = kBPlusCursorEq;
        }
        cursor->leaf_status = status;
    }
    ++cursor->level;
    BPlusEntryDereference(tree, cur_entry);
    return status;
}



static int16_t BPlusAllocElement(BPlusTree* tree, BPlusEntry* entry) {
    int16_t element_id;
    if (entry->type == kBPlusEntryLeaf) {
        element_id = BPlusLeafStaticListPop(&entry->leaf.element_space, 0);
    }
    else {
        element_id = BPlusIndexStaticListPop(&entry->index.element_space, 0);
    }
      assert(element_id >= 0);
    return element_id;
}
static BPlusElement* BPlusFreeElement(BPlusTree* tree, BPlusEntry* entry, int16_t element_id) {
      assert(element_id >= 0);
    if (entry->type == kBPlusEntryLeaf) {
        BPlusLeafStaticListPush(&entry->leaf.element_space, 0, element_id);
        return (BPlusElement*)&entry->leaf.element_space.obj_arr[element_id];
    }
    else {
        BPlusIndexStaticListPush(&entry->index.element_space, 0, element_id);
        return (BPlusElement*)&entry->index.element_space.obj_arr[element_id];
    }
}

/*
* 孩子关系对应如下
*         3     |    6    |    8
*      /          /        /       \
*   1|2         4|5       7        10|11
* 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
*/

/*
* 向元素数组指定位置插入元素
* 不支持处理末尾子节点
*/
static int16_t BPlusInsertElement(BPlusTree* tree, BPlusEntry* entry, BPlusElement* insert_element) {
    int16_t element_id = BPlusAllocElement(tree, entry);
      assert(element_id != -1);
    BPlusElementSet(tree, entry, element_id, insert_element);
    BPlusRbTreePut(&entry->rb_tree, element_id);
    entry->element_count++;
    return element_id;
}

/*
* 从元素数组中删除指定索引的元素
* 不支持处理末尾子节点
* 返回被删除的元素
*/
static BPlusElement* BPlusDeleteElement(BPlusTree* tree, BPlusEntry* entry, int16_t element_id) {
      assert(element_id != -1);
    BPlusRbTreeDelete(&entry->rb_tree, element_id);
    entry->element_count--;
    return BPlusFreeElement(tree, entry, element_id);
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
static BPlusElement BPlusSplitEntry(BPlusTree* tree, BPlusEntry* left, BPlusEntryId left_id, BPlusEntry* parent, int16_t parent_element_id, BPlusElement* insert_element, int16_t insert_id, BPlusEntryId* out_right_id) {
    //  assert(insert_id != -1);
    BPlusEntryId right_id = BPlusEntryCreate(tree, left->type);
    BPlusEntry* right = BPlusEntryReference(tree, right_id);
    BPlusElement up_element;
    int mid;
    int right_count;
    if (left->type == kBPlusEntryLeaf) {
        BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
        // 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
        // 就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧
        mid = tree->leaf_m / 2;
        right_count = left->element_count + 1 - mid;        // +1是因为这个时候entry->count并没有把未插入元素也算上
    }
    else {
        // 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了
        // 先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧
        mid = (tree->index_m - 1) / 2;
        right_count = left->element_count - mid;        // 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入
    }
    int i = right_count - 1;
    int16_t left_elemeng_id = BPlusRbTreeIteratorLast(&left->rb_tree);
    bool insert = false;
    for (; i >= 0; i--) {
        if (!insert && left_elemeng_id == insert_id) {
            BPlusInsertElement(tree, right, insert_element);
            insert = true;
            continue;
        }
          assert(left_elemeng_id != CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_InvalidId);
        int16_t next_elemeng_id = BPlusRbTreeIteratorPrev(&left->rb_tree, left_elemeng_id);
        BPlusInsertElement(tree, right, BPlusDeleteElement(tree, left, left_elemeng_id));
        left_elemeng_id = next_elemeng_id;
    }
    // 新元素还没有插入，将其插入
    if (!insert) {
        BPlusInsertElement(tree, left, insert_element);
    }

    if (left->type == kBPlusEntryLeaf) {
        // 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点，
        up_element = *BPlusElementGet(tree, right, BPlusRbTreeIteratorFirst(&right->rb_tree));
        Key key = up_element.leaf.key;
        up_element.index.key = key;
    }
    else {
        // 假设如下节点需要分裂
        //           15            18
        //         /                |
        //   2   4   8   12        ...
        //   |   |   |   |    \
        //   1   3   5   10   13   
        // ---------------------------
        //   2   4          8   12
        //   |   |   \      |   |   \
        //   1   3   13     5   10   
        // ---------------------------
        //         4       15              18
        //      /          |               |
        //   2             8   12
        //   |  \          |   |   \
        //   1   3         5   10    13
        // 此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下
        right->index.tail_child_id = left->index.tail_child_id;

        // 最后从左节点末尾拿到上升元素，将其摘除
        up_element = *BPlusDeleteElement(tree, left, BPlusRbTreeIteratorLast(&left->rb_tree));
        left->index.tail_child_id = up_element.index.child_id;       // 3指定为2的右侧子节点
    }

    // 上升的4的子节点为左
    up_element.index.child_id = left_id;
    
    // 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点)
    BPlusElementSetChildId(tree, parent, parent_element_id, right_id);

    *out_right_id = right_id;
    BPlusEntryDereference(tree, right);
    return up_element;
}

/*
* 合并节点
* 释放右节点，返回时右节点不再有效
*   2
* 1   3
*/
static void BPlusMergeEntry(BPlusTree* tree, BPlusEntry* left, BPlusEntryId left_id, BPlusEntry* right, BPlusEntryId right_id, BPlusEntry* parent, int16_t parent_index) {
    int16_t right_elemeng_id = BPlusRbTreeIteratorLast(&right->rb_tree);
    for (int i = 0; i < right->element_count; i++) {
          assert(right_elemeng_id != CUTILS_CONTAINER_BPLUS_RB_TREE_REFERENCER_InvalidId);
        BPlusInsertElement(tree, left, BPlusElementGet(tree, right, right_elemeng_id));
        right_elemeng_id = BPlusRbTreeIteratorPrev(&right->rb_tree, right_elemeng_id);
    }
    if (left->type == kBPlusEntryLeaf) {
        BPlusLeafListDeleteEntry(&tree->leaf_list, right_id);
        // 是叶子节点，将right并入left中，并删除父元素
    }
    else {
        // 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素
        int16_t left_element_id = BPlusInsertElement(tree, left, BPlusElementGet(tree, parent, parent_index));
        BPlusElementSetChildId(tree, left, left_element_id, left->index.tail_child_id);       // left的末尾元素此时为下降的父元素，修改其子节点
        BPlusElementSetChildId(tree, left, -1, right->index.tail_child_id);
    }

    // 父元素会被删除，父右兄弟元素的左侧子节点(或尾子节点)原先指向右节点(被删除)，此时继承左节点(合并后剩下的节点)
    BPlusElementSetChildId(tree, parent, BPlusRbTreeIteratorNext(&parent->rb_tree, parent_index), left_id);
    BPlusEntryDereference(tree, right);
    BPlusEntryDelete(tree, right_id);
    // 合并部分完成，删除部分交给调用者
}



/*
* 向节点插入元素
*/
bool BPlusInsertEntry(BPlusTree* tree, BPlusCursor* cursor, BPlusElement* insert_element) {
    BPlusElementPos* cur_pos = BPlusCursorCur(tree, cursor);
    BPlusElementPos* parent_pos = BPlusCursorUp(tree, cursor);
    BPlusEntryId right_id;
    BPlusEntry* cur = BPlusEntryReference(tree, cur_pos->entry_id);
    bool success = true, insert_up = false;
    BPlusElement up_element;
    do {
        if (cursor->leaf_status == kBPlusCursorEq) {
            BPlusElementSet(tree, cur, cur_pos->element_idx, insert_element);
            break;
        }
        uint32_t m = cur->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (cur->element_count < m - 1) {
            // 有空余的位置插入
            BPlusInsertElement(tree, cur, insert_element);
            break;
        }
        // 没有多余位置，需要分裂向上插入，插入的位置需要是第一个小于key的元素，element_idx指向第一个大于key的元素
        if (cur_pos->element_idx == -1) { // 不存在大于key的元素，直接拿末尾元素
            cur_pos->element_idx = BPlusRbTreeIteratorLast(&cur->rb_tree);
        } else {
            cur_pos->element_idx = BPlusRbTreeIteratorPrev(&cur->rb_tree, cur_pos->element_idx);
        }
        if (!parent_pos) {
            // 没有父节点，创建
            BPlusEntryId parent_id = BPlusEntryCreate(tree, kBPlusEntryIndex);
            BPlusEntry* parent = BPlusEntryReference(tree, parent_id);
            up_element = BPlusSplitEntry(tree, cur, cur_pos->entry_id, parent, -1, insert_element, cur_pos->element_idx, &right_id);
            BPlusInsertElement(tree, parent, &up_element);
            tree->root_id = parent_id;
            BPlusEntryDereference(tree, parent);
            break;
        }
        BPlusEntry* parent = BPlusEntryReference(tree, parent_pos->entry_id);
        up_element = BPlusSplitEntry(tree, cur, cur_pos->entry_id, parent, parent_pos->element_idx, insert_element, cur_pos->element_idx, &right_id);
        BPlusEntryDereference(tree, parent);
        insert_up = true;
    } while (false);
    BPlusEntryDereference(tree, cur);
    if (insert_up) {
        return BPlusInsertEntry(tree, cursor, &up_element);
    }
    return success;
}

/*
* 删除指定节点的元素
*/
bool BPlusDeleteEntry(BPlusTree* tree, BPlusCursor* cursor) {
    BPlusElementPos* cur_pos = BPlusCursorCur(tree, cursor);
    BPlusElementPos* parent_pos = BPlusCursorUp(tree, cursor);
    BPlusEntry* entry = BPlusEntryReference(tree, cur_pos->entry_id);
    BPlusEntryId sibling_entry_id = 0;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, delete_up = false;
    BPlusDeleteElement(tree, entry, cur_pos->element_idx);      // 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了
    do {
        uint32_t m = entry->type == kBPlusEntryIndex ? tree->index_m : tree->leaf_m;
        if (entry->element_count >= (m - 1) / 2) {
            break;
        }
        if (!parent_pos) {
            // 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束
            if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
                BPlusEntryId childId = entry->index.tail_child_id;
                tree->root_id = childId;
                BPlusEntryDereference(tree, entry);
                BPlusEntryDelete(tree, cur_pos->entry_id);
                return true;
            }
            else {
                break;
            }
        }
        parent = BPlusEntryReference(tree, parent_pos->entry_id);
        bool left_sibling = true;
        int16_t common_parent_element_id = parent_pos->element_idx;     // 始终是指向左侧元素
        int16_t sibling_element_id;
        if (common_parent_element_id == -1) { // 不存在大于key的父元素，当前节点是末尾节点，是左兄弟
            sibling_element_id = BPlusRbTreeIteratorLast(&parent->rb_tree);
        }
        else {
            sibling_element_id = BPlusRbTreeIteratorPrev(&parent->rb_tree, common_parent_element_id);
            if (sibling_element_id == -1) {     // 当前元素已经是最小的元素
                left_sibling = false;
                sibling_element_id = BPlusRbTreeIteratorNext(&parent->rb_tree, common_parent_element_id);
                if (sibling_element_id == -1) { // 直接找既没有左兄弟也没有右兄弟，那就是末尾节点是右兄弟
                    sibling_entry_id = parent->index.tail_child_id;
                }
            }
        }
        if (sibling_element_id != -1) {
            sibling_entry_id = parent->index.element_space.obj_arr[sibling_element_id].child_id;
        }
        if (left_sibling) {
            common_parent_element_id = sibling_element_id;
            parent_pos->element_idx = sibling_element_id;     // 更新一下，给父节点删除使用
        }
          assert(common_parent_element_id != -1);
          assert(sibling_entry_id != kBPlusEntryInvalidId);
        sibling = BPlusEntryReference(tree, sibling_entry_id);
        if (sibling->element_count > (m - 1) / 2) {
            // 向兄弟借节点
            if (entry->type == kBPlusEntryLeaf) {
                // 叶子节点处理较简单，可以直接移动
                if (left_sibling) {
                    // 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key
                    int16_t last = BPlusRbTreeIteratorLast(&sibling->rb_tree);
                      assert(last != -1);
                    BPlusElement* element = BPlusDeleteElement(tree, sibling, last);
                    BPlusInsertElement(tree, entry, element);
                    parent->index.element_space.obj_arr[common_parent_element_id].key = element->leaf.key;       // 更新索引
                }
                else {
                    // 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素
                    int16_t first = BPlusRbTreeIteratorFirst(&sibling->rb_tree);
                    int16_t new_first = BPlusRbTreeIteratorNext(&sibling->rb_tree, first);
                      assert(first != -1);
                      assert(new_first != -1);
                    BPlusElement* element = BPlusDeleteElement(tree, sibling, first);
                    BPlusInsertElement(tree, entry, element);
                    // 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的
                    parent->index.element_space.obj_arr[common_parent_element_id].key = sibling->leaf.element_space.obj_arr[new_first].key;       // 更新索引
                }
            }
            else {
                // 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换)
                if (left_sibling) {
                    // 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧
                    int16_t last = BPlusRbTreeIteratorLast(&sibling->rb_tree);
                      assert(last != -1);
                    BPlusElement* left_element = BPlusDeleteElement(tree, sibling, last);
                    ObjectSwap(BPlusEntryId, left_element->index.child_id, sibling->index.tail_child_id);        // 要拿的是末尾的子节点，处理一下
                    BPlusElement* par_element = BPlusDeleteElement(tree, parent, common_parent_element_id);
                    par_element->index.child_id = left_element->index.child_id;
                    BPlusInsertElement(tree, entry, par_element);
                    left_element->index.child_id = sibling_entry_id;
                    BPlusInsertElement(tree, parent, left_element);
                }
                else {
                    // 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧
                    int16_t first = BPlusRbTreeIteratorFirst(&sibling->rb_tree);
                      assert(first != -1);
                    BPlusElement* right_element = BPlusDeleteElement(tree, sibling, first);
                    BPlusElement* par_element = BPlusDeleteElement(tree, parent, common_parent_element_id);
                    par_element->index.child_id = right_element->index.child_id;
                    ObjectSwap(BPlusEntryId, par_element->index.child_id, entry->index.tail_child_id);        // 要插入的是末尾的子节点，处理一下
                    BPlusInsertElement(tree, entry, par_element);
                    right_element->index.child_id = cur_pos->entry_id;
                    BPlusInsertElement(tree, parent, right_element);
                }
            }
            break;
        }

        // 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素)
        if (left_sibling) {
            BPlusMergeEntry(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, common_parent_element_id);
            entry = NULL;
        }
        else {
            BPlusMergeEntry(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, common_parent_element_id);
            sibling = NULL;
        }
        delete_up = true;
    } while (false);
    if (parent) { BPlusEntryDereference(tree, parent); }
    if (sibling) { BPlusEntryDereference(tree, sibling); }
    if (entry) { BPlusEntryDereference(tree, entry); }

    if (delete_up) {
        return BPlusDeleteEntry(tree, cursor);     // 尾递归
    }
    return success;
}


/*
* 初始化B+树
*/
void BPlusTreeInit(BPlusTree* tree, uint32_t index_m, uint32_t leaf_m) {
    if (index_m < 3) {
        index_m = 3;      // 最少3阶，否则索引节点分裂会出现一侧没有节点的情况
    }
    if (leaf_m < 3) {       //2
        leaf_m = 3;     // 可以2阶，但是删除有地方需要加强判断
    }
    tree->index_m = index_m;
    tree->leaf_m = leaf_m;
    tree->root_id = BPlusEntryCreate(tree, kBPlusEntryLeaf);
    BPlusLeafListInit(&tree->leaf_list);
    BPlusLeafListPutFirst(&tree->leaf_list, tree->root_id);
}

/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(BPlusTree* tree, Key* key) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tree, &cursor, key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tree, &cursor, key);
    }
    return status == kBPlusCursorEq;
}

/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tree, &cursor, &element->key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tree, &cursor, &element->key);
    }
    bool success = BPlusInsertEntry(tree, &cursor, (BPlusElement*)element);
    BPlusCursorRelease(tree, &cursor);
    return success;
}

///*
//* 从B+树中删除指定key
//*/
bool BPlusTreeDelete(BPlusTree* tree, Key* key) {
    BPlusCursor cursor;
    BPlusCursorStatus status = BPlusCursorFirst(tree, &cursor, key);
    while (status == kBPlusCursorNext) {
        status = BPlusCursorNext(tree, &cursor, key);
    }
    if (status == kBPlusCursorNe) {
        return false;
    }
    bool success = BPlusDeleteEntry(tree, &cursor);
    return success;
}