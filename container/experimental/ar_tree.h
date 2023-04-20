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

ArLeaf* ArGetRawLeaf(id_type child) {
	return (ArLeaf*)(((uintptr_t)child) & (~1));
}

void ArSetLeaf(id_type* child, bool is_leaf) {
	is_leaf ? (*(uintptr_t*)child) |= is_leaf : (*(uintptr_t*)child) &= (~is_leaf);
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

static id_type ArTreeFindChild(ArNode* node, uint8_t key_byte, uint32_t offset, uint32_t key_len) {
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

static bool CheckPrefix(ArNodeHead* head, uint8_t* key, uint32_t* optimistic_offset, uint32_t* offset, uint32_t key_len) {
	uint32_t prefix_len = head->prefix_long ? head->prefix_long_len : head->prefix_len;
	if (prefix_len != 0) {
		if (key_len - *offset < prefix_len) {
			/* 剩余长度不足 */
			return false;
		}
		if (*optimistic_offset == -1) {
			if (head->prefix_long) {
				/* 统一使用乐观，最后再比较 */
				*optimistic_offset = *offset;
			}
			/* 使用悲观策略 */
			else if (memcmp(head->prefix, &key[*offset], prefix_len)) {
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
			cur = (ArNode*)ArGetRawLeaf(cur);
			if (optimistic_offset != -1) {
				/* 需要进行乐观比较 */
				offset = optimistic_offset;
			}
			if (memcmp(ArGetKeyByte(key, offset), ArGetKeyByte(ArGetLeafKey(cur), offset), key_len - offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}
		if (!CheckPrefix(&cur->head, ArGetKeyByte(key, 0), &optimistic_offset, &offset, key_len)) {
			return NULL;
		}
		cur = ArTreeFindChild(cur, *ArGetKeyByte(key, offset), offset, key_len);
		offset++;
		if (offset >= key_len && cur && ArIsLeaf(cur)) {
			/* ArNode2 */
			if (optimistic_offset != -1 && memcmp(ArGetKeyByte(key, optimistic_offset), ArGetKeyByte(ArGetLeafKey(cur), optimistic_offset), key_len - optimistic_offset)) {
				return NULL;
			}
			return ArGetElement(cur);
		}
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

static uint32_t UpdatePrefix(ArNodeHead* head, uint8_t* key1, uint8_t* key2) {
	uint32_t i;
	for (i = 0; key1[i] == key2[i]; i++) {
		head->prefix[i] = key1[i];
	}
	if (i > sizeof(head->prefix)) {
		head->prefix_long = true;
		head->prefix_long_len = i;
	}
	else {
		head->prefix_long = false;
		head->prefix_len = i;
	}
	return i;
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

	uint32_t offset = 0;
	if (ArIsLeaf(cur)) {
		cur = (ArNode*)ArGetRawLeaf(cur);
		key_type* cmp_key = ArGetKey(element);
		key_type* leaf_key = &cur->leaf.element;
		if (*cmp_key == *leaf_key) {
			cur->leaf.element = *element;
			/* only update */
			return;
		}

		ArNode4* node4 = ObjectCreate(ArNode4);
		offset += UpdatePrefix(&node4->head, ArGetKeyByte(cmp_key, offset), ArGetKeyByte(leaf_key, offset));

		id_type new_leaf = (id_type)ObjectCreate(ArLeaf);
		ArNode4Insert(node4, *ArGetKeyByte(leaf_key, offset), cur);
		ArNode4Insert(node4, *ArGetKeyByte(cmp_key, offset), new_leaf);
		return;
	}

	if (cur->head.prefix_len) {
		/* 
		* 如果有前缀，很复杂，与前缀完全匹配的话就可以继续向下，否则需要分裂
		* 有个更麻烦的地方是乐观策略，要暂时跳过
		*/
		UpdatePrefix(&cur->head, ArGetKeyByte(element, offset), cur->head.prefix);

	}

}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AR_TREE_H_