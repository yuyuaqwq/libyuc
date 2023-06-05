/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/



#ifndef CUTILS_CONTAINER_AR_TREE_H_
#define CUTILS_CONTAINER_AR_TREE_H_

#include <CUtils/object.h>
#include <CUtils/algorithm/binary_search.h>
#include <CUtils/algorithm/array.h>
#include <CUtils/container/static_list.h>

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
#define prefix_max_len 2

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
	kArNode2,
	kArNode4,
	kArNode16,
	kArNode48,
	kArNode256,
} ArNodeType;


typedef struct _ArNodeHead {
	uint8_t node_type : 4;
	uint8_t : 4;
	uint8_t child_count;
	uint32_t prefix_len;
	uint32_t vaild_prefix_len;
	union {
		uint8_t prefix[prefix_max_len];
	};
} ArNodeHead;

/* Node2理论上不表示一层树高(类似红黑树的红色节点设计)，只是一个中转用的节点，不能单独存在 */
typedef struct {
	/* sub_node是node的前缀/叶子的子串 */
	uint8_t node_type : 4;
	uint8_t : 4;
	uint32_t sub_node_len;
	struct _ArNode* ar_sub_node;
	struct _ArNode* ar_node;
} ArNode2;

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
CUTILS_CONTAINER_STATIC_LIST_DECLARATION(ArNode48, uint32_t, ArNode48Child, 1, 48)

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
		ArNode2 node2;
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

key_type* ArGetLeafKey(ArNode* child) {
	return ArGetKey(&child->leaf.element);
}


element_type* ArGetElement(ArNode* child) {
	return &child->leaf.element;
}




uint8_t* ArBsAccessor_GetKey(uint8_t* arr, uint8_t* element) {
	return element;
}
CUTILS_ALGORITHM_BINARY_SEARCH_DEFINE(ArNode, uint8_t, uint8_t, int32_t, ArBsAccessor, CUTILS_OBJECT_INDEXER_DEFALUT, CUTILS_OBJECT_COMPARER_DEFALUT)
CUTILS_CONTAINER_STATIC_LIST_DEFINE(ArNode48, uint32_t, ArNode*, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, 1)
CUTILS_ALGORITHM_ARRAY_DEFINE(ArNodeKey, uint8_t, uint32_t)
CUTILS_ALGORITHM_ARRAY_DEFINE(ArNodeChild, ArNode*, uint32_t)

static void ArHeadInit(ArNodeHead* head, ArNodeType type) {
	head->child_count = 0;
	head->node_type = type;
	head->prefix_len = 0;
	head->vaild_prefix_len = 0;
}

void ArNodeCopyHead(ArNodeHead* dst, ArNodeHead* src) {
	memcpy(dst->prefix, src->prefix, min(src->prefix_len, prefix_max_len));
	dst->prefix_len = src->prefix_len;
	dst->vaild_prefix_len = src->vaild_prefix_len;
}



static ArNode** ArTreeFindChild(ArNode* node, uint8_t key_byte) {
	ArNode** node_ptr = InvalidId;
	switch (node->head.node_type) {
	case kArNode4: {
		int32_t i = ArNodeBinarySearch(node->node4.keys, 0, node->head.child_count - 1, &key_byte);
		if (i == -1) {
			break;
		}
		node_ptr = &node->node4.child_arr[i];
		break;
	}
	case kArNode16: {
		int32_t i = ArNodeBinarySearch(node->node16.keys, 0, node->head.child_count - 1, &key_byte);
		if (i == -1) {
			break;
		}
		node_ptr = &node->node16.child_arr[i];
		break;
	}
	case kArNode48: {
		uint8_t index = node->node48.keys[key_byte];
		if (index != 0xff) {
			node_ptr = &node->node48.child_arr.obj_arr[index].child;
			break;
		}
		break;
	}
	case kArNode256: {
		node_ptr = &node->node256.child_arr[key_byte];
		if (!*node_ptr) {
			node_ptr = InvalidId;
		}
		break;
	}
	default: {
		  assert(0);
		break;
	}
	}
	return node_ptr;
}



static ArLeaf* ArLeafCreate(element_type* ele) {
	ArLeaf* leaf = ObjectCreate(ArLeaf);
	leaf->element = *ele;
	leaf->node_type = kArLeaf;
	return leaf;
}

static ArNode2* ArNode2Create() {
	ArNode2* node2 = ObjectCreate(ArNode2);
	node2->node_type = kArNode2;
	node2->sub_node_len = 0;
	return node2;
}

static ArNode4* ArNode4Create() {
	ArNode4* node4 = ObjectCreate(ArNode4);
	for (int i = 0; i < 4; i++) {
		node4->keys[i] = 0;
		node4->child_arr[i] = InvalidId;
	}
	ArHeadInit(&node4->head, kArNode4);
	return node4;
}

static ArNode16* ArNode16Create() {
	ArNode16* node16 = ObjectCreate(ArNode16);
	for (int i = 0; i < 16; i++) {
		node16->keys[i] = 0;
		node16->child_arr[i] = InvalidId;
	}
	ArHeadInit(&node16->head, kArNode16);
	return node16;
}

static ArNode48* ArNode48Create() {
	ArNode48* node48 = ObjectCreate(ArNode48);
	for (int i = 0; i < 256; i++) {
		node48->keys[i] = -1;
	}
	ArNode48StaticListInit(&node48->child_arr, 48);
	ArHeadInit(&node48->head, kArNode48);
	return node48;
}

static ArNode256* ArNode256Create() {
	ArNode256* node256 = ObjectCreate(ArNode256);
	for (int i = 0; i < 256; i++) {
		node256->child_arr[i] = InvalidId;
	}
	ArHeadInit(&node256->head, kArNode256);
	return node256;
}

static void ArLeafRelease(ArLeaf* leaf) {
	ObjectRelease(leaf);
}

static void ArNode2Release(ArNode2* node2) {
	ObjectRelease(node2);
}

static void ArNode4Release(ArNode4* node4) {
	ObjectRelease(node4);
}

static void ArNode16Release(ArNode16* node16) {
	ObjectRelease(node16);
}

static void ArNode48Release(ArNode48* node48) {
	ObjectRelease(node48);
}

static void ArNode256Release(ArNode256* node256) {
	ObjectRelease(node256);
}


static void ArNode256Insert(ArNode256** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode256* node = *node_ptr;
	ArNode* find_node = node->child_arr[key_byte];
	if (find_node == InvalidId) {
		node->head.child_count++;
	}
	node->child_arr[key_byte] = child;
}

static void ArNode48Insert(ArNode48** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode48* node = *node_ptr;
	uint8_t child_index = node->keys[key_byte];
	if (child_index == 0xff) {
		if (node->head.child_count >= 48) {
			ArNode256* new_node256 = ArNode256Create();
			ArNodeCopyHead(&new_node256->head, &node->head);
			*node_ptr = (ArNode48*)new_node256;
			for (int32_t i = 0; i < 256; i++) {
				if (node->keys[i] != 0xff) {
					ArNode256Insert((ArNode256**)node_ptr, i, node->child_arr.obj_arr[node->keys[i]].child);
				}
			}
			ArNode256Insert((ArNode256**)node_ptr, key_byte, child);
			ArNode48Release(node);
			return;
		}
		child_index = ArNode48StaticListPop(&node->child_arr, 0);
		node->keys[key_byte] = child_index;
	}
	node->child_arr.obj_arr[child_index].child = child;
	node->head.child_count++;
}

static void ArNode16Insert(ArNode16** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode16* node = *node_ptr;
	int32_t i;
	if (node->head.child_count > 0) {
		i = ArNodeBinarySearch_Range(node->keys, 0, node->head.child_count - 1, &key_byte);
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
		ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, key_byte);
		ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, child);
		node->head.child_count++;
	}
	else {
		ArNode48* new_node48 = ArNode48Create();
		ArNodeCopyHead(&new_node48->head, &node->head);
		*node_ptr = (ArNode16*)new_node48;
		for (int32_t i = 0; i < 16; i++) {
			ArNode48Insert((ArNode48**)node_ptr, node->keys[i], node->child_arr[i]);
		}
		ArNode48Insert((ArNode48**)node_ptr, key_byte, child);
		ArNode16Release(node);
	}
}

static void ArNode4Insert(ArNode4** node_ptr, uint8_t key_byte, ArNode* child) {
	ArNode4* node = *node_ptr;
	int32_t i;
	if (node->head.child_count > 0) {
		i = ArNodeBinarySearch_Range(node->keys, 0, node->head.child_count - 1, &key_byte);
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
		ArNodeKeyArrayInsert(node->keys, node->head.child_count, i, key_byte);
		ArNodeChildArrayInsert(node->child_arr, node->head.child_count, i, child);
		node->head.child_count++;
	}
	else {
		ArNode16* new_node16 = ArNode16Create();
		ArNodeCopyHead(&new_node16->head, &node->head);
		*node_ptr = (ArNode4*)new_node16;
		for (int32_t i = 0; i < 4; i++) {
			ArNode16Insert((ArNode16**)node_ptr, node->keys[i], node->child_arr[i]);
		}
		ArNode16Insert((ArNode16**)node_ptr, key_byte, child);
		ArNode4Release(node);
	}
}

static void ArNodeInsert(ArNode** node, uint8_t key_byte, ArNode* child) {
	switch ((*node)->head.node_type) {
	case kArNode4: {
		ArNode4Insert((ArNode4**)node, key_byte, child);
		break;
	}
	case kArNode16: {
		ArNode16Insert((ArNode16**)node, key_byte, child);
		return;
	}
	case kArNode48: {
		ArNode48Insert((ArNode48**)node, key_byte, child);
		break;
	}
	case kArNode256: {
		ArNode256Insert((ArNode256**)node, key_byte, child);
		break;
	}
	default: {
		assert(0);
		break;
	}
	}
}

static void ArNode4Delete(ArNode4** node_ptr, uint8_t key_byte) {
	ArNode4* node = *node_ptr;
	int32_t i = ArNodeBinarySearch(node->keys, 0, node->head.child_count - 1, &key_byte);
	if (i != -1) {
		ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
		ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
		node->head.child_count--;
	}
}

static void ArNode16Delete(ArNode16** node_ptr, uint8_t key_byte) {
	ArNode16* node = *node_ptr;
	  assert(node->head.child_count < 3);
	int32_t i = ArNodeBinarySearch(node->keys, 0, node->head.child_count - 1, &key_byte);
	if (i != -1) {
		ArNodeKeyArrayDelete(node->keys, node->head.child_count, i);
		ArNodeChildArrayDelete(node->child_arr, node->head.child_count, i);
		node->head.child_count--;
		if (node->head.child_count <= 2) {
			ArNode4* new_node4 = ArNode4Create();
			ArNodeCopyHead(&new_node4->head, &node->head);
			*node_ptr = (ArNode16*)new_node4;
			for (int i = 0; i < 2; i++) {
				ArNode4Insert((ArNode4**)node_ptr, node->keys[i], node->child_arr[i]);
			}
			ArNode16Release(node);
		}
	}
}

static void ArNode48Delete(ArNode48** node_ptr, uint8_t key_byte) {
	ArNode48* node = *node_ptr;
	uint8_t child_index = node->keys[key_byte];
	if (child_index != 0xff) {
		ArNode48StaticListPush(&node->child_arr, 0, node->keys[key_byte]);
		node->head.child_count--;
		node->keys[key_byte] = 0xff;
		if (node->head.child_count <= 10) {
			ArNode16* new_node16 = ArNode16Create();
			ArNodeCopyHead(&new_node16->head, &node->head);
			*node_ptr = (ArNode48*)new_node16;
			for (int32_t i = 0; i < 256; i++) {
				if (node->keys[i] != 0xff) {
					ArNode16Insert((ArNode16**)node_ptr, node->keys[i], node->child_arr.obj_arr[node->keys[i]].child);
				}
			}
			ArNode48Release(node);
		}
	}
}

static void ArNode256Delete (ArNode256** node_ptr, uint8_t key_byte) {
	ArNode256* node = *node_ptr;
	ArNode* find_node = node->child_arr[key_byte];
	if (find_node != InvalidId) {
		node->head.child_count--;
		node->child_arr[key_byte] = InvalidId;
		if (node->head.child_count <= 32) {
			ArNode48* new_node48 = ArNode48Create();
			ArNodeCopyHead(&new_node48->head, &node->head);
			*node_ptr = (ArNode256*)new_node48;
			for (int32_t i = 0; i < node->head.child_count; i++) {
				ArNode48Insert((ArNode48**)node_ptr, i, node->child_arr[i]);
			}
			ArNode256Release(node);
		}
	}
}

static void ArNodeDelete(ArNode** node, uint8_t key_byte) {
	switch ((*node)->head.node_type) {
	case kArNode4: {
		ArNode4Delete((ArNode4**)node, key_byte);
		break;
	}
	case kArNode16: {
		ArNode16Delete((ArNode16**)node, key_byte);
		return;
	}
	case kArNode48: {
		ArNode48Delete((ArNode48**)node, key_byte);
		break;
	}
	case kArNode256: {
		ArNode256Delete((ArNode256**)node, key_byte);
		break;
	}
	default: {
		  assert(0);
		break;
	}
	}
}

ArLeaf* ArFirst(ArNode* node) {
	switch (node->head.node_type) {
	case kArLeaf:
		return &node->leaf;
	case kArNode2:
		return ArFirst(node->node2.ar_sub_node);
	case kArNode4:
		return ArFirst(node->node4.child_arr[0]);
	case kArNode16:
		return ArFirst(node->node16.child_arr[0]);
	case kArNode48:
		for (int i = 0; i < 255; i++) {
			if (node->node48.keys[i] != 0xff) {
				return ArFirst(node->node48.child_arr.obj_arr[node->node48.keys[i]].child);
			}
		}
	case kArNode256:
		for (int i = 0; i < 255; i++) {
			if (node->node256.child_arr[i] != NULL) {
				return ArFirst(node->node256.child_arr[i]);
			}
		}
	}
	return NULL;
}



/*
* 比较前缀，返回匹配的长度
*/
static uint32_t MatchPrefix(ArNodeHead* head, uint8_t* key, uint32_t key_len, uint32_t offset, bool* optimistic, key_type* optimistic_leaf_node_key, uint32_t* prefix_len) {
	uint8_t* node_key = head->prefix;
	if (head->prefix_len > head->vaild_prefix_len) {
		if (optimistic_leaf_node_key == NULL) {
			/* 乐观策略，只比较已保存的前缀 */
			*prefix_len = head->vaild_prefix_len;
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
	if (head->prefix_len > head->vaild_prefix_len && optimistic_leaf_node_key == NULL && match_len == head->vaild_prefix_len) {
		// 乐观策略的跳过阶段
		match_len = head->prefix_len;
		*prefix_len = match_len;
	}
	return match_len;
}

/*
* 为当前节点头部更新共同前缀，返回长度
*/
static void UpdatePrefix(ArNodeHead* head, uint8_t* prefix, int32_t match_len) {
	head->prefix_len = match_len;
	head->vaild_prefix_len = min(prefix_max_len, match_len);
	for (int32_t i = 0; i < head->vaild_prefix_len; i++) {
		head->prefix[i] = prefix[i];
	}
}

/*
* 将两个节点的前缀合并到第二个节点
*/
static void MergePrefix(ArNodeHead* parent, ArNodeHead* child) {
	if (child->node_type == kArLeaf || child->node_type == kArNode2) {
		return;
	}
	uint32_t new_len = parent->prefix_len + child->prefix_len;
	child->prefix_len = new_len;

	uint8_t temp_buf[prefix_max_len];
	memcpy(temp_buf, child->prefix, child->vaild_prefix_len);

	memcpy(child->prefix, parent->prefix, parent->vaild_prefix_len);
	child->vaild_prefix_len = parent->vaild_prefix_len;

	if (parent->prefix_len > parent->vaild_prefix_len) {
		// 父节点的前缀不完整，没法继续合并了
		return;
	}
	uint32_t child_copy_len = min(prefix_max_len - parent->vaild_prefix_len, child->vaild_prefix_len);
	memcpy(&child->prefix[parent->vaild_prefix_len], temp_buf, child_copy_len);
	child->vaild_prefix_len += child_copy_len;
}


// 节点分裂：
// 会执行节点分裂的情况：
// 1.走到叶子节点(node为叶子)
// 2.查找key长度不足
//   剩余长度为0(正好与前缀相等/查找下一个字符的offset++，且不是叶子)
//   不足以匹配node的前缀
// 3.前缀不匹配
// 
// 查询前缀/叶子匹配情况
// 如果两者不是子串关系，则创建Node4，否则创建Node2

static ArNode* ArSplitNode(ArNode* node, ArLeaf* leaf, uint32_t offset, key_type* optimistic_leaf_node_key) {
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
		if (node->head.prefix_len > node->head.vaild_prefix_len && optimistic_leaf_node_key) {
			node_key_buf = &optimistic_leaf_node_key->buf[offset];
			node_key_len = node->head.prefix_len;
		}
		else {
			node_key_buf = node->head.prefix;
			node_key_len = node->head.vaild_prefix_len;
		}
		
	}
	min_len = min(node_key_len, leaf_key_len);
	uint32_t match_len = 0;
	for (; match_len < min_len; match_len++) {
		if (leaf_key_buf[match_len] != node_key_buf[match_len]) {
			break;
		}
	}
	if (match_len == min_len) {
		// 是子串，根据包含关系构建Node2
		// 如果都为0，优先是node包含leaf，因为node除了前缀还有别的节点，实际长度更长
		ArNode2* new_node2 = ArNode2Create();
		if (match_len == leaf_key_len) {
			new_node2->ar_sub_node = (ArNode*)leaf;
			new_node2->sub_node_len = leaf_key_len;
			new_node2->ar_node = node;
		} else {
			new_node2->ar_sub_node = node;
			new_node2->sub_node_len = node_key_len;
			new_node2->ar_node = (ArNode*)leaf;
		}
		return (ArNode*)new_node2;
	}
	else {
		// 不是子串，相同的前缀组成共同前缀构建Node4
		ArNode4* new_node4 = ArNode4Create();
		UpdatePrefix(&new_node4->head, leaf_key_buf, match_len);
		ArNode4Insert(&new_node4, node_key_buf[match_len], node);
		ArNode4Insert(&new_node4, leaf_key_buf[match_len], (ArNode*)leaf);
		return (ArNode*)new_node4;
	}
}

static ArNode** ArSkipNode2(ArNode** node_ptr, uint8_t key_rem_len) {
	ArNode** cur_ptr = node_ptr;
	ArNode* node = *cur_ptr;
	while (node->head.node_type == kArNode2) {
		/* 余下长度<=sub_node_len则选择sub_node，否则选择node */
		if (key_rem_len <= node->node2.sub_node_len) {
			cur_ptr = &node->node2.ar_sub_node;
		}
		else {
			cur_ptr = &node->node2.ar_node;
		}
		node = *cur_ptr;
	}
	return cur_ptr;
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
		cur = *ArSkipNode2(&cur, key_len - offset);
		  assert(cur->head.node_type != kArNode2);
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
		uint32_t match_len = MatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
		if (match_len != prefix_len) {
			return NULL;
		}
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
		offset += match_len;

		if (offset >= key_len) {
			break;
		}

		ArNode** cur_ptr = ArTreeFindChild(cur, key_buf[offset]);
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
		tree->root = (ArNode*)ArLeafCreate(element);
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
		cur_ptr = ArSkipNode2(cur_ptr, key_len - offset);
		ArNode* cur = *cur_ptr;
		  assert(cur->head.node_type != kArNode2);
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
			*cur_ptr = ArSplitNode(cur, ArLeafCreate(element), offset, optimistic_leaf_node_key);
			return;
			
		}
		uint32_t match_len = 0;
		if (cur->head.prefix_len) {
			/*
			* 乐观策略对前缀的比较不完整，在最后找到叶子时，得到不匹配的偏移
			* 再次从乐观起始位置向下找，根据不匹配的偏移定位到不匹配的节点
			*/
			uint32_t prefix_len;
			match_len = MatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, optimistic_leaf_node_key, &prefix_len);
			if (match_len != prefix_len) {
				// 前缀匹配不完整，需要在这里分裂
				*cur_ptr = ArSplitNode(cur, ArLeafCreate(element), offset, optimistic_leaf_node_key);
				if ((*cur_ptr)->head.node_type == kArNode4) {
					// 如果是分裂成了node4才会更新前缀
					// 分裂后前缀需要去掉一个字节，因为在新的new_node4中的keys记录了当前node4的第一个字符
					// 匹配的前缀已经更新到新的node4上了，不匹配的前缀进行前移
					match_len++;
					cur->head.prefix_len -= match_len;
					if (match_len >= cur->head.vaild_prefix_len) {
						cur->head.vaild_prefix_len = 0;
					}
					else {
						cur->head.vaild_prefix_len -= match_len;
					}

					prefix_len = min(cur->head.vaild_prefix_len, prefix_max_len - match_len);
					for (uint32_t i = 0; i < prefix_len; i++) {
						cur->head.prefix[i] = cur->head.prefix[i + match_len];
					}
				}
				return;
			}
			if (optimistic_offset == -1 && optimistic) { 
				optimistic_offset = offset; 
				optimistic_retry_node_ptr = cur_ptr;
			}
			offset += match_len;
		}
		if (offset >= key_len) {
			// 从前缀匹配的地方重新计起，分裂为node2/node4
			// 这里有重复前缀长度计算(性能有点影响)，实际上上面那个if已经算好了
			if (optimistic && optimistic_leaf_node_key == NULL) {
				//长度不足以定位到叶子节点，此处乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
				optimistic_leaf_node_key = ArGetKey(ArGetElement((ArNode*)ArFirst(cur)));
				cur_ptr = optimistic_retry_node_ptr;
				offset = optimistic_offset;
				continue;
			}
			*cur_ptr = ArSplitNode(cur, ArLeafCreate(element), offset - match_len, optimistic_leaf_node_key);
			return;
		}
		ArNode** down_ptr = ArTreeFindChild(cur, key_buf[offset]);
		if (!down_ptr) {
			if (optimistic && optimistic_leaf_node_key == NULL) {
				// 到这里不能保证前缀是匹配的，乐观策略的重试需要一个叶子的key，故尽可能最短路径下降找叶子
				optimistic_leaf_node_key = ArGetKey(ArGetElement((ArNode*)ArFirst(cur)));
				cur_ptr = optimistic_retry_node_ptr;
				offset = optimistic_offset;
				continue;
			}
			ArLeaf* new_leaf = ArLeafCreate(element);
			ArNodeInsert(cur_ptr, key_buf[offset], (ArNode*)new_leaf);
			break;
		}
		offset++;
		cur_ptr = down_ptr;
	} while (true);

}


/*
* Art不会存在只有一个子节点的Node4的情况，因此删除最多只需要向上走两步
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
		cur_ptr = ArSkipNode2(cur_ptr, key_len - offset);
		ArNode* cur = *cur_ptr;
		  assert(cur->head.node_type != kArNode2);
		if (cur->head.node_type == kArLeaf) {
			if (optimistic) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			key_type* leaf_key = ArGetKey(&cur->leaf.element);
			if (!ArKeyEqual(key, leaf_key, offset)) {
				return NULL;
			}
			element_type* element = ArGetElement(cur);
			if (!parent_ptr) {
				// 是根节点，直接删除
				ArLeafRelease(&cur->leaf);
				return element;
			}
			ArNodeDelete(parent_ptr, parent_byte);
			if ((*parent_ptr)->head.node_type == kArNode4 && (*parent_ptr)->head.child_count == 1) {
				// 路径压缩的合并，使指向父节点的 祖父节点的child_arr元素 指向父节点剩下的一个孩子节点
				ArNode* temp = *parent_ptr;
				ArNode* child = (*parent_ptr)->node4.child_arr[0];
				(*parent_ptr) = child;

				// 将父节点的前缀和余下孩子的前缀进行合并
				MergePrefix(&temp->head, &child->head);

				// 释放父节点
				ArNode4Release(&temp->node4);
			}
			return element;
		}

		

		uint32_t prefix_len;
		uint32_t match_len = MatchPrefix(&cur->head, &key_buf[offset], key_len - offset, offset, &optimistic, NULL, &prefix_len);
		if (match_len != prefix_len) {
			return NULL;
		}
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
		offset += match_len;

		if (offset >= key_len) {
			break;
		}

		parent_ptr = cur_ptr;
		parent_byte = key_buf[offset];
		cur_ptr = ArTreeFindChild(cur, key_buf[offset]);
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

#endif // CUTILS_CONTAINER_AR_TREE_H_