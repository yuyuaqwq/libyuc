#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "C:\Users\yuyu\Desktop\wyhash.h"
#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME Int
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int64_t
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))
#include <libyuc/container/hash_table.h>


#define LIBYUC_CONTAINER_RB_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetKey(main_obj, obj) (&((QVQ*)obj)->key)
#include <libyuc/container/rb_tree.h>


typedef struct QVQ {
	IntRbEntry entry;
	int64_t key;
	//AvlEntry entry;
	//int height;
	//LruEntry lru_entry;
} QVQ;
//





#define LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey(main_obj, obj) ((int64_t*)((uintptr_t)obj + sizeof(IntAvlBsEntry)))
#include <libyuc/container/experimental/avl_tree.h>

#define LIBYUC_CONTAINER_BPLUS_TREE_CLASS_NAME Int
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Key int64_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Id int16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Type_Offset uint16_t
#define LIBYUC_CONTAINER_BPLUS_TREE_ELEMENT_REFERENCER_Const_InvalidId -1
#include <libyuc/container/bplus_tree.h>


//#include <libyuc/container/hash_table.h>
//#include <libyuc/container/experimental/avl_tree.h>
//#define NP
//#ifdef NP
//#include <libyuc/container/rb_tree.h>
//#else
//#include <libyuc/container/experimental/rb_tree.h>
//#endif
////#include <libyuc/container/bplus_tree.h>
//#include <libyuc/container/lru_list.h>
//#include <libyuc/space_manager/buddy.h>
//
//#include <libyuc/container/bplus_tree.h>
//
//



//LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(, int32_t, int32_t)
//
//#pragma pack(push) 
//#pragma pack(4)
//LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(Int, uint32_t, int64_t, int64_t)
//#pragma pack(pop)
//
//#define INT_HASHTABLE_ACCESSOR_GetKey(table, obj) (obj)
//#define INT_HASHTABLE_ACCESSOR INT_HASHTABLE_ACCESSOR
//
//#define BIG_CONSTANT(x) (x##LLU)
//static forceinline uint64_t HashCode_murmur3_fmix64inline(uint64_t k) {
//	k ^= k >> 33;
//	k *= BIG_CONSTANT(0xff51afd7ed558ccd);
//	k ^= k >> 33;
//	k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
//	k ^= k >> 33;
//	return k;
//}


//LIBYUC_CONTAINER_BPLUS_TREE_DECLARATION(Int, LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NOT_LINK, struct _IntBPlusEntry*, size_t, int16_t, int16_t, int64_t, int64_t, 32)


//LIBYUC_CONTAINER_HASH_LIST_DECLARATION(Int, int)

//LIBYUC_CONTAINER_BPLUS_TREE_DECLARATION(Int, LIBYUC_CONTAINER_BPLUS_TREE_LEAF_LINK_MODE_NORMAL, struct _IntBPlusEntry*, int16_t, int, int, 8)

#ifdef __cplusplus
}
#endif