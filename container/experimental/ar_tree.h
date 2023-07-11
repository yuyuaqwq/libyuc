/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/



#ifndef LIBYUC_CONTAINER_AR_TREE_H_
#define LIBYUC_CONTAINER_AR_TREE_H_

#include <libyuc/object.h>
#include <libyuc/algorithm/binary_search.h>
#include <libyuc/algorithm/array.h>
#include <libyuc/container/static_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* Adaptive Radix Tree - 自适应基数树
*/

typedef struct _ArNode* id_type;
struct data {
	uint32_t size;
	uint8_t* buf;
};
typedef struct data key_type;
typedef struct data element_type;
#define InvalidId 0
#define prefix_max_len 8


/*
* 可以通过两个宏选项来声明不同的结构体
* 叶子节点是指针还是嵌入
* 通过传参传递
*/

typedef struct _ArLeaf {
	uint8_t node_type : 4;
	element_type element;
} ArLeaf;


typedef enum {
	kArLeaf,		// 如果是指针的话，可以嵌入，不用放在这里
	kArNode4,
	kArNode16,
	kArNode48,
	kArNode256,
} ArNodeType;


typedef struct _ArNodeHead {
	uint16_t node_type : 4;
	uint16_t : 3;
	uint16_t child_count : 9;		// 需要注意这里是8位的话不够表示node256的最大数量
	uint32_t prefix_len;
	struct _ArLeaf* eof_child;		// 与当前前缀匹配的孩子
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
LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(ArNode48, uint32_t, ArNode48Child, 1, 48)

typedef struct {
	ArNodeHead head;
	uint8_t keys[256];
	ArNode48StaticList child_arr;
} ArNode48;

typedef struct {
	ArNodeHead head;
	struct _ArNode* child_arr[256];
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


key_type* ArGetKey(element_type* element) {
	return element;
}

uint32_t ArGetKeyLen(key_type* key) {
	return key->size;
}

uint8_t* ArGetKeyByte(key_type* key, uint32_t offset) {
	return &key->buf[offset];
}

bool ArKeyEqual(key_type* key1, key_type* key2, uint32_t offset) {
	return key1->size == key2->size && !memcmp(&key1->buf[offset], &key2->buf[offset], key1->size - offset);
}

key_type* ArGetLeafKey(ArLeaf* child) {
	return ArGetKey(&child->element);
}

element_type* ArGetElement(ArNode* child) {
	return &child->leaf.element;
}




uint8_t* ArBsAccessor_GetKey(uint8_t* arr, uint8_t* element) {
	return element;
}
LIBYUC_ALGORITHM_BINARY_SEARCH_DEFINE(ArNodeKey, uint8_t, uint8_t, int32_t, ArBsAccessor, LIBYUC_OBJECT_INDEXER_DEFALUT, LIBYUC_OBJECT_COMPARER_DEFALUT)
LIBYUC_CONTAINER_STATIC_LIST_DEFINE(ArNode48, uint32_t, ArNode*, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, 1)

#define AR_TREE_ARRAY_REFERENCER_InvalidId (-1)
#define AR_TREE_ARRAY_REFERENCER AR_TREE_ARRAY_REFERENCER
LIBYUC_ALGORITHM_ARRAY_DEFINE(ArNodeKey, uint8_t, ptrdiff_t, AR_TREE_ARRAY_REFERENCER, LIBYUC_OBJECT_COMPARER_DEFALUT)
LIBYUC_ALGORITHM_ARRAY_DEFINE(ArNodeChild, ArNode*, ptrdiff_t, AR_TREE_ARRAY_REFERENCER, LIBYUC_OBJECT_COMPARER_DEFALUT)

static void ArNodeHeadInit(ArNodeHead* head, ArNodeType type) {
	head->child_count = 0;
	head->node_type = type;
	head->prefix_len = 0;
	head->eof_child = NULL;
}

void ArNodeHeadCopy(ArNodeHead* dst, ArNodeHead* src) {
	memcpy(dst->prefix, src->prefix, min(prefix_max_len, src->prefix_len));
	dst->prefix_len = src->prefix_len;
	dst->eof_child = src->eof_child;
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
* 顺序查找性能略高
* 如果固定查找长度或许可以使编译器优化为更快的查找(msvc并未进行优化)
*/
static ArNode** ArNode16Find(ArNode16* node, uint8_t key_byte) {
	// ptrdiff_t i = ArNodeKeyBinarySearch(node->keys, 0, node->head.child_count - 1, &key_byte);
	// ptrdiff_t i = ArNodeKeyArrayFind(node->keys, 16, &key_byte);
	// if (i == AR_TREE_ARRAY_REFERENCER_InvalidId || i >= node->head.child_count) {
	ptrdiff_t i = ArNodeKeyArrayFind(node->keys, node->head.child_count, &key_byte);
	if (i == AR_TREE_ARRAY_REFERENCER_InvalidId) {
		return InvalidId;
	}
	return &node->child_arr[i];
}

static ArNode** ArNode4Find(ArNode4* node, uint8_t key_byte) {
	// ptrdiff_t i = ArNodeKeyArrayFind(node->keys, 4, &key_byte);
	// if (i == AR_TREE_ARRAY_REFERENCER_InvalidId || i >= node->head.child_count) {
	 ptrdiff_t i =  ArNodeKeyArrayFind(node->keys, node->head.child_count, &key_byte);
	 if (i == AR_TREE_ARRAY_REFERENCER_InvalidId) {
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


static ArLeaf* ArLeafCreate(ArTree* tree, element_type* ele) {
	ArLeaf* leaf = ObjectCreate(ArLeaf);
	leaf->element = *ele;
	leaf->node_type = kArLeaf;
	return leaf;
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
	ArNode48StaticListInit(&node48->child_arr, 48);
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

static void ArLeafRelease(ArTree* tree, ArLeaf* leaf) {
	ObjectRelease(leaf);
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


static void ArNode256Insert(ArTree* tree, ArNode256** node_ptr, uint8_t key_byte, ArNode* child) {
	  assert(child);
	ArNode256* node = *node_ptr;
	ArNode* find_node = node->child_arr[key_byte];
	if (find_node == InvalidId) {
		node->head.child_count++;
	}
	node->child_arr[key_byte] = child;
	//int n = 0;
	//for (int i = 0; i < 256; i++) {
	//	if (node->child_arr[i]) {
	//		n++;
	//	}
	//}
	//assert(n == node->head.child_count);
}

static void ArNode48Insert(ArTree* tree, ArNode48** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode48* node = *node_ptr;
	uint8_t child_index = node->keys[key_byte];
	if (child_index == 0xff) {
		if (node->head.child_count >= 48) {
			ArNode256* new_node256 = ArNode256Create(tree);
			ArNodeHeadCopy(&new_node256->head, &node->head);
			*node_ptr = (ArNode48*)new_node256;
			for (int32_t i = 0; i < 256; i++) {
				if (node->keys[i] != 0xff) {
					ArNode256Insert(tree, (ArNode256**)node_ptr, i, node->child_arr.obj_arr[node->keys[i]].child);
				}
			}
			ArNode256Insert(tree, (ArNode256**)node_ptr, key_byte, child);
			ArNode48Release(tree, node);
			return;
		}
		child_index = ArNode48StaticListPop(&node->child_arr, 0);
		node->keys[key_byte] = child_index;
	}
	node->child_arr.obj_arr[child_index].child = child;
	node->head.child_count++;
}

static void ArNode16Insert(ArTree* tree, ArNode16** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode16* node = *node_ptr;
	int32_t i;
	if (node->head.child_count > 0) {
		i = ArNodeKeyBinarySearch_Range(node->keys, 0, node->head.child_count - 1, &key_byte);
		if (key_byte == node->keys[i]) {
			node->keys[i] = key_byte;
			node->child_arr[i] = child;
			return;
		}
		if (key_byte > node->keys[i]) i++;
	}
	else {
		i = 0;
	}
	if (node->head.child_count < 16) {
		ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, &key_byte);
		ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, &child);
		node->head.child_count++;
	}
	else {
		ArNode48* new_node48 = ArNode48Create(tree);
		ArNodeHeadCopy(&new_node48->head, &node->head);
		*node_ptr = (ArNode16*)new_node48;
		for (int32_t i = 0; i < 16; i++) {
			ArNode48Insert(tree, (ArNode48**)node_ptr, node->keys[i], node->child_arr[i]);
		}
		ArNode48Insert(tree, (ArNode48**)node_ptr, key_byte, child);
		ArNode16Release(tree, node);
	}
}

static void ArNode4Insert(ArTree* tree, ArNode4** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode4* node = *node_ptr;
	int32_t i;
	if (node->head.child_count > 0) {
		i = ArNodeKeyBinarySearch_Range(node->keys, 0, node->head.child_count - 1, &key_byte);
		if (key_byte == node->keys[i]) {
			node->keys[i] = key_byte;
			node->child_arr[i] = child;
			return;
		}
		if (key_byte > node->keys[i]) i++;
	}
	else {
		i = 0;
	}
	if (node->head.child_count < 4) {
		ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, &key_byte);
		ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, &child);
		node->head.child_count++;
	}
	else {
		ArNode16* new_node16 = ArNode16Create(tree);
		ArNodeHeadCopy(&new_node16->head, &node->head);
		*node_ptr = (ArNode4*)new_node16;
		for (int32_t i = 0; i < 4; i++) {
			ArNode16Insert(tree, (ArNode16**)node_ptr, node->keys[i], node->child_arr[i]);
		}
		ArNode16Insert(tree, (ArNode16**)node_ptr, key_byte, child);
		ArNode4Release(tree, node);
	}
}

static void ArNodeInsert(ArTree* tree, ArNode** node, uint8_t key_byte, ArNode* child) {
	switch ((*node)->head.node_type) {
	case kArNode4: {
		ArNode4Insert(tree, (ArNode4**)node, key_byte, child);
		break;
	}
	case kArNode16: {
		ArNode16Insert(tree, (ArNode16**)node, key_byte, child);
		return;
	}
	case kArNode48: {
		ArNode48Insert(tree, (ArNode48**)node, key_byte, child);
		break;
	}
	case kArNode256: {
		ArNode256Insert(tree, (ArNode256**)node, key_byte, child);
		break;
	}
	default: {
		assert(0);
		break;
	}
	}
}

static void ArNode4Delete(ArTree* tree, ArNode4** node_ptr, uint8_t key_byte) {
	ArNode4* node = *node_ptr;
	  assert(node->head.child_count > 0);
	int32_t i = ArNodeKeyArrayFind(node->keys, node->head.child_count, &key_byte);
	if (i != AR_TREE_ARRAY_REFERENCER_InvalidId) {
		ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
		ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
		node->head.child_count--;
	}
}

static void ArNode16Delete(ArTree* tree, ArNode16** node_ptr, uint8_t key_byte) {
	ArNode16* node = *node_ptr;
	  assert(node->head.child_count >= 2);
	int32_t i = ArNodeKeyBinarySearch(node->keys, 0, node->head.child_count - 1, &key_byte);
	if (i != AR_TREE_ARRAY_REFERENCER_InvalidId) {
		ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
		ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
		node->head.child_count--;
		if (node->head.child_count <= 2) {
			ArNode4* new_node4 = ArNode4Create(tree);
			ArNodeHeadCopy(&new_node4->head, &node->head);
			*node_ptr = (ArNode16*)new_node4;
			for (int i = 0; i < 2; i++) {
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
			ArNodeHeadCopy(&new_node16->head, &node->head);
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
			ArNodeHeadCopy(&new_node48->head, &node->head);
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
	switch (node->head.node_type) {
	case kArLeaf:
		return &node->leaf;
	case kArNode4:
		return ArNodeFirst(node->node4.child_arr[0]);
	case kArNode16:
		return ArNodeFirst(node->node16.child_arr[0]);
	case kArNode48:
		for (int i = 0; i < 255; i++) {
			if (node->node48.keys[i] != 0xff) {
				return ArNodeFirst(node->node48.child_arr.obj_arr[node->node48.keys[i]].child);
			}
		}
	case kArNode256:
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
static uint32_t ArNodeMatchPrefix(ArNodeHead* head, uint8_t* key, uint32_t key_len, uint32_t offset, bool* optimistic, key_type* optimistic_leaf_node_key, uint32_t* prefix_len) {
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
			node_key = &optimistic_leaf_node_key->buf[offset];
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
	if (child->head.node_type == kArLeaf) {
		return;
	}

	uint32_t new_len = len + 1 + child->head.prefix_len;
	child->head.prefix_len = new_len;

	uint32_t child_vaild_prefix_len = min(prefix_max_len, child->head.prefix_len);

	uint8_t temp_buf[prefix_max_len];
	memcpy(temp_buf, child->head.prefix, child_vaild_prefix_len);

	// 先拷贝parent的前缀部分
	uint8_t* buf = ArGetKeyByte(ArGetLeafKey(del_leaf), offset);
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
*   剩余长度为0(正好与前缀相等/查找下一个字符的offset++，且不是叶子)
*   不足以匹配node的前缀
* 3.前缀不匹配
* 
* 返回分裂后的新node4
*/
static ArNode* ArNodeSplit(ArTree* tree, ArNode* node, ArLeaf* leaf, uint32_t offset, key_type* optimistic_leaf_node_key) {
	key_type* leaf_key = ArGetKey(&leaf->element);
	uint8_t* leaf_key_buf = ArGetKeyByte(leaf_key, offset);
	uint32_t leaf_key_len = ArGetKeyLen(leaf_key) - offset;
	uint8_t* node_key_buf;
	uint32_t node_key_len;
	uint32_t min_len;
	if (node->head.node_type == kArLeaf) {
		key_type* node_key = ArGetKey(&node->leaf.element);
		node_key_buf = ArGetKeyByte(node_key, offset);
		node_key_len = ArGetKeyLen(node_key) - offset;
	}
	else {
		if (node->head.prefix_len > min(prefix_max_len, node->head.prefix_len) && optimistic_leaf_node_key) {
			node_key_buf = &optimistic_leaf_node_key->buf[offset];
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
	if (match_len == min_len) {
		// 是子串，子串部分放到eof特殊指针中
		// 如果都为0，优先是node包含leaf，因为node除了前缀还有别的节点，实际长度更长
		if (min_len == leaf_key_len) {
			new_node4->head.eof_child = leaf;
			ArNode4Insert(tree, &new_node4, node_key_buf[match_len], node);
		} else {
			  assert(node->head.node_type == kArLeaf);
			new_node4->head.eof_child = (ArLeaf*)node;
			ArNode4Insert(tree, &new_node4, leaf_key_buf[match_len], (ArNode*)leaf);
		}
	}
	else {
		// 不是子串，直接插入
		ArNode4Insert(tree, &new_node4, node_key_buf[match_len], node);
		ArNode4Insert(tree, &new_node4, leaf_key_buf[match_len], (ArNode*)leaf);
	}
	return (ArNode*)new_node4;
}



void ArTreeInit(ArTree* tree) {
	tree->root = InvalidId;
	tree->element_count = 0;
}

element_type* ArTreeFind(ArTree* tree, key_type* key) {
	ArNode* cur = tree->root;
	bool optimistic = false;
	uint32_t offset = 0;
	uint32_t optimistic_offset = -1;
	uint8_t* key_buf = ArGetKeyByte(key, 0);
	uint32_t key_len = ArGetKeyLen(key);
	while (cur) {
		if (cur->head.node_type == kArLeaf) {
			if (optimistic) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			key_type* leaf_key = ArGetKey(&cur->leaf.element);
			if (!ArKeyEqual(key, leaf_key, offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}

		uint32_t prefix_len;
		uint32_t match_len = ArNodeMatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
		if (match_len != prefix_len) {
			return NULL;
		}
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
		offset += match_len;

		if (offset == key_len) {
			  assert(cur->head.eof_child->node_type == kArLeaf);
			cur = (ArNode*)cur->head.eof_child;
			continue;
		}
		else if (offset > key_len) {
			break;
		}

		ArNode** cur_ptr = ArNodeFind(cur, key_buf[offset]);
		if (!cur_ptr) {
			break;
		}
		cur = *cur_ptr;
		offset++;
	}
	return NULL;
}

void ArTreePut(ArTree* tree, element_type* element) {
	if (tree->root == InvalidId) {
		tree->root = (ArNode*)ArLeafCreate(tree, element);
		tree->element_count = 1;
		tree->root->leaf.element = *element;
		tree->root->head.node_type = kArLeaf;
		return;
	}
	uint32_t offset = 0;
	bool optimistic = false;
	uint32_t optimistic_offset = -1;
	ArNode** optimistic_retry_node_ptr = NULL;
	key_type* optimistic_leaf_node_key = NULL;
	key_type* key = ArGetKey(element);
	uint8_t* key_buf = ArGetKeyByte(key, 0);
	uint32_t key_len = ArGetKeyLen(key);
	ArNode** cur_ptr = &tree->root;
	do {
		ArNode* cur = *cur_ptr;
		if (cur->head.node_type == kArLeaf) {
			if (optimistic && optimistic_leaf_node_key == NULL) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			key_type* leaf_key = ArGetKey(&cur->leaf.element);
			uint8_t* leaf_key_buf = ArGetKeyByte(leaf_key, 0);
			if (ArKeyEqual(key, leaf_key, offset)) {
				cur->leaf.element = *element;
				/* only update */
				return;
			}
			if (optimistic && optimistic_leaf_node_key == NULL) {
				// 乐观策略的重试
				optimistic_leaf_node_key = leaf_key;
				cur_ptr = optimistic_retry_node_ptr;
				continue;
			}
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
				if (optimistic && optimistic_leaf_node_key == NULL) {
					offset = optimistic_offset;
					// 如果之前存在过乐观判断，此时不能确定已比较的部分是匹配的，进行乐观重试
					optimistic_leaf_node_key = ArGetKey(ArGetElement((ArNode*)ArNodeFirst(cur)));
					cur_ptr = optimistic_retry_node_ptr;
					continue;
				}

				// 前缀匹配不完整，需要在这里分裂
				*cur_ptr = ArNodeSplit(tree, cur, ArLeafCreate(tree, element), offset, optimistic_leaf_node_key);

				// 更新前缀
				// 分裂后前缀需要去掉一个字节，因为在新的new_node4中的keys记录了当前node4的第一个字符
				// 匹配的前缀已经更新到新的node4上了，不匹配的前缀进行前移

				uint8_t byte = cur->head.prefix[match_len];
				match_len++;
				uint8_t* new_prefix;
				if (cur->head.prefix_len > prefix_max_len) {
					// 需要拿到一个叶子节点来更新当前前缀
					key_type* temp_key = ArGetKey(ArGetElement((ArNode*)ArNodeFirst(cur)));
					new_prefix = ArGetKeyByte(temp_key, offset + match_len);
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
			if (optimistic && optimistic_leaf_node_key == NULL) {
				offset = optimistic_offset;
				// 长度不足以定位到叶子节点，此处乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
				optimistic_leaf_node_key = ArGetKey(ArGetElement((ArNode*)ArNodeFirst(cur)));
				cur_ptr = optimistic_retry_node_ptr;
				continue;
			}
			*cur_ptr = ArNodeSplit(tree, cur, ArLeafCreate(tree, element), offset - match_len, optimistic_leaf_node_key);
			return;
		}
		else if (offset == key_len) {
			  assert(cur->head.eof_child->node_type == kArLeaf);
			cur_ptr = (ArNode**)&cur->head.eof_child;
			continue;
		}

		ArNode** down_ptr = ArNodeFind(cur, key_buf[offset]);
		if (!down_ptr) {
			// 无法下降，直接插入到此
			if (optimistic && optimistic_leaf_node_key == NULL) {
				offset = optimistic_offset;
				// 到这里不能保证前缀是匹配的，乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
				optimistic_leaf_node_key = ArGetKey(ArGetElement((ArNode*)ArNodeFirst(cur)));
				cur_ptr = optimistic_retry_node_ptr;
				continue;
			}
			ArLeaf* new_leaf = ArLeafCreate(tree, element);
			ArNodeInsert(tree, cur_ptr, key_buf[offset], (ArNode*)new_leaf);
			break;
		}
		offset++;
		cur_ptr = down_ptr;
	} while (true);

}

/*
* Art不会存在只有一个子节点的Node4的情况，因此删除只需要父节点的参与
*/
element_type* ArTreeDelete(ArTree* tree, key_type* key) {
	ArNode** cur_ptr = &tree->root;
	ArNode** parent_ptr = NULL;
	uint8_t parent_byte;
	bool optimistic = false;
	uint32_t offset = 0;
	uint32_t optimistic_offset = -1;
	uint8_t* key_buf = ArGetKeyByte(key, 0);
	uint32_t key_len = ArGetKeyLen(key);
	do {
		ArNode* cur = *cur_ptr;
		if (cur->head.node_type == kArLeaf) {
			uint32_t marge_offset = offset - 1;
			if (optimistic) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			key_type* leaf_key = ArGetKey(&cur->leaf.element);
			if (!ArKeyEqual(key, leaf_key, offset)) {
				return NULL;
			}
			element_type* element = ArGetElement(cur);
			do {
				if (!parent_ptr) {
					// 是根节点，直接删除
					*cur_ptr = NULL;
					break;
				}
				if (cur != (ArNode*)(*parent_ptr)->head.eof_child) {
					ArNodeDelete(tree, parent_ptr, parent_byte);
				}
				if ((*parent_ptr)->head.node_type == kArNode4
					&& 
					(
						(*parent_ptr)->head.child_count == 1 && cur == (ArNode*)(*parent_ptr)->head.eof_child
						|| (*parent_ptr)->head.child_count == 1 && (ArNode*)(*parent_ptr)->head.eof_child == NULL
						|| (*parent_ptr)->head.child_count < 1 
					)
					) {
					// Node4路径压缩的合并处理，使指向父节点的 祖父节点的child_arr元素 指向父节点剩下的一个孩子节点
					ArNode* parent = *parent_ptr;
					ArNode* child;
					if ((*parent_ptr)->head.child_count == 0) {
						child = (ArNode*)parent->node4.head.eof_child;
						  assert(child->head.node_type == kArLeaf);
					}
					else {
						  assert((*parent_ptr)->head.child_count > 0);
						child = parent->node4.child_arr[0];
						// 将父节点的前缀和余下孩子的前缀进行合并
						ArNodeMergePrefix(&cur->leaf, marge_offset - parent->head.prefix_len, parent->head.prefix_len, child, parent->node4.keys[0]);
					}
					(*parent_ptr) = child;

					// 释放父节点
					ArNode4Release(tree, &parent->node4);
				}
				if (cur == (ArNode*)(*parent_ptr)->head.eof_child) {
					*cur_ptr = NULL;
				}
			} while (false);
			ArLeafRelease(tree, &cur->leaf);
			return element;
		}

		uint32_t prefix_len;
		uint32_t match_len = ArNodeMatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
		if (match_len != prefix_len) {
			return NULL;
		}
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
		offset += match_len;


		parent_ptr = cur_ptr;
		if (offset == key_len) {
			  assert(cur->head.eof_child->node_type == kArLeaf);
			cur_ptr = (ArNode**)&cur->head.eof_child;
			continue;
		}
		else if (offset > key_len) {
			break;
		}

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