/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_RB_TREE_H_
#define LIBYUC_CONTAINER_RB_TREE_H_

#include <libyuc/object.h>
#include <libyuc/container/bs_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 以234树为原型的红黑树实现
*/
typedef enum {
  kRbBlack,
  kRbRed,
} RbColor;

#define LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack_name, id_type, offset_type) \
    rb_tree_type_name##RbBsStackVector stack_name; \
    id_type stack_name##_buf[sizeof(offset_type) * 8 * 2]; \
    rb_tree_type_name##RbBsStackVectorInit(&stack_name, sizeof(stack_name##_buf) / sizeof(id_type), stack_name##_buf); \

#define LIBYUC_CONTAINER_RB_TREE_DECLARATION(rb_tree_type_name, id_type, offset_type, key_type) \
  LIBYUC_CONTAINER_BS_TREE_DECLARATION(rb_tree_type_name##Rb, id_type, offset_type, key_type) \
  typedef struct _##rb_tree_type_name##RbEntry { \
    union { \
      struct { \
        id_type left; \
        id_type right; \
      }; \
      rb_tree_type_name##RbBsEntry bs_entry; \
    }; \
  } rb_tree_type_name##RbEntry; \
  typedef struct _##rb_tree_type_name##RbTree { \
    union { \
      rb_tree_type_name##RbBsTree bs_tree; \
      id_type root; \
    }; \
  } rb_tree_type_name##RbTree; \
  typedef struct _##rb_tree_type_name##RbTreeIterator { \
    id_type cur_id; \
    rb_tree_type_name##RbBsStackVector stack; \
    id_type stack_buf[sizeof(offset_type) * 8 * 2]; \
  } rb_tree_type_name##RbTreeIterator; \
  \
  void rb_tree_type_name##RbTreeInit(rb_tree_type_name##RbTree* tree); \
  id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key); \
  bool rb_tree_type_name##RbTreeInsert(rb_tree_type_name##RbTree* tree, id_type insert_entry_id); \
  id_type rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id); \
  bool rb_tree_type_name##RbTreeDelete(rb_tree_type_name##RbTree* tree, key_type* key); \
  bool rb_tree_type_name##RbTreeDeleteByIterator(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator); \
  offset_type rb_tree_type_name##RbTreeGetCount(rb_tree_type_name##RbTree* tree); \
  \
  id_type rb_tree_type_name##RbTreeIteratorLocate(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator, key_type* key, int8_t* cmp_status); \
  id_type rb_tree_type_name##RbTreeIteratorFirst(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator); \
  id_type rb_tree_type_name##RbTreeIteratorLast(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator); \
  id_type rb_tree_type_name##RbTreeIteratorNext(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator); \
  id_type rb_tree_type_name##RbTreeIteratorPrev(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator); \
  \
  bool rb_tree_type_name##RbTreeVerify(rb_tree_type_name##RbTree* tree); \


// 访问器需要提供_GetKey、_Get/SetRight、_Get/SetLeft、_Set/GetColor方法
#define LIBYUC_CONTAINER_RB_TREE_DEFINE(rb_tree_type_name, id_type, offset_type, key_type, referencer, accessor, comparer) \
  LIBYUC_CONTAINER_BS_TREE_DEFINE(rb_tree_type_name##Rb, id_type, offset_type, key_type, referencer, accessor, comparer) \
  /*
  * 取兄弟节点
  */ \
  static id_type rb_tree_type_name##GetSiblingEntry(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbEntry* entry_parent, id_type entry_id, rb_tree_type_name##RbEntry* entry) { \
    id_type ret; \
    if (accessor##_GetLeft(tree, entry_parent) == entry_id) { \
      ret = accessor##_GetRight(tree, entry_parent); \
    } \
    else { \
       assert(accessor##_GetRight(tree, entry_parent) == entry_id); \
      ret = accessor##_GetLeft(tree, entry_parent); \
    }\
    return ret; \
  } \
  \
  /*
  * 向树中插入节点后的平衡操作
  */ \
  static void rb_tree_type_name##RbTreeInsertFixup(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbBsStackVector* stack, id_type ins_entry_id) { \
    rb_tree_type_name##RbEntry* ins_entry = referencer##_Reference(tree, ins_entry_id); \
    accessor##_SetColor(tree, ins_entry, kRbBlack); \
    id_type* cur_id = rb_tree_type_name##RbBsStackVectorPopTail(stack); \
    if (cur_id == NULL) { \
      accessor##_SetColor(tree, ins_entry, kRbBlack); \
      referencer##_Dereference(tree, ins_entry); \
      return; \
    } \
    accessor##_SetColor(tree, ins_entry, kRbRed); \
    referencer##_Dereference(tree, ins_entry); \
    \
    rb_tree_type_name##RbEntry* cur = NULL; \
    /* 开始回溯维护 */ \
    while (cur_id != NULL) { \
      cur = referencer##_Reference(tree, *cur_id); \
      if (accessor##_GetColor(tree, cur) == kRbBlack) { \
        /* 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点 / 3节点的插入，直接合并) */ \
        break; \
      } \
      id_type* parent_id = rb_tree_type_name##RbBsStackVectorPopTail(stack); \
      if (parent_id == NULL) { \
        /* 没有父节点，回溯到根节点了，直接染黑 */ \
        accessor##_SetColor(tree, cur, kRbBlack); \
        break; \
      } \
      rb_tree_type_name##RbEntry* parent = referencer##_Reference(tree, *parent_id); \
      id_type sibling_id = rb_tree_type_name##GetSiblingEntry(tree, parent, *cur_id, cur); \
      referencer##_Dereference(tree, parent); \
      rb_tree_type_name##RbEntry* sibling = referencer##_Reference(tree, sibling_id); \
      if (sibling_id != referencer##_InvalidId && accessor##_GetColor(tree, sibling) == kRbRed) { \
        /* 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上插入，继续回溯 */ \
        accessor##_SetColor(tree, cur, kRbBlack); \
        accessor##_SetColor(tree, sibling, kRbBlack); \
        ins_entry_id = *parent_id;     /* 更新为该节点向上插入 */ \
        ins_entry = referencer##_Reference(tree, ins_entry_id); \
        if (rb_tree_type_name##RbBsStackVectorGetTail(stack) == NULL) { \
          accessor##_SetColor(tree, ins_entry, kRbBlack);   /* 根节点，染黑并结束 */ \
          break; \
        } \
        accessor##_SetColor(tree, ins_entry, kRbRed); \
        parent_id = rb_tree_type_name##RbBsStackVectorPopTail(stack); \
        /* 将parent视作插入节点 */ \
      } \
      else { \
        /* 没有兄弟节点或兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
         *    10b                5b  
         *   5r   20b  ->   !2r     10r  
         * !2r                         20b
        */ \
         assert(sibling_id == referencer##_InvalidId || accessor##_GetColor(tree, sibling) == kRbBlack); \
        id_type new_sub_root_id; \
        id_type old_sub_root_id = *parent_id; \
        rb_tree_type_name##RbEntry* old_sub_root = referencer##_Reference(tree, old_sub_root_id); \
        rb_tree_type_name##RbEntry* new_sub_root_parent = NULL; \
        id_type* new_sub_root_parent_id = rb_tree_type_name##RbBsStackVectorGetTail(stack); \
        if (new_sub_root_parent_id) new_sub_root_parent = referencer##_Reference(tree, *new_sub_root_parent_id); \
        if (accessor##_GetLeft(tree, old_sub_root) == *cur_id) { \
          if (accessor##_GetRight(tree, cur) == ins_entry_id) { \
            rb_tree_type_name##RbRotateLeft(tree, old_sub_root, *cur_id, cur); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, new_sub_root_parent, old_sub_root_id, old_sub_root); \
        } \
        else { \
          if (accessor##_GetLeft(tree, cur) == ins_entry_id) { \
            rb_tree_type_name##RbRotateRight(tree, old_sub_root, *cur_id, cur); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, new_sub_root_parent, old_sub_root_id, old_sub_root); \
        } \
        if (new_sub_root_parent_id) referencer##_Dereference(tree, new_sub_root_parent); \
        rb_tree_type_name##RbEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        accessor##_SetColor(tree, new_sub_root, kRbBlack); \
        accessor##_SetColor(tree, old_sub_root, kRbRed); \
        referencer##_Dereference(tree, new_sub_root); \
        \
        if (tree->root == old_sub_root_id) {    /* 原来的子树根节点可能是整棵树的根节点，因此要检查更新 */ \
          tree->root = new_sub_root_id; \
        } \
        break;    /* 只是并入，未分裂，向上没有改变颜色，不再需要回溯 */ \
      } \
      cur_id = parent_id; \
      referencer##_Dereference(tree, cur); \
    } \
    referencer##_Dereference(tree, cur); \
  } \
  /*
  * 向树中删除节点后的平衡操作
  */ \
   static void rb_tree_type_name##RbTreeDeleteFixup(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbBsStackVector* stack, id_type del_entry_id, bool is_parent_left) { \
    rb_tree_type_name##RbEntry* del_entry = referencer##_Reference(tree, del_entry_id); \
    id_type* parent_id = NULL; \
    \
    RbColor del_color = accessor##_GetColor(tree, del_entry); \
    if (del_color == kRbRed) { /* 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除 */ } \
    /* 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可 */ \
    else if (accessor##_GetLeft(tree, del_entry) != referencer##_InvalidId) { \
      rb_tree_type_name##RbEntry* del_entry_left = referencer##_Reference(tree, accessor##_GetLeft(tree, del_entry)); \
      accessor##_SetColor(tree, del_entry_left, kRbBlack); \
      referencer##_Dereference(tree, del_entry_left); \
    } \
    else if (accessor##_GetRight(tree, del_entry) != referencer##_InvalidId) { \
      rb_tree_type_name##RbEntry* del_entry_right = referencer##_Reference(tree, accessor##_GetRight(tree, del_entry)); \
      accessor##_SetColor(tree, del_entry_right, kRbBlack); \
      referencer##_Dereference(tree, del_entry_right); \
    } \
    else { parent_id = rb_tree_type_name##RbBsStackVectorPopTail(stack); } \
    \
    id_type new_sub_root_id; \
    id_type* grandpa_id = NULL; \
    /* 回溯维护删除黑色节点，即没有子节点(2节点)的情况 */ \
    rb_tree_type_name##RbEntry* parent = NULL, * sibling = NULL, * grandpa = NULL; \
    if (parent_id) parent = referencer##_Reference(tree, *parent_id); \
    while (parent_id != NULL) { \
      id_type sibling_id = is_parent_left ? accessor##_GetRight(tree, parent) : accessor##_GetLeft(tree, parent); \
      sibling = referencer##_Reference(tree, sibling_id); \
      grandpa_id = rb_tree_type_name##RbBsStackVectorPopTail(stack); \
      if (grandpa_id) grandpa = referencer##_Reference(tree, *grandpa_id); \
      else grandpa = NULL; \
      /* 黑色节点一定有兄弟节点 */ \
      if (accessor##_GetColor(tree, sibling) == kRbRed) { \
        /* 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
         旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点 */ \
        id_type old_sub_root_id = *parent_id; \
        rb_tree_type_name##RbEntry* old_sub_root = referencer##_Reference(tree, old_sub_root_id); \
        accessor##_SetColor(tree, old_sub_root, kRbRed); \
        accessor##_SetColor(tree, sibling, kRbBlack); \
        if (accessor##_GetLeft(tree, old_sub_root) == sibling_id) { \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, grandpa, old_sub_root_id, old_sub_root); \
          sibling_id = accessor##_GetLeft(tree, old_sub_root);   /* 下降后挂接过来的节点 */ \
          referencer##_Dereference(tree, sibling); \
          sibling = referencer##_Reference(tree, sibling_id); \
        } \
        else { \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, grandpa, old_sub_root_id, old_sub_root); \
          sibling_id = accessor##_GetRight(tree, old_sub_root);   /* 下降后挂接过来的节点 */ \
          referencer##_Dereference(tree, sibling); \
          sibling = referencer##_Reference(tree, sibling_id); \
        } \
        if (tree->root == old_sub_root_id) { \
          tree->root = new_sub_root_id; \
        } \
        referencer##_Dereference(tree, old_sub_root); \
        /* grandpa变为新根节点 */ \
        referencer##_Dereference(tree, grandpa); \
        grandpa_id = new_sub_root_id; \
        grandpa = referencer##_Reference(tree, grandpa_id); \
      } \
      \
      /* 至此兄弟节点一定为黑 */ \
      \
      /* 侄子节点为红，即兄弟节点是3 / 4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点) */ \
      rb_tree_type_name##RbEntry* sibling_right = referencer##_Reference(tree, accessor##_GetRight(tree, sibling)); \
      rb_tree_type_name##RbEntry* sibling_left = referencer##_Reference(tree, accessor##_GetLeft(tree, sibling)); \
      if (accessor##_GetRight(tree, sibling) != referencer##_InvalidId && accessor##_GetColor(tree, sibling_right) == kRbRed || \
        accessor##_GetLeft(tree, sibling) != referencer##_InvalidId && accessor##_GetColor(tree, sibling_left) == kRbRed) { \
        RbColor parent_color = accessor##_GetColor(tree, parent); \
        accessor##_SetColor(tree, parent, kRbBlack); \
        id_type old_sub_root_id = *parent_id; \
        rb_tree_type_name##RbEntry* new_sub_root_parent = NULL; \
        if (accessor##_GetLeft(tree, parent) == sibling_id) { \
          if (accessor##_GetLeft(tree, sibling) == referencer##_InvalidId || accessor##_GetColor(tree, sibling_left) == kRbBlack) { \
            accessor##_SetColor(tree, sibling_right, kRbBlack); \
            sibling_id = rb_tree_type_name##RbRotateLeft(tree, parent, sibling_id, sibling); \
          } \
          else { \
            accessor##_SetColor(tree, sibling_left, kRbBlack); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, grandpa, *parent_id, parent); \
        } \
        else { \
          if (accessor##_GetRight(tree, sibling) == referencer##_InvalidId || accessor##_GetColor(tree, sibling_right) == kRbBlack) { \
            accessor##_SetColor(tree, sibling_left, kRbBlack); \
            sibling_id = rb_tree_type_name##RbRotateRight(tree, parent, sibling_id, sibling); \
          } \
          else { \
            accessor##_SetColor(tree, sibling_right, kRbBlack); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, grandpa, *parent_id, parent); \
        } \
        referencer##_Dereference(tree, sibling); \
        sibling = referencer##_Reference(tree, sibling_id); \
        /* 该节点会接替原先的子根节点，也要接替颜色 */ \
        accessor##_SetColor(tree, sibling, parent_color); \
        if (tree->root == old_sub_root_id) { \
          tree->root = new_sub_root_id; \
        } \
        referencer##_Dereference(tree, sibling_right); \
        referencer##_Dereference(tree, sibling_left); \
        break; \
      } \
      referencer##_Dereference(tree, sibling_right); \
      referencer##_Dereference(tree, sibling_left); \
      \
      if (accessor##_GetColor(tree, parent) == kRbRed) { \
        /* 父节点为红，即父节点是3/4节点，分裂下降与兄弟节点合并
          |5|8|         |5|
           /  |  \   ->    /   \
          3   6  -9-      3   |6|8| */ \
        accessor##_SetColor(tree, sibling, kRbRed); \
        accessor##_SetColor(tree, parent, kRbBlack); \
        break; \
      } \
      else { \
        /* 父节点为黑，即父节点是2节点，兄弟节点也是2节点，下降父节点与兄弟节点合并，相当于向上删除父节点，继续回溯
         为什么不是3/4节点？因为黑父节点如果是3，兄弟节点是红，4的话回溯时父节点是红 */ \
        accessor##_SetColor(tree, sibling, kRbRed); \
      } \
      id_type child_id = *parent_id; \
      parent_id = grandpa_id; \
      referencer##_Dereference(tree, parent); \
      parent = grandpa; \
      if (parent != NULL) { \
        is_parent_left = accessor##_GetLeft(tree, parent) == child_id; \
      } \
    } \
    referencer##_Dereference(tree, sibling); \
    referencer##_Dereference(tree, parent); \
    \
    rb_tree_type_name##RbEntry* root = referencer##_Reference(tree, tree->root); \
    if (root && accessor##_GetColor(tree, root) == kRbRed) { \
      /* 根节点染黑 */  \
      accessor##_SetColor(tree, root, kRbBlack); \
    } \
    referencer##_Dereference(tree, root); \
  } \
  /*
  * 初始化树
  */ \
  void rb_tree_type_name##RbTreeInit(rb_tree_type_name##RbTree* tree) { \
    rb_tree_type_name##RbBsTreeInit(&tree->bs_tree); \
  } \
  /*
  * 从树中查找节点
  * 存在返回查找到的节点对应的对象，不存在返回NULL/最后一次查找的节点
  */ \
  static id_type rb_tree_type_name##RbTreeFindInternal(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbBsStackVector* stack, key_type* key) { \
    return rb_tree_type_name##RbBsTreeFind(&tree->bs_tree, stack, key); \
  } \
  id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key) { \
    LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack, id_type, offset_type); \
    return rb_tree_type_name##RbTreeFindInternal(&tree->bs_tree, &stack, key); \
  } \
  /*
  * 向树中插入节点
  * 允许重复key，但插入同一个节点时返回false
  */ \
  bool rb_tree_type_name##RbTreeInsert(rb_tree_type_name##RbTree* tree, id_type insert_entry_id) { \
    LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack, id_type, offset_type); \
    if(!rb_tree_type_name##RbBsTreeInsert(&tree->bs_tree, &stack, insert_entry_id)) return false; \
    rb_tree_type_name##RbTreeInsertFixup(tree, &stack, insert_entry_id); \
    return true; \
  } \
  /*
  * 向树中推入节点
  * 允许覆盖重复key，返回被覆盖的原entry，否则InvalidId，如果put_entry_id已经被插入过了，也会被返回(返回值 == put_entry_id)
  */ \
  id_type rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id) { \
    LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack, id_type, offset_type); \
    id_type old_id = rb_tree_type_name##RbBsTreePut(&tree->bs_tree, &stack, put_entry_id); \
    if (old_id == referencer##_InvalidId) rb_tree_type_name##RbTreeInsertFixup(tree, &stack, put_entry_id); \
    return old_id; \
  } \
  /*
  * 删除树中指定节点
  */ \
  static bool rb_tree_type_name##RbTreeDeleteInternal(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbBsStackVector* stack, id_type del_entry_id) { \
     assert(del_entry_id != referencer##_InvalidId); \
    bool is_parent_left; \
    id_type del_min_entry_id = rb_tree_type_name##RbBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left); \
    if (del_min_entry_id == referencer##_InvalidId) { \
      return false; \
    } \
    if (del_min_entry_id != del_entry_id) { \
      rb_tree_type_name##RbEntry* del_entry = referencer##_Reference(tree, del_entry_id); \
      rb_tree_type_name##RbEntry* del_min_entry = referencer##_Reference(tree, del_min_entry_id); \
      /* 需要交换颜色 */; \
      RbColor old_color = accessor##_GetColor(tree, del_min_entry); \
      accessor##_SetColor(tree, del_min_entry, accessor##_GetColor(tree, del_entry)); \
      accessor##_SetColor(tree, del_entry, old_color); \
      referencer##_Dereference(tree, del_min_entry); \
      referencer##_Dereference(tree, del_entry); \
    } \
    rb_tree_type_name##RbTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left); \
    return true; \
  } \
  bool rb_tree_type_name##RbTreeDelete(rb_tree_type_name##RbTree* tree, key_type* key) { \
    LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack, id_type, offset_type); \
    id_type del_entry_id = rb_tree_type_name##RbTreeFindInternal(&tree->bs_tree, &stack, key); \
    if (del_entry_id == referencer##_InvalidId) return false; \
    return rb_tree_type_name##RbTreeDeleteInternal(tree, &stack, del_entry_id); \
  } \
  bool rb_tree_type_name##RbTreeDeleteByIterator(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator) { \
    if (iterator->cur_id == referencer##_InvalidId) return false; \
    return rb_tree_type_name##RbTreeDeleteInternal(tree, &iterator->stack, iterator->cur_id); \
  } \
  offset_type rb_tree_type_name##RbTreeGetCount(rb_tree_type_name##RbTree* tree) { \
    LIBYUC_CONTAINER_RB_TREE_STACK_BUILD(rb_tree_type_name, stack, id_type, offset_type); \
    return rb_tree_type_name##RbBsTreeGetCount((rb_tree_type_name##RbBsTree*)tree, &stack); \
  } \
  /*
  * 迭代器相关
  */ \
  id_type rb_tree_type_name##RbTreeIteratorLocate(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator, key_type* key, int8_t* cmp_status) { \
    rb_tree_type_name##RbBsStackVectorInit(&iterator->stack, sizeof(iterator->stack_buf) / sizeof(id_type), iterator->stack_buf); \
    return rb_tree_type_name##RbBsTreeIteratorLocate((rb_tree_type_name##RbBsTree*)tree, &iterator->stack, key, cmp_status); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorFirst(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator) { \
    rb_tree_type_name##RbBsStackVectorInit(&iterator->stack, sizeof(iterator->stack_buf) / sizeof(id_type), iterator->stack_buf); \
    return rb_tree_type_name##RbBsTreeIteratorFirst((rb_tree_type_name##RbBsTree*)tree, &iterator->stack); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorLast(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator) { \
    rb_tree_type_name##RbBsStackVectorInit(&iterator->stack, sizeof(iterator->stack_buf) / sizeof(id_type), iterator->stack_buf); \
    return rb_tree_type_name##RbBsTreeIteratorLast((rb_tree_type_name##RbBsTree*)tree, &iterator->stack); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorNext(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator) { \
    return rb_tree_type_name##RbBsTreeIteratorNext((rb_tree_type_name##RbBsTree*)tree, &iterator->stack, iterator->cur_id); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorPrev(rb_tree_type_name##RbTree* tree, rb_tree_type_name##RbTreeIterator* iterator) { \
    return rb_tree_type_name##RbBsTreeIteratorPrev((rb_tree_type_name##RbBsTree*)tree, &iterator->stack, iterator->cur_id); \
  } \
  /*
  * 验证红黑树性质
  */ \
  static bool rb_tree_type_name##RbTreeCheckPath(rb_tree_type_name##RbTree* tree, id_type parent_id, id_type entry_id, offset_type cur_high, offset_type max_high) { \
    if (entry_id == referencer##_InvalidId) { \
      return cur_high == max_high; \
    } \
    bool correct = false; \
    rb_tree_type_name##RbEntry* entry = NULL; \
    rb_tree_type_name##RbEntry* parent = NULL; \
    do { \
      entry = referencer##_Reference(tree, entry_id); \
      if (parent_id != referencer##_InvalidId) { \
        parent = referencer##_Reference(tree, parent_id); \
        if (accessor##_GetColor(tree, entry) == kRbRed && accessor##_GetColor(tree, parent) == kRbRed) { \
          break; \
        } \
      } \
      if (accessor##_GetColor(tree, entry) == kRbBlack) { cur_high++; } \
      correct = rb_tree_type_name##RbTreeCheckPath(tree, entry_id, accessor##_GetLeft(tree, entry), cur_high, max_high) && rb_tree_type_name##RbTreeCheckPath(tree, entry_id, accessor##_GetRight(tree, entry), cur_high, max_high); \
    } while (false); \
    if (entry) referencer##_Dereference(tree, entry); \
    if (parent) referencer##_Dereference(tree, parent); \
    return correct; \
  } \
  bool rb_tree_type_name##RbTreeVerify(rb_tree_type_name##RbTree* tree) { \
    rb_tree_type_name##RbEntry* entry = referencer##_Reference(tree, tree->root); \
    if (!entry) return true; \
    bool correct = false; \
    do { \
      if (accessor##_GetColor(tree, entry) != kRbBlack) break; \
      offset_type high = 1; \
      while (accessor##_GetLeft(tree, entry) != referencer##_InvalidId) { \
        id_type entry_id = accessor##_GetLeft(tree, entry); \
        referencer##_Dereference(tree, entry); \
        entry = referencer##_Reference(tree, entry_id); \
        /* 红色节点不表示高度 */ \
        if (accessor##_GetColor(tree, entry) == kRbBlack) { \
          high++; \
        } \
      } \
      correct = rb_tree_type_name##RbTreeCheckPath(tree, referencer##_InvalidId, tree->root, 0, high); \
    } while (false); \
    referencer##_Dereference(tree, entry); \
    return correct; \
  } \
  


//LIBYUC_CONTAINER_RB_TREE_DECLARATION(Defalut, struct _DefalutRbEntry*, size_t, int)
//typedef struct _DefalutRb {
//  DefalutRbEntry rb_entry;
//  int key;
//} DefalutRb;
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetKey(TREE, ENTRY) (((DefalutRb*)ENTRY)->key)
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetParent(TREE, ENTRY) ((DefalutRbEntry*)(((uintptr_t)(((DefalutRbEntry*)ENTRY)->parent_color) & (~((uintptr_t)0x1)))))
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetColor(TREE, ENTRY) ((RbColor)(((uintptr_t)((DefalutRbEntry*)ENTRY)->parent_color) & 0x1))
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_SetParent(TREE, ENTRY, NEW_PARENT_ID) ((DefalutRbEntry*)((DefalutRbEntry*)ENTRY)->parent_color = (DefalutRbEntry*)(((uintptr_t)NEW_PARENT_ID) | ((uintptr_t)LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetColor(TREE, ENTRY))));
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_SetColor(TREE, ENTRY, COLOR) (((DefalutRbEntry*)ENTRY)->parent_color = (((uintptr_t)LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetParent(TREE, ENTRY)) | ((uintptr_t)COLOR)))
//#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_RB_TREE_H_