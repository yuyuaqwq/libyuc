/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_RB_TREE_H_
#define CUTILS_CONTAINER_RB_TREE_H_

#include <CUtils/object.h>
#include <CUtils/container/bs_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 以234树为原型的红黑树实现
*/
//typedef enum _RbColor RbColor;
//
//typedef struct _RbEntry {
//    union {
//        struct {
//            struct _RbEntry* parent_color;      // 颜色嵌入到父节点指针的低1位
//            struct _RbEntry* left;
//            struct _RbEntry* right;
//        };
//        BsEntry bse;
//    };
//} RbEntry;
//
//typedef struct _RbTree {
//    union {
//        struct {
//            RbEntry* root;
//            int entry_field_offset;
//            int key_field_offset;
//            int key_field_size;
//            CmpFunc cmp_func;        // 间接调用增加一定开销
//        };
//        BsTree bst;
//    };
//} RbTree;
//
//typedef struct _RbTreeIterator {
//    RbTree* rb_tree;
//    RbEntry* cur_entry;
//} RbTreeIterator;
//
//RbColor RbEntryGetColor(RbEntry* entry);
//void RbEntrySetColor(RbEntry* entry, RbColor color);
//RbEntry* RbEntryGetParent(RbEntry* entry);
//void RbEntrySetParent(RbEntry* entry, RbEntry* parent);
//
//void RbTreeInsertEntryFixup(RbTree* tree, RbEntry* entry);
//bool RbTreeInsertEntryByKey(RbTree* tree, RbEntry* entry);
//
//void RbTreeInit(RbTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
//#define RbTreeInitByField(tree, objName, entryFieldName, keyFieldName) RbTreeInit((tree), ObjectGetFieldOffset(objName, entryFieldName), ObjectGetFieldOffset(objName, keyFieldName), ObjectGetFieldSize(objName, keyFieldName), NULL)
//void* RbTreeFindEntryByKey(RbTree* tree, void* key);
//void RbTreeDeleteEntryFixup(RbTree* tree, RbEntry* entry, RbEntry* parent, RbEntry* left, RbEntry* right, RbColor color, bool isLeft);
//void RbTreeDeleteEntry(RbTree* tree, RbEntry* entry);
//void* RbTreeDeleteEntryByKey(RbTree* tree, void* key);
//
//void* RbTreeFirst(RbTree* tree, RbTreeIterator* iterator);
//void* RbTreeLast(RbTree* tree, RbTreeIterator* iterator);
//void* RbTreeNext(RbTreeIterator* iterator);
//void* RbTreePrev(RbTreeIterator* iterator);


#define CUTILS_CONTAINER_RB_TREE_DECLARATION(rb_tree_type_name, id_type, key_type) \
    CUTILS_CONTAINER_BS_TREE_DECLARATION(rb_tree_type_name, id_type, key_type) \
    typedef struct _##rb_tree_type_name##RbEntry { \
        union { \
            struct { \
                id_type parent_color; \
                id_type left; \
                id_type right; \
            }; \
            rb_tree_type_name##BsEntry bs_entry; \
        }; \
    } rb_tree_type_name##RbEntry; \
    typedef struct _##rb_tree_type_name##RbTree { \
        union { \
            rb_tree_type_name##BsTree bs_tree; \
            id_type root; \
        }; \
    } rb_tree_type_name##RbTree; \
    \
    void rb_tree_type_name##RbTreeInit(rb_tree_type_name##RbTree* tree); \
    id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key); \
    bool rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id); \

// 访问器需要提供_GetKey方法
#define CUTILS_CONTAINER_RB_TREE_DEFINE(rb_tree_type_name, id_type, key_type, referencer, accessor, comparer) \
    static inline id_type accessor##_GetParent(rb_tree_type_name##RbEntry* entry) { \
        return (id_type)(((uintptr_t)(entry->parent_color) & (~((uintptr_t)0x1)))); \
    } \
    static inline RbColor accessor##_GetColor(rb_tree_type_name##RbEntry* entry) { \
        return (RbColor)(((uintptr_t)entry->parent_color) & 0x1); \
    } \
    static inline void accessor##_SetParent(rb_tree_type_name##RbEntry* entry, id_type new_parent_id){ \
        entry->parent_color = (id_type)(((uintptr_t)new_parent_id) | ((uintptr_t)accessor##_GetColor(entry))); \
    } \
    static inline void accessor##_SetColor(rb_tree_type_name##RbEntry* entry, RbColor color) { \
        entry->parent_color = (id_type)(((uintptr_t)accessor##_GetParent(entry)) | ((uintptr_t)color)); \
    } \
    CUTILS_CONTAINER_BS_TREE_DEFINE(rb_tree_type_name, id_type, key_type, referencer, accessor, comparer) \
    /*
    * 左旋子树
    */ \
    static id_type rb_tree_type_name##RotateLeft(id_type sub_root_id) { \
        rb_tree_type_name##RbEntry* sub_root = referencer##_Reference(tree, sub_root_id); \
        id_type new_sub_root_id = sub_root->right; \
        if (new_sub_root_id == referencer##_InvalidId) { \
            referencer##_Dereference(tree, sub_root); \
            return sub_root_id; \
        } \
        rb_tree_type_name##RbEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        accessor##_SetParent(new_sub_root, accessor##_GetParent(sub_root)); \
        if (accessor##_GetParent(sub_root) != referencer##_InvalidId) { \
            if (accessor##_GetParent(sub_root)->left == sub_root_id) { \
                accessor##_GetParent(sub_root)->left = new_sub_root_id; \
            } \
            else { \
                accessor##_GetParent(sub_root)->right = new_sub_root_id; \
            } \
        } \
        accessor##_SetParent(sub_root, new_sub_root_id); \
        \
        sub_root->right = new_sub_root->left; \
        if (sub_root->right != referencer##_InvalidId) { \
            accessor##_SetParent(sub_root->right, sub_root_id); \
        } \
        \
        new_sub_root->left = sub_root_id; \
        \
        referencer##_Dereference(tree, new_sub_root); \
        referencer##_Dereference(tree, sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 右旋子树
    */ \
    static id_type rb_tree_type_name##RotateRight(id_type sub_root_id) { \
        rb_tree_type_name##RbEntry* sub_root = referencer##_Reference(tree, sub_root_id); \
        id_type new_sub_root_id = referencer##_InvalidId; \
        if (new_sub_root_id == referencer##_InvalidId) { \
            referencer##_Dereference(tree, sub_root); \
            return sub_root_id; \
        } \
        rb_tree_type_name##RbEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
        \
        accessor##_SetParent(new_sub_root, accessor##_GetParent(sub_root)); \
        if (accessor##_GetParent(sub_root) != referencer##_InvalidId) { \
           if (accessor##_GetParent(sub_root)->left == sub_root_id) { \
                accessor##_GetParent(sub_root)->left = new_sub_root_id; \
            } \
            else { \
                accessor##_GetParent(sub_root)->right = new_sub_root_id; \
            } \
        } \
        accessor##_SetParent(sub_root, new_sub_root_id); \
        \
        sub_root->left = new_sub_root->right; \
        if (sub_root->left != referencer##_InvalidId) { \
            accessor##_SetParent(sub_root->left, sub_root_id); \
        } \
        \
        new_sub_root->right = sub_root_id; \
        \
        referencer##_Dereference(tree, new_sub_root); \
        referencer##_Dereference(tree, sub_root); \
        return new_sub_root_id; \
    } \
    /*
    * 取兄弟节点
    */ \
    static id_type rb_tree_type_name##GetSiblingEntry(id_type entry_id, rb_tree_type_name##RbEntry* entry) { \
        id_type parent_id = accessor##_GetParent(entry); \
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
        id_type cur_id = accessor##_GetParent(ins_entry); \
        if (cur_id == referencer##_InvalidId) { \
            accessor##_SetColor(ins_entry, kRbBlack); \
            referencer##_Dereference(tree, ins_entry); \
            return; \
        } \
        accessor##_SetColor(ins_entry, kRbRed); \
        referencer##_Dereference(tree, ins_entry); \
        \
        rb_tree_type_name##RbEntry* cur; \
        /* 开始回溯维护 */ \
        while (cur_id != referencer##_InvalidId) { \
            cur = referencer##_Reference(tree, cur_id); \
            if (accessor##_GetColor(cur) == kRbBlack) { \
                /* 当前节点(插入节点的父节点)是黑色，啥都不用做(是2节点 / 3节点的插入，直接合并) */ \
                break; \
            } \
            if (accessor##_GetParent(cur) == referencer##_InvalidId) { \
                /* 没有父节点，回溯到根节点了，直接染黑 */ \
                accessor##_SetColor(cur, kRbBlack); \
                break; \
            } \
            id_type sibling_id = rb_tree_type_name##GetSiblingEntry(cur_id, cur); \
            rb_tree_type_name##RbEntry* sibling = referencer##_Reference(tree, sibling_id); \
            if (sibling_id != referencer##_InvalidId && accessor##_GetColor(sibling) == kRbRed) { \
                /* 兄弟节点是红色，说明是4节点的插入，分裂(红黑树的体现就是变色)，父节点向上插入，继续回溯 */ \
                accessor##_SetColor(cur, kRbBlack); \
                accessor##_SetColor(sibling, kRbBlack); \
                ins_entry_id = accessor##_GetParent(cur);       /* 更新为该节点向上插入 */ \
                ins_entry = referencer##_Reference(tree, ins_entry_id); \
                if (accessor##_GetParent(ins_entry) == referencer##_InvalidId) { \
                    accessor##_SetColor(ins_entry, kRbBlack);     /* 根节点，染黑并结束 */ \
                    break; \
                } \
                accessor##_SetColor(ins_entry, kRbRed); \
                cur = ins_entry; \
            } \
            else { \
                /* 没有兄弟节点或兄弟节点是黑色，说明是3节点的插入，可以并入，但需要利用旋转将其变为4节点
                 *        10b               5b    
                 *     5r     20b  ->   !2r     10r    
                 * !2r                             20b
                */ \
                id_type new_sub_root_id; \
                id_type old_sub_root_id = accessor##_GetParent(cur); \
                rb_tree_type_name##RbEntry* old_sub_root = referencer##_Reference(tree, old_sub_root_id); \
                if (old_sub_root->left == cur_id) { \
                    if (cur->right == ins_entry_id) { \
                        rb_tree_type_name##RotateLeft(cur_id); \
                    } \
                    new_sub_root_id = rb_tree_type_name##RotateRight(old_sub_root_id); \
                } \
                else { \
                    if (cur->left == ins_entry_id) { \
                        rb_tree_type_name##RotateRight(cur_id); \
                    } \
                    new_sub_root_id = rb_tree_type_name##RotateLeft(old_sub_root_id); \
                } \
                rb_tree_type_name##RbEntry* new_sub_root = referencer##_Reference(tree, new_sub_root_id); \
                accessor##_SetColor(new_sub_root, kRbBlack); \
                accessor##_SetColor(old_sub_root, kRbRed); \
                referencer##_Dereference(tree, new_sub_root); \
                \
                if (tree->root == old_sub_root_id) {        /* 原来的子树根节点可能是整棵树的根节点，因此要检查更新 */ \
                    tree->root = new_sub_root_id; \
                } \
                break;        /* 只是并入，未分裂，向上没有改变颜色，不再需要回溯 */ \
            } \
            cur_id = accessor##_GetParent(cur); \
            referencer##_Dereference(tree, cur); \
        } \
        referencer##_Dereference(tree, cur); \
    } \
    /*
    * 初始化树
    */ \
    void rb_tree_type_name##RbTreeInit(rb_tree_type_name##RbTree* tree) { \
        rb_tree_type_name##BsTreeInit(&tree->bs_tree); \
    } \
    /*
    * 从树中查找节点
    * 存在返回查找到的节点对应的对象，不存在返回NULL
    */ \
    id_type rb_tree_type_name##RbTreeFind(rb_tree_type_name##RbTree* tree, key_type* key) { \
        return rb_tree_type_name##BsTreeFind(&tree->bs_tree, key); \
    } \
    /*
    * 向树中插入节点
    * 成功返回true，失败返回false
    */ \
    bool rb_tree_type_name##RbTreePut(rb_tree_type_name##RbTree* tree, id_type put_entry_id) { \
        if (!rb_tree_type_name##BsTreePut(&tree->bs_tree, put_entry_id)) { \
            return false; \
        } \
        rb_tree_type_name##RbTreeInsertFixup(tree, put_entry_id); \
        return true; \
    } \




CUTILS_CONTAINER_RB_TREE_DECLARATION(Int, struct _IntRbEntry*, int)

typedef struct _IntEntry_Rb {
    IntRbEntry entry;
    int key;
} IntEntry_Rb;

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_RB_TREE_H_