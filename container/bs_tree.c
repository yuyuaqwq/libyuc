/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/bs_tree.h>






//
//
//typedef bool (*BsTreeTraversalCallback)(BsEntry* entry, void* arg);
///*
//* 前序遍历
//* 先根再右再左
//*/
//static void BsTreePreorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
//    if (!entry) return;
//    callback(entry, arg);
//    BsTreePreorder_Callback(entry->left, callback, arg);
//    BsTreePreorder_Callback(entry->right, callback, arg);
//}
//
///*
//* 中序遍历
//* 先左再根再右
//*/
//static void BsTreeMiddleorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
//    if (!entry) return;
//    BsTreeMiddleorder_Callback(entry->left, callback, arg);
//    callback(entry, arg);
//    BsTreeMiddleorder_Callback(entry->right, callback, arg);
//}
//
///*
//* 后序遍历
//* 先左再右再根
//*/
//static void BsTreePostorder_Callback(BsEntry* entry, BsTreeTraversalCallback callback, void* arg) {
//    if (!entry) return;
//    BsTreePostorder_Callback(entry->left, callback, arg);
//    BsTreePostorder_Callback(entry->right, callback, arg);
//    callback(entry, arg);
//}


CUTILS_CONTAINER_BS_TREE_DECLARATION(Int, struct _IntBsEntry*, int)
typedef struct _IntEntry {
    IntBsEntry entry;
    int key;
} IntEntry;
#define ACCESSOR_KEY(int_entry) (&((IntEntry*)int_entry)->key)
#define ACCESSOR_Parent(bs_entry) (&((IntBsEntry*)bs_entry)->parent)
CUTILS_CONTAINER_BS_TREE_DEFINE(Int, struct _IntBsEntry*, int, CUTILS_OBJECT_REFERENCER_DEFALUT, ACCESSOR_KEY, ACCESSOR_Parent, CUTILS_OBJECT_COMPARER_DEFALUT)
