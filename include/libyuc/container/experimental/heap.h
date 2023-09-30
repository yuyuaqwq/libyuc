/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HEAP_H_
#define LIBYUC_CONTAINER_HEAP_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 默认为大顶堆，可以自定义CmpFunc以调整为小顶堆
*/

//typedef struct _Heap {
//    Vector vector;
//    uint32_t keyFieldOffset;
//    uint32_t keyFieldSize;
//    CmpFunc cmpFunc;
//} Heap;
//
//void HeapInit(Heap* heap, uint32_t high);
//void HeapRelease(Heap* heap, bool deleteObj);
//void HeapInsert(Heap* heap, void* obj);
//void* HeapGetTop(Heap* heap);
//void* HeapPopTop(Heap* heap);

/*
* 为了可以实现数组原地建堆，下标从0开始
*/
//
//static void ShiftUp(Heap* heap, uint32_t now) {
//    Vector* vec = &heap->vector;
//    uint32_t next = (now - 1) / 2;        // now / 2
//    
//    while (next > 0) {
//        void** objParent = VectorAt(vec, next, void);
//        void* keyParent = ObjectGetFieldByOffset(*objParent, heap->keyFieldOffset, void);
//        void** objChild = VectorAt(vec, now, void);
//        void* keyChild = ObjectGetFieldByOffset(*objChild, heap->keyFieldOffset, void);
//        
//        // 父亲大于等于孩子就结束
//        if (heap->cmpFunc(keyParent, keyChild , heap->keyFieldSize) >= 0) {
//            break;
//        }
//        void* temp = *objParent;
//        *objParent = *objChild;
//        *objChild = temp;
//        now = next;
//        next = (now - 1) / 2;        // now / 2
//    }
//}
//
//static void ShiftDown(Heap* heap, uint32_t now) {
//    Vector* vec = &heap->vector;
//    uint32_t next = (now + 1) * 2 - 1;        // now * 2
//    while (next < VectorGetCount(&heap->vector)) {
//        void** objParent = VectorAt(vec, now, void);
//        void* keyParent = ObjectGetFieldByOffset(*objParent, heap->keyFieldOffset, void);
//        void** objChild = VectorAt(vec, next, void);
//        void* keyChild = ObjectGetFieldByOffset(*objChild, heap->keyFieldOffset, void);
//
//        // 两个孩子都小于等于父亲就结束
//        if (heap->cmpFunc(keyChild, keyParent, heap->keyFieldSize) <= 0) {
//            next++;
//            objChild = VectorAt(vec, next, void);
//            keyChild = ObjectGetFieldByOffset(*objChild, heap->keyFieldOffset, void);
//            if (heap->cmpFunc(keyChild, keyParent, heap->keyFieldSize) <= 0) {
//                break;
//            }
//        }
//        void* temp = *objParent;
//        *objParent = *objChild;
//        *objChild = temp;
//        now = next;
//        next = (now + 1) * 2 - 1;        // now * 2
//    }
//}
//
//void HeapInit(Heap* heap, uint32_t high, uint32_t keyFieldOffset, uint32_t keyFieldSize, CmpFunc cmpFunc) {
//    size_t capacity = 1;
//    for (uint32_t i = 0; i < high; i++) {
//        capacity *= 2;
//    }
//    VectorInit(&heap->vector, capacity);
//    // VectorPushTail(&heap->vector, NULL);
//
//    heap->keyFieldOffset = keyFieldOffset;
//    heap->keyFieldSize = keyFieldSize;
//
//    if (cmpFunc == NULL) {
//        cmpFunc = MemoryCmpR;
//    }
//    heap->cmpFunc = cmpFunc;
//}
//
//void HeapRelease(Heap* heap, bool deleteObj) {
//    VectorRelease(&heap->vector, deleteObj);
//}
//
//void HeapInsert(Heap* heap, void* obj) {
//    uint32_t tailIndex = VectorPushTail(&heap->vector, obj);
//    ShiftUp(heap, tailIndex);
//}
//
//void* HeapGetTop(Heap* heap) {
//    if (VectorGetCount(&heap->vector) == 0) {         // <= 1
//        return NULL;
//    }
//    return *VectorAt(&heap->vector, 0, void);         // 1
//}
//
//void* HeapPopTop(Heap* heap) {
//    if (VectorGetCount(&heap->vector) == 0) {         // <= 1
//        return NULL;
//    }
//    void* topObj = *VectorAt(&heap->vector, 0, void);         // 1
//    void* tailObj = VectorPopTail(&heap->vector);
//    *VectorAt(&heap->vector, 0, void) = tailObj;         // 1
//    ShiftDown(heap, 0);         // 1
//    return topObj;
//}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HEAP_H_