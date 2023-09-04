/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_B_TREE_H_
#define LIBYUC_CONTAINER_B_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 为内存设计的b-tree
*/

#define CACHE_LINE_SIZE 64

//typedef int64_t key_type;
//typedef key_type element_type;
typedef struct _BTreeEntry* entry_id_type;

#define LIBYUC_CONTAINER_B_TREE_M 129

#define LIBYUC_CONTAINER_B_TREE_SEQUENTIAL_SEARCH_ELEMENT_COUNT LIBYUC_CONTAINER_B_TREE_M - 1       // CACHE_LINE_SIZE / sizeof(element_type)

typedef enum {
    kBTreeIteratorEq,
    kBTreeIteratorDown,
    kBTreeIteratorNext,
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
        // 若key的获取与比较成本较低(int型)，即便是较大的数据量(这里选用129阶进行测试有很好的效果)
        // 使用顺序搜索，相较于二分搜索也有碾压式的性能优势(2~3倍)
        // 猜测是二分搜索对缓存利用较差的缘故(一般cache line是64字节)
        for (iter_pos.cur_element_pos = 0; iter_pos.cur_element_pos < iter_pos.entry->count - 1; iter_pos.cur_element_pos++) {
            if (iter_pos.entry->element[iter_pos.cur_element_pos] >= *key) {
                break;
            }
        }
    }
    else {
        iter_pos.cur_element_pos = BTreeElementArrayOrderFind_Range(iter_pos.entry->element, 0, iter_pos.entry->count - 1, key);
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

element_type* BTreeIteratorFirst(BTree* tree, BTreeIterator* iter) {
    BTreeIteratorStackVectorInit(&iter->stack);
    
    BTreeIteratorPos iter_pos;
    iter_pos.entry = tree->root;
    if (!iter_pos.entry) {
        return NULL;
    }
    iter_pos.cur_element_pos = 0;
    BTreeIteratorStackVectorPushTail(&iter->stack, &iter_pos);
    while (!iter_pos.entry->is_leaf) {
        iter_pos.entry = iter_pos.entry->child[0];
        BTreeIteratorStackVectorPushTail(&iter->stack, &iter_pos);
    }
    return &iter_pos.entry->element[0];
}

element_type* BTreeIteratorNext(BTree* tree, BTreeIterator* iter) {
    BTreeIteratorPos* iter_pos = BTreeIteratorCur(tree, iter);
    ++iter_pos->cur_element_pos;
    if (iter_pos->entry->is_leaf) {
        // 是叶子就尝试找连续的下一节点，如果不存在则向上取第一个不越界的父节点
        if (iter_pos->cur_element_pos >= iter_pos->entry->count) {
            do {
                iter_pos = BTreeIteratorUp(tree, iter);
                if (iter_pos == NULL) {
                    return NULL;
                }
            } while (iter_pos->cur_element_pos >= iter_pos->entry->count);
        }
    }
    else {
        // 是内部节点则定位到第一个叶子节点
        while (!iter_pos->entry->is_leaf) {
            BTreeIteratorPos iter_pos_temp;
            iter_pos_temp.entry = iter_pos->entry->child[iter_pos->cur_element_pos];
            iter_pos_temp.cur_element_pos = 0;
            iter_pos = BTreeIteratorStackVectorPushTail(&iter->stack, &iter_pos_temp);
        }
    }
    return &iter_pos->entry->element[iter_pos->cur_element_pos];
}


void BTreePutElement(BTree* tree, BTreeIterator* iter, element_type* element, entry_id_type left_child, entry_id_type right_child) {
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
                    entry->child[i] = right_child;      // 由下一趟循环/跳出循环下面的代码赋值
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
        BTreePutElement(tree, iter, &entry->element[entry->count], entry, right);
        return;
    }

    // 一个节点分裂时，其父元素会变为新上升的节点，原先的父元素的孩子就会变更为新的右节点
    BTreeElementArrayInsert(entry->element, entry->count, insert_pos, element);
    entry->count++;
    if (!entry->is_leaf) {
        BTreeChildArrayInsert(entry->child, entry->count, insert_pos, &left_child);
        entry->child[insert_pos + 1] = right_child;
    }
}

void BTreeDeleteElement(BTree* tree, BTreeIterator* iter) {
    BTreeIteratorPos* iter_pos = BTreeIteratorCur(tree, iter);

    entry_id_type entry = iter_pos->entry;

    uint32_t delete_pos = iter_pos->cur_element_pos;
    BTreeElementArrayDelete(entry->element, entry->count, delete_pos);
    if (!entry->is_leaf) {
        // 由合并触发的，右节点已被删除，左节点挂到此处
        entry->child[delete_pos + 1] = entry->child[delete_pos];
        BTreeChildArrayDelete(entry->child, entry->count + 1, delete_pos);
    }
    --entry->count;
    if (entry->count >= LIBYUC_CONTAINER_B_TREE_M * 40 / 100) {
        return;
    }

    BTreeIteratorPos* parent_iter_pos = BTreeIteratorUp(tree, iter);
    if (parent_iter_pos == NULL) {
        if (entry->count == 0) {
            if (entry->is_leaf) {
                tree->root = NULL;
            }
            else {
                tree->root = entry->child[0];
            }
            MemoryFree(entry);
        }
        return;
    }

    // 需要向兄弟借节点或合并节点
    entry_id_type parent = parent_iter_pos->entry;
    entry_id_type sibling;
    uint32_t parent_element_pos;
    bool is_left_sibling;
    if (parent_iter_pos->cur_element_pos < parent->count) {
        parent_element_pos = parent_iter_pos->cur_element_pos;
        sibling = parent->child[parent_iter_pos->cur_element_pos + 1];
        is_left_sibling = false;
    }
    else {
         assert(parent_iter_pos->cur_element_pos > 0);
        parent_element_pos = parent_iter_pos->cur_element_pos - 1;
        sibling = parent->child[parent_iter_pos->cur_element_pos - 1];
        is_left_sibling = true;
    }
    if (sibling->count > LIBYUC_CONTAINER_B_TREE_M * 40 / 100) {
        // 向兄弟借节点，即兄弟元素上升，替代父元素，父元素下降
        if (is_left_sibling) {
            // 左兄弟末尾元素上升，父元素插入到当前节点的头部
            BTreeElementArrayInsert(entry->element, entry->count, 0, &parent->element[parent_element_pos]);
            if (!entry->is_leaf) {
                BTreeChildArrayInsert(entry->child, entry->count + 1, 0, &sibling->child[sibling->count]);
            }
            ++entry->count;
            parent->element[parent_element_pos] = sibling->element[--sibling->count];
        }
        else {
            // 右兄弟头部元素上升，父元素插入到当前节点的尾部
            entry->element[entry->count++] = parent->element[parent_element_pos];
            if (!entry->is_leaf) {
                entry->child[entry->count] = sibling->child[0];
            }
            parent->element[parent_element_pos] = sibling->element[0];
            BTreeElementArrayDelete(sibling->element, sibling->count, 0);
            if (!entry->is_leaf) {
                BTreeChildArrayDelete(sibling->child, sibling->count + 1, 0);
            }
            --sibling->count;
        }
        return;
    }

    // 合并，即向上删除
    entry_id_type left;
    entry_id_type right;
    if (is_left_sibling) {
        left = sibling;
        right = entry;
    }
    else {
        left = entry;
        right = sibling;
    }
    // 右侧元素合并到左侧元素，中间下降一个父元素
    left->element[left->count++] = parent->element[parent_element_pos];
    for (int i = left->count, j = 0; j < right->count; i++, j++) {
        left->element[i] = right->element[j];
        if (!entry->is_leaf) {
            left->child[i] = right->child[j];
        }
    }
    left->count += right->count;
    if (!entry->is_leaf) {
        left->child[left->count] = right->child[right->count];
    }

    MemoryFree(right);

    parent_iter_pos->cur_element_pos = parent_element_pos;
    BTreeDeleteElement(tree, iter);
}

bool BTreeFind(BTree* tree, const key_type* key) {
    BTreeIterator iter;
    BTreeIteratorStatus status = BTreeIteratorTop(tree, &iter, key);
    while (status == kBTreeIteratorDown) {
        status = BTreeIteratorDown(tree, &iter, key);
    }
    return status == kBTreeIteratorEq;
}

void BTreePut(BTree* tree, element_type* element) {
    const key_type* key = element;
    BTreeIterator iter;
    BTreeIteratorStatus status = BTreeIteratorTop(tree, &iter, key);
    while (status == kBTreeIteratorDown) {
        status = BTreeIteratorDown(tree, &iter, key);
    }
    BTreePutElement(tree, &iter, element, NULL, NULL);
}

bool BTreeDelete(BTree* tree, const key_type* key) {
    BTreeIterator iter;
    BTreeIteratorStatus status = BTreeIteratorTop(tree, &iter, key);
    while (status == kBTreeIteratorDown) {
        status = BTreeIteratorDown(tree, &iter, key);
    }
    if (status != kBTreeIteratorEq) {
        return false;
    }

    BTreeIteratorPos* iter_pos = BTreeIteratorCur(tree, &iter);
    // 不是叶子节点，将删除转为叶子节点
    if (!iter_pos->entry->is_leaf) {
        element_type* cur_element = &iter_pos->entry->element[iter_pos->cur_element_pos];
        element_type* next_element = BTreeIteratorNext(tree, &iter);
         assert(next_element);
        *cur_element = *next_element;
    }

    BTreeDeleteElement(tree, &iter);
    return true;
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_B_TREE_H_