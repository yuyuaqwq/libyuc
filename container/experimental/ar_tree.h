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

#ifdef __cplusplus
extern "C" {
#endif

/*
* Adaptive Radix Tree - 自适应基数树
*/

typedef struct _ArNode* id_type;
typedef int key_type;
typedef int element_type;
#define InvaildId 0
#define prefix_max_len 8

/*
* 可以通过两个宏选项来声明不同的结构体
* 叶子节点是指针还是嵌入
* 通过传参传递
*/

typedef struct _ArLeaf {
	element_type element;
} ArLeaf;


typedef enum {
	kArNode2,
	kArNode4,
	kArNode16,
	kArNode48,
	kArNode256,
} ArNodeType;

typedef enum {
	kArNotPrefix,
	kArInlinePrefix,
	kArLongPrefix,
} ArPrefixType;

typedef struct _ArNodeHead {
	uint8_t node_type : 4;
	uint8_t prefix_type : 4;
	uint8_t child_count;
	uint8_t prefix_len;
	union {
		uint8_t prefix[prefix_max_len];
		uint32_t prefix_long_len;
	};
} ArNodeHead;

typedef struct {
	ArNodeHead head;
	id_type ar_leaf;
	id_type ar_node;
} ArNode2;

typedef struct {
	ArNodeHead head;
	uint8_t keys[4];
	id_type child[4];
} ArNode4;

typedef struct {
	ArNodeHead head;
	uint8_t keys[16];
	id_type child[16];
} ArNode16;

typedef struct {
	ArNodeHead head;
	uint8_t keys[256];
	id_type child[48];
} ArNode48;

typedef struct {
	ArNodeHead head;
	id_type child[256];
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
	id_type root;
	uint32_t element_count;
} ArTree;


bool ArIsLeaf(id_type child) {
	return ((uintptr_t)child) & 1;
}

ArLeaf* ArGetRawLeaf(id_type child) {
	return (ArLeaf*)(((uintptr_t)child) & (~1));
}

void ArSetLeaf(id_type* child, bool is_leaf) {
	if (is_leaf) { 
		(*(uintptr_t*)child) |= is_leaf;
	}
	else {
		(*(uintptr_t*)child) &= (~is_leaf);
	}
}

key_type* ArGetLeafKey(id_type child) {
	return &child->leaf.element;
}


key_type* ArGetKey(element_type* element) {
	return element;
}

uint32_t ArGetKeyLen(key_type* key) {
	return sizeof(*key);
}

uint8_t* ArGetKeyByte(key_type* key, uint32_t offset) {
	return &((uint8_t*)key)[offset];
}


element_type* ArGetElement(id_type child) {
	return &child->leaf.element;
}



void ArTreeInit(ArTree* tree) {
	tree->root = InvaildId;
	tree->element_count = 0;
}

uint8_t* ArBsAccessor_GetKey(uint8_t* arr, uint8_t* element) {
	return element;
}
CUTILS_ALGORITHM_SEARCH_DEFINE(Ar, uint8_t, uint8_t, int32_t, ArBsAccessor, CUTILS_OBJECT_INDEXER_DEFALUT, CUTILS_OBJECT_COMPARER_DEFALUT)

static id_type ArTreeFindChild(ArNode* node, uint8_t key_byte, uint32_t key_len) {
	switch (node->head.node_type) {
	case kArNode2: {
		return key_len == 0 ? node->node2.ar_leaf : node->node2.ar_node;
	}
	case kArNode4: {
		for (int i = 0; i < node->head.child_count; i++) {
			if (node->node4.keys[i] == key_byte) {
				return node->node4.child[i];
			}
		}
		break;
	}
	case kArNode16: {
		int32_t i = ArBinarySearch(node->node16.keys, 0, node->head.child_count - 1, &key_byte);
		if (i == -1) {
			break;
		}
		return node->node16.child[i];
	}
	case kArNode48: {
		uint8_t index = node->node48.keys[key_byte];
		if (index != 255) {
			return node->node48.child[index];
		}
		break;
	}
	case kArNode256: {
		return node->node256.child[key_byte];
		break;
	}
	default: {
		  assert(0);
		break;
	}
	}
	return InvaildId;
}

/*
* 比较前缀，返回匹配的长度
*/
static uint32_t MatchPrefix(ArNodeHead* head, uint8_t* key, uint32_t key_len, bool* optimistic, uint32_t* prefix_len) {
	if (head->prefix_type == kArLongPrefix) {
		*prefix_len = head->prefix_long_len;
	}
	else if (head->prefix_type == kArInlinePrefix) {
		*prefix_len = head->prefix_len;
	}
	else {
		*prefix_len = 0;
	}
	uint32_t match_len = min(*prefix_len, key_len);
	if (match_len) {
		if (*optimistic = false) {
			uint32_t match_length;
			if (head->prefix_type == kArLongPrefix) {
				/* 统一使用乐观，最后再比较 */
				optimistic = true;
			}
			else  {
				/* 使用悲观策略 */
				uint32_t i = 0;
				for (; i < match_len; i++) {
					if (key[i] != head->prefix[i]) {
						--i;
						break;
					}
				}
				match_len = i;
			}
		}
		else {
			/* 使用乐观策略，跳过比较 */
		}
	}
	return match_len;
}

/*
* 为当前节点头部更新共同前缀，返回长度
*/
static uint32_t UpdatePrefix(ArNodeHead* head, uint8_t* key1, uint8_t* key2) {
	uint32_t i;
	for (i = 0; key1[i] == key2[i]; i++);
	if (i > sizeof(head->prefix)) {
		head->prefix_type = kArLongPrefix;
		head->prefix_long_len = i;
	}
	else if (i > 0) {
		head->prefix_type = kArInlinePrefix;
		memcpy(head->prefix, key1, i);
		head->prefix_len = i;
	}
	else {
		head->prefix_type = kArNotPrefix;
	}
	return i;
}

/*
* 前缀冲突，分裂节点
*/
static void SplitNode() {

}


element_type* ArTreeFind(ArTree* tree, key_type* key) {
	id_type cur = tree->root;
	bool optimistic = false;
	uint32_t offset = 0;
	uint32_t optimistic_offset = -1;
	uint8_t* key_buf = ArGetKeyByte(key, 0);
	uint32_t key_len = ArGetKeyLen(key);
	while (cur) {
		if (ArIsLeaf(cur)) {
			cur = (ArNode*)ArGetRawLeaf(cur);
			if (optimistic) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			if (memcmp(&key_buf[offset], ArGetKeyByte(ArGetLeafKey(cur), offset), key_len - offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}

		if (offset >= key_len) {
			break;
		}

		uint32_t prefix_len;
		uint32_t match_len = MatchPrefix(&cur->head, &key_buf[offset], key_len - offset, &optimistic, &prefix_len);
		if (match_len != prefix_len) {
			return NULL;
		}
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }

		cur = ArTreeFindChild(cur, key_buf[offset], key_len - offset);
		offset++;
	}
	return NULL;
}



CUTILS_ALGORITHM_ARRAY_DEFINE(Ar, element_type, uint32_t)

static void ArNode4Insert(ArNode4* node, uint8_t key_byte, ArNode* child) {
	uint8_t count = node->head.child_count++;
	node->keys[count] = key_byte;
	node->child[count] = child;
}

static void ArNode16Insert(ArNode16* node, uint8_t key_byte, ArNode* child) {

}

void ArTreeInsert(ArTree* tree, element_type* element) {
	if (tree->root == InvaildId) {
		tree->root = (id_type)ObjectCreate(ArLeaf);
		tree->element_count = 1;
		ArSetLeaf(&tree->root, true);
		tree->root->leaf.element = *element;
		return;
	}

	key_type* key = ArGetKey(element);
	uint8_t* key_buf = ArGetKeyByte(key, 0);
	uint32_t offset = 0;

	id_type cur = tree->root;
	if (ArIsLeaf(cur)) {
		cur = (ArNode*)ArGetRawLeaf(cur);
		
		key_type* leaf_key = &cur->leaf.element;
		if (*key == *leaf_key) {
			cur->leaf.element = *element;
			/* only update */
			return;
		}

		ArNode4* node4 = ObjectCreate(ArNode4);
		offset += UpdatePrefix(&node4->head, &key_buf[offset], ArGetKeyByte(leaf_key, offset));

		id_type new_leaf = (id_type)ObjectCreate(ArLeaf);
		ArNode4Insert(node4, *ArGetKeyByte(leaf_key, offset), cur);
		ArNode4Insert(node4, key_buf[offset], new_leaf);
		return;
	}

	bool optimistic = false;
	uint32_t optimistic_offset = -1;
	uint32_t key_len = ArGetKeyLen(key);
	if (cur->head.prefix_type != kArNotPrefix) {
		/* 
		* 如果有前缀，很复杂，与前缀完全匹配的话就可以继续向下，否则需要分裂为node2
		* 有个更麻烦的地方是乐观策略，要暂时跳过，最后匹配的时候，得到不匹配的偏移
		* 再次从乐观起始位置向下找，根据偏移定位到不匹配的节点
		*/

		uint32_t prefix_len;
		uint32_t match_len = MatchPrefix(&cur->head, &key_buf[offset], key_len - offset, &optimistic, &prefix_len);
		if (optimistic_offset == -1 && optimistic) { optimistic_offset = offset; }
		if (match_len != prefix_len) {
			/* 前缀不匹配，从不匹配的地方开始分裂为ArNode2 */

			ArNode2* node2 = ObjectCreate(ArNode2);
			id_type new_leaf = (id_type)ObjectCreate(ArLeaf);

			uint32_t update_len = UpdatePrefix(&node2->head, &key_buf[offset], cur->head.prefix);
			UpdatePrefix(&cur->head, &key_buf[offset + update_len], &cur->head.prefix[update_len]);
			node2->ar_leaf = new_leaf;
			node2->ar_node = cur;
			return;
		}

	}

}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AR_TREE_H_