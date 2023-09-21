/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 为磁盘设计的b+tree
* 为支持变长kv的设计，不实现m阶等概念
*/

#include <libyuc/container/bplus_tree.def>


typedef enum _BPlusEntryType {
    kBPlusEntryIndex = 0,
    kBPlusEntryLeaf = 1,
} BPlusEntryType;
typedef enum {
    kBPlusIteratorNe,
    kBPlusIteratorEq,
    kBPlusIteratorDown,
    kBPlusIteratorEnd,
} BPlusIteratorStatus;


/*
* 页内红黑树
*/\
//LIBYUC_CONTAINER_RB_TREE_DECLARATION(
//    BPlusEntry,
//    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id,
//    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset,
//    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key
//    

#include <libyuc/container/bplus_tree_BPlusEntryRbTree.cfg>
#include <libyuc/container/rb_tree.h>



/*
* B+树迭代器
*/
typedef struct {
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id entry_id;
    BPlusEntryRbTreeIterator element_iterator;
} BPlusElementPos;

#include <libyuc/container/bplus_tree_BPlusIteratorStackVector.cfg>
#include <libyuc/container/vector.h>



typedef struct BPlusIterator {
    BPlusIteratorStackVector stack;
    // BPlusElementPos default_stack[default_stack_size];
    int32_t level;
    BPlusIteratorStatus leaf_status;
} BPlusIterator;
   

typedef struct BPlusTree {
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id root_id;
    //BPlusLeafListHead leaf_list;
} BPlusTree;
/*
* B+树
*/
typedef struct BPlusLeafElement {
    BPlusEntryRbEntry rb_entry;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key key;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Value value;
} BPlusLeafElement;
typedef struct BPlusIndexElement {
    BPlusEntryRbEntry rb_entry;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key key;
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id child_id;
} BPlusIndexElement;
typedef struct BPlusElement {
    union {
        BPlusIndexElement index;
        BPlusLeafElement leaf;
        struct {
            BPlusEntryRbEntry rb_entry;
            LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key key;       /* 需要保证leaf和index定位到key都是相同的偏移 */
        };
    };
} BPlusElement;
typedef struct BPlusIndexEntry {
    LIBYUC_CONTAINER_BPLUS_TREE_ENTRY_REFERENCER_Type_Id tail_child_id;         /* 末尾孩子id存在这里 */
} BPlusIndexEntry;
typedef struct BPlusLeafEntry {
    //BPlusLeafListEntry list_entry;         /* 连接所有叶子节点 */
    uint8_t invalid[];
} BPlusLeafEntry;
typedef struct BPlusEntry {
    BPlusEntryRbTree rb_tree;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset type : 1;
    LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset element_count : sizeof(LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset) * 8 - 1;
    union {
        BPlusIndexEntry index;
        BPlusLeafEntry leaf;
    };
} BPlusEntry;
   
   
BPlusElementPos* BPlusIteratorCur(BPlusTree* tree, BPlusIterator* iterator);
BPlusElementPos* BPlusIteratorUp(BPlusTree* tree, BPlusIterator* iterator);
BPlusIteratorStatus BPlusIteratorTop(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key);
void BPlusIteratorRelease(BPlusTree* tree, BPlusIterator* iterator);
BPlusIteratorStatus BPlusIteratorDown(BPlusTree* tree, BPlusIterator* iterator, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key);
   
void BPlusTreeInit(BPlusTree* tree);
bool BPlusTreeFind(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key);
bool BPlusTreeInsert(BPlusTree* tree, BPlusLeafElement* element);
bool BPlusTreeDelete(BPlusTree* tree, LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key* key);

#include <libyuc/container/bplus_tree.undef>


#ifdef __cplusplus
}
#endif
