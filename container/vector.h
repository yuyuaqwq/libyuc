/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_VECTOR_H_
#define LIBYUC_CONTAINER_VECTOR_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/array.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLASS_NAME
#define CLASS_NAME
#endif

#ifndef Type_Element
#define Type_Element int
#endif

#ifndef Const_ElementCount
#define Const_ElementCount 1
#endif

#ifndef REFERENCER_Const_InvalidId
#define REFERENCER_Const_InvalidId (-1)
#endif

#ifndef REFERENCER_Type_Id
#define REFERENCER_Type_Id size_t
#endif

#ifndef REFERENCER_Type_Offset
#define REFERENCER_Type_Offset size_t
#endif



//#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC
//#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
typedef struct MAKE_NAME(CLASS_NAME, Vector) {
    REFERENCER_Type_Offset count;
    Type_Element obj_arr[Const_ElementCount];
} MAKE_NAME(CLASS_NAME, Vector);
#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
typedef struct MAKE_NAME(CLASS_NAME, Vector) {
    REFERENCER_Type_Offset count;
    REFERENCER_Type_Offset capacity;
    Type_Element* obj_arr;
} MAKE_NAME(CLASS_NAME, Vector);
#endif

typedef struct MAKE_NAME(CLASS_NAME, VectorIterator) {
    REFERENCER_Type_Id cur_index;
} MAKE_NAME(CLASS_NAME, VectorIterator);

#define Vector MAKE_NAME(CLASS_NAME, Vector)
#define VectorIterator MAKE_NAME(CLASS_NAME, VectorIterator)

REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, VectorGetCount)(Vector* vector);
void MAKE_NAME(CLASS_NAME, VectorSetCount)(Vector* vector, REFERENCER_Type_Offset count);
REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, VectorGetCapacity)(Vector* vector);
Type_Element* MAKE_NAME(CLASS_NAME, VectorGetArray)(Vector* vector);
Type_Element* MAKE_NAME(CLASS_NAME, VectorIndex)(Vector* vector, REFERENCER_Type_Id index);

Type_Element* MAKE_NAME(CLASS_NAME, VectorPushMultipleTail)(Vector* vector, const Type_Element* obj, REFERENCER_Type_Offset count);
Type_Element* MAKE_NAME(CLASS_NAME, VectorPushTail)(Vector* vector, const Type_Element* obj);
Type_Element* MAKE_NAME(CLASS_NAME, VectorGetTail)(Vector* vector);
Type_Element* MAKE_NAME(CLASS_NAME, VectorPopTail)(Vector* vector);

Type_Element* MAKE_NAME(CLASS_NAME, VectorPut)(Vector* vector, const Type_Element* obj);
Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorLocate)(Vector* vector, VectorIterator* iter, const Type_Element* obj);
Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorIndex)(Vector* vector, VectorIterator* iter);
void MAKE_NAME(CLASS_NAME, VectorIteratorDelete)(Vector* vector, VectorIterator* iter);
Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorFirst)(Vector* vector, VectorIterator* iter);
Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorNext)(Vector* vector, VectorIterator* iter);

#undef Vector
#undef VectorIterator

#undef CLASS_NAME
#undef Const_ElementCount
#undef REFERENCER_Const_InvalidId
#undef REFERENCER_Type_Id
#undef REFERENCER_Type_Offset



#ifndef Const_ElementCount
#define Const_ElementCount 1
#endif

#ifndef REFERENCER_Const_InvalidId
#define REFERENCER_Const_InvalidId (-1)
#endif

#ifndef REFERENCER_Type_Id
#define REFERENCER_Type_Id size_t
#endif

#ifndef REFERENCER_Type_Offset
#define REFERENCER_Type_Offset size_t
#endif

#ifndef ALLOCATOR_Create
#define ALLOCATOR_Create(main_obj, obj_type) MemoryAlloc(sizeof(obj_type))
#endif
#ifndef ALLOCATOR_CreateMultiple
#define ALLOCATOR_CreateMultiple(main_obj, obj_type, count) ((obj_type*)MemoryAlloc(sizeof(obj_type) * count))
#endif
#ifndef ALLOCATOR_Release
#define ALLOCATOR_Release(main_obj, obj) MemoryFree(obj)
#endif

#ifndef CALLBACK_Expand
#define CALLBACK_Expand
#endif

#define Vector MAKE_NAME(CLASS_NAME, Vector)
#define VectorIterator MAKE_NAME(CLASS_NAME, VectorIterator)

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_STATIC
void MAKE_NAME(CLASS_NAME, VectorInit)(Vector* vector) {
    MAKE_NAME(CLASS_NAME, VectorSetCount)(vector, 0);
}

void MAKE_NAME(CLASS_NAME, VectorRelease)(Vector* vector) {
}


REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, VectorGetCapacity)(Vector* vector) {
    return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]);
}

#endif

#ifdef LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
void MAKE_NAME(CLASS_NAME, VectorInit)(Vector* vector, REFERENCER_Type_Offset capacity) {
    vector->obj_arr = NULL;
    if (capacity != 0) {
        MAKE_NAME(CLASS_NAME, VectorResetCapacity)(vector, capacity);
    }
    else {
        vector->capacity = 0;
    }
    vector->count = 0;
}
void MAKE_NAME(CLASS_NAME, VectorRelease)(Vector* vector) {
    if (vector->obj_arr) {
        ALLOCATOR_Release(vector, vector->obj_arr);
        vector->obj_arr = NULL;
    }
    vector->capacity = 0;
    vector->count = 0;
}

REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, VectorGetCapacity)(Vector* vector) {
    return vector->capacity;
}

void MAKE_NAME(CLASS_NAME, VectorResetCapacity)(Vector* vector, REFERENCER_Type_Offset capacity) {
    Type_Element* new_buf = ALLOCATOR_CreateMultiple(arr, Type_Element, capacity);
    if (vector->obj_arr) {
        MemoryCopy(new_buf, vector->obj_arr, sizeof(Type_Element) * vector->count);
        ALLOCATOR_Release(vector, vector->obj_arr);
    }
    vector->obj_arr = new_buf;
    vector->capacity = capacity;
}
void MAKE_NAME(CLASS_NAME, VectorExpand)(Vector* vector, REFERENCER_Type_Offset add_count) {
    REFERENCER_Type_Offset old_capacity = MAKE_NAME(CLASS_NAME, VectorGetCapacity)(vector);
    REFERENCER_Type_Offset cur_capacity = old_capacity;
    REFERENCER_Type_Offset target_count = cur_capacity + add_count;
    if (cur_capacity == 0) {
        cur_capacity = 1;
    }
    while (cur_capacity < target_count) {
        cur_capacity *= 2;
    }
    MAKE_NAME(CLASS_NAME, VectorResetCapacity)(vector, cur_capacity);
    CALLBACK_Expand(vector, old_capacity, cur_capacity);
}
#endif

bool MAKE_NAME(CLASS_NAME, VectorIsEmpty)(Vector* vector) {
    return vector->count == 0;
}
REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, VectorGetCount)(Vector* vector) {
    return vector->count;
}
void MAKE_NAME(CLASS_NAME, VectorSetCount)(Vector* vector, REFERENCER_Type_Offset count) {
    vector->count = count;
}
Type_Element* MAKE_NAME(CLASS_NAME, VectorGetArray)(Vector* vector) {
    return vector->obj_arr;
}
Type_Element* MAKE_NAME(CLASS_NAME, VectorIndex)(Vector* vector, REFERENCER_Type_Id index) {
    return &vector->obj_arr[index];
}

Type_Element* MAKE_NAME(CLASS_NAME, VectorPushTail)(Vector* vector, const Type_Element* obj) {
    if (MAKE_NAME(CLASS_NAME, VectorGetCapacity)(vector) <= MAKE_NAME(CLASS_NAME, VectorGetCount)(vector)) {

    }
    MemoryCopy(MAKE_NAME(CLASS_NAME, VectorIndex)(vector, (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector)), obj, sizeof(Type_Element));
    MAKE_NAME(CLASS_NAME, VectorSetCount)(vector, MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) + 1);
    return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) - 1);
}
Type_Element* MAKE_NAME(CLASS_NAME, VectorPushMultipleTail)(Vector* vector, const Type_Element* obj, REFERENCER_Type_Offset count) {
    if (MAKE_NAME(CLASS_NAME, VectorGetCapacity)(vector) <= (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) + (REFERENCER_Type_Id)count) {
        
    }
    MemoryCopy(MAKE_NAME(CLASS_NAME, VectorIndex)(vector, MAKE_NAME(CLASS_NAME, VectorGetCount)(vector)), obj, sizeof(Type_Element)* count);
    MAKE_NAME(CLASS_NAME, VectorSetCount)(vector, MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) + count);
    return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) - (REFERENCER_Type_Id)count);
}
Type_Element* MAKE_NAME(CLASS_NAME, VectorGetTail)(Vector* vector) {
    if (MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) == 0) {
        return NULL;
    }
    return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) - 1);
}
Type_Element* MAKE_NAME(CLASS_NAME, VectorPopTail)(Vector* vector) {
    if (MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) == 0) {
        return NULL;
    }
    MAKE_NAME(CLASS_NAME, VectorSetCount)(vector, MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) - 1);
    return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, (REFERENCER_Type_Id)MAKE_NAME(CLASS_NAME, VectorGetCount)(vector));
}


//Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorGetIndex)(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == REFERENCER_Const_InvalidId || iter->cur_index >= MAKE_NAME(CLASS_NAME, VectorGetCount)(vector)) return NULL;
//        return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, iter->cur_index);
//}
//void MAKE_NAME(CLASS_NAME, VectorIteratorDelete)(Vector* vector, VectorIterator* iter) {
//      assert(MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) > 0);
//      assert(iter->cur_index < MAKE_NAME(CLASS_NAME, VectorGetCount)(vector));
//    MAKE_NAME(CLASS_NAME, VectorArrayDelete)(MAKE_NAME(CLASS_NAME, VectorGetArray)(vector), MAKE_NAME(CLASS_NAME, VectorGetCount)(vector), iter->cur_index);
//    MAKE_NAME(CLASS_NAME, VectorSetCount)(vector, MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) - 1);
//}
//Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorFirst)(Vector* vector, VectorIterator* iter) {
//    iter->cur_index = 0;
//    return MAKE_NAME(CLASS_NAME, VectorGetCount)(vector) > 0 ? MAKE_NAME(CLASS_NAME, VectorIndex)(vector, 0) : NULL;
//}
//Type_Element* MAKE_NAME(CLASS_NAME, VectorIteratorNext)(Vector* vector, VectorIterator* iter) {
//    if (iter->cur_index == REFERENCER_Const_InvalidId || iter->cur_index >= MAKE_NAME(CLASS_NAME, VectorGetCount)(vector)) return NULL;
//        ++iter->cur_index;
//        return MAKE_NAME(CLASS_NAME, VectorIndex)(vector, iter->cur_index);
//}

#undef Vector
#undef VectorIterator

#undef CLASS_NAME
#undef Const_ElementCount
#undef REFERENCER_Const_InvalidId
#undef REFERENCER_Type_Id
#undef REFERENCER_Type_Offset
#undef ALLOCATOR_Create
#undef ALLOCATOR_CreateMultiple
#undef ALLOCATOR_Release
#undef CALLBACK_Expand

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_VECTOR_H_