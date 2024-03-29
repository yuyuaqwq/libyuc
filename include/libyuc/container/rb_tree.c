#include <libyuc/container/rb_tree.def>

#include <libyuc/container/rb_tree_RbBsTree.cfg>
#include <libyuc/container/bs_tree.c>

#define RbBsTreeStackVectorInit MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorInit)
#define RbBsTreeStackVectorIndex MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorIndex)
#define RbBsTreeStackVectorGetCount MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorGetCount)
#define RbBsTreeStackVectorSetCount MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorSetCount)
#define RbBsTreeStackVectorGetTail MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorGetTail)
#define RbBsTreeStackVectorPopTail MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeStackVectorPopTail)

#define RbBsTreeInit MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeInit)
#define RbBsTreeFind MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeFind)
#define RbBsTreeInsert MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeInsert)
#define RbBsTreePut MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreePut)
#define RbBsTreeDelete MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeDelete)
#define RbBsTreeGetCount MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeGetCount)
#define RbBsTreeIteratorLocate MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeIteratorLocate)
#define RbBsTreeIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeIteratorFirst)
#define RbBsTreeIteratorLast MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeIteratorLast)
#define RbBsTreeIteratorNext MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeIteratorNext)
#define RbBsTreeIteratorPrev MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsTreeIteratorPrev)
#define RbBsRotateLeft MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsRotateLeft)
#define RbBsRotateRight MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbBsRotateRight)

#define GetSiblingEntry MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, GetSiblingEntry)
#define RbTreeInsertFixup MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbTreeInsertFixup)
#define RbTreeDeleteFixup MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbTreeDeleteFixup)
#define RbTreeFindInternal MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbTreeFindInternal)
#define RbTreeDeleteInternal MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbTreeDeleteInternal)
#define RbTreeCheckPath MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbTreeCheckPath)
#define RbRotateLeft MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbRotateLeft)
#define RbRotateRight MAKE_NAME(LIBYUC_CONTAINER_RB_TREE_CLASS_NAME, RbRotateRight)


static LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbRotateRight(RbTree* tree, RbEntry* sub_root_parent, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id sub_root_id, RbEntry* sub_root) {
    return (LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id)RbBsRotateRight(&tree->bs_tree, (RbBsEntry*)sub_root_parent, sub_root_id, (RbBsEntry*)sub_root);
}

static LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbRotateLeft(RbTree* tree, RbEntry* sub_root_parent, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id sub_root_id, RbEntry* sub_root) {
    return (LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id)RbBsRotateLeft(&tree->bs_tree, (RbBsEntry*)sub_root_parent, sub_root_id, (RbBsEntry*)sub_root);
}


/*
* 取兄弟节点
*/
static LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id GetSiblingEntry(RbTree* tree, RbEntry* entry_parent, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id entry_id, RbEntry* entry) {
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id ret;
    if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, entry_parent) == entry_id) {
        ret = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, entry_parent);
    }
    else {
            assert(LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, entry_parent) == entry_id);
        ret = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, entry_parent);
    }
    return ret;
}
   
/*
* 向树中插入节点后的平衡操作
*/
static void RbTreeInsertFixup(RbTree* tree, RbBsTreeStackVector* stack, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id ins_entry_id) {
    RbEntry* ins_entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, ins_entry_id);
    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, ins_entry, kRbBlack);
    if (RbBsTreeStackVectorGetCount(stack) == 0) {
        LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, ins_entry, kRbBlack);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, ins_entry);
        return;
    }
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id* cur_id_ptr = RbBsTreeStackVectorPopTail(stack);

    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, ins_entry, kRbRed);
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, ins_entry);
       
    RbEntry* cur = NULL;
    /* 开始回溯维护 */
    while (cur_id_ptr != NULL) {
        cur = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *cur_id_ptr);
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, cur) == kRbBlack) {
            /* 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点 / 3节点的插入，直接合并) */
            break;
        }
        if (RbBsTreeStackVectorGetCount(stack) == 0) {
            /* 没有父节点，回溯到根节点了，直接染黑 */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, cur, kRbBlack);
            break;
        }
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id* parent_id_ptr = RbBsTreeStackVectorPopTail(stack);
        RbEntry* parent = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *parent_id_ptr);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id sibling_id = GetSiblingEntry(tree, parent, *cur_id_ptr, cur);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, parent);
        RbEntry* sibling = NULL;
        if (sibling_id != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
            sibling = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, sibling_id);
        }
        if (sibling && LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling) == kRbRed) {
            /* 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上插入，继续回溯 */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, cur, kRbBlack);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling, kRbBlack);
            ins_entry_id = *parent_id_ptr;         /* 更新为该节点向上插入 */
            ins_entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, ins_entry_id);
            if (RbBsTreeStackVectorGetCount(stack) == 0) {
                LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, ins_entry, kRbBlack);     /* 根节点，染黑并结束 */
                break;
            }
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, ins_entry, kRbRed);
            parent_id_ptr = RbBsTreeStackVectorPopTail(stack);
            /* 将parent视作插入节点 */
        }
        else {
            /* 没有兄弟节点或兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
                *        10b                                5b    
                *     5r     20b    ->     !2r         10r    
                * !2r                                                 20b
            */
              assert(sibling_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId || LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling) == kRbBlack);
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id new_sub_root_id;
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id old_sub_root_id = *parent_id_ptr;
            RbEntry* old_sub_root = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, old_sub_root_id);
            RbEntry* new_sub_root_parent = NULL;
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id* new_sub_root_parent_id_ptr = NULL;
            if (RbBsTreeStackVectorGetCount(stack) > 0) {
                new_sub_root_parent_id_ptr = RbBsTreeStackVectorGetTail(stack);
                new_sub_root_parent = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *new_sub_root_parent_id_ptr);
            }
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, old_sub_root) == *cur_id_ptr) {
                if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, cur) == ins_entry_id) {
                    RbRotateLeft(tree, old_sub_root, *cur_id_ptr, cur);
                }
                new_sub_root_id = RbRotateRight(tree, new_sub_root_parent, old_sub_root_id, old_sub_root);
            }
            else {
                if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, cur) == ins_entry_id) {
                    RbRotateRight(tree, old_sub_root, *cur_id_ptr, cur);
                }
                new_sub_root_id = RbRotateLeft(tree, new_sub_root_parent, old_sub_root_id, old_sub_root);
            }
            if (new_sub_root_parent) LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, new_sub_root_parent);
            RbEntry* new_sub_root = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, new_sub_root_id);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, new_sub_root, kRbBlack);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, old_sub_root, kRbRed);
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, new_sub_root);
               
            if (tree->root == old_sub_root_id) {        /* 原来的子树根节点可能是整棵树的根节点，因此要检查更新 */
                tree->root = new_sub_root_id;
            }
            break;        /* 只是并入，未分裂，向上没有改变颜色，不再需要回溯 */
        }
        if (sibling) LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling);

        cur_id_ptr = parent_id_ptr;
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, cur);
    }
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, cur);
}
/*
* 向树中删除节点后的平衡操作
*/
static void RbTreeDeleteFixup(RbTree* tree, RbBsTreeStackVector* stack, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id del_entry_id, bool is_parent_left) {
    RbEntry* del_entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id* parent_id_ptr = NULL;
       
    RbColor del_color = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, del_entry);
    if (del_color == kRbRed) { /* 是红色的，是3/4节点，因为此时一定是叶子节点(红节点不可能只有一个子节点)，直接移除 */ }
    /* 是黑色的，但是有一个子节点，说明是3节点，变为2节点即可 */
    else if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, del_entry) != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        RbEntry* del_entry_left = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, del_entry));
        LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, del_entry_left, kRbBlack);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, del_entry_left);
    }
    else if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, del_entry) != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        RbEntry* del_entry_right = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, del_entry));
        LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, del_entry_right, kRbBlack);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, del_entry_right);
    }
    else { 
        if (RbBsTreeStackVectorGetCount(stack) > 0) {
            parent_id_ptr = RbBsTreeStackVectorPopTail(stack);
        }
    }
       
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id new_sub_root_id;
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id* grandpa_id_ptr = NULL;
    /* 回溯维护删除黑色节点，即没有子节点(2节点)的情况 */
    RbEntry* parent = NULL, * sibling = NULL, * grandpa = NULL;
    if (parent_id_ptr) parent = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *parent_id_ptr);
    while (parent) {
        if (RbBsTreeStackVectorGetCount(stack) > 0) {
            grandpa_id_ptr = RbBsTreeStackVectorPopTail(stack);
            grandpa = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *grandpa_id_ptr);
        }
        else {
            grandpa = NULL;
        }
        /* 黑色节点一定有兄弟节点 */
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id sibling_id = is_parent_left ? LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, parent) : LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, parent);
        sibling = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, sibling_id);
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling) == kRbRed) {
            /* 兄弟节点为红，说明兄弟节点与父节点形成3节点，真正的兄弟节点应该是红兄弟节点的子节点
                旋转，此时只是使得兄弟节点和父节点形成的3节点红色链接位置调换，当前节点的兄弟节点变为原兄弟节点的子节点 */
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id old_sub_root_id = *parent_id_ptr;
            RbEntry* old_sub_root = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, old_sub_root_id);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, old_sub_root, kRbRed);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling, kRbBlack);
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling);
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, old_sub_root) == sibling_id) {
                new_sub_root_id = RbRotateRight(tree, grandpa, old_sub_root_id, old_sub_root);
                sibling_id = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, old_sub_root);     /* 下降后挂接过来的节点 */
            }
            else {
                new_sub_root_id = RbRotateLeft(tree, grandpa, old_sub_root_id, old_sub_root);
                sibling_id = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, old_sub_root);     /* 下降后挂接过来的节点 */
            }
              assert(sibling_id != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId);
            sibling = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, sibling_id);
            if (tree->root == old_sub_root_id) {
                tree->root = new_sub_root_id;
            }
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, old_sub_root);
            /* grandpa变为新根节点 */
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, grandpa);
            grandpa_id_ptr = &new_sub_root_id;
            grandpa = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, *grandpa_id_ptr);
        }
           
        /* 至此兄弟节点一定为黑 */
           
        /* 侄子节点为红，即兄弟节点是3 / 4节点的情况，向兄弟借节点(上升兄弟节点，下降父亲节点) */
        bool nephew_red = false;
        RbEntry* sibling_right = NULL, * sibling_left = NULL;
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, sibling) != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
            sibling_right = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, sibling));
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling_right) == kRbRed) {
                nephew_red = true;
            }
        }
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, sibling) != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
            sibling_left = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, sibling));
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling_left) == kRbRed) {
                nephew_red = true;
            }
        }
        if (nephew_red) {
            RbColor parent_color = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, parent);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, parent, kRbBlack);
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id old_sub_root_id = *parent_id_ptr;
            RbEntry* new_sub_root_parent = NULL;
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, parent) == sibling_id) {
                if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, sibling) == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId || LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling_left) == kRbBlack) {
                    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling_right, kRbBlack);
                    sibling_id = RbRotateLeft(tree, parent, sibling_id, sibling);
                }
                else {
                    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling_left, kRbBlack);
                }
                new_sub_root_id = RbRotateRight(tree, grandpa, *parent_id_ptr, parent);
            }
            else {
                if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, sibling) == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId || LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, sibling_right) == kRbBlack) {
                    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling_left, kRbBlack);
                    sibling_id = RbRotateRight(tree, parent, sibling_id, sibling);
                }
                else {
                    LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling_right, kRbBlack);
                }
                new_sub_root_id = RbRotateLeft(tree, grandpa, *parent_id_ptr, parent);
            }
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling);
            sibling = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, sibling_id);
            /* 该节点会接替原先的子根节点，也要接替颜色 */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling, parent_color);
            if (tree->root == old_sub_root_id) {
                tree->root = new_sub_root_id;
            }
        }
        if (sibling_right) {
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling_right);
        }
        if (sibling_left) {
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling_left);
        }
        if (nephew_red) {
            break;
        }
           
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, parent) == kRbRed) {
            /* 父节点为红，即父节点是3/4节点，分裂下降与兄弟节点合并
                |5|8|                 |5|
                    /    |        ->        /    
                3     6    -9-            3     |6|8| */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling, kRbRed);
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, parent, kRbBlack);
            break;
        }
        else {
            /* 父节点为黑，即父节点是2节点，兄弟节点也是2节点，下降父节点与兄弟节点合并，相当于向上删除父节点，继续回溯
                为什么不是3/4节点？因为黑父节点如果是3，兄弟节点是红，4的话回溯时父节点是红 */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, sibling, kRbRed);
        }
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id child_id = *parent_id_ptr;
        parent_id_ptr = grandpa_id_ptr;
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, parent);
        parent = grandpa;
        if (parent != NULL) {
            is_parent_left = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, parent) == child_id;
        }
    }
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, sibling);
       
    if (tree->root != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        RbEntry* root = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, tree->root);
        if (root && LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, root) == kRbRed) {
            /* 根节点染黑 */
            LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, root, kRbBlack);
        }
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, root);
    }
}
/*
* 初始化树
*/
void RbTreeInit(RbTree* tree) {
    RbBsTreeInit(&tree->bs_tree);
}
/*
* 从树中查找节点
* 存在返回查找到的节点对应的对象，不存在返回NULL/最后一次查找的节点
*/
static LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeFindInternal(RbTree* tree, RbBsTreeStackVector* stack, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key) {
    return RbBsTreeFind(&tree->bs_tree, stack, key);
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeFind(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key) {
    RbBsTreeStackVector stack;
    RbBsTreeStackVectorInit(&stack); 
    return RbTreeFindInternal(tree, &stack, key);
}
/*
* 向树中插入节点
* 允许重复key，但插入同一个节点时返回false
*/
bool RbTreeInsert(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id insert_entry_id) {
    RbBsTreeStackVector stack;
    RbBsTreeStackVectorInit(&stack);
    if(!RbBsTreeInsert(&tree->bs_tree, &stack, insert_entry_id)) return false;
    RbTreeInsertFixup(tree, &stack, insert_entry_id);
    return true;
}
/*
* 向树中推入节点
* 允许覆盖重复key，返回被覆盖的原entry，否则InvalidId，如果put_entry_id已经被插入过了，也会被返回(返回值 == put_entry_id)
*/
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreePut(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id put_entry_id) {
    RbBsTreeStackVector stack;
    RbBsTreeStackVectorInit(&stack);
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id old_id = RbBsTreePut(&tree->bs_tree, &stack, put_entry_id);
    if (old_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) RbTreeInsertFixup(tree, &stack, put_entry_id);
    return old_id;
}
/*
* 删除树中指定节点
*/
static bool RbTreeDeleteInternal(RbTree* tree, RbBsTreeStackVector* stack, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id del_entry_id) {
      assert(del_entry_id != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId);
    bool is_parent_left;
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id del_min_entry_id = RbBsTreeDelete(&tree->bs_tree, stack, del_entry_id, &is_parent_left);
    if (del_min_entry_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        return false;
    }
    if (del_min_entry_id != del_entry_id) {
        RbEntry* del_entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, del_entry_id);
        RbEntry* del_min_entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, del_min_entry_id);
        /* 需要交换颜色 */;
        RbColor old_color = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, del_min_entry);
        LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, del_min_entry, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, del_entry));
        LIBYUC_CONTAINER_RB_TREE_ACCESSOR_SetColor(tree, del_entry, old_color);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, del_min_entry);
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, del_entry);
    }
    RbTreeDeleteFixup(tree, stack, del_entry_id, is_parent_left);
    return true;
}
bool RbTreeDelete(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key) {
    RbBsTreeStackVector stack;
    RbBsTreeStackVectorInit(&stack);
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id del_entry_id = RbTreeFindInternal(tree, &stack, key);
    if (del_entry_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) return false;
    return RbTreeDeleteInternal(tree, &stack, del_entry_id);
}
bool RbTreeDeleteByIterator(RbTree* tree, RbTreeIterator* iterator) {
    if (iterator->cur_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) return false;
    bool success = RbTreeDeleteInternal(tree, &iterator->stack, iterator->cur_id);
    iterator->cur_id = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId;
    return success;
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset RbTreeGetCount(RbTree* tree) {
    RbBsTreeStackVector stack;
    RbBsTreeStackVectorInit(&stack);
    return RbBsTreeGetCount((RbBsTree*)tree, &stack);
}
/*
* 迭代器相关
*/
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorLocate(RbTree* tree, RbTreeIterator* iterator, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key, LIBYUC_CONTAINER_RB_TREE_COMPARER_Type_Diff* cmp_diff) {
    RbBsTreeStackVectorInit(&iterator->stack);
    iterator->cur_id = RbBsTreeIteratorLocate((RbBsTree*)tree, &iterator->stack, key, cmp_diff);
    return iterator->cur_id;
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorFirst(RbTree* tree, RbTreeIterator* iterator) {
    RbBsTreeStackVectorInit(&iterator->stack);
    iterator->cur_id = RbBsTreeIteratorFirst((RbBsTree*)tree, &iterator->stack);
    return iterator->cur_id;
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorLast(RbTree* tree, RbTreeIterator* iterator) {
    RbBsTreeStackVectorInit(&iterator->stack);
    iterator->cur_id = RbBsTreeIteratorLast((RbBsTree*)tree, &iterator->stack);
    return iterator->cur_id;
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorNext(RbTree* tree, RbTreeIterator* iterator) {
    if (iterator->cur_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        RbTreeIteratorFirst(tree, iterator);
        return iterator->cur_id;
    }
    iterator->cur_id = RbBsTreeIteratorNext((RbBsTree*)tree, &iterator->stack, iterator->cur_id);
    return iterator->cur_id;
}
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorPrev(RbTree* tree, RbTreeIterator* iterator) {
    if (iterator->cur_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        RbTreeIteratorLast(tree, iterator);
        return iterator->cur_id;
    }
    iterator->cur_id = RbBsTreeIteratorPrev((RbBsTree*)tree, &iterator->stack, iterator->cur_id);
    return iterator->cur_id;
}
void RbTreeIteratorCopy(RbTreeIterator* dst_iterator, const RbTreeIterator* src_iterator) {
    dst_iterator->cur_id = src_iterator->cur_id;
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset count = RbBsTreeStackVectorGetCount((RbBsTreeStackVector*)&src_iterator->stack);
    RbBsTreeStackVectorSetCount(&dst_iterator->stack, count);
    for (LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset i = 0; i < count; i++) {
        *RbBsTreeStackVectorIndex(&dst_iterator->stack, i) = *RbBsTreeStackVectorIndex((RbBsTreeStackVector*)&src_iterator->stack, i);
    }
}
/*
* 验证红黑树性质
*/
static bool RbTreeCheckPath(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id parent_id, 
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset cur_high, 
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset max_high) {
    if (entry_id == LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
        return cur_high == max_high;
    }
    bool correct = false;
    RbEntry* entry = NULL;
    RbEntry* parent = NULL;
    do {
        entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, entry_id);
        if (parent_id != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
            parent = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, parent_id);
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, entry) == kRbRed && LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, parent) == kRbRed) {
                break;
            }
        }
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, entry) == kRbBlack) { cur_high++; }
        correct = RbTreeCheckPath(tree, entry_id, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, entry), cur_high, max_high) && RbTreeCheckPath(tree, entry_id, LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetRight(tree, entry), cur_high, max_high);
    } while (false);
    if (entry) LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, entry);
    if (parent) LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, parent);
    return correct;
}
bool RbTreeVerify(RbTree* tree) {
    RbEntry* entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, tree->root);
    if (!entry) return true;
    bool correct = false;
    do {
        if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, entry) != kRbBlack) break;
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset high = 1;
        while (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, entry) != LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId) {
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id entry_id = LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetLeft(tree, entry);
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, entry);
            entry = LIBYUC_CONTAINER_RB_TREE_REFERENCER_Reference(tree, entry_id);
            /* 红色节点不表示高度 */
            if (LIBYUC_CONTAINER_RB_TREE_ACCESSOR_GetColor(tree, entry) == kRbBlack) {
                high++;
            }
        }
        correct = RbTreeCheckPath(tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Const_InvalidId, tree->root, 0, high);
    } while (false);
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Dereference(tree, entry);
    return correct;
}

#undef RbBsTreeStackVectorInit
#undef RbBsTreeStackVectorIndex
#undef RbBsTreeStackVectorGetCount
#undef RbBsTreeStackVectorSetCount
#undef RbBsTreeStackVectorGetTail
#undef RbBsTreeStackVectorPopTail

#undef RbBsTreeInit
#undef RbBsTreeFind
#undef RbBsTreeInsert
#undef RbBsTreePut
#undef RbBsTreeDelete
#undef RbBsTreeGetCount
#undef RbBsTreeIteratorLocate
#undef RbBsTreeIteratorFirst
#undef RbBsTreeIteratorLast
#undef RbBsTreeIteratorNext
#undef RbBsTreeIteratorPrev
#undef RbBsRotateLeft
#undef RbBsRotateLeft

#undef GetSiblingEntry
#undef RbTreeInsertFixup
#undef RbTreeDeleteFixup
#undef RbTreeFindInternal
#undef RbTreeDeleteInternal
#undef RbTreeCheckPath
#undef RbRotateLefta
#undef RbRotateRight


#include <libyuc/container/rb_tree.undef>