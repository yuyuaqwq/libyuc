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


#define LIBYUC_CONTAINER_VECTOR_DECLARATION(vector_type_name, offset_type, id_type, element_type) \
    typedef struct _##vector_type_name##Vector { \
        offset_type count; \
        offset_type capacity; \
        bool is_vector_alloc; \
        element_type* obj_arr; \
    } vector_type_name##Vector; \
    typedef struct _##vector_type_name##VectorIterator { \
        offset_type cur_index; \
    } vector_type_name##VectorIterator; \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, offset_type capacity, element_type* external_arr); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr); \
    offset_type vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, offset_type count); \
    offset_type vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj); \
    element_type* vector_type_name##VectorGetTail(vector_type_name##Vector* arr); \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* arr); \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* arr); \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* arr); \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* arr, offset_type index); \
    \
    element_type* vector_type_name##VectorIteratorLocate(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter, const element_type* obj); \
    element_type* vector_type_name##VectorIteratorIndex(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter); \
    void vector_type_name##VectorIteratorDelete(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter); \
    element_type* vector_type_name##VectorIteratorFirst(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter); \
    element_type* vector_type_name##VectorIteratorNext(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter); \

#define LIBYUC_CONTAINER_VECTOR_DEFINE(vector_type_name, offset_type, id_type, element_type, accessor, allocator, callbacker, comparer, referencer) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* arr, offset_type capacity) { \
        element_type* new_buf = allocator##_CreateMultiple(arr, element_type, capacity); \
        if (arr->obj_arr) { \
            MemoryCopy(new_buf, arr->obj_arr, sizeof(element_type) * arr->count); \
            if (arr->is_vector_alloc) { \
                allocator##_Release(arr, arr->obj_arr); \
            } \
        } \
        arr->is_vector_alloc = true; \
        arr->obj_arr = new_buf; \
        arr->capacity = capacity; \
    } \
    void vector_type_name##VectorExpand(vector_type_name##Vector* arr, offset_type add_count) { \
        offset_type old_capacity = arr->capacity; \
        offset_type cur_capacity = old_capacity; \
        offset_type target_count = cur_capacity + add_count; \
        if (cur_capacity == 0) { \
            cur_capacity = 1; \
        } \
        while (cur_capacity < target_count) { \
            cur_capacity *= 2; \
        } \
        vector_type_name##VectorResetCapacity(arr, cur_capacity); \
        callbacker##_Expand(arr, old_capacity, cur_capacity); \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, offset_type capacity, element_type* external_arr) { \
        arr->obj_arr = NULL; \
        arr->is_vector_alloc = !external_arr; \
        if (capacity != 0 && !external_arr) { \
            vector_type_name##VectorResetCapacity(arr, capacity); \
        } \
        else { \
            arr->capacity = capacity; \
            arr->obj_arr = external_arr; \
        } \
        arr->count = 0; \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr) { \
        if (arr->obj_arr && arr->is_vector_alloc) { \
            allocator##_Release(arr, arr->obj_arr); \
            arr->obj_arr = NULL; \
        } \
        arr->capacity = 0; \
        arr->count = 0; \
    } \
    offset_type vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj) { \
        if (arr->capacity <= arr->count) { \
            vector_type_name##VectorExpand(arr, 1); \
        } \
        MemoryCopy(&arr->obj_arr[arr->count++], obj, sizeof(element_type)); \
        return arr->count - 1; \
    } \
    offset_type vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, offset_type count) { \
        if (arr->capacity <= arr->count + count) { \
            vector_type_name##VectorExpand(arr, count); \
        } \
        MemoryCopy(&arr->obj_arr[arr->count], obj, sizeof(element_type) * count); \
        arr->count+=count; \
        return arr->count - count; \
    } \
    element_type* vector_type_name##VectorGetTail(vector_type_name##Vector* arr) { \
        if (arr->count == 0) { \
            return NULL; \
        } \
        return &arr->obj_arr[arr->count-1]; \
    } \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* arr) { \
        if (arr->count == 0) { \
            return NULL; \
        } \
        return &arr->obj_arr[--arr->count]; \
    } \
    offset_type vector_type_name##VectorGetCount(vector_type_name##Vector* arr) { return arr->count; } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* arr) { return arr->capacity; } \
    element_type* vector_type_name##VectorIndex(vector_type_name##Vector* arr, offset_type index) { \
        return &arr->obj_arr[index]; \
    } \
    \
    LIBYUC_ALGORITHM_ARRAY_DEFINE(vector_type_name##Vector, offset_type, id_type, element_type, accessor, comparer, referencer); \
    element_type* vector_type_name##VectorIteratorLocate(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter, const element_type* obj) { \
        iter->cur_index = vector_type_name##VectorArrayFind(arr->obj_arr, arr->count, obj); \
        return vector_type_name##VectorIteratorIndex(arr, iter); \
    } \
    element_type* vector_type_name##VectorIteratorIndex(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter) { \
        if (iter->cur_index == referencer##_InvalidId || vector_type_name##VectorGetCount(arr) < iter->cur_index) return NULL; \
        return vector_type_name##VectorIndex(arr, iter->cur_index); \
    } \
    void vector_type_name##VectorIteratorDelete(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter) { \
        vector_type_name##VectorArrayDelete(arr->obj_arr, arr->count, iter->cur_index); \
    } \
    element_type* vector_type_name##VectorIteratorFirst(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter) { \
        iter->cur_index = 0; \
        return vector_type_name##VectorGetCount(arr) > 0 ? vector_type_name##VectorIndex(arr, 0) : NULL; \
    }; \
    element_type* vector_type_name##VectorIteratorNext(vector_type_name##Vector* arr, vector_type_name##VectorIterator* iter) { \
        if (iter->cur_index == referencer##_InvalidId || vector_type_name##VectorGetCount(arr) < iter->cur_index) return NULL; \
        ++iter->cur_index; \
        return vector_type_name##VectorIndex(arr, iter->cur_index); \
    }; \

#define LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT_Expand(ARR, OLD_CAPACITY, NEW_CAPACITY)
#define LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT

#define LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT

#define LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID

#define LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_VECTOR_H_