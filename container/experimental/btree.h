/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_B_TREE_H_
#define LIBYUC_CONTAINER_B_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/container/list.h>
#include <libyuc/container/vector.h>
#include <libyuc/container/static_list.h>
#include <libyuc/container/rb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 为内存设计的b-tree
*/

#define CACHE_LINE_SIZE 64

typedef int64_t key_type;
typedef key_type element_type;
typedef struct _BTreeEntry* entry_id_type;

#define LIBYUC_CONTAINER_B_TREE_M 129

#define LIBYUC_CONTAINER_B_TREE_SEQUENTIAL_SEARCH_ELEMENT_COUNT 8       // CACHE_LINE_SIZE / sizeof(element_type)

typedef enum {
    kBTreeIteratorEq,
    kBTreeIteratorDown,
    kBTreeIteratorEnd,
} BTreeIteratorStatus;

typedef struct _BTreeIteratorElement {
    entry_id_type entry;
    uint32_t cur_element_pos;
} BTreeIteratorPos;

LIBYUC_CONTAINER_VECTOR_DECLARATION(
    BTreeIteratorStack,
    LIBYUC_CONTAINER_VECTOR_MODE_STATIC,
    uint32_t,
    uint32_t,
    BTreeIteratorPos,
    sizeof(entry_id_type) * 8
)

LIBYUC_CONTAINER_VECTOR_DEFINE(
    BTreeIteratorStack,
    LIBYUC_CONTAINER_VECTOR_MODE_STATIC,
    uint32_t,
    uint32_t,
    BTreeIteratorPos,
    BTreeIteratorPos,
    LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT,
    LIBYUC_BASIC_ALLOCATOR_DEFALUT,
    LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT,
    LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID,
    LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT,
)


LIBYUC_ALGORITHM_ARRAY_DEFINE(
    BTreeElement, 
    uint32_t,
    uint32_t, 
    element_type, 
    key_type,
    LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT,
    LIBYUC_BASIC_COMPARER_DEFALUT, 
    LIBYUC_BASIC_REFERENCER_DEFALUT
)

LIBYUC_ALGORITHM_ARRAY_DEFINE(
    BTreeChild,
    uint32_t,
    uint32_t,
    entry_id_type,
    entry_id_type,
    LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT,
    LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID,
    LIBYUC_BASIC_REFERENCER_DEFALUT
)

typedef struct _BTree {
    entry_id_type root;
} BTree;

typedef struct _BTreeEntry {
    uint32_t is_leaf : 1;
    uint32_t count : 31;
    element_type element[LIBYUC_CONTAINER_B_TREE_M - 1];
    entry_id_type child[LIBYUC_CONTAINER_B_TREE_M];     // 若为叶子节点则不存在该字段
} BTreeEntry;



typedef struct _BTreeIterator {
    BTreeIteratorStackVector stack;
} BTreeIterator;




void BTreeInit(BTree* tree) {
    tree->root = NULL;
}

BTreeIteratorStatus BTreeIteratorDown(BTree* tree, BTreeIterator* iter, const key_type* key);

BTreeIteratorStatus BTreeIteratorTop(BTree* tree, BTreeIterator* iter, const key_type* key) {
    BTreeIteratorStackVectorInit(&iter->stack);
    return BTreeIteratorDown(tree, iter, key);
}

BTreeIteratorPos* BTreeIteratorCur(BTree* tree, BTreeIterator* iter) {
    BTreeIteratorPos* entry = BTreeIteratorStackVectorGetTail(&iter->stack);
    return entry;
}

BTreeIteratorPos* BTreeIteratorUp(BTree* tree, BTreeIterator* iter) {
    BTreeIteratorStackVectorPopTail(&iter->stack);
    return BTreeIteratorCur(tree, iter);
}

BTreeIteratorStatus BTreeIteratorDown(BTree* tree, BTreeIterator* iter, const key_type* key) {
    BTreeIteratorPos iter_pos;
    if (BTreeIteratorStackVectorGetCount(&iter->stack) == 0) {
        iter_pos.entry = tree->root;
    }
    else {
        BTreeIteratorPos* parent = BTreeIteratorStackVectorGetTail(&iter->stack);
        if (parent->entry->is_leaf) {
            return kBTreeIteratorEnd;
        }
        iter_pos.entry = parent->entry->child[parent->cur_element_pos];
    }
    if (iter_pos.entry == NULL) {
        return kBTreeIteratorEnd;
    }
    
    if (iter_pos.entry->count <= LIBYUC_CONTAINER_B_TREE_SEQUENTIAL_SEARCH_ELEMENT_COUNT) {
        // 节点数量少时使用顺序搜索，能极大程度提高性能
        iter_pos.cur_element_pos = BTreeElementArrayOrderFind_Range(iter_pos.entry->element, 0, iter_pos.entry->count - 1, key);
    }
    else {
        for (iter_pos.cur_element_pos = 0; iter_pos.cur_element_pos < iter_pos.entry->count - 1; iter_pos.cur_element_pos++) {
            if (iter_pos.entry->element[iter_pos.cur_element_pos] >= *key) {
                break;
            }
        }
    }
    if (iter_pos.entry->element[iter_pos.cur_element_pos] == *key) {
        BTreeIteratorStackVectorPushTail(&iter->stack, &iter_pos);
        return kBTreeIteratorEq;
    }
    // 保证 iter_pos.cur_element_pos始终指向大于等于key的元素位置(可能越界)
    if (iter_pos.entry->element[iter_pos.cur_element_pos] < *key) {
        iter_pos.cur_element_pos++;
    }
    BTreeIteratorStackVectorPushTail(&iter->stack, &iter_pos);
    return kBTreeIteratorDown;
}


void BTreeEntryPut(BTree* tree, BTreeIterator* iter, element_type* element, entry_id_type left_child, entry_id_type right_child) {
    BTreeIteratorPos* iter_pos = BTreeIteratorCur(tree, iter);
    entry_id_type entry;
    if (!iter_pos) {
        // 创建新根节点
        if (left_child) {
            entry = (BTreeEntry*)MemoryAlloc(sizeof(BTreeEntry));
            entry->child[0] = left_child;
            entry->child[1] = right_child;
            entry->is_leaf = 0;
        }
        else {
            entry = (BTreeEntry*)MemoryAlloc(sizeof(BTreeEntry) - sizeof(tree->root->child));
            entry->is_leaf = 1;
        }
        entry->element[0] = *element;
        entry->count = 1;
        tree->root = entry;
        return;
    }

    entry = iter_pos->entry;

    const key_type* key = element;
    do {
        uint32_t insert_pos = iter_pos->cur_element_pos;
        uint32_t right_count_inc = 0;
        if (insert_pos == entry->count) {
            // 如果是插入到末尾的元素，增加一次循环计数
            right_count_inc = 1;
        }
        else if (*key == entry->element[insert_pos]) {
            entry->element[insert_pos] = *element;
            entry->child[insert_pos] = right_child;
            return;
        }

        if (entry->count == LIBYUC_CONTAINER_B_TREE_M - 1) {
            // 分裂
            entry_id_type right;
            if (entry->is_leaf) {
                right = (BTreeEntry*)MemoryAlloc(sizeof(BTreeEntry) - sizeof(entry->child));
            }
            else {
                right = (BTreeEntry*)MemoryAlloc(sizeof(BTreeEntry));
            }
            right->is_leaf = entry->is_leaf;
            int right_count = LIBYUC_CONTAINER_B_TREE_M / 2;
            int left_count = entry->count - right_count;

            int insert_right = 0;

            for (int i = right_count, j = 0; i < entry->count + right_count_inc; i++, j++) {
                if (insert_right == 0 && i == insert_pos) {
                    insert_right = 1;
                    right->element[j] = *element;
                    if (!entry->is_leaf) {
                        right->child[j] = left_child;
                        entry->child[i] = right_child;      // 下一趟循环由下面的代码赋值
                    }
                    i--;
                    continue;
                }
                right->element[j] = entry->element[i];
                if (!entry->is_leaf) {
                    right->child[j] = entry->child[i];
                }
            }

            if (insert_right == 0) {
                BTreeElementArrayInsert(entry->element, left_count, insert_pos, element);
                left_count++;
                if (!entry->is_leaf) {
                    BTreeChildArrayInsert(entry->child, left_count, insert_pos, &left_child);
                    entry->child[insert_pos + 1] = right_child;
                }
            }
            else {
                right_count++;
            }
            
            right->count = right_count;

            // 左侧末尾元素的孩子应当放到右节点的末尾
            if (!entry->is_leaf) {
                right->child[right_count] = entry->child[entry->count];
            }

            // 上升左节点的末尾元素
            left_count--;
            entry->count = left_count;
            BTreeIteratorUp(tree, iter);
            BTreeEntryPut(tree, iter, &entry->element[entry->count], entry, right);
            return;
        }

        // 一个节点分裂时，其父元素是新上升的节点，原先的父元素的孩子就会变更为新的右节点
        BTreeElementArrayInsert(entry->element, entry->count, insert_pos, element);
        entry->count++;
        if (!entry->is_leaf) {
            BTreeChildArrayInsert(entry->child, entry->count, insert_pos, &left_child);
            entry->child[insert_pos + 1] = right_child;
        }
        
        break;
    } while (true);
   

}

void BTreePut(BTree* tree, element_type* element) {
    const key_type* key = element;
    BTreeIterator iter;
    BTreeIteratorStatus status = BTreeIteratorTop(tree, &iter, key);
    while (status == kBTreeIteratorDown) {
        status = BTreeIteratorDown(tree, &iter, key);
    }
    BTreeEntryPut(tree, &iter, element, NULL, NULL);
}

bool BTreeFind(BTree* tree, const key_type* key) {
    BTreeIterator iter;
    BTreeIteratorStatus status = BTreeIteratorTop(tree, &iter, key);
    while (status == kBTreeIteratorDown) {
        status = BTreeIteratorDown(tree, &iter, key);
    }
    return status == kBTreeIteratorEq;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_B_TREE_H_