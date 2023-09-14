/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME
#define LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_Type_Key
#define LIBYUC_CONTAINER_AVL_TREE_Type_Key int
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey(main_obj, obj)
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(main_obj, obj) (((uintptr_t)(obj)->left & 0x3) == 3 ? -1 : (uintptr_t)(obj)->left & 0x3)
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(main_obj, obj) ((LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id)((uintptr_t)(obj)->left & (~(uintptr_t)0x3)))
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight(main_obj, obj) ((obj)->right)
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(main_obj, obj, new_bf) ((obj)->left = (LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id)(((uintptr_t)(LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft((main_obj), (obj))) | (uintptr_t)(new_bf  & 0x3))))
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetLeft
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetLeft(main_obj, obj, new_left) ((obj)->left = (LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id)((uintptr_t)new_left | (uintptr_t)(LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor((main_obj), (obj)) & 0x3)))
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetRight
#define LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetRight(main_obj, obj, new_right) ((obj)->right = new_right)
#endif


#ifndef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id struct AvlEntry*
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset size_t
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId NULL
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference
#define LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(main_obj, reference)
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_COMPARER_Less
#define LIBYUC_CONTAINER_AVL_TREE_COMPARER_Less(main_obj, obj1, obj2) (*(obj1) < *(obj2))
#endif

#ifndef LIBYUC_CONTAINER_AVL_TREE_COMPARER_Greater
#define LIBYUC_CONTAINER_AVL_TREE_COMPARER_Greater(main_obj, obj1, obj2) (*(obj1) > *(obj2))
#endif

/*
* 嵌入平衡因子的Avl树
*/

//LIBYUC_CONTAINER_BS_TREE_DECLARATION(
//    Avl,
//    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id,
//    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset,
//    LIBYUC_CONTAINER_AVL_TREE_Type_Key,
//    sizeof(LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset) * 8 * 2 
//    )


/* 1.44 * log(n + 2) - 1.328 */
#define LIBYUC_CONTAINER_BS_TREE_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, Avl)
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Const_InvalidId LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Reference LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference
#define LIBYUC_CONTAINER_BS_TREE_REFERENCER_Dereference LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetKey LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetRight LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_GetLeft LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetRight LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetRight
#define LIBYUC_CONTAINER_BS_TREE_ACCESSOR_SetLeft LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetLeft
#define LIBYUC_CONTAINER_BS_TREE_COMPARER_Less LIBYUC_CONTAINER_AVL_TREE_COMPARER_Less
#define LIBYUC_CONTAINER_BS_TREE_COMPARER_Greater LIBYUC_CONTAINER_AVL_TREE_COMPARER_Greater
#include <libyuc/container/bs_tree.h>


#define AvlBsTree MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTree)
#define AvlBsEntry MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsEntry)

#define AvlBsTreeStackVector MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVector)


#define AvlTree MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTree)
#define AvlEntry MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlEntry)
typedef struct AvlEntry {
    union {
        struct {
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id left;
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id right;
        };
        AvlBsEntry bs_entry;
    };
} AvlEntry;
typedef struct AvlTree {
    union {
        AvlBsTree bs_tree;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id root;
    };
} AvlTree;



#define AvlTreeInit MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeInit)
#define AvlTreeFind MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeFind)
#define AvlTreePut MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreePut)
#define AvlTreeDelete MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeDelete)
#define AvlTreeVerify MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeVerify)


void AvlTreeInit(AvlTree* tree);
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreeFind(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_Type_Key* key);
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreePut(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id put_entry_id);
bool AvlTreeDelete(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id del_entry_id);
bool AvlTreeVerify(AvlTree* tree);

// 访问器需要提供_GetKey、_Set/GetParent、_Set/GetBalanceFactor方法


#ifdef LIBYUC_TEMPLATE_C_INCLUDE
#define AvlBsTreeStackVectorInit MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorInit)
#define AvlBsTreeStackVectorIndex MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorIndex)
#define AvlBsTreeStackVectorGetCount MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorGetCount)
#define AvlBsTreeStackVectorSetCount MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorSetCount)
#define AvlBsTreeStackVectorGetTail MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorGetTail)
#define AvlBsTreeStackVectorPopTail MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeStackVectorPopTail)

#define AvlBsTreeInit MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeInit)
#define AvlBsTreeFind MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeFind)
#define AvlBsTreeInsert MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeInsert)
#define AvlBsTreePut MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreePut)
#define AvlBsTreeDelete MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeDelete)
#define AvlBsTreeGetCount MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeGetCount)
#define AvlBsTreeIteratorLocate MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeIteratorLocate)
#define AvlBsTreeIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeIteratorFirst)
#define AvlBsTreeIteratorLast MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeIteratorLast)
#define AvlBsTreeIteratorNext MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeIteratorNext)
#define AvlBsTreeIteratorPrev MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlBsTreeIteratorPrev)
#define AvlRotateLeft MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlRotateLeft)
#define AvlRotateRight MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlRotateRight)

#define RotateByBalanceFactor MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, RotateByBalanceFactor)
#define AvlTreeInsertFixup MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeInsertFixup)
#define AvlTreeDeleteFixup MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeDeleteFixup)
#define AvlTreeCheckPath MAKE_NAME(LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME, AvlTreeCheckPath)

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
*/
/*
* 根据平衡因子来旋转子树
* 并负责更新平衡因子
* 子树高度变化返回true，高度未变化返回false
*/
static bool RotateByBalanceFactor(AvlTree* tree, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id parent_id, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id* sub_root_id_io, AvlEntry** sub_root_io, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id deep_child_id, AvlEntry* deep_child, int8_t cur_bf) {
    bool rotate = false;
    bool height_update = true;
    AvlEntry* new_sub_root_id = NULL;
    AvlEntry* sub_root = *sub_root_io;
    AvlEntry* parent = parent_id != LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId ? LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, parent_id) : NULL;
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id sub_root_id = *sub_root_id_io;
        assert(cur_bf >= -2 || cur_bf <= 2);
    if (cur_bf == 2) {
        /* 是失衡节点(最小失衡子树的根节点)，左子树高度高于右子树高度 */
        /* AvlEntry* deep_child = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, sub_root); */
        int8_t deep_child_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, deep_child);
        /* 判断需要单旋还是双旋 */
        if (deep_child_bf == -1) {
                /* 失衡节点的左子树的右子树更深，先对失衡节点的左子树左旋，再对失衡节点右旋
                    o
                o
                    o
                需要先左旋转 */
            AvlEntry* deep_child_right = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight(tree, deep_child));
                assert(deep_child_right);
            int8_t right_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, deep_child_right);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child_right, 0);
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, tree, deep_child_right);
            AvlRotateLeft(tree, sub_root, deep_child_id, deep_child);
            new_sub_root_id = AvlRotateRight(tree, parent, sub_root_id, sub_root);
               
            if (right_bf == -1) {
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, 1);
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
            }
            else {
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, 0);
                if (right_bf == 1) {
                    LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, -1);
                } else {
                    LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
                }
            }
        }
        else if (deep_child_bf == 0) {
                /* 删除节点时可能出现的情况，即右子树的右子树高度低于左子树的左子树1，旋转过来并不完美平衡
                    o
                    o
                o     o

                        o4, 2
                o3, 0         o2(1) */
            height_update = false;         /* 这种情况的旋转不会改变该子树的高度 */
            new_sub_root_id = AvlRotateRight(tree, parent, sub_root_id, sub_root);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, -1);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 1);
        }
        else {
            /* 此时失衡节点的左子树的左子树更深，右旋即可
                可能失衡节点与左节点交换位置，需要保存结果，如果是失衡节点是根节点再返回新的根节点 */
            new_sub_root_id = AvlRotateRight(tree, parent, sub_root_id, sub_root);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child , 0);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
        }
        rotate = true;
    }
    else if (cur_bf == -2) {
        /* 是失衡节点，右子树高度高于左子树高度 */
        /* AvlEntry* deep_child = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight(tree, sub_root) */;
        int8_t deep_child_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, deep_child);
        /* 判断需要单旋还是双旋 */
        if (deep_child_bf == 1) {
                /* 失衡节点的右子树的左子树更深，先对失衡节点的右子树右旋，再对失衡节点左旋
                o
                    o
                    o
                需要先右旋转 */
            AvlEntry* deep_child_left = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, deep_child));
                assert(deep_child_left);
            int8_t left_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, deep_child_left);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child_left, 0);
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, deep_child_left);
            AvlRotateRight(tree, sub_root, deep_child_id, deep_child);
            new_sub_root_id = AvlRotateLeft(tree, parent, sub_root_id, sub_root);
               
            if (left_bf == 1) {
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, -1);
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
            }
            else {
                LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, 0);
                if (left_bf == -1) {
                    LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 1);
                } else {
                    LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
                }
            }
        }
        else if (deep_child_bf == 0) {
                /* 删除节点时可能出现的情况，即左子树的左子树高度低于右子树的右子树1，旋转过来并不完美平衡
                o
                    o
                    o     o

                        o4, -2
                o2(1)         o3, 0 */
            height_update = false;         /* 这种情况的旋转不会改变该子树的高度 */
            new_sub_root_id = AvlRotateLeft(tree, parent, sub_root_id, sub_root);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, 1);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, -1);
        }
        else {        /* child_bf == -1 */
            /* 此时失衡节点的右子树的右子树更深，左旋即可 */
            new_sub_root_id = AvlRotateLeft(tree, parent, sub_root_id, sub_root);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, deep_child, 0);
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, 0);
        }
        rotate = true;
    }
    else {
        height_update = false;
        LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, sub_root, cur_bf);
    }
    if (rotate) {
        if (tree->root == sub_root) {        /* 原来的子树根节点可能是整棵树的根节点，因此要检查更新 */
            tree->root = new_sub_root_id;
        }
        *sub_root_id_io = new_sub_root_id;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, sub_root);
        *sub_root_io = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, new_sub_root_id);
    }
    if (parent) LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, parent);
    return height_update;
}
/*
* 向树中插入节点的平衡操作
*/
static void AvlTreeInsertFixup(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id ins_entry_id) {
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id* cur_id_ptr = AvlBsTreeStackVectorPopTail(stack);
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id cur_id = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId;
    if (cur_id_ptr) cur_id = *cur_id_ptr;
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id child_id = ins_entry_id;
    AvlEntry* cur = NULL;
    AvlEntry* child = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, child_id);
    LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, child, 0);
    /* 插入节点后平衡因子可能发生变化，回溯维护平衡因子 */
    while (cur_id != LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId) {
        cur = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, cur_id);
        child = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, child_id);
        int8_t cur_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, cur);
        if (child_id == LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, cur)) cur_bf++;        /* 新节点插入到当前节点的左子树 */
        else cur_bf--;         /* 新节点插入到当前节点的右子树 */
           
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id* parent_id_ptr = AvlBsTreeStackVectorPopTail(stack) ;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id parent_id = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId;
        if (parent_id_ptr) parent_id = *parent_id_ptr;
        if (RotateByBalanceFactor(tree, parent_id, &cur_id, &cur, child_id, child, cur_bf) || cur_bf == 0) {
            /* 旋转后当前节点高度不变，或原先高度就不变，停止回溯 */
            break;
        }
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, child);
        child_id = cur_id;
        child = cur;
        cur = NULL;
        cur_id = parent_id;
    }
    if (cur) LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, cur);
    if (child) LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, child);
}
/*
* 从树中删除节点的平衡操作
*/
static void AvlTreeDeleteFixup(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id del_entry_id, bool is_parent_left) {
    /* 删除节点后节点平衡因子可能发生变化，回溯维护节点平衡因子 */
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id* cur_id_ptr = AvlBsTreeStackVectorPopTail(stack);
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id cur_id = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId;
    if (cur_id_ptr) cur_id = *cur_id_ptr;
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id child_id = del_entry_id;
    AvlEntry* cur = NULL;
    while (cur_id != LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId) {
        cur = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, cur_id);
        int8_t cur_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, cur);
        if (is_parent_left) { cur_bf--; }
        else { cur_bf++; }
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id* parent_id_ptr = AvlBsTreeStackVectorPopTail(stack) ;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id parent_id = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId;
        if (parent_id_ptr) parent_id = *parent_id_ptr;
        if (cur_bf != 0) {
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id deep_child_id = is_parent_left ? LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight(tree, cur) : LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, cur);
            AvlEntry* deep_child = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, deep_child_id);
            if (RotateByBalanceFactor(tree, parent_id, &cur_id, &cur, deep_child_id, deep_child, cur_bf) == false) {
                /* 另一侧高度相等或更深且无需旋转，则当前节点高度不变 */
                LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, deep_child);
                break;
            }
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, deep_child);
        }
        else {
            LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, cur, cur_bf);
        }
        child_id = cur_id;
        cur_id = parent_id;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, cur);
        cur = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, cur_id);
        if (cur) {
            is_parent_left = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, cur) == child_id;
        }
    }
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, cur);
}
/*
* 初始化树
*/
void AvlTreeInit(AvlTree* tree) {
    AvlBsTreeInit(&tree->bs_tree);
}
/*
* 从树中查找节点
* 存在返回查找到的节点对应的对象，不存在返回NULL
*/
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreeFind(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_Type_Key* key) {
    return AvlBsTreeFind(&tree->bs_tree, stack, key);
}
/*
* 向树中插入节点
* 覆盖重复key
*/
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreePut(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id put_entry_id) {
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id old_id = AvlBsTreePut(&tree->bs_tree, stack, put_entry_id);
    if (old_id == LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId) AvlTreeInsertFixup(tree, stack, put_entry_id);
    return old_id;
}
/*
* 删除树中指定节点
*/
bool AvlTreeDelete(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id del_entry_id) {
    bool is_parent_left;
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id del_min_entry_id = AvlBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left);
    if (del_min_entry_id == LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId) {
        return false;
    }
    if (del_min_entry_id != del_entry_id) {
        AvlEntry* del_entry = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, del_entry_id);
        AvlEntry* del_min_entry = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, del_min_entry_id);
        /* 需要交换平衡因子 */;
        int8_t old_bf = LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, del_min_entry);
        LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, del_min_entry, LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, del_entry));
        LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor(tree, del_entry, old_bf);
    }
    AvlTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left);
    return true;
}
/*
* 验证avl树性质
*/
static bool AvlTreeCheckPath(AvlTree* tree, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset* cur_height) {
    if (entry_id == LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId) {
        ++*cur_height;
        return true;
    }
    bool correct = false;
    AvlEntry* entry = NULL;
    AvlEntry* parent = NULL;
    do {
        entry = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, entry_id);
        ++*cur_height;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset left_height = *cur_height, right_height = *cur_height;
        correct = AvlTreeCheckPath(tree, LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft(tree, entry), &left_height) && AvlTreeCheckPath(tree, LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight(tree, entry), &right_height);
        correct = correct && (left_height - right_height == LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor(tree, entry));
        *cur_height = max(left_height, right_height);
    } while (false);
    if (entry) LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, entry);
    if (parent) LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, parent);
    return correct;
}
bool AvlTreeVerify(AvlTree* tree) {
    AvlEntry* entry = LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference(tree, tree->root);
    if (!entry) return true;
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset cur_height = 0;
    bool correct = AvlTreeCheckPath(tree, tree->root, &cur_height);
    LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference(tree, entry);
    return correct;
}


#undef AvlBsTreeStackVectorInit
#undef AvlBsTreeStackVectorIndex
#undef AvlBsTreeStackVectorGetCount
#undef AvlBsTreeStackVectorSetCount
#undef AvlBsTreeStackVectorGetTail
#undef AvlBsTreeStackVectorPopTail

#undef AvlBsTreeInit
#undef AvlBsTreeFind
#undef AvlBsTreeInsert
#undef AvlBsTreePut
#undef AvlBsTreeDelete
#undef AvlBsTreeGetCount
#undef AvlBsTreeIteratorLocate
#undef AvlBsTreeIteratorFirst
#undef AvlBsTreeIteratorLast
#undef AvlBsTreeIteratorNext
#undef AvlBsTreeIteratorPrev
#undef AvlRotateLeft
#undef AvlRotateRight

#define RotateByBalanceFactor
#define AvlTreeInsertFixup
#define AvlTreeDeleteFixup
#define AvlTreeCheckPath
#endif // LIBYUC_TEMPLATE_C_INCLUDE
#undef AvlBsTree
#undef AvlBsEntry
#undef AvlBsTreeStackVector

#undef AvlEntry
#undef AvlTree


#undef AvlTreeInit
#undef AvlTreeFind
#undef AvlTreePut
#undef AvlTreeDelete
#undef AvlTreeVerify


#undef LIBYUC_CONTAINER_AVL_TREE_CLASS_NAME
#undef LIBYUC_CONTAINER_AVL_TREE_Type_Key
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetKey
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetBalanceFactor
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetLeft
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_GetRight
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetBalanceFactor
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetLeft
#undef LIBYUC_CONTAINER_AVL_TREE_ACCESSOR_SetRight
#undef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id
#undef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Offset
#undef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Const_InvalidId
#undef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Reference
#undef LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Dereference
#undef LIBYUC_CONTAINER_AVL_TREE_COMPARER_Less
#undef LIBYUC_CONTAINER_AVL_TREE_COMPARER_Greater


#ifdef __cplusplus
}
#endif
