/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_AVL_TREE_H_
#define LIBYUC_CONTAINER_AVL_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/container/bs_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 嵌入平衡因子的Avl树
*/
#define LIBYUC_CONTAINER_AVL_TREE_DECLARATION(avl_tree_type_name, id_type, offset_type, key_type) \
    LIBYUC_CONTAINER_BS_TREE_DECLARATION( \
        avl_tree_type_name##Avl, \
        id_type, \
        offset_type, \
        key_type, \
        sizeof(offset_type) * 8 * 2 /* 1.44 * log(n + 2) - 1.328 */ \
        ) \
    typedef struct _##avl_tree_type_name##AvlEntry { \
        union { \
            struct { \
                id_type left; \
                id_type right; \
            }; \
            avl_tree_type_name##AvlBsEntry bs_entry; \
        }; \
    } avl_tree_type_name##AvlEntry; \
    typedef struct _##avl_tree_type_name##AvlTree { \
        union { \
            avl_tree_type_name##AvlBsTree bs_tree; \
            id_type root; \
        }; \
    } avl_tree_type_name##AvlTree; \
    \
    void avl_tree_type_name##AvlTreeInit(avl_tree_type_name##AvlTree* tree); \
    id_type avl_tree_type_name##AvlTreeFind(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, key_type* key); \
    id_type avl_tree_type_name##AvlTreePut(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type put_entry_id); \
    bool avl_tree_type_name##AvlTreeDelete(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type del_entry_id); \
    \
    bool avl_tree_type_name##AvlTreeVerify(avl_tree_type_name##AvlTree* tree); \

// 访问器需要提供_GetKey、_Set/GetParent、_Set/GetBalanceFactor方法
#define LIBYUC_CONTAINER_AVL_TREE_DEFINE(avl_tree_type_name, id_type, offset_type, key_type, referencer, accessor, comparer) \
    LIBYUC_CONTAINER_BS_TREE_DEFINE( \
        avl_tree_type_name##Avl, \
        id_type, \
        offset_type, \
        key_type, \
        referencer, \
        accessor, \
        comparer \
        ) \
    /*
    * 旋转平衡因子调整(右旋为例)：
    * 单旋场景：
    *
    * 原根节点的平衡因子是2，新根节点的平衡因子是1
    * 说明原根节点的右子树深度是小于左子树2的
    *
    * 情况1：平衡因子置0
    *                 o6, 2                    o5, 0
    *         o5, 1         o3    ->        o4            o6(4), 0
    *     o4     o3                                o3        o3
    * 新根节点的右子树(o3)高度一定低于新根节点2，与原根节点(o6)的右子树高度相等，因此原根节点的平衡因子置0
    *
    *
    *
    * 情况2：调整平衡因子(左旋为例)
    * 删除节点时可能会出现新根节点平衡因子为0的情况，如下删除的是o1
    *                    o4, -2                                    o3(4), 1
    *         o2(1), 0             o3, 0         ->             o4(3), -1            o2
    *                -o1-         o2         o2         o2(1), 0         o2
    * 此时不能简单置0
    * 原根节点(o4)高度高于新根节点(o3)子树(o2, o2)1，而o4的左子树(o2(1))又低于o3子树(o2, o2)1
    * 故原根节点(o4)置为-1
    * o4高度高于o3子树1，而o4的左子树又低于o3子树1
    *
    * 因为挂接过去的节点高度可能与当前高度不同，因此需要调整
    *
    *
    *
    *
    * 双旋场景：
    *
    * 原根节点的平衡因子是2，新根节点的平衡因子是-1
    * 直接旋转：
    *                 o6, 2                         o5(6), -2
    *         o5, -1         o3    ->         o3                 o6(5), 1
    *     o3     o4                                         o4            o3
    *
    *
    * 先左旋：
    * 情况1：
    *                        o6, 2                                         o6, 2
    *            o5, -1            o3    ->                    o4(5), 1                 o3
    *     o3            o4, -1                        o5(4),1            o3
    *                o2         o3                 o3            o2
    * o4的子节点高度是o2或者o3，挂到o5下可能影响o5的平衡因子，因此右旋后需要调整o5的平衡因子
    *
    * 依据是o4，-1时(o2和o3)置为1，0(o3和o3)和1(o3和o2)时置为0
    * o5的新右子节点是o4的左子节点，o4的左子节点要么o2要么o3
    *
    *
    * 情况2：
    *
    *                        o6, 2                                         o6, 2
    *            o5, -1            o3    ->                    o4(5), 2                 o3
    *     o3            o4, 1                        o5(4),0            o2
    *                o3         o2                o3            o3
    * 情况2是第一次旋完之后，o4的右子节点o2与o6的右子节点o3不同高度
    * 即相当于旋完后还需要更新o6的平衡因子
    * 依据是o4，1时(o3和o2，o2要给o6做左子节点)置为-1
    *
    */ \
    /*
    * 根据平衡因子来旋转子树
    * 并负责更新平衡因子
    * 子树高度变化返回true，高度未变化返回false
    */ \
    static bool avl_tree_type_name##RotateByBalanceFactor(avl_tree_type_name##AvlTree* tree, id_type parent_id, id_type* sub_root_id_io, avl_tree_type_name##AvlEntry** sub_root_io, id_type deep_child_id, avl_tree_type_name##AvlEntry* deep_child, int8_t cur_bf) { \
        bool rotate = false; \
        bool height_update = true; \
        avl_tree_type_name##AvlEntry* new_sub_root_id = NULL; \
        avl_tree_type_name##AvlEntry* sub_root = *sub_root_io; \
        avl_tree_type_name##AvlEntry* parent = parent_id != referencer##_InvalidId ? referencer##_Reference(tree, parent_id) : NULL; \
        id_type sub_root_id = *sub_root_id_io; \
         assert(cur_bf >= -2 || cur_bf <= 2); \
        if (cur_bf == 2) { \
            /* 是失衡节点(最小失衡子树的根节点)，左子树高度高于右子树高度 */ \
            /* avl_tree_type_name##AvlEntry* deep_child = accessor##_GetLeft(tree, sub_root); */ \
            int8_t deep_child_bf = accessor##_GetBalanceFactor(tree, deep_child); \
            /* 判断需要单旋还是双旋 */ \
            if (deep_child_bf == -1) { \
                 /* 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
                        o
                    o
                     o
                 需要先左旋转 */ \
                avl_tree_type_name##AvlEntry* deep_child_right = referencer##_Reference(tree, accessor##_GetRight(tree, deep_child)); \
                    assert(deep_child_right); \
                int8_t right_bf = accessor##_GetBalanceFactor(tree, deep_child_right); \
                accessor##_SetBalanceFactor(tree, deep_child_right, 0); \
                referencer##_Dereference(tree, tree, deep_child_right); \
                avl_tree_type_name##AvlRotateLeft(tree, sub_root, deep_child_id, deep_child); \
                new_sub_root_id = avl_tree_type_name##AvlRotateRight(tree, parent, sub_root_id, sub_root); \
                \
                if (right_bf == -1) { \
                    accessor##_SetBalanceFactor(tree, deep_child, 1); \
                    accessor##_SetBalanceFactor(tree, sub_root, 0); \
                } \
                else { \
                    accessor##_SetBalanceFactor(tree, deep_child, 0); \
                    if (right_bf == 1) { \
                        accessor##_SetBalanceFactor(tree, sub_root, -1); \
                    } else { \
                        accessor##_SetBalanceFactor(tree, sub_root, 0); \
                    } \
                } \
            } \
            else if (deep_child_bf == 0) { \
                 /* 删除节点时可能出现的情况，即右子树的右子树高度低于左子树的左子树1，旋转过来并不完美平衡
                        o
                     o
                 o     o

                         o4, 2
                 o3, 0         o2(1) */ \
                height_update = false;         /* 这种情况的旋转不会改变该子树的高度 */ \
                new_sub_root_id = avl_tree_type_name##AvlRotateRight(tree, parent, sub_root_id, sub_root); \
                accessor##_SetBalanceFactor(tree, deep_child, -1); \
                accessor##_SetBalanceFactor(tree, sub_root, 1); \
            } \
            else { \
                /* 此时失衡节点的左子树的左子树更深，右旋即可
                 可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点 */ \
                new_sub_root_id = avl_tree_type_name##AvlRotateRight(tree, parent, sub_root_id, sub_root); \
                accessor##_SetBalanceFactor(tree, deep_child , 0); \
                accessor##_SetBalanceFactor(tree, sub_root, 0); \
            } \
            rotate = true; \
        } \
        else if (cur_bf == -2) { \
            /* 是失衡节点，右子树高度高于左子树高度 */ \
            /* avl_tree_type_name##AvlEntry* deep_child = accessor##_GetRight(tree, sub_root) */; \
            int8_t deep_child_bf = accessor##_GetBalanceFactor(tree, deep_child); \
            /* 判断需要单旋还是双旋 */ \
            if (deep_child_bf == 1) { \
                 /* 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
                    o
                        o
                        o
                 需要先右旋转 */ \
                avl_tree_type_name##AvlEntry* deep_child_left = referencer##_Reference(tree, accessor##_GetLeft(tree, deep_child)); \
                    assert(deep_child_left); \
                int8_t left_bf = accessor##_GetBalanceFactor(tree, deep_child_left); \
                accessor##_SetBalanceFactor(tree, deep_child_left, 0); \
                referencer##_Dereference(tree, deep_child_left); \
                avl_tree_type_name##AvlRotateRight(tree, sub_root, deep_child_id, deep_child); \
                new_sub_root_id = avl_tree_type_name##AvlRotateLeft(tree, parent, sub_root_id, sub_root); \
                \
                if (left_bf == 1) { \
                    accessor##_SetBalanceFactor(tree, deep_child, -1); \
                    accessor##_SetBalanceFactor(tree, sub_root, 0); \
                } \
                else { \
                    accessor##_SetBalanceFactor(tree, deep_child, 0); \
                    if (left_bf == -1) { \
                        accessor##_SetBalanceFactor(tree, sub_root, 1); \
                    } else { \
                        accessor##_SetBalanceFactor(tree, sub_root, 0); \
                    } \
                } \
            } \
            else if (deep_child_bf == 0) { \
                 /* 删除节点时可能出现的情况，即左子树的左子树高度低于右子树的右子树1，旋转过来并不完美平衡
                    o
                        o
                        o     o

                         o4, -2
                 o2(1)         o3, 0 */ \
                height_update = false;         /* 这种情况的旋转不会改变该子树的高度 */ \
                new_sub_root_id = avl_tree_type_name##AvlRotateLeft(tree, parent, sub_root_id, sub_root); \
                accessor##_SetBalanceFactor(tree, deep_child, 1); \
                accessor##_SetBalanceFactor(tree, sub_root, -1); \
            } \
            else {        /* child_bf == -1 */ \
                /* 此时失衡节点的右子树的右子树更深，左旋即可 */ \
                new_sub_root_id = avl_tree_type_name##AvlRotateLeft(tree, parent, sub_root_id, sub_root); \
                accessor##_SetBalanceFactor(tree, deep_child, 0); \
                accessor##_SetBalanceFactor(tree, sub_root, 0); \
            } \
            rotate = true; \
        } \
        else { \
            height_update = false; \
            accessor##_SetBalanceFactor(tree, sub_root, cur_bf); \
        } \
        if (rotate) { \
            if (tree->root == sub_root) {        /* 原来的子树根节点可能是整棵树的根节点，因此要检查更新 */ \
                tree->root = new_sub_root_id; \
            } \
            *sub_root_id_io = new_sub_root_id; \
            referencer##_Dereference(tree, sub_root); \
            *sub_root_io = referencer##_Reference(tree, new_sub_root_id); \
        } \
        if (parent) referencer##_Dereference(tree, parent); \
        return height_update; \
    } \
    /*
    * 向树中插入节点的平衡操作
    */ \
    static void avl_tree_type_name##AvlTreeInsertFixup(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type ins_entry_id) { \
        id_type* cur_id_ptr = avl_tree_type_name##AvlBsStackVectorPopTail(stack); \
        id_type cur_id = referencer##_InvalidId; \
        if (cur_id_ptr) cur_id = *cur_id_ptr; \
        id_type child_id = ins_entry_id; \
        avl_tree_type_name##AvlEntry* cur = NULL; \
        avl_tree_type_name##AvlEntry* child = referencer##_Reference(tree, child_id); \
        accessor##_SetBalanceFactor(tree, child, 0); \
        /* 插入节点后平衡因子可能发生变化，回溯维护平衡因子 */ \
        while (cur_id != referencer##_InvalidId) { \
            cur = referencer##_Reference(tree, cur_id); \
            child = referencer##_Reference(tree, child_id); \
            int8_t cur_bf = accessor##_GetBalanceFactor(tree, cur); \
            if (child_id == accessor##_GetLeft(tree, cur)) cur_bf++;        /* 新节点插入到当前节点的左子树 */ \
            else cur_bf--;         /* 新节点插入到当前节点的右子树 */ \
            \
            id_type* parent_id_ptr = avl_tree_type_name##AvlBsStackVectorPopTail(stack) ; \
            id_type parent_id = referencer##_InvalidId; \
            if (parent_id_ptr) parent_id = *parent_id_ptr; \
            if (avl_tree_type_name##RotateByBalanceFactor(tree, parent_id, &cur_id, &cur, child_id, child, cur_bf) || cur_bf == 0) { \
                /* 旋转后当前节点高度不变，或原先高度就不变，停止回溯 */ \
                break; \
            } \
            referencer##_Dereference(tree, child); \
            child_id = cur_id; \
            child = cur; \
            cur = NULL; \
            cur_id = parent_id; \
        } \
        if (cur) referencer##_Dereference(tree, cur); \
        if (child) referencer##_Dereference(tree, child); \
    } \
    /*
    * 从树中删除节点的平衡操作
    */ \
    static void avl_tree_type_name##AvlTreeDeleteFixup(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type del_entry_id, bool is_parent_left) { \
        /* 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子 */ \
        id_type* cur_id_ptr = avl_tree_type_name##AvlBsStackVectorPopTail(stack); \
        id_type cur_id = referencer##_InvalidId; \
        if (cur_id_ptr) cur_id = *cur_id_ptr; \
        id_type child_id = del_entry_id; \
        avl_tree_type_name##AvlEntry* cur = NULL; \
        while (cur_id != referencer##_InvalidId) { \
            cur = referencer##_Reference(tree, cur_id); \
            int8_t cur_bf = accessor##_GetBalanceFactor(tree, cur); \
            if (is_parent_left) { cur_bf--; } \
            else { cur_bf++; } \
            id_type* parent_id_ptr = avl_tree_type_name##AvlBsStackVectorPopTail(stack) ; \
            id_type parent_id = referencer##_InvalidId; \
            if (parent_id_ptr) parent_id = *parent_id_ptr; \
            if (cur_bf != 0) { \
                id_type deep_child_id = is_parent_left ? accessor##_GetRight(tree, cur) : accessor##_GetLeft(tree, cur); \
                avl_tree_type_name##AvlEntry* deep_child = referencer##_Reference(tree, deep_child_id); \
                if (avl_tree_type_name##RotateByBalanceFactor(tree, parent_id, &cur_id, &cur, deep_child_id, deep_child, cur_bf) == false) { \
                    /* 另一侧高度相等或更深且无需旋转，则当前节点高度不变 */ \
                    referencer##_Dereference(tree, deep_child); \
                    break; \
                } \
                referencer##_Dereference(tree, deep_child); \
            } \
            else { \
                accessor##_SetBalanceFactor(tree, cur, cur_bf); \
            } \
            child_id = cur_id; \
            cur_id = parent_id; \
            referencer##_Dereference(tree, cur); \
            cur = referencer##_Reference(tree, cur_id); \
            if (cur) { \
                is_parent_left = accessor##_GetLeft(tree, cur) == child_id; \
            } \
        } \
        referencer##_Dereference(tree, cur); \
    } \
    /*
    * 初始化树
    */ \
    void avl_tree_type_name##AvlTreeInit(avl_tree_type_name##AvlTree* tree) { \
        avl_tree_type_name##AvlBsTreeInit(&tree->bs_tree); \
    } \
    /*
    * 从树中查找节点
    * 存在返回查找到的节点对应的对象，不存在返回NULL
    */ \
    id_type avl_tree_type_name##AvlTreeFind(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, key_type* key) { \
        return avl_tree_type_name##AvlBsTreeFind(&tree->bs_tree, stack, key); \
    } \
    /*
    * 向树中插入节点
    * 覆盖重复key
    */ \
    id_type avl_tree_type_name##AvlTreePut(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type put_entry_id) { \
        id_type old_id = avl_tree_type_name##AvlBsTreePut(&tree->bs_tree, stack, put_entry_id); \
        if (old_id == referencer##_InvalidId) avl_tree_type_name##AvlTreeInsertFixup(tree, stack, put_entry_id); \
        return old_id; \
    } \
    /*
    * 删除树中指定节点
    */ \
    bool avl_tree_type_name##AvlTreeDelete(avl_tree_type_name##AvlTree* tree, avl_tree_type_name##AvlBsStackVector* stack, id_type del_entry_id) { \
        bool is_parent_left; \
        id_type del_min_entry_id = avl_tree_type_name##AvlBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left); \
        if (del_min_entry_id == referencer##_InvalidId) { \
            return false; \
        } \
        if (del_min_entry_id != del_entry_id) { \
            avl_tree_type_name##AvlEntry* del_entry = referencer##_Reference(tree, del_entry_id); \
            avl_tree_type_name##AvlEntry* del_min_entry = referencer##_Reference(tree, del_min_entry_id); \
            /* 需要交换平衡因子 */; \
            int8_t old_bf = accessor##_GetBalanceFactor(tree, del_min_entry); \
            accessor##_SetBalanceFactor(tree, del_min_entry, accessor##_GetBalanceFactor(tree, del_entry)); \
            accessor##_SetBalanceFactor(tree, del_entry, old_bf); \
        } \
        avl_tree_type_name##AvlTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left); \
        return true; \
    } \
    /*
    * 验证avl树性质
    */ \
    static bool avl_tree_type_name##AvlTreeCheckPath(avl_tree_type_name##AvlTree* tree, id_type entry_id, offset_type* cur_height) { \
        if (entry_id == referencer##_InvalidId) { \
            ++*cur_height; \
            return true; \
        } \
        bool correct = false; \
        avl_tree_type_name##AvlEntry* entry = NULL; \
        avl_tree_type_name##AvlEntry* parent = NULL; \
        do { \
            entry = referencer##_Reference(tree, entry_id); \
            ++*cur_height; \
            offset_type left_height = *cur_height, right_height = *cur_height; \
            correct = avl_tree_type_name##AvlTreeCheckPath(tree, accessor##_GetLeft(tree, entry), &left_height) && avl_tree_type_name##AvlTreeCheckPath(tree, accessor##_GetRight(tree, entry), &right_height); \
            correct = correct && (left_height - right_height == accessor##_GetBalanceFactor(tree, entry)); \
            *cur_height = max(left_height, right_height); \
        } while (false); \
        if (entry) referencer##_Dereference(tree, entry); \
        if (parent) referencer##_Dereference(tree, parent); \
        return correct; \
    } \
    bool avl_tree_type_name##AvlTreeVerify(avl_tree_type_name##AvlTree* tree) { \
        avl_tree_type_name##AvlEntry* entry = referencer##_Reference(tree, tree->root); \
        if (!entry) return true; \
        offset_type cur_height = 0; \
        bool correct = avl_tree_type_name##AvlTreeCheckPath(tree, tree->root, &cur_height); \
        referencer##_Dereference(tree, entry); \
        return correct; \
    } \


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_AVL_TREE_H_