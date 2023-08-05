#pragma once

#include <libyuc/container/vector.h>
#include <libyuc/container/hash_table.h>
#include <libyuc/container/list.h>
#include <libyuc/container/experimental/avl_tree.h>
#define NP
#ifdef NP
#include <libyuc/container/rb_tree.h>
#else
#include <libyuc/container/experimental/rb_tree.h>
#endif
//#include <libyuc/container/bplus_tree.h>
#include <libyuc/container/hash_list.h>
#include <libyuc/space_manager/buddy.h>

#include <libyuc/container/bplus_tree.h>


#ifdef __cplusplus
extern "C" {
#endif


LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(, int32_t)


LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(Int, uint32_t, int64_t, int64_t)
#define INT_HASHTABLE_ACCESSOR_GetKey(table, obj) (obj)
#define INT_HASHTABLE_ACCESSOR INT_HASHTABLE_ACCESSOR

#define BIG_CONSTANT(x) (x##LLU)
static forceinline uint64_t HashCode_murmur3_fmix64inline(uint64_t k) {
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xff51afd7ed558ccd);
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
	k ^= k >> 33;
	return k;
}

#define INT_HASHTABLE_HASHER_KEY(table, key)  HashCode_murmur3_fmix64inline(*(key)) // XXH32_int(key)

//		LIBYUC_CONTAINER_AVL_TREE_DECLARATION(Int, struct _IntAvlEntry*, int)
//		typedef struct _IntEntry_Avl {
//		IntAvlEntry entry;
//		int key;
//	} IntEntry_Avl;


LIBYUC_CONTAINER_AVL_TREE_DECLARATION(Int, struct _IntAvlEntry*, uint32_t, int64_t)
typedef struct _IntEntry_Avl {
	IntAvlEntry entry;
	int64_t key;
} IntEntry_Avl;
#define INT_AVL_TREE_ACCESSOR_GetKey(TREE, ENTRY) (&((IntEntry_Avl*)ENTRY)->key)
#define INT_AVL_TREE_ACCESSOR_GetLeft(TREE, ENTRY) ((IntAvlEntry*)((uintptr_t)(ENTRY)->left & ~3))
#define  INT_AVL_TREE_ACCESSOR_GetBalanceFactor(TREE, ENTRY) ((int8_t)(((uintptr_t)((IntAvlEntry*)ENTRY)->left) & 0x3) == 3 ? -1 : (int8_t)(((uintptr_t)((IntAvlEntry*)ENTRY)->left) & 0x3))
#define INT_AVL_TREE_ACCESSOR_SetLeft(TREE, ENTRY, NEW_ID) ((ENTRY)->left = (uintptr_t)NEW_ID | ((uintptr_t)INT_AVL_TREE_ACCESSOR_GetBalanceFactor(TREE, ENTRY)) & 0x3);
#define INT_AVL_TREE_ACCESSOR_SetBalanceFactor(TREE, ENTRY, BF) (ENTRY->left = (IntAvlEntry*)(((uintptr_t)INT_AVL_TREE_ACCESSOR_GetLeft(TREE, ENTRY)) | ((uintptr_t)BF & 0x3)))
#define INT_AVL_TREE_ACCESSOR_GetRight(TREE, ENTRY) ((ENTRY)->right)
#define INT_AVL_TREE_ACCESSOR_SetRight(TREE, ENTRY, NEW_ID) ((ENTRY)->right = NEW_ID);
#define INT_AVL_TREE_ACCESSOR INT_AVL_TREE_ACCESSOR

#define INT_AVL_TREE_REFERENCER_Reference(TREE, OBJ_ID) ((IntAvlBsEntry*)OBJ_ID)
#define INT_AVL_TREE_REFERENCER_Dereference(TREE, OBJ)
#define INT_AVL_TREE_REFERENCER_InvalidId (NULL)
#define INT_AVL_TREE_REFERENCER INT_AVL_TREE_REFERENCER



	LIBYUC_CONTAINER_RB_TREE_DECLARATION(Int, struct _IntRbEntry*, uint32_t, int64_t)
		typedef struct _IntEntry_Rb {
		IntRbEntry entry;
		int64_t key;
	} IntEntry_Rb;
#define INT_RB_TREE_ACCESSOR_GetKey(TREE, ENTRY) (&((IntEntry_Rb*)ENTRY)->key)

#ifdef NP
#define INT_RB_TREE_ACCESSOR_GetLeft(TREE, ENTRY) ((IntRbEntry*)((uintptr_t)(ENTRY)->left & ~1))
#define INT_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY) ((RbColor)((uintptr_t)(ENTRY)->left & 1))
#define INT_RB_TREE_ACCESSOR_SetLeft(TREE, ENTRY, NEW_ID) ((ENTRY)->left = (uintptr_t)NEW_ID | (uintptr_t)INT_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY));
#define INT_RB_TREE_ACCESSOR_SetColor(TREE, ENTRY, NEW_COLOR) ((ENTRY)->left = (uintptr_t)INT_RB_TREE_ACCESSOR_GetLeft(TREE, ENTRY) | (uintptr_t)NEW_COLOR);
#define INT_RB_TREE_ACCESSOR_GetRight(TREE, ENTRY) ((ENTRY)->right)
#define INT_RB_TREE_ACCESSOR_SetRight(TREE, ENTRY, NEW_ID) ((ENTRY)->right = NEW_ID);
#else
#define INT_RB_TREE_ACCESSOR_GetKey(TREE, ENTRY) (&((IntEntry_Rb*)ENTRY)->key)
#define INT_RB_TREE_ACCESSOR_GetLeft(TREE, ENTRY) (((IntRbEntry*)(ENTRY))->left)
#define INT_RB_TREE_ACCESSOR_SetLeft(TREE, ENTRY, NEW_ID) (((IntRbEntry*)(ENTRY))->left = NEW_ID);
#define INT_RB_TREE_ACCESSOR_GetParent(TREE, ENTRY) ((IntRbEntry*)((uintptr_t)((IntRbEntry*)ENTRY)->parent_color & ~1))
#define INT_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY) ((RbColor)((uintptr_t)((IntRbEntry*)ENTRY)->parent_color & 1))
#define INT_RB_TREE_ACCESSOR_SetParent(TREE, ENTRY, NEW_ID) (((IntRbEntry*)ENTRY)->parent_color = (uintptr_t)NEW_ID | (uintptr_t)INT_RB_TREE_ACCESSOR_GetColor(TREE, ENTRY));
#define INT_RB_TREE_ACCESSOR_SetColor(TREE, ENTRY, NEW_COLOR) (((IntRbEntry*)ENTRY)->parent_color = (uintptr_t)INT_RB_TREE_ACCESSOR_GetParent(TREE, ENTRY) | (uintptr_t)NEW_COLOR);
#define INT_RB_TREE_ACCESSOR_GetRight(TREE, ENTRY) ((ENTRY)->right)
#define INT_RB_TREE_ACCESSOR_SetRight(TREE, ENTRY, NEW_ID) ((ENTRY)->right = NEW_ID);
#endif

#define INT_RB_TREE_ACCESSOR INT_RB_TREE_ACCESSOR
#define INT_RB_TREE_REFERENCER_Reference(TREE, OBJ_ID) ((IntRbBsEntry*)OBJ_ID)
#define INT_RB_TREE_REFERENCER_Dereference(TREE, OBJ)
#define INT_RB_TREE_REFERENCER_InvalidId (NULL)
#define INT_RB_TREE_REFERENCER INT_RB_TREE_REFERENCER



LIBYUC_CONTAINER_BPLUS_TREE_DECLARATION(Int, LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK, struct _IntBPlusEntry*, size_t, int16_t, int16_t, int64_t, int64_t, 32)


//	LIBYUC_CONTAINER_HASH_LIST_DECLARATION(Int, int)

//		LIBYUC_CONTAINER_BPLUS_TREE_DECLARATION(Int, LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL, struct _IntBPlusEntry*, int16_t, int, int, 8)

#ifdef __cplusplus
}
#endif