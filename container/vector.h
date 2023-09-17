/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_TEMPLATE_UNDECLARE
#ifndef LIBYUC_CONTAINER_VECTOR_CLASS_NAME
#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_ACCESSOR_GetArray
#define LIBYUC_CONTAINER_VECTOR_ACCESSOR_GetArray(main_obj) ((obj)->obj_arr)
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create(main_obj, obj_type) MemoryAlloc(sizeof(obj_type))
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple(main_obj, obj_type, count) ((obj_type*)MemoryAlloc(sizeof(obj_type) * count))
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(main_obj, obj) MemoryFree(obj)
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand
#define LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand
#endif


#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_DynamicArray
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_DynamicArray LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element int
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount 1
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId (-1)
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray (NULL)
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id size_t
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#endif

//#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC
//#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC


#define Vector MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, Vector)
#define VectorIterator MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIterator)


#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
typedef struct Vector {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element obj_arr[LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount];
} Vector;
#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
typedef struct Vector {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_DynamicArray obj_arr;
} Vector;
#endif

typedef struct VectorIterator {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id cur_index;
} VectorIterator;

#define VectorInit MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorInit)
#define VectorRelease MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorRelease)
#define VectorGetCount MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorGetCount)
#define VectorSetCount MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorSetCount)
#define VectorGetCapacity MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorGetCapacity)
#define VectorIsEmpty MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIsEmpty)
#define VectorResetCapacity MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorResetCapacity)
#define VectorGetArray MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorGetArray)
#define VectorIndex MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIndex)
#define VectorPushMultipleTail MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorPushMultipleTail)
#define VectorPushTail MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorPushTail)
#define VectorGetTail MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorGetTail)
#define VectorPopTail MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorPopTail)
#define VectorPut MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorPut)
#define VectorIteratorLocate MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIteratorLocate)
#define VectorIteratorIndex MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIteratorIndex)
#define VectorIteratorDelete MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIteratorDelete)
#define VectorIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIteratorFirst)
#define VectorIteratorNext MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorIteratorNext)

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
void VectorInit(Vector* vector);
void VectorRelease(Vector* vector);
#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
void VectorInit(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity);
void VectorRelease(Vector* vector);
void VectorResetCapacity(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity);
#define VectorExpand MAKE_NAME(LIBYUC_CONTAINER_VECTOR_CLASS_NAME, VectorExpand)
#endif

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCount(Vector* vector);
void VectorSetCount(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCapacity(Vector* vector);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetArray(Vector* vector);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIndex(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id index);

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushMultipleTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetTail(Vector* vector);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPopTail(Vector* vector);

//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPut(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj);
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorLocate(Vector* vector, VectorIterator* iter, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj);
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorIndex(Vector* vector, VectorIterator* iter);
//void VectorIteratorDelete(Vector* vector, VectorIterator* iter);
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorFirst(Vector* vector, VectorIterator* iter);
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorNext(Vector* vector, VectorIterator* iter);
#endif


#if !defined(LIBYUC_TEMPLATE_DEFINE) || defined(LIBYUC_TEMPLATE_UNDECLARE)
#undef Vector
#undef VectorIterator

#undef VectorGetCount
#undef VectorSetCount
#undef VectorGetCapacity
#undef VectorGetArray
#undef VectorIndex
#undef VectorIsEmpty
#undef VectorExpand
#undef VectorPushMultipleTail
#undef VectorPushTail
#undef VectorGetTail
#undef VectorPopTail
#undef VectorPut
#undef VectorIteratorLocate
#undef VectorIteratorIndex
#undef VectorIteratorDelete
#undef VectorIteratorFirst
#undef VectorIteratorNext

#undef LIBYUC_CONTAINER_VECTOR_CLASS_NAME
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_ElementCount
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset

#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release
#undef LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand


#undef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
#undef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#endif

#ifdef __cplusplus
}
#endif
