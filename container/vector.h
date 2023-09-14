/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#include <libyuc/basic.h>
#include <libyuc/algorithm/array.h>

#ifdef __cplusplus
extern "C" {
#endif

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
#ifndef LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidId
#define LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidId (-1)
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidDynamicArray
#define LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidDynamicArray (NULL)
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




#ifdef LIBYUC_TEMPLATE_C_INCLUDE

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
void VectorInit(Vector* vector) {
    VectorSetCount(vector, 0);
}

void VectorRelease(Vector* vector) {
}


LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCapacity(Vector* vector) {
    return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]);
}

#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
void VectorInit(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity) {
    vector->obj_arr = NULL;
    if (capacity != 0) {
        VectorResetCapacity(vector, capacity);
    }
    else {
        vector->capacity = 0;
    }
    vector->count = 0;
}
void VectorRelease(Vector* vector) {
    if (vector->obj_arr) {
        LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(vector, vector->obj_arr);
        vector->obj_arr = LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidDynamicArray;
    }
    vector->capacity = 0;
    vector->count = 0;
}

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCapacity(Vector* vector) {
    return vector->capacity;
}

void VectorResetCapacity(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset capacity) {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* new_buf = LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple(arr, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element, capacity);
    if (vector->obj_arr) {
        MemoryCopy(new_buf, vector->obj_arr, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element) * vector->count);
        LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(vector, vector->obj_arr);
    }
    vector->obj_arr = new_buf;
    vector->capacity = capacity;
}
void VectorExpand(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset add_count) {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset old_capacity = VectorGetCapacity(vector);
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset cur_capacity = old_capacity;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset target_count = cur_capacity + add_count;
    if (cur_capacity == 0) {
        cur_capacity = 1;
    }
    while (cur_capacity < target_count) {
        cur_capacity *= 2;
    }
    VectorResetCapacity(vector, cur_capacity);
    LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand(vector, old_capacity, cur_capacity);
}
#endif

bool VectorIsEmpty(Vector* vector) {
    return vector->count == 0;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset VectorGetCount(Vector* vector) {
    return vector->count;
}
void VectorSetCount(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count) {
    vector->count = count;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetArray(Vector* vector) {
    return vector->obj_arr;
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIndex(Vector* vector, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id index) {
    return &vector->obj_arr[index];
}

LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj) {
    if (VectorGetCapacity(vector) <= VectorGetCount(vector)) {
#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
        VectorExpand(vector, 1);
#endif
    }
    MemoryCopy(VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector)), obj, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element));
    VectorSetCount(vector, VectorGetCount(vector) + 1);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - 1);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPushMultipleTail(Vector* vector, const LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* obj, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset count) {
    if (VectorGetCapacity(vector) <= (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) + (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)count) {
#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
        VectorExpand(vector, count);
#endif
    }
    MemoryCopy(VectorIndex(vector, VectorGetCount(vector)), obj, sizeof(LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element) * count);
    VectorSetCount(vector, VectorGetCount(vector) + count);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)count);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorGetTail(Vector* vector) {
    if (VectorGetCount(vector) == 0) {
        return NULL;
    }
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector) - 1);
}
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorPopTail(Vector* vector) {
    if (VectorGetCount(vector) == 0) {
        return NULL;
    }
    VectorSetCount(vector, VectorGetCount(vector) - 1);
    return VectorIndex(vector, (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id)VectorGetCount(vector));
}


//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorGetIndex(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidId || iter->cur_index >= VectorGetCount(vector)) return NULL;
//        return VectorIndex(vector, iter->cur_index);
//}
//void VectorIteratorDelete(Vector* vector, VectorIterator* iter) {
//      assert(VectorGetCount(vector) > 0);
//      assert(iter->cur_index < VectorGetCount(vector));
//    VectorArrayDelete(VectorGetArray(vector), VectorGetCount(vector), iter->cur_index);
//    VectorSetCount(vector, VectorGetCount(vector) - 1);
//}
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorFirst(Vector* vector, VectorIterator* iter) {
//    iter->cur_index = 0;
//    return VectorGetCount(vector) > 0 ? VectorIndex(vector, 0) : NULL;
//}
//LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* VectorIteratorNext(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidId || iter->cur_index >= VectorGetCount(vector)) return NULL;
//        ++iter->cur_index;
//        return VectorIndex(vector, iter->cur_index);
//}
#endif

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
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_ElementCount
#undef LIBYUC_CONTAINER_VECTOR_Indexer_Const_InvalidId
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#undef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release
#undef LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand


#undef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
#undef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC

#ifdef __cplusplus
}
#endif
