/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/vector.def>

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


#include <libyuc/container/vector.undef>

#ifdef __cplusplus
}
#endif
