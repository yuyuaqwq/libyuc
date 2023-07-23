/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_RB_TREE_H_
#define LIBYUC_CONTAINER_RB_TREE_H_

#include <libyuc/object.h>
#include <libyuc/container/experimental/bs_tree.h>

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

#define LIBYUC_CONTAINER_RB_TREE_DECLARATION(rb_tree_type_name, id_type, key_type) \
  LIBYUC_CONTAINER_BS_TREE_DECLARATION(rb_tree_type_name##Rb, id_type, key_type) \
  typedef struct _##rb_tree_type_name##RbEntry { \
    union { \
      struct { \
        id_type parent_color; \
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
  \
  void rb_tree_type_name##RbTreeInit(rb_tree_type_name##RbTree* tree); \
  id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key); \
  bool rb_tree_type_name##RbTreeInsert(rb_tree_type_name##RbTree* tree, id_type insert_entry_id); \
  id_type rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id); \
  bool rb_tree_type_name##RbTreeDelete(rb_tree_type_name##RbTree* tree, id_type del_entry_id); \
  bool rb_tree_type_name##RbTreeVerify(rb_tree_type_name##RbTree* tree); \
  id_type rb_tree_type_name##RbTreeIteratorLocate(rb_tree_type_name##RbTree* tree, key_type* key, int8_t* cmp_status); \
  id_type rb_tree_type_name##RbTreeIteratorFirst(rb_tree_type_name##RbTree* tree); \
  id_type rb_tree_type_name##RbTreeIteratorLast(rb_tree_type_name##RbTree* tree); \
  id_type rb_tree_type_name##RbTreeIteratorNext(rb_tree_type_name##RbTree* tree, id_type cur_id); \
  id_type rb_tree_type_name##RbTreeIteratorPrev(rb_tree_type_name##RbTree* tree, id_type cur_id); \

// 访问器需要提供_GetKey、_Set/GetParent、_Set/GetColor方法
#define LIBYUC_CONTAINER_RB_TREE_DEFINE(rb_tree_type_name, id_type, key_type, referencer, accessor, comparer) \
  LIBYUC_CONTAINER_BS_TREE_DEFINE(rb_tree_type_name##Rb, id_type, key_type, referencer, accessor, comparer) \
  /*
  * 取兄弟节点
  */ \
  static id_type rb_tree_type_name##GetSiblingEntry(rb_tree_type_name##RbTree* tree, id_type entry_id, rb_tree_type_name##RbEntry* entry) { \
    id_type parent_id = accessor##_GetParent(tree, entry); \
    rb_tree_type_name##RbEntry* parent = referencer##_Reference(tree, parent_id); \
    id_type ret; \
    if (parent->left == entry_id) { \
      ret = parent->right; \
    } \
    else { \
      ret = parent->left; \
    }\
    referencer##_Dereference(tree, parent); \
    return ret; \
  } \
  \
  /*
  * 向树中插入节点后的平衡操作
  */ \
  static void rb_tree_type_name##RbTreeInsertFixup(rb_tree_type_name##RbTree* tree, id_type ins_entry_id) { \
    rb_tree_type_name##RbEntry* ins_entry = referencer##_Reference(tree, ins_entry_id); \
    accessor##_SetColor(tree, ins_entry, kRbBlack); \
    id_type cur_id = accessor##_GetParent(tree, ins_entry); \
    if (cur_id == referencer##_InvalidId) { \
      accessor##_SetColor(tree, ins_entry, kRbBlack); \
      referencer##_Dereference(tree, ins_entry); \
      return; \
    } \
    accessor##_SetColor(tree, ins_entry, kRbRed); \
    referencer##_Dereference(tree, ins_entry); \
    \
    rb_tree_type_name##RbEntry* cur = NULL; \
    /* 开始回溯维护 */ \
    while (cur_id != referencer##_InvalidId) { \
      cur = referencer##_Reference(tree, cur_id); \
      if (accessor##_GetColor(tree, cur) == kRbBlack) { \
        /* 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点 / 3节点的插入，直接合并) */ \
        break; \
      } \
      if (accessor##_GetParent(tree, cur) == referencer##_InvalidId) { \
        /* 没有父节点，回溯到根节点了，直接染黑 */ \
        accessor##_SetColor(tree, cur, kRbBlack); \
        break; \
      } \
      id_type sibling_id = rb_tree_type_name##GetSiblingEntry(tree, cur_id, cur); \
      rb_tree_type_name##RbEntry* sibling = referencer##_Reference(tree, sibling_id); \
      if (sibling_id != referencer##_InvalidId && accessor##_GetColor(tree, sibling) == kRbRed) { \
        /* 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上插入，继续回溯 */ \
        accessor##_SetColor(tree, cur, kRbBlack); \
        accessor##_SetColor(tree, sibling, kRbBlack); \
        ins_entry_id = accessor##_GetParent(tree, cur);     /* 更新为该节点向上插入 */ \
        ins_entry = referencer##_Reference(tree, ins_entry_id); \
        if (accessor##_GetParent(tree, ins_entry) == referencer##_InvalidId) { \
          accessor##_SetColor(tree, ins_entry, kRbBlack);   /* 根节点，染黑并结束 */ \
          break; \
        } \
        accessor##_SetColor(tree, ins_entry, kRbRed); \
        cur = ins_entry; \
      } \
      else { \
        /* 没有兄弟节点或兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
         *    10b              5b  
         *   5r   20b  ->   !2r   10r  
         * !2r                       20b
        */ \
          assert(sibling_id == referencer##_InvalidId || accessor##_GetColor(tree, sibling) == kRbBlack); \
        id_type new_sub_root_id; \
        id_type old_sub_root_id = accessor##_GetParent(tree, cur); \
        rb_tree_type_name##RbEntry* old_sub_root = referencer##_Reference(tree, old_sub_root_id); \
        if (old_sub_root->left == cur_id) { \
          if (cur->right == ins_entry_id) { \
            rb_tree_type_name##RbRotateLeft(tree, cur_id, cur); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, old_sub_root_id, old_sub_root); \
        } \
        else { \
          if (cur->left == ins_entry_id) { \
            rb_tree_type_name##RbRotateRight(tree, cur_id, cur); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, old_sub_root_id, old_sub_root); \
        } \
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
      cur_id = accessor##_GetParent(tree, cur); \
      referencer##_Dereference(tree, cur); \
    } \
    referencer##_Dereference(tree, cur); \
  } \
  /*
  * 向树中删除节点后的平衡操作
  */ \
   static void rb_tree_type_name##RbTreeDeleteFixup(rb_tree_type_name##RbTree* tree, id_type del_entry_id, bool is_parent_left) { \
    rb_tree_type_name##RbEntry* del_entry = referencer##_Reference(tree, del_entry_id); \
    id_type parent_id = referencer##_InvalidId; \
    \
    RbColor del_color = accessor##_GetColor(tree, del_entry); \
    if (del_color == kRbRed) { /* 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除 */ } \
    /* 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可 */ \
    else if (del_entry->left != referencer##_InvalidId) { \
      rb_tree_type_name##RbEntry* del_entry_left = referencer##_Reference(tree, del_entry->left); \
      accessor##_SetColor(tree, del_entry_left, kRbBlack); \
      referencer##_Dereference(tree, del_entry_left); \
    } \
    else if (del_entry->right != referencer##_InvalidId) { \
      rb_tree_type_name##RbEntry* del_entry_right = referencer##_Reference(tree, del_entry->right); \
      accessor##_SetColor(tree, del_entry_right, kRbBlack); \
      referencer##_Dereference(tree, del_entry_right); \
    } \
    else { parent_id = accessor##_GetParent(tree, del_entry); } \
    \
    id_type new_sub_root_id; \
    /* 回溯维护删除黑色节点，即没有子节点(2节点)的情况 */ \
    rb_tree_type_name##RbEntry* parent = NULL, * sibling = NULL; \
    parent = referencer##_Reference(tree, parent_id); \
    while (parent_id != referencer##_InvalidId) { \
      id_type sibling_id = is_parent_left ? parent->right : parent->left; \
      sibling = referencer##_Reference(tree, sibling_id); \
      /* 黑色节点一定有兄弟节点 */ \
      if (accessor##_GetColor(tree, sibling) == kRbRed) { \
        /* 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
         旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点 */ \
        id_type old_sub_root_id = accessor##_GetParent(tree, sibling); \
        rb_tree_type_name##RbEntry* old_sub_root = referencer##_Reference(tree, old_sub_root_id); \
        accessor##_SetColor(tree, old_sub_root, kRbRed); \
        accessor##_SetColor(tree, sibling, kRbBlack); \
        if (old_sub_root->left == sibling_id) { \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, old_sub_root_id, old_sub_root); \
          sibling_id = old_sub_root->left;   /* 下降后挂接过来的节点 */ \
          referencer##_Dereference(tree, sibling); \
          sibling = referencer##_Reference(tree, sibling_id); \
        } \
        else { \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, old_sub_root_id, old_sub_root); \
          sibling_id = old_sub_root->right;   /* 下降后挂接过来的节点 */ \
          referencer##_Dereference(tree, sibling); \
          sibling = referencer##_Reference(tree, sibling_id); \
        } \
        if (tree->root == old_sub_root_id) { \
          tree->root = new_sub_root_id; \
        } \
        referencer##_Dereference(tree, old_sub_root); \
      } \
      \
      /* 至此兄弟节点一定为黑 */ \
      \
      /* 侄子节点为红，即兄弟节点是3 / 4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点) */ \
      rb_tree_type_name##RbEntry* sibling_right = referencer##_Reference(tree, sibling->right); \
      rb_tree_type_name##RbEntry* sibling_left = referencer##_Reference(tree, sibling->left); \
      if (sibling->right != referencer##_InvalidId && accessor##_GetColor(tree, sibling_right) == kRbRed || \
        sibling->left != referencer##_InvalidId && accessor##_GetColor(tree, sibling_left) == kRbRed) { \
        RbColor parent_color = accessor##_GetColor(tree, parent); \
        accessor##_SetColor(tree, parent, kRbBlack); \
        id_type old_sub_root_id = parent_id; \
        if (parent->left == sibling_id) { \
          if (sibling->left == referencer##_InvalidId || accessor##_GetColor(tree, sibling_left) == kRbBlack) { \
            accessor##_SetColor(tree, sibling_right, kRbBlack); \
            sibling_id = rb_tree_type_name##RbRotateLeft(tree, sibling_id, sibling); \
          } \
          else { \
            accessor##_SetColor(tree, sibling_left, kRbBlack); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateRight(tree, parent_id, parent); \
        } \
        else { \
          if (sibling->right == referencer##_InvalidId || accessor##_GetColor(tree, sibling_right) == kRbBlack) { \
            accessor##_SetColor(tree, sibling_left, kRbBlack); \
            sibling_id = rb_tree_type_name##RbRotateRight(tree, sibling_id, sibling); \
          } \
          else { \
            accessor##_SetColor(tree, sibling_right, kRbBlack); \
          } \
          new_sub_root_id = rb_tree_type_name##RbRotateLeft(tree, parent_id, parent); \
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
      id_type child_id = parent_id; \
      parent_id = accessor##_GetParent(tree, parent); \
      if (parent_id != referencer##_InvalidId) { \
        referencer##_Dereference(tree, parent); \
        parent = referencer##_Reference(tree, parent_id); \
        is_parent_left = parent->left == child_id; \
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
  id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key) { \
    return rb_tree_type_name##RbBsTreeFind(&tree->bs_tree, key); \
  } \
  /*
  * 向树中插入节点
  * 允许重复key，但插入同一个节点时返回false
  */ \
  bool rb_tree_type_name##RbTreeInsert(rb_tree_type_name##RbTree* tree, id_type insert_entry_id) { \
    if(!rb_tree_type_name##RbBsTreeInsert(&tree->bs_tree, insert_entry_id)) return false; \
    rb_tree_type_name##RbTreeInsertFixup(tree, insert_entry_id); \
    return true; \
  } \
  /*
  * 向树中推入节点
  * 允许覆盖重复key，返回被覆盖的原entry，否则InvalidId，如果put_entry_id已经被插入过了，也会被返回(返回值 == put_entry_id)
  */ \
  id_type rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id) { \
    id_type old_id = rb_tree_type_name##RbBsTreePut(&tree->bs_tree, put_entry_id); \
    if (old_id == referencer##_InvalidId) rb_tree_type_name##RbTreeInsertFixup(tree, put_entry_id); \
    return old_id; \
  } \
  /*
  * 删除树中指定节点
  */ \
  bool rb_tree_type_name##RbTreeDelete(rb_tree_type_name##RbTree* tree, id_type del_entry_id) { \
    bool is_parent_left; \
    id_type del_min_entry_id = rb_tree_type_name##RbBsTreeDelete(&tree->bs_tree, del_entry_id, &is_parent_left); \
    if(del_min_entry_id == referencer##_InvalidId) { \
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
    rb_tree_type_name##RbTreeDeleteFixup(tree, del_entry_id, is_parent_left); \
    return true; \
  } \
  /*
  * 验证红黑树性质
  */ \
  static bool rb_tree_type_name##RbTreeCheckPath(rb_tree_type_name##RbTree* tree, id_type entry_id, int32_t cur_high, int32_t max_high) { \
    if (entry_id == referencer##_InvalidId) { \
      return cur_high == max_high; \
    } \
    bool correct = false; \
    rb_tree_type_name##RbEntry* entry = NULL; \
    rb_tree_type_name##RbEntry* parent = NULL; \
    do { \
      entry = referencer##_Reference(tree, entry_id); \
      id_type parent_id = accessor##_GetParent(tree, entry); \
      if (parent_id != referencer##_InvalidId) { \
        parent = referencer##_Reference(tree, parent_id); \
        if (accessor##_GetColor(tree, entry) == kRbRed && accessor##_GetColor(tree, parent) == kRbRed) { \
          break; \
        } \
      } \
      if (accessor##_GetColor(tree, entry) == kRbBlack) { cur_high++; } \
      correct = rb_tree_type_name##RbTreeCheckPath(tree, entry->left, cur_high, max_high) && rb_tree_type_name##RbTreeCheckPath(tree, entry->right, cur_high, max_high); \
    } while (false); \
    if (entry) referencer##_Dereference(tree, entry); \
    if (parent) referencer##_Dereference(tree, parent); \
    return correct; \
  } \
  bool rb_tree_type_name##RbTreeVerify(rb_tree_type_name##RbTree* tree) { \
    rb_tree_type_name##RbEntry* entry = referencer##_Reference(tree, tree->root); \
    bool correct = false; \
    do { \
      if (accessor##_GetColor(tree, entry) != kRbBlack) break; \
      int32_t high = 1; \
      while (entry->left != referencer##_InvalidId) { \
        id_type entry_id = entry->left; \
        referencer##_Dereference(tree, entry); \
        entry = referencer##_Reference(tree, entry_id); \
        /* 红色节点不表示高度 */ \
        if (accessor##_GetColor(tree, entry) == kRbBlack) { \
          high++; \
        } \
      } \
      correct = rb_tree_type_name##RbTreeCheckPath(tree, tree->root, 0, high); \
    } while (false); \
    referencer##_Dereference(tree, entry); \
    return correct; \
  } \
  size_t rb_tree_type_name##RbTreeGetCount(rb_tree_type_name##RbTree* tree) { \
    return rb_tree_type_name##RbBsTreeGetCount((rb_tree_type_name##RbBsTree*)tree); \
  } \
  /*
  * 迭代器相关
  */ \
  id_type rb_tree_type_name##RbTreeIteratorLocate(rb_tree_type_name##RbTree* tree, key_type* key, int8_t* cmp_status) { \
    return rb_tree_type_name##RbBsTreeIteratorLocate((rb_tree_type_name##RbBsTree*)tree, key, cmp_status); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorFirst(rb_tree_type_name##RbTree* tree) { \
    return rb_tree_type_name##RbBsTreeIteratorFirst((rb_tree_type_name##RbBsTree*)tree); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorLast(rb_tree_type_name##RbTree* tree) { \
    return rb_tree_type_name##RbBsTreeIteratorLast((rb_tree_type_name##RbBsTree*)tree); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorNext(rb_tree_type_name##RbTree* tree, id_type cur_id) { \
    return rb_tree_type_name##RbBsTreeIteratorNext((rb_tree_type_name##RbBsTree*)tree, cur_id); \
  } \
  id_type rb_tree_type_name##RbTreeIteratorPrev(rb_tree_type_name##RbTree* tree, id_type cur_id) { \
    return rb_tree_type_name##RbBsTreeIteratorPrev((rb_tree_type_name##RbBsTree*)tree, cur_id); \
  } \



LIBYUC_CONTAINER_RB_TREE_DECLARATION(Defalut, struct _DefalutRbEntry*, int)
typedef struct _DefalutRb {
  DefalutRbEntry rb_entry;
  int key;
} DefalutRb;
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetKey(TREE, ENTRY) (((DefalutRb*)ENTRY)->key)
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetLeft(TREE, ENTRY) ((DefalutRbEntry*)(((uintptr_t)(((DefalutRbEntry*)ENTRY)->left) & (~((uintptr_t)0x1)))))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetColor(TREE, ENTRY) ((RbColor)(((uintptr_t)((DefalutRbEntry*)ENTRY)->left) & 0x1))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_SetLeft(TREE, ENTRY, NEW_ID) ((DefalutRbEntry*)((DefalutRbEntry*)ENTRY)->left = (DefalutRbEntry*)(((uintptr_t)NEW_ID) | ((uintptr_t)LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetColor(TREE, ENTRY))))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_SetColor(TREE, ENTRY, COLOR) (((DefalutRbEntry*)ENTRY)->parent_color = (((uintptr_t)LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetLeft(TREE, ENTRY)) | ((uintptr_t)COLOR)))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_GetRight(TREE, ENTRY) ((DefalutRbEntry*)((uintptr_t)((DefalutRbEntry*)ENTRY)->right))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT_SetRight(TREE, ENTRY, NEW_ID) ((DefalutRbEntry*)((uintptr_t)((DefalutRbEntry*)ENTRY)->right = NEW_ID))
#define LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT LIBYUC_CONTINUE_RB_TREE_ACCESSOR_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_RB_TREE_H_