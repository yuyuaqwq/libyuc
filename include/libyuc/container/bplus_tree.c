#include <libyuc/basic.h>


#include <libyuc/container/bplus_tree.def>

/*

entry：
    请确保1个entry至少能分配3个element

    Rate自行根据页面来指定
    比如页面是4096，就返回4096分比

    entry访问器需要提供
        GetMergeThresholdRate(获取合并阈值n分比，推荐为40%(但不是40，而是百分之40，需要根据n分比换算))
            大于该百分比的entry至少有2个element
        GetFreeRate(获取空闲n分比，可分配的最大空闲)
        GetFillRate(获取已填充n分比)
        GetMaxRate(获取n)
        GetTempCopyEntry(获取临时Entry，避免分配释放用)

element:
    基本element定长，附属kv可能不定长
    element访问器需要提供
        GetNeedRate(获取element需要的n分比)


插入时，element所需的空间不足插入新element时触发分裂
    分裂前，当前entry必须至少有2个element(叶子可以1个，索引必须2个，否则没有上升节点，统一规定2个)
    首先计算当前entry的填充率，再加上新element的总占用率再/2
    分裂的时候根据填充率进行分裂，使得被插入的一侧分裂后能有足够的空位分配并插入新元素
    value最大占用25%, key最大占用12.5%
        简化实现，可以是2的次幂，entry至少要2个element, 因此必须限制单个element总占用率不能超过总页面的50%，需要确保每次分裂后都一定能装下新元素
        过大的value就分配独立页面
    分裂思路：倒着遍历节点并摘除，移动的时候还需要插入element的参与
        一直释放旧，插入新，一直到旧entry填充率<=1/2就停止分裂
        如果此时需要插入element，直接插入到新的一侧，等到旧entry填充率达到标准即返回
        如果旧entry填充率<=1/2，还没有插入element
            检查旧entry空位是否足够插入element，不足就触发碎片整理，再插入


删除时，两边填充率<=40%(两边加起来<=80%)则触发合并
    由于两侧都是40%，一定能够合并成功(并不一定，有可能有空间碎片问题)
一边填充率<=40%(两边加起来>80%)则触发element移动
    element一定可以移动，因为单个element最大是25%(并不一定，有可能有空间碎片)
    如果移动一个之后还不足40%(就继续移动)
    可能会出现移动之后兄弟entry的填充率不足40%(比如末尾直接是25%占用率的element)
        因此每次移动前判断移动后兄弟的填充率会变为多少，如果低于40%就不再移动，直接返回


碎片整理：
    如果出现空间碎片(即总空闲足够但无法分配)，就需要进行碎片整理
    碎片整理流程指导：
        buddy:
            按占用率从大到小重新分配所有block

    碎片整理问题：
        必须保证整理时b+树不存在任何引用element的情况，否则将会导致element_id失效
        b+树保证调用InsertElement的上下文中，不会存在引用entry中element的情况，故建议在InsertElement中进行碎片整理

    如果使用相等大小的块进行分配，逻辑上连续的块通过链表连接则不存在内存碎片
        缺点是大块的分配释放较慢


kv分离是外层处理的，b+树操作的只有element
碎片整理也在外边进行，当空闲位足够但分配失败时处理


*/

/*
* 不通过有序链表建树的方法，保留可能未来修改为art时使用
*/
//static BPlusElement BPlusEntrySplit(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* parent, int16_t parent_element_id, BPlusElement* insert_element, int16_t insert_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id* out_right_id) {
//    /* assert(insert_id != BPlusEntryRbReferencer_InvalidId); */
//    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id = BPlusEntryCreate(tree, left->type);
//    BPlusEntry* right = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
//    BPlusElement up_element;
//    int32_t mid;
//    int32_t right_count;
//    if (left->type == kBPlusEntryLeaf) {
//        //BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
//        /* 原地分裂思路：mid将未插入的元素也算上，好计算newCount，4阶插入后4节点就是2(左2右2)，5阶插入后5节点还是2(左2右3)
//            就是提前算好右侧应当有多少个元素，拷贝过去，中间遇到新元素插入就代替这一次的拷贝，没插入再插入到左侧 */
//        mid = tree->leaf_m / 2;
//        right_count = left->element_count + 1 - mid;        /* +1是因为这个时候entry->count并没有把未插入元素也算上 */
//    }
//    else {
//        /* 原地分裂思路：mid将未插入的元素和即将上升的元素都算上，好计算newCount，4阶插入后4节点就是4/2=2(左1右2)，5阶插入后5节点也是2(左2右2)，少了一个是因为上升的也算上了
//            先将后半部分拷贝到新节点，如果中间遇到了索引的插入，那就一并插入，最后的midkey是entry->indexData[entry->count-1]，因为右侧的数量是提前算好的，多出来的一定放到左侧 */
//        mid = (tree->index_m - 1) / 2;
//        right_count = left->element_count - mid;        /* 这个时候entry->count并没有把未插入元素也算上，但是会上升一个元素，抵消故不计入 */
//    }
//    int32_t i = right_count - 1;
//    int16_t left_elemeng_id = BPlusEntryRbTreeIteratorLast(&left->rb_tree);
//    bool insert = false;
//    for (; i >= 0; i--) {
//        if (!insert && left_elemeng_id == insert_id) {
//            BPlusEntryInsertElement(tree, right, insert_element);
//            insert = true;
//            continue;
//        }
//         assert(left_elemeng_id != BPlusEntryRbReferencer_InvalidId);
//        int16_t next_elemeng_id = BPlusEntryRbTreeIteratorPrev(&left->rb_tree, left_elemeng_id);
//        BPlusEntryInsertElement(tree, right, BPlusEntryDeleteElement(tree, left, left_elemeng_id));
//        left_elemeng_id = next_elemeng_id;
//    }
//    /* 新元素还没有插入，将其插入 */
//    if (!insert) {
//        BPlusEntryInsertElement(tree, left, insert_element);
//    }
//   
//    if (left->type == kBPlusEntryLeaf) {
//        /* 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点 */
//        up_element = *BPlusElementGet(tree, right, BPlusEntryRbTreeIteratorFirst(&right->rb_tree));
//        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key key = up_element.leaf.key;
//        up_element.index.key = key;
//    }
//    else {
//        /* 假设如下节点需要分裂
//                        15            18
//                    /                |
//                2     4     8     12        ...
//                |     |     |     |   
//                1     3     5     10     13     
//            ---------------------------
//                2     4            8     12
//                |     |            |     |    
//                1     3     13     5     10     
//            ---------------------------
//                    4         15                18
//                /            |                 |
//                2             8     12
//                |               |     |    
//                1     3         5     10    13
//            此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下 */
//        right->index.tail_child_id = left->index.tail_child_id;
//       
//        /* 最后从左节点末尾拿到上升元素，将其摘除 */
//        up_element = *BPlusEntryDeleteElement(tree, left, BPlusEntryRbTreeIteratorLast(&left->rb_tree));
//        left->index.tail_child_id = up_element.index.child_id;         /* 3指定为2的右侧子节点 */
//    }
//    /* 上升的4的子节点为左 */
//    up_element.index.child_id = left_id;
//   
//    /* 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点) */
//    BPlusElementSetChildId(tree, parent, parent_element_id, right_id);
//   
//    *out_right_id = right_id;
//    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
//    return up_element;
//}


#include <libyuc/container/bplus_tree_BPlusIteratorStackVector.cfg>
#include <libyuc/container/vector.c>

/*
* B+树迭代器
*/\
static const LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusLeafEntryReferencer_InvalidId = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId;



/*
* 页内红黑树
*/\
static const LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbReferencer_InvalidId = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
forceinline BPlusEntryRbEntry* BPlusEntryRbReferencer_Reference(BPlusEntryRbTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        return NULL;
    }
    BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
    return &(LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(entry, element_id)->rb_entry);
}
forceinline void BPlusEntryRbReferencer_Dereference(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry) {
    BPlusEntry* entry = ObjectGetFromField(tree, BPlusEntry, rb_tree);
    BPlusElement* element = ObjectGetFromField(rb_entry, BPlusElement, rb_entry);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(entry, element);
}

typedef struct {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left : sizeof(LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id) * 8 - 1;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset color : 1;
} BPlusEntryRbParentColor;

forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbAccessor_GetLeft(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry) {
    return ((BPlusEntryRbParentColor*)rb_entry)->left;
}
forceinline BPlusEntryRbColor BPlusEntryRbAccessor_GetColor(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry) {
    return ((BPlusEntryRbParentColor*)rb_entry)->color;
}
forceinline void BPlusEntryRbAccessor_SetLeft(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_id) {
    ((BPlusEntryRbParentColor*)rb_entry)->left = new_id;
}
forceinline void BPlusEntryRbAccessor_SetColor(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry, BPlusEntryRbColor new_color) {
    ((BPlusEntryRbParentColor*)rb_entry)->color = new_color;
}
forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryRbAccessor_GetRight(BPlusEntryRbTree* tree, BPlusEntryRbEntry* bs_entry) {
    return (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id)bs_entry->right;
}
forceinline void BPlusEntryRbAccessor_SetRight(BPlusEntryRbTree* tree, BPlusEntryRbEntry* rb_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_id) {
    rb_entry->right = new_id;
}
forceinline LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* BPlusEntryRbAccessor_GetKey(BPlusEntryRbTree* tree, BPlusEntryRbEntry* bs_entry) {
    BPlusElement* element = ObjectGetFromField(bs_entry, BPlusElement, rb_entry);
    return &element->key;
}


#include <libyuc/container/bplus_tree_BPlusEntryRbTree.cfg>
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference BPlusEntryRbReferencer_Reference
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference BPlusEntryRbReferencer_Dereference
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft BPlusEntryRbAccessor_GetLeft
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight BPlusEntryRbAccessor_GetRight
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor BPlusEntryRbAccessor_GetColor
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetLeft BPlusEntryRbAccessor_SetLeft
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetRight BPlusEntryRbAccessor_SetRight
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor BPlusEntryRbAccessor_SetColor
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetKey BPlusEntryRbAccessor_GetKey
#include <libyuc/container/rb_tree.c>



/*
* B+树
*/
static void BPlusElementSet(BPlusEntry* dst_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id, BPlusEntry* src_entry, BPlusElement* element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id element_child_id) {
    assert(element_id >= 0);
    BPlusElement* dst_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(dst_entry, element_id);
    if (dst_entry->type == kBPlusEntryLeaf) {
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->leaf.key);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetValue(dst_entry, dst_element, src_entry, &element->leaf.value);
    }
    else {
        /* 如果是插入到index_entry，可能是来自leaf的element */
        if (src_entry->type == kBPlusEntryLeaf) {
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->leaf.key);
            assert(element_child_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
            dst_element->index.child_id = element_child_id;
        }
        else {
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetKey(dst_entry, dst_element, src_entry, &element->index.key);
            /* 如果有附带的孩子id，则使用附带的孩子id */
            if (element_child_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId) {
                dst_element->index.child_id = element_child_id;
            }
            else {
                dst_element->index.child_id = element->index.child_id;
            }
        }
    }
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(dst_entry, dst_element);
}
static LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusElementGetChildId(const BPlusEntry* index, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        return index->index.tail_child_id;
    }
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(index, element_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id child_id = element->index.child_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(index, element);
    return child_id;
}
static void BPlusElementSetChildId(BPlusEntry* index, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id) {
    if (element_id == BPlusEntryRbReferencer_InvalidId) {
        index->index.tail_child_id = entry_id;
        return;
    }
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(index, element_id);
    element->index.child_id = entry_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(index, element);
}
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusElementCreate(BPlusEntry* entry) {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_CreateBySize(entry, entry->type == kBPlusEntryLeaf ? sizeof(BPlusLeafElement) : sizeof(BPlusIndexElement));
      assert(element_id >= 0);
    return element_id;
}
static BPlusElement* BPlusElementRelease(BPlusEntry* entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id) {
      assert(element_id >= 0);
    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(entry, element_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(entry, element);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ALLOCATOR_Release(entry, element_id);
    return element;
}


/* B+树迭代器 */
BPlusIteratorStatus BPlusIteratorTop(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIteratorStackVectorInit(&iterator->stack/*, STACK_SIZE*/);
    iterator->stack.count = 0;
    iterator->level = -1;
    return BPlusIteratorDown(tree, iterator, key);
}
void BPlusIteratorRelease(BPlusTree* tree, BPlusIterator* iterator) {
    BPlusIteratorStackVectorRelease(&iterator->stack);
}
BPlusElementPos* BPlusIteratorCur(BPlusTree* tree, BPlusIterator* iterator) {
    if (iterator->level < 0) {
        return NULL;
    }
    return &iterator->stack.obj_arr[iterator->level];
}
BPlusElementPos* BPlusIteratorUp(BPlusTree* tree, BPlusIterator* iterator) {
    if (iterator->level <= 0) {
        return NULL;
    }
    return &iterator->stack.obj_arr[--iterator->level];
}
BPlusIteratorStatus BPlusIteratorDown(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusElementPos cur;
    BPlusElementPos* parent = BPlusIteratorCur(tree, iterator);
    if (parent) {
        BPlusEntry* parent_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent->entry_id);
        if (parent_entry->type == kBPlusEntryLeaf) {
            return kBPlusIteratorEnd;
        }
        cur.entry_id = BPlusElementGetChildId(parent_entry, parent->element_iterator.cur_id);
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent_entry);
    }
    else {
        cur.entry_id = tree->root_id;
    }
    BPlusEntry* cur_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur.entry_id);
    ptrdiff_t cmp_status = -1;
    if (cur_entry->element_count > 0) {
        BPlusEntryRbTreeIteratorLocate(&cur_entry->rb_tree, &cur.element_iterator, key, &cmp_status);
        if (cmp_status < 0) { /* key小于当前定位元素 */ }
        else { /* key大于等于当前定位元素 */
            if (cur_entry->type == kBPlusEntryIndex || cmp_status > 0) {
                BPlusEntryRbTreeIteratorNext(&cur_entry->rb_tree, &cur.element_iterator);
            }
        }
    }
    else {
        cur.element_iterator.cur_id = BPlusEntryRbReferencer_InvalidId;
    }
    BPlusIteratorStackVectorPushTail(&iterator->stack, &cur);
    BPlusIteratorStatus status = kBPlusIteratorDown;
    if (cur_entry->type == kBPlusEntryLeaf) {
        if (cmp_status != 0) {
            status = kBPlusIteratorNe;
        }
        else {
            status = kBPlusIteratorEq;
        }
        iterator->leaf_status = status;
    }
    ++iterator->level;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, cur_entry);
    return status;
}


/*
* 孩子关系对应如下
*         3     |    6    |    8
*        /            /        /
*     1|2         4|5         7        10|11
* 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
*/
/*
* 向节点插入元素
* 不支持处理末尾子节点
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntryInsertElement(BPlusEntry* dst_entry, BPlusEntry* src_entry, BPlusElement* insert_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id element_child_id) {
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id element_id = BPlusElementCreate(dst_entry);
      assert(element_id != BPlusEntryRbReferencer_InvalidId);
    BPlusElementSet(dst_entry, element_id, src_entry, insert_element, element_child_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id old_element_id = BPlusEntryRbTreePut(&dst_entry->rb_tree, element_id);
    if (old_element_id != BPlusEntryRbReferencer_InvalidId && old_element_id != element_id) BPlusElementRelease(dst_entry, old_element_id);
    dst_entry->element_count++;
    return element_id;
}
/*
* 从节点中删除指定的元素
* 不支持处理末尾子节点
* 返回被删除的元素
*/
static void BPlusEntryDeleteElement(BPlusEntry* entry, BPlusEntryRbTreeIterator* element_iterator) {
    assert(element_iterator->cur_id != BPlusEntryRbReferencer_InvalidId);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id del_element_id = element_iterator->cur_id;
    BPlusEntryRbTreeDeleteByIterator(&entry->rb_tree, element_iterator);
    entry->element_count--;
    BPlusElementRelease(entry, del_element_id);
}

void BPlusEntryInit(BPlusTree* tree, BPlusEntry* entry, BPlusEntryType type) {
    entry->type = type;
    entry->element_count = 0;
    BPlusEntryRbTreeInit(&entry->rb_tree);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_InitCallback(tree, entry);
}
LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id BPlusEntryCreate(BPlusTree* tree, BPlusEntryType type) {
    size_t size;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id;
    if (type == kBPlusEntryIndex) {
        entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_CreateBySize(tree, sizeof(BPlusEntry) - sizeof(BPlusLeafEntry) + sizeof(BPlusIndexEntry));
    }
    else {
        entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_CreateBySize(tree, sizeof(BPlusEntry) - sizeof(BPlusIndexEntry) + sizeof(BPlusLeafEntry));
    }
    BPlusEntry* entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, entry_id);
    BPlusEntryInit(tree, entry, type);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry);
    return entry_id;
}
void BPlusEntryRelease(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id) {
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ALLOCATOR_Release(tree, entry_id);
}

/*
* 有序链表建树，参考https://leetcode.cn/problems/convert-sorted-list-to-binary-search-tree/solution/you-xu-lian-biao-zhuan-huan-er-cha-sou-suo-shu-1-3/
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BuildRbTree(BPlusEntry* src_entry, BPlusEntryRbTreeIterator* element_iterator, BPlusEntry* dst_entry, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id right, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id parent_id, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset max_level, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset level) {
    if (left > right) return LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id mid = (left + right + 1) / 2;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id new_element_id = BPlusElementCreate(dst_entry);
    BPlusElement* new_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(dst_entry, new_element_id);
    BPlusEntryRbEntry* rb_entry = &new_element->rb_entry;
    BPlusEntryRbAccessor_SetColor(&dst_entry->rb_tree, rb_entry, level == max_level && max_level >= 1 ? kRbRed : kRbBlack);
    BPlusEntryRbAccessor_SetLeft(&dst_entry->rb_tree, rb_entry, BuildRbTree(src_entry, element_iterator, dst_entry, left, mid - 1, new_element_id, max_level, level + 1));
    assert(element_iterator->cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    BPlusElement* src_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, element_iterator->cur_id);
    BPlusElementSet(dst_entry, new_element_id, src_entry, src_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(src_entry, src_element);
    BPlusEntryRbTreeIteratorNext(&src_entry->rb_tree, element_iterator);
    BPlusEntryRbAccessor_SetRight(&dst_entry->rb_tree, rb_entry, BuildRbTree(src_entry, element_iterator, dst_entry, mid + 1, right, new_element_id, max_level, level + 1));
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(dst_entry, new_element);
    return new_element_id;
}
/*
* 分裂节点
* 返回上升的索引元素
*
* 同一个叶节点多次分裂会导致重复的key上升吗？
* 如果叶节点不存在相同的key，那么是不会的
* 因为分裂后选择右节点的最左元素的key作为上升的key
* 这个时候无论怎么插入元素都不会插入到该节点最左元素的左侧(比它小的会被分到左侧节点，因为父元素key等于该元素)，该节点再分裂也就不存在最左元素再次上升的可能了
*
* 会修改src_entry指向新的right_entry，但不会释放原来的src_entry
*/
static LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id BPlusEntrySplit(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* parent, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id parent_element_id, BPlusEntry** src_entry, BPlusElement* insert_element, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id insert_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id insert_element_child_id, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id* out_right_id, BPlusEntryRbTreeIterator* up_element_iterator) {
    /* assert(insert_id != BPlusEntryRbReferencer_InvalidId); */
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id = BPlusEntryCreate(tree, (BPlusEntryType)left->type);
    BPlusEntry* right = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id up_element_id;
    if (left->type == kBPlusEntryLeaf) {
        //BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
    }
    BPlusEntryRbTreeIterator left_element_iterator;
    BPlusEntryRbTreeIteratorLast(&left->rb_tree, &left_element_iterator);
    int insert_right = 0;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset fill_rate = (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, left) + LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(*src_entry, insert_element)) / 2;
    /*
    * 计算出右侧两侧各自的节点数量
    */
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset right_count = 1, left_count = 0;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset left_fill_rate = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, left);
    while (true) {
        if (!insert_right && left_element_iterator.cur_id == insert_id) {
            insert_right = 1;
        }
        if (left_fill_rate <= fill_rate || left->element_count - right_count <= 2 + insert_right) {
            break;
        }
        left_fill_rate -= LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(*src_entry, insert_element);
        BPlusEntryRbTreeIteratorPrev(&left->rb_tree, &left_element_iterator);
        ++right_count;
    }
    assert(left_element_iterator.cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    assert(right_count > 0);
    /* 先构建右侧节点的rb树 */
    BPlusEntryRbTreeIterator mid_element_iterator;
    BPlusEntryRbTreeIteratorCopy(&mid_element_iterator, &left_element_iterator);
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset logn = 0;
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = right_count; i > 0; i /= 2)
        ++logn;
    --logn;
    right->rb_tree.root = BuildRbTree(left, &left_element_iterator, right, 0, right_count - 1, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId, logn, 0);

    /* 接下来是左侧 */
    /* 拷贝到临时节点，在原节点中重构rb树 */
    BPlusEntry* temp_entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetTempCopyEntry(tree, left);
    BPlusEntryInit(tree, left, (BPlusEntryType)right->type);
    do {
        BPlusEntryRbTreeIteratorPrev(&temp_entry->rb_tree, &mid_element_iterator);
        ++left_count;
    } while (mid_element_iterator.cur_id != LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId);
    --left_count;
    BPlusEntryRbTreeIteratorFirst(&temp_entry->rb_tree, &left_element_iterator);
    logn = 0;
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = left_count; i > 0; i /= 2)
        ++logn;
    --logn;
    left->rb_tree.root = BuildRbTree(temp_entry, &left_element_iterator, left, 0, left_count - 1, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId, logn, 0);

    /* 最终插入节点 */
    if (insert_right == 1) {
        BPlusEntryInsertElement(right, *src_entry, insert_element, insert_element_child_id);
        ++right_count;
    }
    else {
        BPlusEntryInsertElement(left, *src_entry, insert_element, insert_element_child_id);
        ++left_count;
    }
    left->element_count = left_count;
    right->element_count = right_count;

    BPlusElement* up_element;
    if (left->type == kBPlusEntryLeaf) {
        /* 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点，叶子上升不需要被摘除 */
        *src_entry = right;
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, right_id);
        up_element_id = BPlusEntryRbTreeIteratorFirst(&right->rb_tree, up_element_iterator);
        up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
    }
    else {
        /* 假设如下节点需要分裂
                    15             18
                    /                |
                2     4     8     12        ...
                |     |     |     |
                1     3     5     10     13
            ---------------------------
                2     4              8     12
                |     |            |     |
                1     3     13       5     10
            ---------------------------
                    4             15                18
                /              |                 |
                2               8     12
                |             |     |
                1     3         5     10    13
            此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下 */
        right->index.tail_child_id = left->index.tail_child_id;

        /* 最后从左节点末尾拿到上升元素，等待将其摘除 */
        up_element_id = BPlusEntryRbTreeIteratorLast(&left->rb_tree, up_element_iterator);
        *src_entry = left;
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, left_id); /* 累加一次计数 */
        up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(*src_entry, up_element_id);
        left->index.tail_child_id = up_element->index.child_id;         /* 3指定为2的右侧子节点 */
    }
    /* 上升的4的子节点为左 */
    /*up_element->index.child_id = left_id;     up_element可能是leaf的元素，不能直接在此处赋值 */

    /* 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点) */
    BPlusElementSetChildId(parent, parent_element_id, right_id);

    *out_right_id = right_id;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
    assert(left->element_count >= 1);
    assert(right->element_count >= 1);
    return up_element_id;
}
/*
* 合并节点
* 释放右节点，返回时右节点不再有效
*     2
* 1     3
*/
static void BPlusEntryMerge(BPlusTree* tree, BPlusEntry* left, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id left_id, BPlusEntry* right, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id, BPlusEntry* parent, BPlusEntryRbTreeIterator* parent_iterator) {
    BPlusEntryRbTreeIterator element_iterator;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id right_element_id = BPlusEntryRbTreeIteratorLast(&right->rb_tree, &element_iterator);
    for (LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset i = 0; i < right->element_count; i++) {
        assert(right_element_id != BPlusEntryRbReferencer_InvalidId);
        BPlusElement* right_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(right, right_element_id);
        BPlusEntryInsertElement(left, right, right_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(right, right_element);
        right_element_id = BPlusEntryRbTreeIteratorPrev(&right->rb_tree, &element_iterator);
    }
    if (left->type == kBPlusEntryLeaf) {
        //BPlusLeafListDeleteEntry(&tree->leaf_list, right_id);
        /* 是叶子节点，将right并入left中，并删除父元素 */
    }
    else {
        /* 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素 */
        BPlusElement* parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, parent_iterator->cur_id);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id left_element_id = BPlusEntryInsertElement(left, parent, parent_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, parent_element);
        BPlusElementSetChildId(left, left_element_id, left->index.tail_child_id);         /* left的末尾元素此时为下降的父元素，修改其子节点 */
        BPlusElementSetChildId(left, -1, right->index.tail_child_id);
    }

    /* 父元素会被删除，父右兄弟元素的左侧子节点(或尾子节点)原先指向右节点(被删除)，此时继承左节点(合并后剩下的节点) */
    BPlusElementSetChildId(parent, BPlusEntryRbTreeIteratorNext(&parent->rb_tree, parent_iterator), left_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, right);
    BPlusEntryRelease(tree, right_id);
    /* 合并部分完成，删除部分交给调用者 */
}

/*
* 向树插入元素
* 会抢夺src_entry的所有权
*/
static bool BPlusTreeInsertElement(BPlusTree* tree, BPlusIterator* iterator, BPlusEntry* src_entry, BPlusElement* insert_element, BPlusEntryRbTreeIterator* insert_element_iterator, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id insert_element_child_id) {
    BPlusElementPos* cur_pos = BPlusIteratorCur(tree, iterator);
    BPlusElementPos* parent_pos = BPlusIteratorUp(tree, iterator);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id right_id;
    BPlusEntry* old_src_entry = src_entry;
    BPlusEntry* cur = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur_pos->entry_id);
    bool success = true, insert_up = false;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id up_element_id = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId;
    BPlusEntryRbTreeIterator up_element_iterator;
    do {
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset free_rate = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFreeRate(tree, cur);
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset need_rate = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(old_src_entry, insert_element);
        if (iterator->leaf_status == kBPlusIteratorEq) {
            BPlusElement* raw = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(cur, cur_pos->element_iterator.cur_id);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset raw_rate = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(cur, raw);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(cur, raw);
            if (free_rate + need_rate >= raw_rate) {
                /* SetValue会先释放原有空间，因此空闲空间可以计入已分配的部分 */
                LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_SetValue(cur, raw, cur, &insert_element->leaf.value);
                break;
            }
            else {
                /* 空间不足将会触发分裂，这里什么都不做，在分裂时重新SetValue */
                assert(0);
            }
        }
        else if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFreeRate(tree, cur) >= LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_ACCESSOR_GetNeedRate(old_src_entry, insert_element)) {
            /* 有空余的位置插入 */
            BPlusEntryInsertElement(cur, old_src_entry, insert_element, insert_element_child_id);
            break;
        }
        else {
            /* 没有多余位置，需要分裂向上插入，插入的位置需要是第一个小于key的元素，element_id指向第一个大于key的元素 */
            if (cur_pos->element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的元素，直接拿末尾元素 */
                BPlusEntryRbTreeIteratorLast(&cur->rb_tree, &cur_pos->element_iterator);
            }
            else {
                BPlusEntryRbTreeIteratorPrev(&cur->rb_tree, &cur_pos->element_iterator);
            }
        }
        if (!parent_pos) {
            /* 没有父节点，创建 */
            LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id parent_id = BPlusEntryCreate(tree, kBPlusEntryIndex);
            BPlusEntry* parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_id);
            up_element_id = BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, -1, &src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, &right_id, &up_element_iterator);
            BPlusElement* up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, up_element_id);
            BPlusEntryInsertElement(parent, src_entry, up_element, cur_pos->entry_id);
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(src_entry, up_element);
            tree->root_id = parent_id;
            LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent);
            if (src_entry->type == kBPlusEntryIndex) {
                /* 创建父节点的话，不会再需要上升了，直接摘除要上升的节点 */
                BPlusEntryDeleteElement(src_entry, &up_element_iterator);
            }
            break;
        }
        BPlusEntry* parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_pos->entry_id);
        up_element_id = BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, parent_pos->element_iterator.cur_id, &src_entry, insert_element, cur_pos->element_iterator.cur_id, insert_element_child_id, &right_id, &up_element_iterator);
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent);
        insert_up = true;
    } while (false);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, cur);
    if (old_src_entry) {
        /* 索引节点上升后原节点需要被摘除 */
        LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(old_src_entry, insert_element);
        if (old_src_entry->type == kBPlusEntryIndex) {
            BPlusEntryDeleteElement(old_src_entry, insert_element_iterator);
        }
        LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, old_src_entry);
    }
    if (insert_up) {
        BPlusElement* up_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(src_entry, up_element_id);
        return BPlusTreeInsertElement(tree, iterator, src_entry, up_element, &up_element_iterator, cur_pos->entry_id);
    }
    return success;
}

/*
* 删除树中的指定元素
*/
static bool BPlusTreeDeleteElement(BPlusTree* tree, BPlusIterator* iterator) {
    BPlusElementPos* cur_pos = BPlusIteratorCur(tree, iterator);
    BPlusElementPos* parent_pos = BPlusIteratorUp(tree, iterator);
    BPlusEntry* entry = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, cur_pos->entry_id);
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id sibling_entry_id = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId;
    BPlusEntry* sibling = NULL;
    BPlusEntry* parent = NULL;
    bool success = true, delete_up = false;
    BPlusEntryDeleteElement(entry, &cur_pos->element_iterator);        /* 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了 */
    do {
        if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, entry) >= LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetMergeThresholdRate(tree, entry)) {
            break;
        }
        if (!parent_pos) {
            /* 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束 */
            if (entry->type == kBPlusEntryIndex && entry->element_count == 0) {
                LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id childId = entry->index.tail_child_id;
                tree->root_id = childId;
                LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry);
                BPlusEntryRelease(tree, cur_pos->entry_id);
                return true;
            }
            else {
                break;
            }
        }
        parent = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, parent_pos->entry_id);
        bool left_sibling = true;
        BPlusEntryRbTreeIterator common_parent_element_iterator;
        BPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &parent_pos->element_iterator);     /* 始终是指向左侧元素 */
        BPlusEntryRbTreeIterator sibling_element_iterator;
        if (common_parent_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的父元素，当前节点是末尾节点，是左兄弟 */
            BPlusEntryRbTreeIteratorLast(&parent->rb_tree, &sibling_element_iterator);
        }
        else {
            BPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
            BPlusEntryRbTreeIteratorPrev(&parent->rb_tree, &sibling_element_iterator);
            if (sibling_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) {     /* 当前元素已经是最小的元素 */
                left_sibling = false;
                BPlusEntryRbTreeIteratorCopy(&sibling_element_iterator, &common_parent_element_iterator);
                BPlusEntryRbTreeIteratorNext(&parent->rb_tree, &sibling_element_iterator);
                if (sibling_element_iterator.cur_id == BPlusEntryRbReferencer_InvalidId) { /* 直接找既没有左兄弟也没有右兄弟，那就是末尾节点是右兄弟 */
                    sibling_entry_id = parent->index.tail_child_id;
                }
            }
        }
        if (sibling_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId) {
            BPlusElement* sibling_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, sibling_element_iterator.cur_id);
            sibling_entry_id = sibling_element->index.child_id;
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, sibling_element);
        }
        if (left_sibling) {
            BPlusEntryRbTreeIteratorCopy(&common_parent_element_iterator, &sibling_element_iterator);
            BPlusEntryRbTreeIteratorCopy(&parent_pos->element_iterator, &sibling_element_iterator);       /* 更新一下，给父节点删除使用 */
        }
        assert(common_parent_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
        assert(sibling_entry_id != LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
        sibling = LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Reference(tree, sibling_entry_id);
        if (LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetFillRate(tree, sibling) > LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_ACCESSOR_GetMergeThresholdRate(tree, sibling)) {
            /* 向兄弟借节点 */
            assert(sibling->element_count >= 2);
            if (entry->type == kBPlusEntryLeaf) {
                /* 叶子节点处理较简单，可以直接移动 */
                if (left_sibling) {
                    /* 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key */
                    BPlusEntryRbTreeIterator last_element_iterator;
                    BPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator);
                    assert(last_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, last_element_iterator.cur_id);
                    BPlusEntryInsertElement(entry, sibling, element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, element);
                    BPlusEntryDeleteElement(sibling, &last_element_iterator);

                    BPlusElement* common_parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    common_parent_element->index.key = element->leaf.key;         /* 更新索引 */
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, common_parent_element);
                }
                else {
                    /* 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素 */
                    BPlusEntryRbTreeIterator first_element_iterator;
                    BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator);
                    BPlusEntryRbTreeIterator new_element_iterator;
                    BPlusEntryRbTreeIteratorCopy(&new_element_iterator, &first_element_iterator);
                    BPlusEntryRbTreeIteratorNext(&sibling->rb_tree, &new_element_iterator);
                    assert(first_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    assert(new_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, first_element_iterator.cur_id);
                    BPlusEntryInsertElement(entry, sibling, element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, element);
                    BPlusEntryDeleteElement(sibling, &first_element_iterator);
                    /* 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的 */
                    BPlusElement* common_parent_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    BPlusElement* sibling_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, new_element_iterator.cur_id);
                    common_parent_element->index.key = sibling_element->leaf.key;         /* 更新索引 */
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, common_parent_element);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, sibling_element);
                }
            }
            else {
                /* 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换) */
                if (left_sibling) {
                    /* 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧 */
                    BPlusEntryRbTreeIterator last_element_iterator;
                    BPlusEntryRbTreeIteratorLast(&sibling->rb_tree, &last_element_iterator);
                    assert(last_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* left_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, last_element_iterator.cur_id);
                    ObjectSwap(LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id, left_element->index.child_id, sibling->index.tail_child_id);        /* 要拿的是末尾的子节点，处理一下 */
                    BPlusElement* par_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    par_element->index.child_id = left_element->index.child_id;
                    BPlusEntryInsertElement(entry, parent, par_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, par_element);
                    BPlusEntryDeleteElement(parent, &common_parent_element_iterator);
                    left_element->index.child_id = sibling_entry_id;
                    BPlusEntryInsertElement(parent, sibling, left_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, left_element);
                    BPlusEntryDeleteElement(sibling, &last_element_iterator);
                }
                else {
                    /* 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧 */
                    BPlusEntryRbTreeIterator first_element_iterator;
                    BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree, &first_element_iterator);
                    assert(first_element_iterator.cur_id != BPlusEntryRbReferencer_InvalidId);
                    BPlusElement* right_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(sibling, first_element_iterator.cur_id);
                    BPlusElement* par_element = LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Reference(parent, common_parent_element_iterator.cur_id);
                    par_element->index.child_id = right_element->index.child_id;
                    ObjectSwap(LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id, par_element->index.child_id, entry->index.tail_child_id);        /* 要插入的是末尾的子节点，处理一下 */
                    BPlusEntryInsertElement(entry, parent, par_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(parent, par_element);
                    BPlusEntryDeleteElement(parent, &common_parent_element_iterator);
                    right_element->index.child_id = cur_pos->entry_id;
                    BPlusEntryInsertElement(parent, sibling, right_element, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
                    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Dereference(sibling, right_element);
                    BPlusEntryDeleteElement(sibling, &first_element_iterator);
                }
            }
            break;
        }

        /* 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素) */
        if (left_sibling) {
            BPlusEntryMerge(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, &common_parent_element_iterator);
            entry = NULL;
        }
        else {
            BPlusEntryMerge(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, &common_parent_element_iterator);
            sibling = NULL;
        }
        delete_up = true;
    } while (false);
    if (parent) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, parent); }
    if (sibling) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, sibling); }
    if (entry) { LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Dereference(tree, entry); }

    if (delete_up) {
        return BPlusTreeDeleteElement(tree, iterator);     /* 尾递归 */
    }
    return success;
}

/*
* 初始化B+树
*/
void BPlusTreeInit(BPlusTree* tree) {
    /* 索引最少3阶，否则索引节点分裂会出现一侧没有节点的情况 */
    /* 叶子可以2阶，但是删除有地方需要加强判断 */
    tree->root_id = BPlusEntryCreate(tree, kBPlusEntryLeaf);
    //BPlusLeafListInit(&tree->leaf_list); BPlusLeafListPutFirst(&tree->leaf_list, tree->root_id);
}
/*
* 从B+树中查找指定key
*/
bool BPlusTreeFind(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, key);
    }
    return status == kBPlusIteratorEq;
}
/*
* 从B+树中插入指定key
*/
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, &element->key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, &element->key);
    }
    bool success = BPlusTreeInsertElement(tree, &iterator, NULL, (BPlusElement*)element, NULL, LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Const_InvalidId);
    BPlusIteratorRelease(tree, &iterator);
    return success;
}
/*
* 从B+树中删除指定key
*/
bool BPlusTreeDelete(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key) {
    BPlusIterator iterator;
    BPlusIteratorStatus status = BPlusIteratorTop(tree, &iterator, key);
    while (status == kBPlusIteratorDown) {
        status = BPlusIteratorDown(tree, &iterator, key);
    }
    if (status == kBPlusIteratorNe) {
        return false;
    }
    bool success = BPlusTreeDeleteElement(tree, &iterator);
    return success;
}

#include <libyuc/container/bplus_tree.undef>