/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HEAP_H_
#define LIBYUC_CONTAINER_HEAP_H_

#include <libyuc/object.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 默认为大顶堆，可以自定义CmpFunc以调整为小顶堆
*/

//typedef struct _Heap {
//  Vector vector;
//  uint32_t keyFieldOffset;
//  uint32_t keyFieldSize;
//  CmpFunc cmpFunc;
//} Heap;
//
//void HeapInit(Heap* heap, uint32_t high);
//void HeapRelease(Heap* heap, bool deleteObj);
//void HeapInsert(Heap* heap, void* obj);
//void* HeapGetTop(Heap* heap);
//void* HeapPopTop(Heap* heap);

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HEAP_H_