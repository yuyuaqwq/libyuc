/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/



#ifndef CUTILS_CONTAINER_AR_TREE_H_
#define CUTILS_CONTAINER_AR_TREE_H_

#include <CUtils/object.h>

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

typedef struct _ArLeaf {
	uint8_t is_leaf : 1;
	uint8_t : 7;
	element_type element;
} ArLeaf;

typedef struct _ArNodeHead {
	struct {
		uint8_t is_leaf : 1;
		uint8_t type : 7;
	};
	uint8_t child_count;
	int16_t prefix_len;
	uint8_t prefix[4];
} ArNodeHead;

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
		ArNodeHead head;
		ArNode4 node4;
		ArNode16 node16;
		ArNode48 node48;
		ArNode256 node256;
		ArLeaf leaf;
	};
} ArNode;

typedef enum {
	kNode4,
	kNode16,
	kNode48,
	kNode256,
} ArNodeType;

typedef struct {
	id_type root;
	uint32_t element_count;
} ArTree;


void ArTreeInit(ArTree* tree) {
	tree->root = InvaildId;
	tree->element_count = 0;
}

static ArNode* ArTreeFindChild(ArNode* node, uint8_t key_byte) {
	  assert(node->head.is_leaf == false);
	switch (node->head.type) {
	case kNode4: {
		for (int i = 0; i < node->head.child_count; i++) {
			if (node->node4.keys[i] == key_byte) {
				return node->node4.child[i];
			}
		}
		break;
	}
	case kNode16: {
		for (int i = 0; i < node->head.child_count; i++) {
			if (node->node16.keys[i] == key_byte) {
				return node->node16.child[i];
			}
		}
		break;
	}
	case kNode48: {
		uint8_t index = node->node48.keys[key_byte];
		if (index != 255) {
			return node->node48.child[index];
		}
		break;
	}
	case kNode256: {
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


element_type* ArTreeFind(ArTree* tree, key_type* key) {
	ArNode* cur = tree->root;
	int depth = 0;
	while (cur) {
		if (cur->head.is_leaf == true) {
			if (*key == cur->leaf.element) {
				return &cur->leaf.element;
			}
			return NULL;
		}
		if (cur->head.prefix_len != 0) {
			if (memcmp(cur->head.prefix, key, cur->head.prefix_len)) {
				return NULL;
			}
			depth += cur->head.prefix_len;
		}
		cur = ArTreeFindChild(cur, ((uint8_t*)key)[depth]);
		depth++;
	}
	return NULL;
}

static void ArTreeInsertElement(ArNode* node, element_type* element, int depth) {
	if (node->head.is_leaf == true) {
		ArNode4* node4 = ObjectCreate(ArNode4);
		key_type* key = element;
		key_type* key2 = &node->leaf.element;
		int i;
		for (i = depth; ((uint8_t*)key)[i] == ((uint8_t*)key2)[i]; i++) {
			node4->head.prefix[i - depth] = key[i];
		}
		node4->head.prefix_len = i - depth;
		depth += node4->head.prefix_len;

		return;
	}
}

void ArTreeInsert(ArTree* tree, element_type* element) {
	if (tree->root == InvaildId) {
		tree->root = (id_type)ObjectCreate(ArLeaf);
		tree->element_count = 1;
		tree->root->leaf.is_leaf = true;
		tree->root->leaf.element = *element;
		return;
	}
	ArTreeInsertElement(tree->root, element);
}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AR_TREE_H_