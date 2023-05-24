/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BPLUS_TREE_H_
#define CUTILS_CONTAINER_BPLUS_TREE_H_

#include <CUtils/object.h>
#include <CUtils/container/list.h>
#include <CUtils/container/vector.h>
#include <CUtils/container/static_list.h>
#include <CUtils/container/rb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum _BPlusEntryType {
    kBPlusEntryIndex = 0,
    kBPlusEntryLeaf = 1,
} BPlusEntryType;
typedef enum {
    kBPlusCursorNe,
    kBPlusCursorEq,
    kBPlusCursorNext,
    kBPlusCursorEnd,
} BPlusCursorStatus;


/*
为支持变长kv，不实现m阶等概念
*/


#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_1(bp_tree_type_name, entry_id_type) CUTILS_CONTAINER_LIST_DECLARATION(bp_tree_type_name##BPlusLeaf, entry_id_type)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_2(bp_tree_type_name) bp_tree_type_name##BPlusLeafListHead leaf_list;
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DECLARATION_3(bp_tree_type_name) bp_tree_type_name##BPlusLeafListEntry list_entry;       /* 连接所有叶子节点 */

#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_1(bp_tree_type_name, entry_id_type, entry_referencer) \
    forceinline bp_tree_type_name##BPlusLeafListEntry* bp_tree_type_name##BPlusLeafEntryReferencer_Reference(bp_tree_type_name##BPlusLeafListHead* head, entry_id_type entry_id) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, entry_id); \
        return &entry->leaf.list_entry; \
    } \
    forceinline void bp_tree_type_name##BPlusLeafEntryReferencer_Dereference(bp_tree_type_name##BPlusLeafListHead* head, bp_tree_type_name##BPlusLeafListEntry* list_entry) { \
        bp_tree_type_name##BPlusTree* tree = ObjectGetFromField(head,  bp_tree_type_name##BPlusTree, leaf_list); \
        bp_tree_type_name##BPlusLeafEntry* entry = ObjectGetFromField(list_entry,  bp_tree_type_name##BPlusLeafEntry, list_entry); \
        entry_referencer##_Dereference(tree, (bp_tree_type_name##BPlusEntry*)entry); \
    } \
    CUTILS_CONTAINER_LIST_DEFINE(bp_tree_type_name##BPlusLeaf, entry_id_type, bp_tree_type_name##BPlusLeafEntryReferencer) \

#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_2(bp_tree_type_name) bp_tree_type_name##BPlusLeafListPutEntryNext(&tree->leaf_list, left_id, right_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_3(bp_tree_type_name) bp_tree_type_name##BPlusLeafListDeleteEntry(&tree->leaf_list, right_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL_DEFINE_4(bp_tree_type_name) bp_tree_type_name##BPlusLeafListInit(&tree->leaf_list); bp_tree_type_name##BPlusLeafListPutFirst(&tree->leaf_list, tree->root_id);
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL


#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_1(bp_tree_type_name, entry_id_type)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_2(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DECLARATION_3(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_1(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_2(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_3(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK_DEFINE_4(bp_tree_type_name)
#define CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK CUTILS_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK


#define CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(bp_tree_type_name, leaf_link_mode, entry_id_type, element_id_type, key_type, value_type) \
    /*
    * B+树游标
    */ \
    typedef struct { \
        entry_id_type entry_id; \
        element_id_type element_id; \
    } bp_tree_type_name##BPlusElementPos; \
    CUTILS_CONTAINER_VECTOR_DECLARATION(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos) \
    typedef struct _##bp_tree_type_name##BPlusCursor { \
        bp_tree_type_name##BPlusCursorStackVector stack; \
        element_id_type level; \
        BPlusCursorStatus leaf_status; \
    } bp_tree_type_name##BPlusCursor; \
    \
    /*
    * 页内红黑树
    */\
    CUTILS_CONTAINER_RB_TREE_DECLARATION(bp_tree_type_name##BPlusEntry, element_id_type, key_type) \
    leaf_link_mode##_DECLARATION_1(bp_tree_type_name, entry_id_type) \
    typedef struct _##bp_tree_type_name##BPlusTree { \
        entry_id_type root_id; \
        leaf_link_mode##_DECLARATION_2(bp_tree_type_name) \
    } bp_tree_type_name##BPlusTree; \
    /*
    * B+树
    */ \
    typedef struct _##bp_tree_type_name##BPlusLeafElement { \
        bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        key_type key; \
        value_type value; \
    } bp_tree_type_name##BPlusLeafElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexElement { \
        bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        entry_id_type child_id; \
        key_type key; \
    } bp_tree_type_name##BPlusIndexElement; \
    typedef struct _##bp_tree_type_name##BPlusElement { \
        union { \
            bp_tree_type_name##BPlusIndexElement index; \
            bp_tree_type_name##BPlusLeafElement leaf; \
            bp_tree_type_name##BPlusEntryRbEntry rb_entry; \
        }; \
    } bp_tree_type_name##BPlusElement; \
    typedef struct _##bp_tree_type_name##BPlusIndexEntry { \
        entry_id_type tail_child_id;       /* 末尾孩子id存在这里 */ \
    } bp_tree_type_name##BPlusIndexEntry; \
    typedef struct _##bp_tree_type_name##BPlusLeafEntry { \
        leaf_link_mode##_DECLARATION_3(bp_tree_type_name) \
        uint8_t buf[]; \
    } bp_tree_type_name##BPlusLeafEntry; \
    typedef struct _##bp_tree_type_name##BPlusEntry { \
        bp_tree_type_name##BPlusEntryRbTree rb_tree; \
        u##element_id_type type : 1; \
        u##element_id_type element_count : 15; \
        union { \
            bp_tree_type_name##BPlusIndexEntry index; \
            bp_tree_type_name##BPlusLeafEntry leaf; \
        }; \
    } bp_tree_type_name##BPlusEntry; \
    \
    \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorCur(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorUp(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorDown(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorFirst(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key); \
    void bp_tree_type_name##BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor); \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key); \
    \
    void bp_tree_type_name##BPlusTreeInit(bp_tree_type_name##BPlusTree* tree); \
    bool bp_tree_type_name##BPlusTreeFind(bp_tree_type_name##BPlusTree* tree, key_type* key); \
    bool bp_tree_type_name##BPlusTreeInsert(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusLeafElement* element); \
    bool bp_tree_type_name##BPlusTreeDelete(bp_tree_type_name##BPlusTree* tree, key_type* key); \


/*

entry：
    请确保1个entry至少能分配3个element

    Rate自行根据页面来指定
    比如页面是4096，就返回4096分比

    entry访问器需要提供
        GetMergeThresholdRate(获取合并阈值n分比)
            >该百分比的entry至少有2个element
        GetFreeRate(获取空闲n分比，可分配的最大空闲)
        GetFillRate(获取已填充n分比)

element:
    基本element定长，附属kv可能不定长
    element访问器需要提供
        GetNeedRate(获取element需要使用n分比)
        用于获取element的使用n分比



插入时，element所需的空间不足插入新element时触发分裂
    分裂前，当前entry必须至少有2个element(叶子可以1个，索引必须2个，否则没有上升节点，统一规定2个)
    首先计算当前entry的填充率，再加上新element的总占用率再/2
    分裂的时候根据填充率进行分裂，使得被插入的一侧分裂后能有足够的空位分配并插入新元素
    value最大占用25%, key最大占用12.5%
        为了简化实现，必须是2的次幂，entry至少要2个element, 因此必须限制单个element总占用率不能超过总页面的50%，需要确保每次分裂后都一定能装下新元素
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

    如果使用相等大小的块进行分配，逻辑上连续的块通过链表连接则不存在内存碎片


kv分离是外层处理的，b+树操作的只有element
碎片整理也在外边进行，当空闲位足够但分配失败时处理


*/
#define CUTILS_CONTAINER_BPLUS_TREE_DEFINE(bp_tree_type_name, leaf_link_mode, entry_id_type, element_id_type, key_type, value_type, cursor_allocator, entry_allocator, entry_referencer, entry_accessor, element_accessor, element_referencer, element_allocator, rb_accessor, rb_comparer) \
    /*
    * B+树游标
    */\
    static const entry_id_type bp_tree_type_name##BPlusLeafEntryReferencer_InvalidId = entry_referencer##_InvalidId; \
    leaf_link_mode##_DEFINE_1(bp_tree_type_name, entry_id_type, entry_referencer) \
    CUTILS_CONTAINER_VECTOR_DEFINE(bp_tree_type_name##BPlusCursorStack, bp_tree_type_name##BPlusElementPos, cursor_allocator, CUTILS_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    \
    /*
    * 页内红黑树
    */\
    static const element_id_type bp_tree_type_name##BPlusEntryRbReferencer_InvalidId = (-1) ; \
    forceinline bp_tree_type_name##BPlusEntryRbEntry* bp_tree_type_name##BPlusEntryRbReferencer_Reference(bp_tree_type_name##BPlusEntryRbTree* tree, element_id_type element_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            return NULL; \
        } \
        bp_tree_type_name##BPlusEntry* entry = ObjectGetFromField(tree, bp_tree_type_name##BPlusEntry, rb_tree); \
        return &(element_referencer##_Reference(entry, element_id)->rb_entry); \
    } \
    forceinline void bp_tree_type_name##BPlusEntryRbReferencer_Dereference(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbEntry* rb_entry) { \
        bp_tree_type_name##BPlusEntry* entry = ObjectGetFromField(tree, bp_tree_type_name##BPlusEntry, rb_tree); \
        bp_tree_type_name##BPlusElement* element = ObjectGetFromField(rb_entry, bp_tree_type_name##BPlusElement, rb_entry); \
        element_referencer##_Dereference(entry, element); \
    } \
    \
    typedef struct { \
        element_id_type color : 1; \
        element_id_type parent : sizeof(element_id_type) * 8 - 1; \
    } bp_tree_type_name##BPlusEntryRbParentColor; \
    forceinline element_id_type rb_accessor##_GetParent(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry) { \
        return (((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->parent); \
    } \
    forceinline RbColor rb_accessor##_GetColor(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry) { \
        return (((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->color == -1 ? 1 : 0); \
    } \
    forceinline void rb_accessor##_SetParent(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry, element_id_type new_id) { \
        ((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->parent = new_id; \
    } \
    forceinline void rb_accessor##_SetColor(bp_tree_type_name##BPlusEntryRbTree* tree, bp_tree_type_name##BPlusEntryRbBsEntry* bs_entry, RbColor new_color) { \
        return ((bp_tree_type_name##BPlusEntryRbParentColor*)&(((bp_tree_type_name##BPlusEntryRbEntry*)bs_entry)->parent_color))->color = new_color; \
    } \
    CUTILS_CONTAINER_RB_TREE_DEFINE(bp_tree_type_name##BPlusEntry, element_id_type, key_type, bp_tree_type_name##BPlusEntryRbReferencer, rb_accessor, rb_comparer) \
    \
    /*
    * B+树
    */\
    static void bp_tree_type_name##BPlusElementSet(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, element_id_type element_id, bp_tree_type_name##BPlusElement* element) { \
          assert(element_id >= 0); \
        bp_tree_type_name##BPlusElement* dst_element = element_referencer##_Reference(entry, element_id); \
        if (entry->type == kBPlusEntryLeaf) { \
            dst_element->leaf.key = element->leaf.key; \
            dst_element->leaf.value = element->leaf.value; \
        } \
        else { \
            dst_element->index.key = element->index.key; \
            dst_element->index.child_id = element->index.child_id; \
        } \
        element_referencer##_Dereference(entry, dst_element); \
    } \
    static entry_id_type bp_tree_type_name##BPlusElementGetChildId(bp_tree_type_name##BPlusTree* tree, const bp_tree_type_name##BPlusEntry* index, element_id_type element_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            return index->index.tail_child_id; \
        } \
        bp_tree_type_name##BPlusElement* element = element_referencer##_Reference(index, element_id); \
        entry_id_type child_id = element->index.child_id; \
        element_referencer##_Dereference(index, element); \
        return child_id; \
    } \
    static void bp_tree_type_name##BPlusElementSetChildId(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* index, element_id_type element_id, entry_id_type entry_id) { \
        if (element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            index->index.tail_child_id = entry_id; \
            return; \
        } \
        bp_tree_type_name##BPlusElement* element = element_referencer##_Reference(index, element_id); \
        element->index.child_id = entry_id; \
        element_referencer##_Dereference(index, element); \
    } \
    static element_id_type bp_tree_type_name##BPlusElementCreate(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry) { \
        element_id_type element_id = element_allocator##_CreateBySize(entry, entry->type == kBPlusEntryLeaf ? sizeof(bp_tree_type_name##BPlusLeafElement) : sizeof(bp_tree_type_name##BPlusIndexElement)); \
          assert(element_id >= 0); \
        return element_id; \
    } \
    static bp_tree_type_name##BPlusElement* bp_tree_type_name##BPlusElementRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, element_id_type element_id) { \
          assert(element_id >= 0); \
        bp_tree_type_name##BPlusElement* element = element_referencer##_Reference(entry, element_id); \
        element_referencer##_Dereference(entry, element); \
        element_allocator##_Release(entry, element_id); \
        return element; \
    } \
    \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorCur(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level < 0) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[cursor->level];\
    } \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorUp(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level <= 0) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[--cursor->level]; \
    } \
    bp_tree_type_name##BPlusElementPos* bp_tree_type_name##BPlusCursorDown(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->level + 1 >= cursor->stack.count) { \
            return NULL; \
        } \
        return &cursor->stack.obj_arr[++cursor->level]; \
    } \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorFirst(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusCursorStackVectorInit(&cursor->stack, 8, true); \
        cursor->stack.count = 0; \
        cursor->level = -1; \
        return bp_tree_type_name##BPlusCursorNext(tree, cursor, key); \
    } \
    void bp_tree_type_name##BPlusCursorRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        if (cursor->stack.capacity != 0) { \
            bp_tree_type_name##BPlusCursorStackVectorRelease(&cursor->stack); \
        } \
    } \
    BPlusCursorStatus bp_tree_type_name##BPlusCursorNext(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, key_type* key) { \
        bp_tree_type_name##BPlusElementPos cur; \
        bp_tree_type_name##BPlusElementPos* parent = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        if (parent) { \
            bp_tree_type_name##BPlusEntry* parent_entry = entry_referencer##_Reference(tree, parent->entry_id); \
            if (parent_entry->type == kBPlusEntryLeaf) { \
                return kBPlusCursorEnd; \
            } \
            cur.entry_id = bp_tree_type_name##BPlusElementGetChildId(tree, parent_entry, parent->element_id); \
            entry_referencer##_Dereference(tree, parent_entry); \
        } \
        else { \
            cur.entry_id = tree->root_id; \
        } \
        bp_tree_type_name##BPlusEntry* cur_entry = entry_referencer##_Reference(tree, cur.entry_id); \
        int8_t cmp_status = -1; \
        if (cur_entry->element_count > 0) { \
            cur.element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLocate(&cur_entry->rb_tree, key, &cmp_status); \
            if (cmp_status == -1) { /* key小于当前定位元素 */ } \
            else { /* key大于等于当前定位元素 */ \
                if (cur_entry->type == kBPlusEntryIndex || cmp_status == 1) { \
                    cur.element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&cur_entry->rb_tree, cur.element_id); \
                } \
            } \
        } \
        else { \
            cur.element_id = bp_tree_type_name##BPlusEntryRbReferencer_InvalidId; \
        } \
        bp_tree_type_name##BPlusCursorStackVectorPushTail(&cursor->stack, &cur); \
        BPlusCursorStatus status = kBPlusCursorNext; \
        if (cur_entry->type == kBPlusEntryLeaf) { \
            if (cmp_status != 0) { \
                status = kBPlusCursorNe; \
            } \
            else { \
                status = kBPlusCursorEq; \
            } \
            cursor->leaf_status = status; \
        } \
        ++cursor->level; \
        entry_referencer##_Dereference(tree, cur_entry); \
        return status; \
    } \
    \
    \
    /*
    * 孩子关系对应如下
    *         3     |    6    |    8
    *      /          /        /       \
    *   1|2         4|5       7        10|11
    * 如上面画的示例，默认是3对应1|2，6对应4|5、8对应7
    */ \
    /*
    * 向节点插入元素
    * 不支持处理末尾子节点
    */ \
    static element_id_type bp_tree_type_name##BPlusEntryInsertElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, bp_tree_type_name##BPlusElement* insert_element) { \
        element_id_type element_id = bp_tree_type_name##BPlusElementCreate(tree, entry); \
          assert(element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
        bp_tree_type_name##BPlusElementSet(tree, entry, element_id, insert_element); \
        element_id_type old_element_id = bp_tree_type_name##BPlusEntryRbTreePut(&entry->rb_tree, element_id); \
        if (old_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId && old_element_id != element_id) bp_tree_type_name##BPlusElementRelease(tree, entry, old_element_id); \
        entry->element_count++; \
        return element_id; \
    } \
    /*
    * 从节点中删除指定的元素
    * 不支持处理末尾子节点
    * 返回被删除的元素
    */ \
    static bp_tree_type_name##BPlusElement* bp_tree_type_name##BPlusEntryDeleteElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry, element_id_type element_id) { \
          assert(element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
        bp_tree_type_name##BPlusEntryRbTreeDelete(&entry->rb_tree, element_id); \
        entry->element_count--; \
        return bp_tree_type_name##BPlusElementRelease(tree, entry, element_id); \
    } \
    entry_id_type bp_tree_type_name##BPlusEntryCreate(bp_tree_type_name##BPlusTree* tree, BPlusEntryType type) { \
        size_t size; \
        entry_id_type entry_id; \
        if (type == kBPlusEntryIndex) { \
            entry_id = entry_allocator##_CreateBySize(tree, sizeof(bp_tree_type_name##BPlusEntry) - sizeof(bp_tree_type_name##BPlusLeafEntry) + sizeof(bp_tree_type_name##BPlusIndexEntry)); \
        } \
        else { \
            entry_id = entry_allocator##_CreateBySize(tree, sizeof(bp_tree_type_name##BPlusEntry) - sizeof(bp_tree_type_name##BPlusIndexEntry) + sizeof(bp_tree_type_name##BPlusLeafEntry)); \
        } \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, entry_id); \
        entry->type = type; \
        entry->element_count = 0; \
        entry_referencer##_Dereference(tree, entry); \
        bp_tree_type_name##BPlusEntryRbTreeInit(&entry->rb_tree); \
        return entry_id; \
    } \
    void bp_tree_type_name##BPlusEntryRelease(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* entry) { \
        entry_allocator##_Release(tree, entry); \
    } \
    \
    /*
    * 分裂节点
    * 返回上升的索引元素
    *
    * 同一个叶节点多次分裂会导致重复的key上升吗？
    * 如果叶节点不存在相同的key，那么是不会的
    * 因为分裂后选择右节点的最左元素的key作为上升的key
    * 这个时候无论怎么插入元素都不会插入到该节点最左元素的左侧(比它小的会被分到左侧节点，因为父元素key等于该元素)，该节点再分裂也就不存在最左元素再次上升的可能了
    */ \
    static bp_tree_type_name##BPlusElement bp_tree_type_name##BPlusEntrySplit(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* left, entry_id_type left_id, bp_tree_type_name##BPlusEntry* parent, element_id_type parent_element_id, bp_tree_type_name##BPlusElement* insert_element, element_id_type insert_id, entry_id_type* out_right_id) { \
        /* assert(insert_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); */ \
        entry_id_type right_id = bp_tree_type_name##BPlusEntryCreate(tree, left->type); \
        bp_tree_type_name##BPlusEntry* right = entry_referencer##_Reference(tree, right_id); \
        bp_tree_type_name##BPlusElement up_element; \
        if (left->type == kBPlusEntryLeaf) { \
            leaf_link_mode##_DEFINE_2(bp_tree_type_name) \
        } \
        element_id_type left_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&left->rb_tree); \
        bool insert = false; \
        int32_t fill_rate = (entry_accessor##_GetFillRate(tree, left) + element_accessor##_GetNeedRate(left, insert_element)) / 2; \
        while (left_elemeng_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
            /* 检查填充率 */ \
            if (entry_accessor##_GetFillRate(tree, left) <= fill_rate || left->element_count == 2) { \
                break; \
            } \
            if (!insert && left_elemeng_id == insert_id) { \
                bp_tree_type_name##BPlusEntryInsertElement(tree, right, insert_element); \
                insert = true; \
                continue; \
            } \
            element_id_type next_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&left->rb_tree, left_elemeng_id); \
            bp_tree_type_name##BPlusEntryInsertElement(tree, right, bp_tree_type_name##BPlusEntryDeleteElement(tree, left, left_elemeng_id)); \
            left_elemeng_id = next_elemeng_id; \
        } \
        /* 新元素还没有插入，将其插入 */ \
        if (!insert) { \
            bp_tree_type_name##BPlusEntryInsertElement(tree, left, insert_element); \
        } \
        \
        if (left->type == kBPlusEntryLeaf) { \
            /* 从mid拿到上升元素，叶子元素转换为索引元素，上升元素的子节点指向左节点 */ \
            bp_tree_type_name##BPlusElement* first_element = element_referencer##_Reference(right, bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&right->rb_tree)); \
            up_element = *first_element; \
            element_referencer##_Dereference(right, first_element); \
            key_type key = up_element.leaf.key; \
            up_element.index.key = key; \
        } \
        else { \
            /* 假设如下节点需要分裂
                       15            18
                     /                |
               2   4   8   12        ...
               |   |   |   |    \
               1   3   5   10   13   
             ---------------------------
               2   4          8   12
               |   |   \      |   |   \
               1   3   13     5   10   
             ---------------------------
                     4       15              18
                  /          |               |
               2             8   12
               |  \          |   |   \
               1   3         5   10    13
             此时右节点缺少了一条链接，我们最终选用左节点的末尾元素(4)作为上升元素，故左节点的末尾元素的右侧子节点(13)就可以挂接到右节点的末尾元素的右侧子节点下 */ \
            right->index.tail_child_id = left->index.tail_child_id; \
            \
            /* 最后从左节点末尾拿到上升元素，将其摘除 */ \
            up_element = *bp_tree_type_name##BPlusEntryDeleteElement(tree, left, bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&left->rb_tree)); \
            left->index.tail_child_id = up_element.index.child_id;       /* 3指定为2的右侧子节点 */ \
        } \
        /* 上升的4的子节点为左 */ \
        up_element.index.child_id = left_id; \
        \
        /* 4上升后，原先指向4的父元素，就指向8|12，(原先指向左节点的父元素指向右节点，因为上升的元素会变成父元素的兄弟，指向左节点) */ \
        bp_tree_type_name##BPlusElementSetChildId(tree, parent, parent_element_id, right_id); \
        \
        *out_right_id = right_id; \
        entry_referencer##_Dereference(tree, right); \
          assert(left->element_count >= 1); \
          assert(right->element_count >= 1); \
        return up_element; \
    } \
    /*
    * 合并节点
    * 释放右节点，返回时右节点不再有效
    *   2
    * 1   3
    */ \
    static void bp_tree_type_name##BPlusEntryMerge(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusEntry* left, entry_id_type left_id, bp_tree_type_name##BPlusEntry* right, entry_id_type right_id, bp_tree_type_name##BPlusEntry* parent, element_id_type parent_index) { \
        element_id_type right_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&right->rb_tree); \
        for (int32_t i = 0; i < right->element_count; i++) { \
              assert(right_elemeng_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
            bp_tree_type_name##BPlusElement* right_elemeng = element_referencer##_Reference(right, right_elemeng_id); \
            bp_tree_type_name##BPlusEntryInsertElement(tree, left, right_elemeng); \
            element_referencer##_Dereference(right, right_elemeng); \
            right_elemeng_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&right->rb_tree, right_elemeng_id); \
        } \
        if (left->type == kBPlusEntryLeaf) { \
            leaf_link_mode##_DEFINE_3(bp_tree_type_name) \
            /* 是叶子节点，将right并入left中，并删除父元素 */ \
        } \
        else { \
            /* 是索引节点，将父元素(子节点原先指向左和右，下降需要指向左的末尾子节点)和right都并入到left中，向上传递删除父元素 */ \
            bp_tree_type_name##BPlusElement* parent_element = element_referencer##_Reference(parent, parent_index); \
            element_id_type left_element_id = bp_tree_type_name##BPlusEntryInsertElement(tree, left, parent_element); \
            element_referencer##_Dereference(parent, parent_element); \
            bp_tree_type_name##BPlusElementSetChildId(tree, left, left_element_id, left->index.tail_child_id);       /* left的末尾元素此时为下降的父元素，修改其子节点 */ \
            bp_tree_type_name##BPlusElementSetChildId(tree, left, -1, right->index.tail_child_id); \
        } \
        \
        /* 父元素会被删除，父右兄弟元素的左侧子节点(或尾子节点)原先指向右节点(被删除)，此时继承左节点(合并后剩下的节点) */ \
        bp_tree_type_name##BPlusElementSetChildId(tree, parent, bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&parent->rb_tree, parent_index), left_id); \
        entry_referencer##_Dereference(tree, right); \
        bp_tree_type_name##BPlusEntryRelease(tree, right_id); \
        /* 合并部分完成，删除部分交给调用者 */ \
    } \
    \
    /*
    * 向树插入元素
    */ \
    static bool bp_tree_type_name##BPlusTreeInsertElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor, bp_tree_type_name##BPlusElement* insert_element) { \
        bp_tree_type_name##BPlusElementPos* cur_pos = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        bp_tree_type_name##BPlusElementPos* parent_pos = bp_tree_type_name##BPlusCursorUp(tree, cursor); \
        entry_id_type right_id; \
        bp_tree_type_name##BPlusEntry* cur = entry_referencer##_Reference(tree, cur_pos->entry_id); \
        bool success = true, insert_up = false; \
        bp_tree_type_name##BPlusElement up_element; \
        do { \
            if (cursor->leaf_status == kBPlusCursorEq) { \
                bp_tree_type_name##BPlusElementSet(tree, cur, cur_pos->element_id, insert_element); \
                break; \
            } \
            if (entry_accessor##_GetFreeRate(tree, cur) >= element_accessor##_GetNeedRate(cur, insert_element)) { \
                /* 有空余的位置插入 */ \
                bp_tree_type_name##BPlusEntryInsertElement(tree, cur, insert_element); \
                break; \
            } \
            /* 没有多余位置，需要分裂向上插入，插入的位置需要是第一个小于key的元素，element_id指向第一个大于key的元素 */ \
            if (cur_pos->element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的元素，直接拿末尾元素 */ \
                cur_pos->element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&cur->rb_tree); \
            } else { \
                cur_pos->element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&cur->rb_tree, cur_pos->element_id); \
            } \
            if (!parent_pos) { \
                /* 没有父节点，创建 */ \
                entry_id_type parent_id = bp_tree_type_name##BPlusEntryCreate(tree, kBPlusEntryIndex); \
                bp_tree_type_name##BPlusEntry* parent = entry_referencer##_Reference(tree, parent_id); \
                up_element = bp_tree_type_name##BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, -1, insert_element, cur_pos->element_id, &right_id); \
                bp_tree_type_name##BPlusEntryInsertElement(tree, parent, &up_element); \
                tree->root_id = parent_id; \
                entry_referencer##_Dereference(tree, parent); \
                break; \
            } \
            bp_tree_type_name##BPlusEntry* parent = entry_referencer##_Reference(tree, parent_pos->entry_id); \
            up_element = bp_tree_type_name##BPlusEntrySplit(tree, cur, cur_pos->entry_id, parent, parent_pos->element_id, insert_element, cur_pos->element_id, &right_id); \
            entry_referencer##_Dereference(tree, parent); \
            insert_up = true; \
        } while (false); \
        entry_referencer##_Dereference(tree, cur); \
        if (insert_up) { \
            return bp_tree_type_name##BPlusTreeInsertElement(tree, cursor, &up_element); \
        } \
        return success; \
    } \
    \
    /*
    * 删除树中的指定元素
    */ \
    static bool bp_tree_type_name##BPlusTreeDeleteElement(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusCursor* cursor) { \
        bp_tree_type_name##BPlusElementPos* cur_pos = bp_tree_type_name##BPlusCursorCur(tree, cursor); \
        bp_tree_type_name##BPlusElementPos* parent_pos = bp_tree_type_name##BPlusCursorUp(tree, cursor); \
        bp_tree_type_name##BPlusEntry* entry = entry_referencer##_Reference(tree, cur_pos->entry_id); \
        entry_id_type sibling_entry_id = 0; \
        bp_tree_type_name##BPlusEntry* sibling = NULL; \
        bp_tree_type_name##BPlusEntry* parent = NULL; \
        bool success = true, delete_up = false; \
        bp_tree_type_name##BPlusEntryDeleteElement(tree, entry, cur_pos->element_id);      /* 直接删除即可，叶子元素没有子节点，索引元素在合并时已经处理子节点了 */ \
        do { \
            if (entry_accessor##_GetFillRate(tree, entry) >= entry_accessor##_GetMergeThresholdRate(tree, entry)) { \
                break; \
            } \
            if (!parent_pos) { \
                /* 没有父节点就已经到根节点了，是叶子节点就跳过，是索引节点则判断是否没有任何子节点了，是则变更余下最后一个子节点为根节点，否则直接结束 */ \
                if (entry->type == kBPlusEntryIndex && entry->element_count == 0) { \
                    entry_id_type childId = entry->index.tail_child_id; \
                    tree->root_id = childId; \
                    entry_referencer##_Dereference(tree, entry); \
                    bp_tree_type_name##BPlusEntryRelease(tree, cur_pos->entry_id); \
                    return true; \
                } \
                else { \
                    break; \
                } \
            } \
            parent = entry_referencer##_Reference(tree, parent_pos->entry_id); \
            bool left_sibling = true; \
            element_id_type common_parent_element_id = parent_pos->element_id;     /* 始终是指向左侧元素 */ \
            element_id_type sibling_element_id; \
            if (common_parent_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 不存在大于key的父元素，当前节点是末尾节点，是左兄弟 */ \
                sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&parent->rb_tree); \
            } \
            else { \
                sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorPrev(&parent->rb_tree, common_parent_element_id); \
                if (sibling_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) {     /* 当前元素已经是最小的元素 */ \
                    left_sibling = false; \
                    sibling_element_id = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&parent->rb_tree, common_parent_element_id); \
                    if (sibling_element_id == bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { /* 直接找既没有左兄弟也没有右兄弟，那就是末尾节点是右兄弟 */ \
                        sibling_entry_id = parent->index.tail_child_id; \
                    } \
                } \
            } \
            if (sibling_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId) { \
                bp_tree_type_name##BPlusElement* sibling_element = element_referencer##_Reference(parent, sibling_element_id); \
                sibling_entry_id = sibling_element->index.child_id; \
                element_referencer##_Dereference(parent, sibling_element); \
            } \
            if (left_sibling) { \
                common_parent_element_id = sibling_element_id; \
                parent_pos->element_id = sibling_element_id;     /* 更新一下，给父节点删除使用 */ \
            } \
              assert(common_parent_element_id != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
              assert(sibling_entry_id != entry_referencer##_InvalidId); \
            sibling = entry_referencer##_Reference(tree, sibling_entry_id); \
            if (entry_accessor##_GetFillRate(tree, sibling) > entry_accessor##_GetMergeThresholdRate(tree, entry)) { \
                /* 向兄弟借节点 */ \
                  assert(sibling->element_count >= 2); \
                if (entry->type == kBPlusEntryLeaf) { \
                    /* 叶子节点处理较简单，可以直接移动 */ \
                    if (left_sibling) { \
                        /* 左兄弟节点的末尾的元素插入到当前节点的头部，更新父元素key为借来的key */ \
                        element_id_type last = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&sibling->rb_tree); \
                          assert(last != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, last); \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, element); \
                        \
                        bp_tree_type_name##BPlusElement* common_parent_element = element_referencer##_Reference(parent, common_parent_element_id); \
                        common_parent_element->index.key = element->leaf.key;       /* 更新索引 */ \
                        element_referencer##_Dereference(parent, common_parent_element); \
                    } \
                    else { \
                        /* 右兄弟节点的头部的元素插入到当前节点的尾部，并新父元素key为右兄弟的新首元素 */ \
                        element_id_type first = bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree); \
                        element_id_type new_first = bp_tree_type_name##BPlusEntryRbTreeIteratorNext(&sibling->rb_tree, first); \
                          assert(first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                          assert(new_first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, first); \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, element); \
                        /* 右节点的头元素key可能正好和共同父节点相等(此时和索引相等的key跑到左边，就会导致找不到)，因此key更新为新的首元素是最好的 */ \
                        bp_tree_type_name##BPlusElement* common_parent_element = element_referencer##_Reference(parent, common_parent_element_id); \
                        bp_tree_type_name##BPlusElement* sibling_element = element_referencer##_Reference(sibling, new_first); \
                        common_parent_element->index.key = sibling_element->leaf.key;       /* 更新索引 */ \
                        element_referencer##_Dereference(parent, common_parent_element); \
                        element_referencer##_Dereference(sibling, sibling_element); \
                    } \
                } \
                else { \
                    /* 索引节点处理较复杂，需要下降父节点中当前节点和兄弟节点的共同父元素，上升兄弟的元素到共同父元素(即交换) */ \
                    if (left_sibling) { \
                        /* 左兄弟节点的末尾元素上升到父节点的头部，父节点的对应元素下降到当前节点的头部，上升元素其右子节点挂在下降的父节点元素的左侧 */ \
                        element_id_type last = bp_tree_type_name##BPlusEntryRbTreeIteratorLast(&sibling->rb_tree); \
                          assert(last != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* left_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, last); \
                        ObjectSwap(entry_id_type, left_element->index.child_id, sibling->index.tail_child_id);        /* 要拿的是末尾的子节点，处理一下 */ \
                        bp_tree_type_name##BPlusElement* par_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, parent, common_parent_element_id); \
                        par_element->index.child_id = left_element->index.child_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, par_element); \
                        left_element->index.child_id = sibling_entry_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, parent, left_element); \
                    } \
                    else { \
                        /* 右兄弟节点的头元素上升到父节点的头部，父节点的对应元素下降到当前节点的尾部，上升元素其左子节点挂在下降的父节点元素的右侧 */ \
                        element_id_type first = bp_tree_type_name##BPlusEntryRbTreeIteratorFirst(&sibling->rb_tree); \
                          assert(first != bp_tree_type_name##BPlusEntryRbReferencer_InvalidId); \
                        bp_tree_type_name##BPlusElement* right_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, sibling, first); \
                        bp_tree_type_name##BPlusElement* par_element = bp_tree_type_name##BPlusEntryDeleteElement(tree, parent, common_parent_element_id); \
                        par_element->index.child_id = right_element->index.child_id; \
                        ObjectSwap(entry_id_type, par_element->index.child_id, entry->index.tail_child_id);        /* 要插入的是末尾的子节点，处理一下 */ \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, entry, par_element); \
                        right_element->index.child_id = cur_pos->entry_id; \
                        bp_tree_type_name##BPlusEntryInsertElement(tree, parent, right_element); \
                    } \
                } \
                break; \
            } \
            \
            /* 兄弟节点不够借，需要合并(合并了也不会超过m-1，因为一边不足m-1的一半，一边是m-1的一半，是索引节点的合并也足够下降一个父元素) */ \
            if (left_sibling) { \
                bp_tree_type_name##BPlusEntryMerge(tree, sibling, sibling_entry_id, entry, cur_pos->entry_id, parent, common_parent_element_id); \
                entry = NULL; \
            } \
            else { \
                bp_tree_type_name##BPlusEntryMerge(tree, entry, cur_pos->entry_id, sibling, sibling_entry_id, parent, common_parent_element_id); \
                sibling = NULL; \
            } \
            delete_up = true; \
        } while (false); \
        if (parent) { entry_referencer##_Dereference(tree, parent); } \
        if (sibling) { entry_referencer##_Dereference(tree, sibling); } \
        if (entry) { entry_referencer##_Dereference(tree, entry); } \
        \
        if (delete_up) { \
            return bp_tree_type_name##BPlusTreeDeleteElement(tree, cursor);     /* 尾递归 */ \
        } \
        return success; \
    } \
    \
    /*
    * 初始化B+树
    */ \
    void bp_tree_type_name##BPlusTreeInit(bp_tree_type_name##BPlusTree* tree) { \
        /* 索引最少3阶，否则索引节点分裂会出现一侧没有节点的情况 */  \
        /* 叶子可以2阶，但是删除有地方需要加强判断 */ \
        tree->root_id = bp_tree_type_name##BPlusEntryCreate(tree, kBPlusEntryLeaf); \
        leaf_link_mode##_DEFINE_4(bp_tree_type_name) \
    } \
    /*
    * 从B+树中查找指定key
    */ \
    bool bp_tree_type_name##BPlusTreeFind(bp_tree_type_name##BPlusTree* tree, key_type* key) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, key); \
        } \
        return status == kBPlusCursorEq; \
    } \
    /*
    * 从B+树中插入指定key
    */ \
    bool bp_tree_type_name##BPlusTreeInsert(bp_tree_type_name##BPlusTree* tree, bp_tree_type_name##BPlusLeafElement* element) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, &element->key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, &element->key); \
        } \
        bool success = bp_tree_type_name##BPlusTreeInsertElement(tree, &cursor, (bp_tree_type_name##BPlusElement*)element); \
        bp_tree_type_name##BPlusCursorRelease(tree, &cursor); \
        return success; \
    } \
    /*
    * 从B+树中删除指定key
    */ \
    bool bp_tree_type_name##BPlusTreeDelete(bp_tree_type_name##BPlusTree* tree, key_type* key) { \
        bp_tree_type_name##BPlusCursor cursor; \
        BPlusCursorStatus status = bp_tree_type_name##BPlusCursorFirst(tree, &cursor, key); \
        while (status == kBPlusCursorNext) { \
            status = bp_tree_type_name##BPlusCursorNext(tree, &cursor, key); \
        } \
        if (status == kBPlusCursorNe) { \
            return false; \
        } \
        bool success = bp_tree_type_name##BPlusTreeDeleteElement(tree, &cursor); \
        return success; \
    } \


//CUTILS_CONTAINER_BPLUS_TREE_DECLARATION(Int, struct _IntBPlusEntry*, int, int)
//forceinline int* CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR_GetKey(IntBPlusEntryRbTree* tree, IntBPlusEntryRbBsEntry* bs_entry) {
//    if (((IntBPlusEntry*)tree)->type == kBPlusEntryLeaf) {
//        return &((IntBPlusLeafElement*)bs_entry)->key;
//    }
//    else {
//        return &((IntBPlusIndexElement*)bs_entry)->key;
//    }
//}
//#define CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR
//CUTILS_CONTAINER_BPLUS_TREE_DEFINE(Int, struct _IntBPlusEntry*, int, int, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_REFERENCER_DEFALUT, CUTILS_CONTAINER_BPLUS_RB_TREE_ACCESSOR, CUTILS_OBJECT_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BPLUS_TREE_H_