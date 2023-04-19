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

typedef struct _ArNodeHead {
	uint8_t node_type : 7;
	uint8_t prefix_long : 1;
	uint8_t child_count;
	uint8_t prefix_len;
	union {
		uint8_t prefix[prefix_max_len];
		uint32_t prefix_long_len;
	};
} ArNodeHead;

typedef struct {
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

typedef enum {
	kArNode2,
	kArNode4,
	kArNode16,
	kArNode48,
	kArNode256,
} ArNodeType;

typedef struct {
	id_type root;
	uint32_t element_count;
} ArTree;


bool ArIsLeaf(id_type child) {
	return ((uintptr_t)child) & 1;
}

void ArSetLeaf(id_type* child, bool is_leaf) {
	return is_leaf ? ((uintptr_t)*child |= is_leaf) : ((uintptr_t)*child &= (~is_leaf));
}

key_type* ArGetKey(id_type child) {
	return &child->leaf.element;
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

static id_type ArTreeFindChild(ArNode* node, uint8_t key_byte, uint32_t offset, uint32_t key_len) {
	  assert(node->head.is_leaf == false);
	switch (node->head.node_type) {
	case kArNode2: {
		return offset + 1 == key_len ? node->node2.ar_leaf : node->node2.ar_node;
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
		for (int i = 0; i < node->head.child_count; i++) {
			if (node->node16.keys[i] == key_byte) {
				return node->node16.child[i];
			}
		}
		break;
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

static bool CheckPrefix(ArNodeHead* head, key_type* key, uint32_t* optimistic_offset, uint32_t* offset, uint32_t key_len) {
	uint32_t prefix_len = head->prefix_long ? head->prefix_long_len : head->prefix_len;
	if (prefix_len != 0) {
		if (key_len - *offset < prefix_len) {
			/* 剩余长度不足 */
			return false;
		}
		if (optimistic_offset == -1) {
			if (head->prefix_long) {
				/* 统一使用乐观，最后再比较 */
				optimistic_offset = offset;
			}
			/* 使用悲观策略 */
			else if (memcmp(head->prefix, ArGetKeyByte(key, offset), prefix_len)) {
				return false;
			}
		}
		else {
			/* 使用乐观策略 */
		}
		*offset += prefix_len;
	}
	return true;
}

element_type* ArTreeFind(ArTree* tree, key_type* key) {
	id_type cur = tree->root;
	uint32_t offset = 0;
	uint32_t optimistic_offset = -1;
	uint32_t key_len = ArGetKeyLen(key);
	while (cur && offset < key_len) {
		if (ArIsLeaf(cur)) {
			if (optimistic_offset != -1) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			if (memcmp(ArGetKeyByte(key, offset), ArGetKeyByte(ArGetKey(cur), offset), key_len - offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}
		if (!CheckPrefix(&cur->head, key, &optimistic_offset, &offset, key_len)) {
			return NULL;
		}
		cur = ArTreeFindChild(cur, *ArGetKeyByte(key, offset), offset, key_len);
		offset++;
		if (offset >= key_len && cur && ArIsLeaf(cur)) {
			/* ArNode2 */
			if (optimistic_offset != -1 && memcmp(ArGetKeyByte(key, optimistic_offset), ArGetKeyByte(ArGetKey(cur), optimistic_offset), key_len - optimistic_offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}
	}
	return NULL;
}

CUTILS_ALGORITHM_ARRAY_DEFINE(Ar, element_type, uint32_t)
CUTILS_ALGORITHM_SEARCH_DEFINE(Ar, element_type, key_type, )

static void ArNode4Insert(ArNode4* node, uint8_t key_byte, ArNode* child) {
	
}

void ArTreeInsert(ArTree* tree, element_type* element) {
	if (tree->root == InvaildId) {
		tree->root = (id_type)ObjectCreate(ArLeaf);
		tree->element_count = 1;
		ArSetLeaf(&tree->root, true);
		tree->root->leaf.element = *element;
		return;
	}
	id_type cur = tree->root;

	
	if (ArIsLeaf(cur)) {
		ArNode4* node4 = ObjectCreate(ArNode4);
		key_type* key = element;
		key_type* key2 = &cur->leaf.element;
		int i;
		for (i = depth; ((uint8_t*)key)[i] == ((uint8_t*)key2)[i]; i++) {
			node4->head.prefix[i - depth] = key[i];
		}
		node4->head.prefix_len = i - depth;
		depth += node4->head.prefix_len;

		return;
	}
}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AR_TREE_H_