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

#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DECLARATION_1(vector_type_name, offset_type, element_type, static_count) \
    typedef struct _##vector_type_name##Vector { \
        offset_type count; \
        element_type obj_arr[static_count]; \
    } vector_type_name##Vector;
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DECLARATION_2(vector_type_name, offset_type) \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector); 
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    bool vector_type_name##VectorIsEmpty(vector_type_name##Vector* vector) { \
        return vector->count == 0; \
    } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* vector) { \
        return sizeof(vector->obj_arr) / sizeof(vector->obj_arr[0]); \
    } \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* vector) { \
        return vector->count; \
    } \
    void vector_type_name##VectorSetCount(vector_type_name##Vector* vector, offset_type count) { \
        vector->count = count; \
    } \
    element_type* vector_type_name##VectorGetArray(vector_type_name##Vector* vector) { \
        return vector->obj_arr; \
    } \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* vector, offset_type index) { \
        return &vector->obj_arr[index]; \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector) { \
        vector_type_name##VectorSetCount(vector, 0); \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector) { }
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DEFINE_2(vector_type_name, expand_count) \
    return NULL;
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC LIBYUC_CONTAINER_VECTOR_MODE_STATIC


#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DECLARATION_1(vector_type_name, offset_type, element_type, static_count) \
    typedef struct _##vector_type_name##Vector { \
        offset_type count; \
        offset_type capacity; \
        element_type* obj_arr; \
    } vector_type_name##Vector;
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DECLARATION_2(vector_type_name, offset_type) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* vector, offset_type capacity); \
    void vector_type_name##VectorExpand(vector_type_name##Vector* vector, offset_type add_count); \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector, offset_type capacity); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector);
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    bool vector_type_name##VectorIsEmpty(vector_type_name##Vector* vector) { \
        return vector->count == 0; \
    } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* vector) { \
        return vector->capacity; \
    } \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* vector) { \
        return vector->count; \
    } \
    void vector_type_name##VectorSetCount(vector_type_name##Vector* vector, offset_type count) { \
        vector->count = count; \
    } \
    element_type* vector_type_name##VectorGetArray(vector_type_name##Vector* vector) { \
        return vector->obj_arr; \
    } \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* vector, offset_type index) { \
        return &vector->obj_arr[index]; \
    } \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* vector, offset_type capacity) { \
        element_type* new_buf = allocator##_CreateMultiple(arr, element_type, capacity); \
        if (vector->obj_arr) { \
            MemoryCopy(new_buf, vector->obj_arr, sizeof(element_type) * vector->count); \
            allocator##_Release(vector, vector->obj_arr); \
        } \
        vector->obj_arr = new_buf; \
        vector->capacity = capacity; \
    } \
    void vector_type_name##VectorExpand(vector_type_name##Vector* vector, offset_type add_count) { \
        offset_type old_capacity = vector_type_name##VectorGetCapacity(vector); \
        offset_type cur_capacity = old_capacity; \
        offset_type target_count = cur_capacity + add_count; \
        if (cur_capacity == 0) { \
            cur_capacity = 1; \
        } \
        while (cur_capacity < target_count) { \
            cur_capacity *= 2; \
        } \
        vector_type_name##VectorResetCapacity(vector, cur_capacity); \
        callbacker##_Expand(vector, old_capacity, cur_capacity); \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector, offset_type capacity) { \
        vector->obj_arr = NULL; \
        if (capacity != 0) { \
            vector_type_name##VectorResetCapacity(vector, capacity); \
        } else { \
            vector->capacity = 0; \
        } \
        vector->count = 0; \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector) { \
        if (vector->obj_arr) { \
            allocator##_Release(vector, vector->obj_arr); \
            vector->obj_arr = NULL; \
        } \
        vector->capacity = 0; \
        vector->count = 0; \
    }
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DEFINE_2(vector_type_name, expand_count) \
    vector_type_name##VectorExpand(vector, expand_count);
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC


#define LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC_DECLARATION_1(vector_type_name, offset_type, element_type, static_count) \
    typedef struct _##vector_type_name##Vector { \
        offset_type count; \
        offset_type capacity; \
        element_type obj_arr[]; \
    } *vector_type_name##Vector;
#define LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC_DECLARATION_2(vector_type_name, offset_type) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* vector, offset_type capacity); \
    void vector_type_name##VectorExpand(vector_type_name##Vector* vector, offset_type add_count); \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector, offset_type capacity); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector);


#define LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    bool vector_type_name##VectorIsEmpty(vector_type_name##Vector* vector) { \
        return *vector == NULL || (*vector)->count == 0; \
    } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* vector) { \
        if (*vector == NULL) return 0; \
        return (*vector)->capacity; \
    } \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* vector) { \
        if (*vector == NULL) return 0; \
        return (*vector)->count; \
    } \
    void vector_type_name##VectorSetCount(vector_type_name##Vector* vector, offset_type count) { \
         assert(*vector != NULL); \
        (*vector)->count = count; \
    } \
    element_type* vector_type_name##VectorGetArray(vector_type_name##Vector* vector) { \
        if (*vector == NULL) return NULL; \
        return (*vector)->obj_arr; \
    } \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* vector, offset_type index) { \
        if (*vector == NULL) return NULL; \
        return &(*vector)->obj_arr[index]; \
    } \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* vector, offset_type capacity) { \
        vector_type_name##Vector new_vector = (vector_type_name##Vector)allocator##_CreateBySize(vector, sizeof(**vector) + sizeof(element_type) * capacity); \
        if (*vector) { \
            MemoryCopy(new_vector, *vector, sizeof(**vector) + sizeof(element_type) * vector_type_name##VectorGetCount(vector)); \
            allocator##_Release(vector, *vector); \
        } else { \
            vector_type_name##VectorSetCount(&new_vector, 0) ; \
        } \
        *vector = new_vector; \
        (*vector)->capacity = capacity; \
    } \
    void vector_type_name##VectorExpand(vector_type_name##Vector* vector, offset_type add_count) { \
        offset_type old_capacity = vector_type_name##VectorGetCapacity(vector); \
        offset_type cur_capacity = old_capacity; \
        offset_type target_count = cur_capacity + add_count; \
        if (cur_capacity == 0) { \
            cur_capacity = 1; \
        } \
        while (cur_capacity < target_count) { \
            cur_capacity *= 2; \
        } \
        vector_type_name##VectorResetCapacity(vector, cur_capacity); \
        callbacker##_Expand(vector, old_capacity, cur_capacity); \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* vector, offset_type capacity) { \
        if (capacity != 0) { \
            vector_type_name##VectorResetCapacity(vector, capacity); \
        } else { \
            *vector = NULL; \
        } \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector) { \
        if (*vector) { \
            allocator##_Release(vector, *vector); \
            *vector = NULL; \
        } \
    } \
    
#define LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC_DEFINE_2(vector_type_name, expand_count) \
    vector_type_name##VectorExpand(vector, expand_count);
#define LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC LIBYUC_CONTAINER_VECTOR_MODE_INLINE_DYNAMIC



#define LIBYUC_CONTAINER_VECTOR_DECLARATION(vector_type_name, mode, offset_type, id_type, element_type, static_count) \
    mode##_DECLARATION_1(vector_type_name, offset_type, element_type, static_count) \
    typedef struct _##vector_type_name##VectorIterator { \
        offset_type cur_index; \
    } vector_type_name##VectorIterator; \
    mode##_DECLARATION_2(vector_type_name, offset_type) \
    \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* vector); \
    void vector_type_name##VectorSetCount(vector_type_name##Vector* vector, offset_type count); \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* vector); \
    element_type* vector_type_name##VectorGetArray(vector_type_name##Vector* vector); \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* vector, offset_type index); \
    \
    element_type* vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* vector, const element_type* obj, offset_type count); \
    element_type* vector_type_name##VectorPushTail(vector_type_name##Vector* vector, const element_type* obj); \
    element_type* vector_type_name##VectorGetTail(vector_type_name##Vector* vector); \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* vector); \
    \
    element_type* vector_type_name##VectorPut(vector_type_name##Vector* vector, const element_type* obj); \
    element_type* vector_type_name##VectorIteratorLocate(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter, const element_type* obj); \
    element_type* vector_type_name##VectorIteratorIndex(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter); \
    void vector_type_name##VectorIteratorDelete(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter); \
    element_type* vector_type_name##VectorIteratorFirst(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter); \
    element_type* vector_type_name##VectorIteratorNext(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter); \


/*
* 提供_GetKey方法，即可支持O(n)的Find、Put、Delete
*/
#define LIBYUC_CONTAINER_VECTOR_DEFINE(vector_type_name, mode, offset_type, id_type, element_type, key_type, accessor, allocator, callbacker, comparer, referencer) \
    mode##_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    element_type* vector_type_name##VectorPushTail(vector_type_name##Vector* vector, const element_type* obj) { \
        if (vector_type_name##VectorGetCapacity(vector) <= vector_type_name##VectorGetCount(vector)) { \
            mode##_DEFINE_2(vector_type_name, 1) \
        } \
        MemoryCopy(vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector)), obj, sizeof(element_type)); \
        vector_type_name##VectorSetCount(vector, vector_type_name##VectorGetCount(vector) + 1); \
        return vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector) - 1); \
    } \
    element_type* vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* vector, const element_type* obj, offset_type count) { \
        if (vector_type_name##VectorGetCapacity(vector) <= vector_type_name##VectorGetCount(vector) + count) { \
            mode##_DEFINE_2(vector_type_name, count) \
        } \
        MemoryCopy(vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector)), obj, sizeof(element_type) * count); \
        vector_type_name##VectorSetCount(vector, vector_type_name##VectorGetCount(vector) + count); \
        return vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector) - count); \
    } \
    element_type* vector_type_name##VectorGetTail(vector_type_name##Vector* vector) { \
        if (vector_type_name##VectorGetCount(vector) == 0) { \
            return NULL; \
        } \
        return vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector)-1); \
    } \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* vector) { \
        if (vector_type_name##VectorGetCount(vector) == 0) { \
            return NULL; \
        } \
        vector_type_name##VectorSetCount(vector, vector_type_name##VectorGetCount(vector) - 1); \
        return vector_type_name##VectorIndex(vector, vector_type_name##VectorGetCount(vector)); \
    } \
    \
    LIBYUC_ALGORITHM_ARRAY_DEFINE(vector_type_name##Vector, offset_type, id_type, element_type, key_type, accessor, comparer, referencer) \
    \
    element_type* vector_type_name##VectorPut(vector_type_name##Vector* vector, const element_type* obj) { \
        const key_type* key = accessor##_GetKey(vector, obj); \
        offset_type index = vector_type_name##VectorArrayFind(vector_type_name##VectorGetArray(vector), vector_type_name##VectorGetCount(vector), key); \
        if (index == referencer##_InvalidId) { \
            return vector_type_name##VectorPushTail(vector, obj); \
        } else { \
            element_type* ele = vector_type_name##VectorIndex(vector, index); \
            MemoryCopy(ele, obj, sizeof(element_type)); \
            return ele; \
        } \
    } \
    element_type* vector_type_name##VectorIteratorLocate(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter, const key_type* key) { \
        iter->cur_index = vector_type_name##VectorArrayFind(vector_type_name##VectorGetArray(vector), vector_type_name##VectorGetCount(vector), key); \
        return vector_type_name##VectorIteratorIndex(vector, iter); \
    } \
    element_type* vector_type_name##VectorIteratorIndex(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter) { \
        if (iter->cur_index == referencer##_InvalidId || iter->cur_index >= vector_type_name##VectorGetCount(vector)) return NULL; \
        return vector_type_name##VectorIndex(vector, iter->cur_index); \
    } \
    void vector_type_name##VectorIteratorDelete(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter) { \
         assert(vector_type_name##VectorGetCount(vector) > 0); \
         assert(iter->cur_index < vector_type_name##VectorGetCount(vector)); \
        vector_type_name##VectorArrayDelete(vector_type_name##VectorGetArray(vector), vector_type_name##VectorGetCount(vector), iter->cur_index); \
        vector_type_name##VectorSetCount(vector, vector_type_name##VectorGetCount(vector) - 1); \
    } \
    element_type* vector_type_name##VectorIteratorFirst(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter) { \
        iter->cur_index = 0; \
        return vector_type_name##VectorGetCount(vector) > 0 ? vector_type_name##VectorIndex(vector, 0) : NULL; \
    } \
    element_type* vector_type_name##VectorIteratorNext(vector_type_name##Vector* vector, vector_type_name##VectorIterator* iter) { \
        if (iter->cur_index == referencer##_InvalidId || iter->cur_index >= vector_type_name##VectorGetCount(vector)) return NULL; \
        ++iter->cur_index; \
        return vector_type_name##VectorIndex(vector, iter->cur_index); \
    } \

#define LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT_Expand(ARR, OLD_CAPACITY, NEW_CAPACITY)
#define LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT

#define LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT_GetKey(ARR, OBJ) LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT_GetKey(ARR, OBJ)
#define LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT

#define LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID

#define LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT_InvalidId LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT_InvalidId
#define LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_VECTOR_H_