/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_CB_TREE_H_
#define LIBYUC_CONTAINER_CB_TREE_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/* complete binary tree - 完全二叉树 */

#define LIBYUC_CONTAINER_CB_TREE_ZERO_GET_PARENT(index) (((index) + 1) / 2 - 1)
#define LIBYUC_CONTAINER_CB_TREE_ZERO_GET_LEFT(index) ((index) * 2 + 1)
#define LIBYUC_CONTAINER_CB_TREE_ZERO_GET_RIGHT(index) ((index) * 2 + 2)

#define LIBYUC_CONTAINER_CB_TREE_ONE_GET_PARENT(index) ((index) / 2)
#define LIBYUC_CONTAINER_CB_TREE_ONE_GET_LEFT(index) ((index) * 2)
#define LIBYUC_CONTAINER_CB_TREE_ONE_GET_RIGHT(index) ((index) * 2 + 1)


#define LIBYUC_CONTAINER_CB_TREE_DECLARATION(cb_tree_type_name, id_type, count) \
	typedef struct _##cb_tree_type_name##CbTree { \
		id_type entry[count]; /* 叶子计数*2才是总计数 */ \
	} cb_tree_type_name##CbTree; \
	
#define LIBYUC_CONTAINER_CB_TREE_DEFINE(cb_tree_type_name, id_type, count, indexer) \
	static id_type cb_tree_type_name##CbTreeToExponentOf2(id_type power) { \
		id_type exponent = 0; \
		while (power != 0) { \
			exponent++; \
			power >>= 1; \
		} \
		return exponent - 1; \
	} \
	static id_type cb_tree_type_name##CbTreeAlignToPowersOf2(id_type size) { \
		for (int i = 1; i < sizeof(size) * 8 / 2 + 1; i *= 2) { \
			size |= size >> i; \
		} \
		return size + 1; \
	} \
	void cb_tree_type_name##CbTreeInit(cb_tree_type_name##CbTree* cb_tree) { \
		for (id_type i = 0; i < count; i++) { \
			cb_tree->entry[i] = 0; \
		} \
	} \
	

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_CB_TREE_H_