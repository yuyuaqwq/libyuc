/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/



#ifndef LIBYUC_CONTAINER_AR_TREE_H_
#define LIBYUC_CONTAINER_AR_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/bit.h>



#ifdef __cplusplus
extern "C" {
#endif

/*
* Adaptive Radix Tree - 自适应基数树
*/

/*
* 定长key优化思路：
* key直接比较(不通过memcmp)
* 共同前缀保持与key长度一致，通过位移存放/更新，以及直接比较，不需要乐观策略，另外可以增加一个位用于减少最后叶子的一次指针访问
* 当前定长key最大支持8字节
*/

#define LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED

#define id_type struct _ArNode*

#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
struct data {
    uint32_t size;
    uint8_t* buf;
};

typedef struct data art_key_type_t;
typedef struct data art_element_type;
#else
typedef uint64_t art_key_type_t;
typedef art_key_type_t art_element_type;
#endif


#define InvalidId 0
#define prefix_max_len sizeof(art_key_type_t)


/*
* 可以通过两个宏选项来声明不同的结构体
* 叶子节点是指针还是嵌入
* 通过传参传递
*/

typedef art_element_type ArLeaf;


typedef enum {
    kArNode4,
    kArNode16,
    kArNode48,
    kArNode256,
} ArNodeType;


typedef struct _ArNodeHead {
    uint16_t node_type : 2;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    uint16_t : 4;       // 无效
    uint16_t eof: 1;
#else
    uint16_t : 2;       // 无效
    uint16_t prefix_len : 3;        // 0~8，对于4/8字节定长key足够
#endif
    uint16_t child_count : 9;        // 需要注意这里是8位的话不够表示node256的最大数量
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    uint32_t prefix_len;
#endif
    union {
        uint8_t prefix[prefix_max_len];
    };
} ArNodeHead;


typedef struct {
    ArNodeHead head;
    uint8_t keys[4];
    struct _ArNode* child_arr[4];
} ArNode4;

typedef struct {
    ArNodeHead head;
    uint8_t keys[16];
    struct _ArNode* child_arr[16];
} ArNode16;


typedef struct {
    union {
        uintptr_t next;
        struct _ArNode* child;
    };
} ArNode48Child;

#define LIBYUC_TEMPLATE_DEFINE
#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME ArNode48
#define LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount 48
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element ArNode48Child
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id uint32_t
#include <libyuc/container/static_list.h>




typedef struct {
    ArNodeHead head;
    uint8_t keys[256];
    ArNode48StaticList child_arr;
} ArNode48;

typedef struct {
    ArNodeHead head;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    struct _ArNode* child_arr[257];
#else
    struct _ArNode* child_arr[256];
#endif
    
} ArNode256;

typedef struct _ArNode {
    union {
        ArLeaf leaf;
        ArNodeHead head;
        ArNode4 node4;
        ArNode16 node16;
        ArNode48 node48;
        ArNode256 node256;
    };
} ArNode;

typedef struct {
    ArNode* root;
    uint32_t element_count;
} ArTree;


/*
* Key
*/
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
uint32_t ArKeyGetLen(art_key_type_t* key) {
    return key->size;
}

uint8_t* ArKeyGetByteBuf(art_key_type_t** key, uint32_t offset) {
    return &(*key)->buf[offset];
}

bool ArKeyEqual(art_key_type_t* key1, art_key_type_t* key2, uint32_t offset) {
    return key1->size == key2->size && !memcmp(&key1->buf[offset], &key2->buf[offset], key1->size - offset);
}
#else
uint32_t ArKeyGetLen(art_key_type_t* key) {
    return sizeof(*key);
}

uint8_t* ArKeyGetByteBuf(art_key_type_t** key, uint32_t offset) {
    return ((uint8_t*)*key) + offset;
    // return ((uint8_t*)key) + offset;
}

bool ArKeyEqual(art_key_type_t* key1, art_key_type_t* key2, uint32_t offset) {
    // return !memcmp(ArGetKeyByte(key1, offset), ArGetKeyByte(key2, offset), ArGetKeyLen(key1) - offset);
    // 完整key比较时才会调用，整数就直接比较
    return *key1 == *key2;
    // return key1 == key2;
}
#endif


/*
* Element
*/
art_key_type_t* ArElementGetKey(art_element_type* element) {
    return element;
}


/*
* Leaf
*/
art_element_type* ArLeafGetElement(ArLeaf* leaf) {
    return (art_element_type*)((uintptr_t)leaf & (~(uintptr_t)1));
}

void ArLeafSetElement(ArLeaf** leaf, art_element_type* element) {
    *leaf = (ArLeaf*)((uintptr_t)element | (uintptr_t)1);
}

static ArLeaf* ArLeafCreate(ArTree* tree, art_element_type* element) {
    return (ArLeaf*)((uintptr_t)element | (uintptr_t)1);
}

static void ArLeafRelease(ArTree* tree, ArLeaf* leaf) {
    // ObjectRelease(leaf);
}


/*
* Node
*/
bool ArNodeIsLeaf(ArNode* node) {
    return ((uintptr_t)node & 1) == 1;
}

ArLeaf* ArNodeGetLeaf(ArNode* node) {
    return (ArLeaf*)node;
}

void ArNodeSetLeaf(ArNode** node, ArLeaf* leaf) {
    *node = (ArNode*)(uintptr_t)leaf;
}


#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
ArLeaf** ArNodeGetEofChild(ArNode* node) {
    if (!node->head.eof) {
        return InvalidId;
    }
    ArLeaf** leaf;
    switch (node->head.node_type) {
    case kArNode4: {
        leaf = (ArLeaf**)&node->node4.child_arr[3];
        break;
    }
    case kArNode16: {
        leaf = (ArLeaf**)&node->node16.child_arr[15];
        break;
    }
    case kArNode48: {
        leaf = (ArLeaf**)&node->node16.child_arr[47];
        break;
    }
    case kArNode256: {
        leaf = (ArLeaf**)&node->node16.child_arr[256];
        break;
    }
    default: {
        assert(0);
        leaf = NULL;
        break;
    }
    }
    return leaf;
}

void ArNodeSetEofChild(ArNode* node, ArLeaf* leaf) {
    node->head.eof = 1;
    *ArNodeGetEofChild(node) = leaf;
}
#endif


uint8_t* ArBsAccessor_GetKey(uint8_t* arr, uint8_t* element) {
    return element;
}

#define AR_TREE_ARRAY_InvalidId (-1)


#define LIBYUC_ALGORITHM_ARRAY_CLASS ArNodeKey
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element uint8_t
#include <libyuc/algorithm/array.h>

#define LIBYUC_ALGORITHM_ARRAY_CLASS ArNodeChild
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element ArNode*
#include <libyuc/algorithm/array.h>



static void ArNodeHeadInit(ArNodeHead* head, ArNodeType type) {
    head->child_count = 0;
    head->node_type = type;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    head->eof = 0;
    // head->int_key_len_shift = 0;
#endif
    head->prefix_len = 0;
}

void ArNodeHeadCopy(ArNode* dst, ArNode* src) {
    // dst->int_key_len_shift = src->int_key_len_shift;
    memcpy(dst->head.prefix, src->head.prefix, min(prefix_max_len, src->head.prefix_len));
    dst->head.prefix_len = src->head.prefix_len;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    if (src->head.eof) {
        ArNodeSetEofChild(dst, *ArNodeGetEofChild(src));
    }
#endif
}


static forceinline uint16_t ArNodeGetFullCount(ArNode* node) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    return node->head.child_count + node->head.eof;
#else
    return node->head.child_count;
#endif
}

static ArNode4* ArNode4Create(ArTree* tree) {
    ArNode4* node4 = ObjectCreate(ArNode4);
    for (int i = 0; i < 4; i++) {
        node4->keys[i] = 0;
        node4->child_arr[i] = InvalidId;
    }
    ArNodeHeadInit(&node4->head, kArNode4);
    return node4;
}

static ArNode16* ArNode16Create(ArTree* tree) {
    ArNode16* node16 = ObjectCreate(ArNode16);
    for (int i = 0; i < 16; i++) {
        node16->keys[i] = 0;
        node16->child_arr[i] = InvalidId;
    }
    ArNodeHeadInit(&node16->head, kArNode16);
    return node16;
}

static ArNode48* ArNode48Create(ArTree* tree) {
    ArNode48* node48 = ObjectCreate(ArNode48);
    for (int i = 0; i < 256; i++) {
        node48->keys[i] = -1;
    }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    ArNode48StaticListInit(&node48->child_arr, 47);
#else
    ArNode48StaticListInit(&node48->child_arr, 48);
#endif
    ArNodeHeadInit(&node48->head, kArNode48);
    return node48;
}

static ArNode256* ArNode256Create(ArTree* tree) {
    ArNode256* node256 = ObjectCreate(ArNode256);
    for (int i = 0; i < 256; i++) {
        node256->child_arr[i] = InvalidId;
    }
    ArNodeHeadInit(&node256->head, kArNode256);
    return node256;
}

static void ArNode4Release(ArTree* tree, ArNode4* node4) {
    ObjectRelease(node4);
}

static void ArNode16Release(ArTree* tree, ArNode16* node16) {
    ObjectRelease(node16);
}

static void ArNode48Release(ArTree* tree, ArNode48* node48) {
    ObjectRelease(node48);
}

static void ArNode256Release(ArTree* tree, ArNode256* node256) {
    ObjectRelease(node256);
}


static ArNode** ArNode256Find(ArNode256* node, uint8_t key_byte) {
    ArNode** node_ptr = &node->child_arr[key_byte];
    if (!*node_ptr) {
        node_ptr = InvalidId;
    }
    return node_ptr;
}

static ArNode** ArNode48Find(ArNode48* node, uint8_t key_byte) {
    uint8_t index = node->keys[key_byte];
    if (index != 0xff) {
        return &node->child_arr.obj_arr[index].child;
    }
    return InvalidId;
}

/*
* 在个人运行环境下测试，16字节查找性能如下：SIMD(400+ms) > 顺序查找性能(1000+ms) > 二分查找(1600+ms)
*/

#ifdef _MSC_VER
#include <intrin.h>
#endif
static forceinline ptrdiff_t ArNode16KeySearch(ArNode16* node, uint8_t key_byte) {
#ifdef _MSC_VER
    __m128i results = _mm_cmpeq_epi8(_mm_set1_epi8(key_byte), _mm_loadu_si128((__m128i*) & node->keys[0]));
    ptrdiff_t mask = (1 << node->head.child_count) - 1;
    ptrdiff_t i = _mm_movemask_epi8(results) & mask;
    if (!i) {
        return AR_TREE_ARRAY_InvalidId;
    }
    i = _tzcnt_u32(i);
#else
    ptrdiff_t i = ArNodeKeyArrayOrderFind(node->keys, 0, node->head.child_count - 1, &key_byte);
    if (i == AR_TREE_ARRAY_InvalidId) {
        return AR_TREE_ARRAY_InvalidId;
    }
#endif
    return i;
}

static ArNode** ArNode16Find(ArNode16* node, uint8_t key_byte) {
    ptrdiff_t i = ArNode16KeySearch(node, key_byte);
    if (i == AR_TREE_ARRAY_InvalidId) {
        return InvalidId;
    }
    return &node->child_arr[i];
}

static ArNode** ArNode4Find(ArNode4* node, uint8_t key_byte) {
    // ptrdiff_t i = ArNodeKeyArrayFind(node->keys, 4, &key_byte);
    // if (i == AR_TREE_ARRAY_InvalidId || i >= node->head.child_count) {
    ptrdiff_t i = ArNodeKeyArrayFind(node->keys, node->head.child_count, &key_byte);
    if (i == AR_TREE_ARRAY_InvalidId) {
        return InvalidId;
    }
    return &node->child_arr[i];
}

static ArNode** ArNodeFind(ArNode* node, uint8_t key_byte) {
    switch (node->head.node_type) {
    case kArNode4: {
        return ArNode4Find(&node->node4, key_byte);
    }
    case kArNode16: {
        return ArNode16Find(&node->node16, key_byte);
    }
    case kArNode48: {
        return ArNode48Find(&node->node48, key_byte);
    }
    case kArNode256: {
        return ArNode256Find(&node->node256, key_byte);
    }
    default: {
        assert(0);
        break;
    }
    }
    return InvalidId;
}


static ArNode** ArNode256Insert(ArTree* tree, ArNode256** node_ptr, uint8_t key_byte, ArNode* child) {
     assert(child);
    ArNode256* node = *node_ptr;
    ArNode* find_node = node->child_arr[key_byte];
    if (find_node == InvalidId) {
        node->head.child_count++;
    }
    node->child_arr[key_byte] = child;
    return &node->child_arr[key_byte];
    //int n = 0;
    //for (int i = 0; i < 256; i++) {
    //    if (node->child_arr[i]) {
    //        n++;
    //    }
    //}
    //assert(n == node->head.child_count);
}

static ArNode** ArNode48Insert(ArTree* tree, ArNode48** node_ptr, uint8_t key_byte, ArNode* child) {
    ArNode48* node = *node_ptr;
    uint8_t child_index = node->keys[key_byte];
    if (child_index == 0xff) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        if (node->head.child_count >= 47) {
#else
        if (node->head.child_count >= 48) {
#endif
            ArNode256* new_node256 = ArNode256Create(tree);
            ArNodeHeadCopy((ArNode*)new_node256, (ArNode*)node);
            *node_ptr = (ArNode48*)new_node256;
            for (int32_t i = 0; i < 256; i++) {
                if (node->keys[i] != 0xff) {
                    ArNode256Insert(tree, (ArNode256**)node_ptr, i, node->child_arr.obj_arr[node->keys[i]].child);
                }
            }
            ArNode** new_child = ArNode256Insert(tree, (ArNode256**)node_ptr, key_byte, child);
            ArNode48Release(tree, node);
            return new_child;
        }
        child_index = ArNode48StaticListPop(&node->child_arr, 0);
        node->keys[key_byte] = child_index;
    }
    node->child_arr.obj_arr[child_index].child = child;
    node->head.child_count++;
    return &node->child_arr.obj_arr[child_index].child;
}

static ArNode** ArNode16Insert(ArTree* tree, ArNode16** node_ptr, uint8_t key_byte, ArNode* child) {
    ArNode16* node = *node_ptr;
    ptrdiff_t i;
    if (node->head.child_count > 0) {
        i = ArNodeKeyArrayOrderFind_Range(node->keys, 0, node->head.child_count - 1, &key_byte);
        if (key_byte == node->keys[i]) {
            node->keys[i] = key_byte;
            node->child_arr[i] = child;
            return &node->child_arr[i];
        }
        if (key_byte > node->keys[i]) i++;
    }
    else {
        i = 0;
    }
    if (ArNodeGetFullCount((ArNode*)node) < 16) {
        ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, &key_byte);
        ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, &child);
        node->head.child_count++;
        return &node->child_arr[i];
    }
    else {
        ArNode48* new_node48 = ArNode48Create(tree);
        ArNodeHeadCopy((ArNode*)new_node48, (ArNode*)node);
        *node_ptr = (ArNode16*)new_node48;
        for (ptrdiff_t i = 0; i < node->head.child_count; i++) {
            ArNode48Insert(tree, (ArNode48**)node_ptr, node->keys[i], node->child_arr[i]);
        }
        ArNode** new_child = ArNode48Insert(tree, (ArNode48**)node_ptr, key_byte, child);
        ArNode16Release(tree, node);
        return new_child;
    }
}

static ArNode** ArNode4Insert(ArTree* tree, ArNode4** node_ptr, uint8_t key_byte, ArNode* child) {
    ArNode4* node = *node_ptr;
    ptrdiff_t i;
    if (node->head.child_count > 0) {
        for (i = 0; i < node->head.child_count; i++) {
            if (key_byte == node->keys[i]) {
                node->keys[i] = key_byte;
                node->child_arr[i] = child;
                return &node->child_arr[i];
            }
            else if (key_byte < node->keys[i]) {
                break;
            }
        }
    }
    else {
        i = 0;
    }
    if (ArNodeGetFullCount((ArNode*)node) < 4) {
        ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, &key_byte);
        ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, &child);
        node->head.child_count++;
        return &node->child_arr[i];
    }
    else {
        ArNode16* new_node16 = ArNode16Create(tree);
        ArNodeHeadCopy((ArNode*)new_node16, (ArNode*)node);
        *node_ptr = (ArNode4*)new_node16;
        for (int32_t i = 0; i < node->head.child_count; i++) {
            ArNode16Insert(tree, (ArNode16**)node_ptr, node->keys[i], node->child_arr[i]);
        }
        ArNode** new_child = ArNode16Insert(tree, (ArNode16**)node_ptr, key_byte, child);
        ArNode4Release(tree, node);
        return new_child;
    }
}

static ArNode** ArNodeInsert(ArTree* tree, ArNode** node, uint8_t key_byte, ArNode* child) {
    ArNode** new_chile;
    switch ((*node)->head.node_type) {
    case kArNode4: {
        new_chile = ArNode4Insert(tree, (ArNode4**)node, key_byte, child);
        break;
    }
    case kArNode16: {
        new_chile = ArNode16Insert(tree, (ArNode16**)node, key_byte, child);
        break;
    }
    case kArNode48: {
        new_chile = ArNode48Insert(tree, (ArNode48**)node, key_byte, child);
        break;
    }
    case kArNode256: {
        new_chile = ArNode256Insert(tree, (ArNode256**)node, key_byte, child);
        break;
    }
    default: {
         assert(0);
        new_chile = NULL;
        break;
    }
    }
    return new_chile;
}


static void ArNode4Delete(ArTree* tree, ArNode4** node_ptr, uint8_t key_byte) {
    ArNode4* node = *node_ptr;
     assert(ArNodeGetFullCount((ArNode*)node) > 0);
    int32_t i = ArNodeKeyArrayFind(node->keys, node->head.child_count, &key_byte);
    if (i != AR_TREE_ARRAY_InvalidId) {
        ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
        ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
        node->head.child_count--;
    }
}

static void ArNode16Delete(ArTree* tree, ArNode16** node_ptr, uint8_t key_byte) {
    ArNode16* node = *node_ptr;
     assert(ArNodeGetFullCount((ArNode*)node) >= 2);
    int32_t i = ArNode16KeySearch(node, key_byte);
    if (i != AR_TREE_ARRAY_InvalidId) {
        ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
        ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
        node->head.child_count--;
        if (ArNodeGetFullCount((ArNode*)node) <= 2) {
            ArNode4* new_node4 = ArNode4Create(tree);
            ArNodeHeadCopy((ArNode*)new_node4, (ArNode*)node);
            *node_ptr = (ArNode16*)new_node4;
            for (int i = 0; i < node->head.child_count; i++) {
                ArNode4Insert(tree, (ArNode4**)node_ptr, node->keys[i], node->child_arr[i]);
            }
            ArNode16Release(tree, node);
        }
    }
}

static void ArNode48Delete(ArTree* tree, ArNode48** node_ptr, uint8_t key_byte) {
    ArNode48* node = *node_ptr;
     assert(node->head.child_count >= 10);
    uint8_t child_index = node->keys[key_byte];
    if (child_index != 0xff) {
        ArNode48StaticListPush(&node->child_arr, 0, node->keys[key_byte]);
        node->head.child_count--;
        node->keys[key_byte] = 0xff;
        if (node->head.child_count <= 10) {
            ArNode16* new_node16 = ArNode16Create(tree);
            ArNodeHeadCopy((ArNode*)new_node16, (ArNode*)node);
            *node_ptr = (ArNode48*)new_node16;
            for (int32_t i = 0; i < 256; i++) {
                if (node->keys[i] != 0xff) {
                    ArNode16Insert(tree, (ArNode16**)node_ptr, i, node->child_arr.obj_arr[node->keys[i]].child);
                }
            }
            ArNode48Release(tree, node);
        }
    }
}

static void ArNode256Delete(ArTree* tree, ArNode256** node_ptr, uint8_t key_byte) {
    ArNode256* node = *node_ptr;
        assert(node->head.child_count >= 32);
    ArNode* find_node = node->child_arr[key_byte];
    if (find_node != InvalidId) {
        node->head.child_count--;
        node->child_arr[key_byte] = InvalidId;
        if (node->head.child_count <= 32) {
            ArNode48* new_node48 = ArNode48Create(tree);
            ArNodeHeadCopy((ArNode*)new_node48, (ArNode*)node);
            *node_ptr = (ArNode256*)new_node48;
            for (int32_t i = 0; i < 256; i++) {
                if (node->child_arr[i] != NULL) {
                    ArNode48Insert(tree, (ArNode48**)node_ptr, i, node->child_arr[i]);
                }
            }
            ArNode256Release(tree, node);
        }
    }
}

static void ArNodeDelete(ArTree* tree, ArNode** node, uint8_t key_byte) {
    switch ((*node)->head.node_type) {
    case kArNode4: {
        ArNode4Delete(tree, (ArNode4**)node, key_byte);
        break;
    }
    case kArNode16: {
        ArNode16Delete(tree, (ArNode16**)node, key_byte);
        return;
    }
    case kArNode48: {
        ArNode48Delete(tree, (ArNode48**)node, key_byte);
        break;
    }
    case kArNode256: {
        ArNode256Delete(tree, (ArNode256**)node, key_byte);
        break;
    }
    default: {
            assert(0);
        break;
    }
    }
}



ArLeaf* ArNodeFirst(ArNode* node) {
    if (ArNodeIsLeaf(node)) {
        return ArNodeGetLeaf(node);
    }
    switch (node->head.node_type) {
    case kArNode4:
         assert(node->head.child_count >= 1 && node->head.child_count <= 4);
        return ArNodeFirst(node->node4.child_arr[0]);
    case kArNode16:
         assert(node->head.child_count >= 5 && node->head.child_count <= 16);
        return ArNodeFirst(node->node16.child_arr[0]);
    case kArNode48:
         assert(node->head.child_count >= 17 && node->head.child_count <= 48);
        for (int i = 0; i < 255; i++) {
            if (node->node48.keys[i] != 0xff) {
                return ArNodeFirst(node->node48.child_arr.obj_arr[node->node48.keys[i]].child);
            }
        }
    case kArNode256:
        assert(node->head.child_count >= 49 && node->head.child_count <= 256);
        for (int i = 0; i < 255; i++) {
            if (node->node256.child_arr[i] != NULL) {
                return ArNodeFirst(node->node256.child_arr[i]);
            }
        }
    }
    return NULL;
}


/*
* 比较前缀，返回匹配的长度
*/
static uint32_t ArNodeMatchPrefix(ArNodeHead* head, uint8_t* key, uint32_t key_len, uint32_t offset, bool* optimistic, art_key_type_t* optimistic_leaf_node_key, uint32_t* prefix_len) {
    uint8_t* node_key = head->prefix;
    if (head->prefix_len > prefix_max_len) {
        if (optimistic_leaf_node_key == NULL) {
            /* 乐观策略，只比较已保存的前缀 */
            *prefix_len = prefix_max_len;
            if (!*optimistic) *optimistic = true;
        }
        else {
            /* 已经是重试状态，完整进行比较 */
            *prefix_len = head->prefix_len;
            
            node_key = ArKeyGetByteBuf(&optimistic_leaf_node_key, offset);
        }
    }
    else {
        *prefix_len = head->prefix_len;
    }
    uint32_t match_len = min(*prefix_len, key_len);
    if (match_len) {
        uint32_t i = 0;
        for (; i < match_len; i++) {
            if (key[i] != node_key[i]) {
                break;
            }
        }
        match_len = i;
    }
    if (head->prefix_len > prefix_max_len && optimistic_leaf_node_key == NULL && match_len == prefix_max_len) {
        // 乐观策略的跳过阶段
        match_len = head->prefix_len;
        *prefix_len = match_len;
    }
    return match_len;
}

/*
* 为当前节点头部更新共同前缀，返回长度
*/
static void ArNodeUpdatePrefix(ArNodeHead* head, uint8_t* prefix, int32_t match_len) {
    head->prefix_len = match_len;
    for (int32_t i = 0; i < min(prefix_max_len, match_len); i++) {
        head->prefix[i] = prefix[i];
    }
}

/*
* parent即将被删除，将parent的前缀、索引child的字节合并到child的前缀中
*/
static void ArNodeMergePrefix(ArLeaf* del_leaf, uint32_t offset, uint32_t len, ArNode* child, uint8_t key_byte) {
    if (ArNodeIsLeaf(child)) {
        return;
    }

    uint32_t new_len = len + 1 + child->head.prefix_len;
    child->head.prefix_len = new_len;

    uint32_t child_vaild_prefix_len = min(prefix_max_len, child->head.prefix_len);

    uint8_t temp_buf[prefix_max_len];
    memcpy(temp_buf, child->head.prefix, child_vaild_prefix_len);

    // 先拷贝parent的前缀部分
    art_element_type* element = ArElementGetKey(ArLeafGetElement(del_leaf));
    uint8_t* buf = ArKeyGetByteBuf(&element, offset);
    uint32_t parent_copy_len = min(prefix_max_len, len);
    memcpy(child->head.prefix, buf, parent_copy_len);
    
    // 中间是索引字节
    uint32_t child_byte_copy_len = min(prefix_max_len - parent_copy_len, 1);
    memcpy(&child->head.prefix[len], &key_byte, child_byte_copy_len);

    // 最后是child原本的前缀
    uint32_t child_copy_len = min(prefix_max_len - parent_copy_len - child_byte_copy_len, child_vaild_prefix_len);
    memcpy(&child->head.prefix[len + 1], temp_buf, child_copy_len);
}

/*
* 分裂节点为node4
* 会执行节点分裂的情况：
* 1.走到叶子节点(node为叶子)
* 2.查找key长度不足
*     剩余长度为0(正好与前缀相等/查找下一个字符的offset++，且不是叶子)
*     不足以匹配node的前缀
* 3.前缀不匹配
* 
* 返回分裂后的新node4
*/
static ArNode* ArNodeSplit(ArTree* tree, ArNode* node, ArLeaf* leaf, uint32_t offset, art_key_type_t* optimistic_leaf_node_key) {
    art_key_type_t* leaf_key = ArElementGetKey(ArLeafGetElement(leaf));
    uint8_t* leaf_key_buf = ArKeyGetByteBuf(&leaf_key, offset);
    uint32_t leaf_key_len = ArKeyGetLen(leaf_key) - offset;
    art_key_type_t* node_key;
    uint8_t* node_key_buf;
    uint32_t node_key_len;
    uint32_t min_len;
    if (ArNodeIsLeaf(node)) {
        ArLeaf* node_leaf = ArNodeGetLeaf(node);
        node_key = ArElementGetKey(ArLeafGetElement(node_leaf));
        node_key_buf = ArKeyGetByteBuf(&node_key, offset);
        node_key_len = ArKeyGetLen(node_key) - offset;
    }
    else {
        if (node->head.prefix_len > min(prefix_max_len, node->head.prefix_len) && optimistic_leaf_node_key) {
            node_key_buf = ArKeyGetByteBuf(&optimistic_leaf_node_key, offset);
            node_key_len = node->head.prefix_len;
        }
        else {
            node_key_buf = node->head.prefix;
            node_key_len = min(prefix_max_len, node->head.prefix_len);
        }
    }
    min_len = min(node_key_len, leaf_key_len);
    uint32_t match_len = 0;
    for (; match_len < min_len; match_len++) {
        if (leaf_key_buf[match_len] != node_key_buf[match_len]) {
            break;
        }
    }
    ArNode4* new_node4 = ArNode4Create(tree);
    ArNodeUpdatePrefix(&new_node4->head, leaf_key_buf, match_len);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    if (match_len == min_len) {
        // 是子串，子串部分放到eof特殊指针中
        // 如果都为0，优先是node包含leaf，因为node除了前缀还有别的节点，实际长度更长
        if (min_len == leaf_key_len) {
            ArNodeSetEofChild((ArNode*)new_node4, leaf);
            ArNode4Insert(tree, &new_node4, node_key_buf[match_len], node);
        } else {
             assert(ArNodeIsLeaf(node));
            ArNodeSetEofChild((ArNode*)new_node4, (ArLeaf*)node);
            ArNode4Insert(tree, &new_node4, leaf_key_buf[match_len], (ArNode*)leaf);
        }
    }
    else {
#endif
        // 不是子串，直接插入
        ArNode4Insert(tree, &new_node4, node_key_buf[match_len], node);
        ArNode4Insert(tree, &new_node4, leaf_key_buf[match_len], (ArNode*)leaf);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
    }
#endif
    return (ArNode*)new_node4;
}



void ArTreeInit(ArTree* tree) {
    tree->root = InvalidId;
    tree->element_count = 0;
}

art_element_type* ArTreeFind(ArTree* tree, art_key_type_t* key) {
    ArNode* cur = tree->root;
    bool optimistic = false;
    uint32_t offset = 0;
    uint32_t optimistic_offset = -1;
    uint8_t* key_buf = ArKeyGetByteBuf(&key, 0);
    uint32_t key_len = ArKeyGetLen(key);
    while (cur) {
        if (ArNodeIsLeaf(cur)) {
            ArLeaf* cur_leaf = ArNodeGetLeaf(cur);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
            if (optimistic) {
                /* 需要进行乐观比较 */
                offset = optimistic_offset;
            }
#endif
            art_element_type* cur_element = ArLeafGetElement(cur_leaf);
            art_key_type_t* leaf_key = ArElementGetKey(cur_element);
            if (!ArKeyEqual(key, leaf_key, offset)) {
                return NULL;
            }
            return cur_element;
        }

        uint32_t prefix_len;
        uint32_t match_len = ArNodeMatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
        if (match_len != prefix_len) {
            return NULL;
        }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
#endif
        offset += match_len;

        if (offset > key_len) {
            break;
        }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        else if (offset == key_len) {
            ArLeaf* eof_child = *ArNodeGetEofChild(cur);
             assert(ArNodeIsLeaf((ArNode*)eof_child));
            cur = (ArNode*)eof_child;
            continue;
        }
#endif

        ArNode** cur_ptr = ArNodeFind(cur, key_buf[offset]);
        if (!cur_ptr) {
            break;
        }
        cur = *cur_ptr;
        offset++;
    }
    return NULL;
}

void ArTreePut(ArTree* tree, art_element_type* element) {
    if (tree->root == InvalidId) {
        ArLeaf* leaf = ArLeafCreate(tree, element);
        tree->element_count = 1;
        ArNodeSetLeaf(&tree->root, leaf);
        return;
    }

    uint32_t offset = 0;
    bool optimistic = false;
    uint32_t optimistic_offset = -1;
    ArNode** optimistic_retry_node_ptr = NULL;
    art_key_type_t* optimistic_leaf_node_key = NULL;
    art_key_type_t* key = ArElementGetKey(element);
    uint8_t* key_buf = ArKeyGetByteBuf(&key, 0);
    uint32_t key_len = ArKeyGetLen(key);
    ArNode** cur_ptr = &tree->root;
    do {
        ArNode* cur = *cur_ptr;
        if (ArNodeIsLeaf(cur)) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
            if (optimistic && optimistic_leaf_node_key == NULL) {
                /* 需要进行乐观比较 */
                offset = optimistic_offset;
            }
#endif
            ArLeaf* cur_leaf = ArNodeGetLeaf(cur);
            art_key_type_t* leaf_key = ArElementGetKey(ArLeafGetElement(cur_leaf));
            uint8_t* leaf_key_buf = ArKeyGetByteBuf(&leaf_key, 0);
            if (ArKeyEqual(key, leaf_key, offset)) {
                ArLeafSetElement(&cur_leaf, element);
                /* only update */
                return;
            }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
            if (optimistic && optimistic_leaf_node_key == NULL) {
                // 乐观策略的重试
                optimistic_leaf_node_key = leaf_key;
                cur_ptr = optimistic_retry_node_ptr;
                continue;
            }
#endif
            *cur_ptr = ArNodeSplit(tree, cur, ArLeafCreate(tree, element), offset, optimistic_leaf_node_key);
            return;
        }

        uint32_t match_len = 0;
        if (cur->head.prefix_len) {
            /*
            * 乐观策略对前缀的比较不完整，在最后找到叶子时，得到不匹配的偏移
            * 再次从乐观起始位置向下找，根据不匹配的偏移定位到不匹配的节点
            */
            uint32_t prefix_len;
            match_len = ArNodeMatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, optimistic_leaf_node_key, &prefix_len);
            if (optimistic_offset == -1 && optimistic) {
                optimistic_offset = offset;
                optimistic_retry_node_ptr = cur_ptr;
            }
            if (match_len != prefix_len) {
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
                if (optimistic && optimistic_leaf_node_key == NULL) {
                    offset = optimistic_offset;
                    // 如果之前存在过乐观判断，此时不能确定已比较的部分是匹配的，进行乐观重试
                    optimistic_leaf_node_key = ArElementGetKey(ArLeafGetElement(ArNodeFirst(cur)));
                    cur_ptr = optimistic_retry_node_ptr;
                    continue;
                }
#endif

                // 前缀匹配不完整，需要在这里分裂
                *cur_ptr = ArNodeSplit(tree, cur, ArLeafCreate(tree, element), offset, optimistic_leaf_node_key);

                // 更新前缀
                // 分裂后前缀需要去掉一个字节，因为在新的new_node4中的keys记录了当前node4的第一个字符
                // 匹配的前缀已经更新到新的node4上了，不匹配的前缀进行前移

                uint8_t byte = cur->head.prefix[match_len];
                match_len++;
                uint8_t* new_prefix;
                art_key_type_t* temp_key;
                if (cur->head.prefix_len > prefix_max_len) {
                    // 需要拿到一个叶子节点来更新当前前缀
                    temp_key = ArElementGetKey(ArLeafGetElement(ArNodeFirst(cur)));
                    new_prefix = ArKeyGetByteBuf(&temp_key, offset + match_len);
                }
                else {
                    new_prefix = &cur->head.prefix[match_len];
                }
                cur->head.prefix_len -= match_len;
                prefix_len = min(cur->head.prefix_len, prefix_max_len);
                for (uint32_t i = 0; i < prefix_len; i++) {
                    cur->head.prefix[i] = new_prefix[i];
                }
                return;
            }
            
            offset += match_len;
        }
        if (offset > key_len) {
            // 从前缀匹配的地方重新计起，分裂为node4
            // 这里有重复前缀长度计算(性能有点影响)，实际上上面那个if已经算好了
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
            if (optimistic && optimistic_leaf_node_key == NULL) {
                offset = optimistic_offset;
                // 长度不足以定位到叶子节点，此处乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
                optimistic_leaf_node_key = ArElementGetKey(ArLeafGetElement(ArNodeFirst(cur)));
                cur_ptr = optimistic_retry_node_ptr;
                continue;
            }
#endif
            *cur_ptr = ArNodeSplit(tree, cur, ArLeafCreate(tree, element), offset - match_len, optimistic_leaf_node_key);
            return;
        }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        else if (offset == key_len) {
            ArLeaf** cur_ptr = ArNodeGetEofChild(cur);
             assert(ArNodeIsLeaf((ArNode*)(*cur_ptr)));
            continue;
        }
#endif

        ArNode** down_ptr = ArNodeFind(cur, key_buf[offset]);
        if (!down_ptr) {
            // 无法下降，直接插入到此
            if (optimistic && optimistic_leaf_node_key == NULL) {
                offset = optimistic_offset;
                // 到这里不能保证前缀是匹配的，乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
                optimistic_leaf_node_key = ArElementGetKey(ArLeafGetElement(ArNodeFirst(cur)));
                cur_ptr = optimistic_retry_node_ptr;
                continue;
            }
            ArNodeInsert(tree, cur_ptr, key_buf[offset], (ArNode*)ArLeafCreate(tree, element));
            break;
        }
        offset++;
        cur_ptr = down_ptr;
    } while (true);

}

/*
* Art不会存在只有一个子节点的Node4的情况，因此删除只需要父节点的参与
*/
art_element_type* ArTreeDelete(ArTree* tree, art_key_type_t* key) {
    ArNode** cur_ptr = &tree->root;
    ArNode** parent_ptr = NULL;
    uint8_t parent_byte;
    bool optimistic = false;
    uint32_t offset = 0;
    uint32_t optimistic_offset = -1;
    uint8_t* key_buf = ArKeyGetByteBuf(&key, 0);
    uint32_t key_len = ArKeyGetLen(key);
    do {
        ArNode* cur = *cur_ptr;
        if (ArNodeIsLeaf(cur)) {
            ArLeaf* cur_leaf = ArNodeGetLeaf(cur);
            art_element_type* cur_element = ArLeafGetElement(cur_leaf);

            uint32_t marge_offset = offset - 1;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
            if (optimistic) {
                /* 需要进行乐观比较 */
                offset = optimistic_offset;
            }
#endif
            art_key_type_t* leaf_key = ArElementGetKey(cur_element);
            if (!ArKeyEqual(key, leaf_key, offset)) {
                return NULL;
            }
            do {
                if (!parent_ptr) {
                    // 是根节点，直接删除
                    *cur_ptr = NULL;
                    break;
                }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
                ArNode** eof_chile = (ArNode**)ArNodeGetEofChild(*parent_ptr);
                // 先删除节点
                if (cur_ptr != eof_chile) {
#endif
                    ArNodeDelete(tree, parent_ptr, parent_byte);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
                }
                else {
                    cur->head.eof = 0;
                }
#endif
                 assert(!ArNodeIsLeaf(*parent_ptr));
                 assert((*parent_ptr)->head.child_count > 0);
                if ((*parent_ptr)->head.node_type == kArNode4 && ArNodeGetFullCount(*parent_ptr) == 1) {
                    // 只剩下一个子节点，Node4路径压缩的合并处理，使指向父节点的 祖父节点的child_arr元素 指向父节点剩下的一个孩子节点
                    ArNode* parent = *parent_ptr;
                    ArNode* child;
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
                    if ((*parent_ptr)->head.child_count == 0) {
                        // 如果剩下了一个eof
                        child = *eof_chile;
                         assert(ArNodeIsLeaf(child));
                    }
                    else {
#endif
                         assert((*parent_ptr)->head.child_count > 0);
                        child = parent->node4.child_arr[0];
                        // 将父节点的前缀和余下孩子的前缀进行合并
                        ArNodeMergePrefix(cur_leaf, marge_offset - parent->head.prefix_len, parent->head.prefix_len, child, parent->node4.keys[0]);
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
                    }
#endif
                    (*parent_ptr) = child;

                    // 释放父节点
                    ArNode4Release(tree, &parent->node4);
                }
            } while (false);
            ArLeafRelease(tree, cur_leaf);
            return cur_element;
        }

        uint32_t prefix_len;
        uint32_t match_len = ArNodeMatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
        if (match_len != prefix_len) {
            return NULL;
        }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
#endif
        offset += match_len;

        parent_ptr = cur_ptr;
        if (offset > key_len) {
            break;
        }
#ifndef LIBYUC_CONTAINER_AR_TREE_KEY_MODE_FIXED
        else if (offset == key_len) {
            ArLeaf** cur_ptr = ArNodeGetEofChild((ArNode*)cur);
             assert(ArNodeIsLeaf((ArNode*)(*cur_ptr)));
            continue;
        }
#endif
        parent_byte = key_buf[offset];

        cur_ptr = ArNodeFind(cur, key_buf[offset]);
        if (!cur_ptr) {
            break;
        }
        cur = *cur_ptr;
        offset++;
    } while (true);
    return NULL;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_AR_TREE_H_