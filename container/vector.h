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

#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DECLARATION_1(vector_type_name, element_type, static_count) \
    element_type obj_arr[static_count];
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DECLARATION_2(vector_type_name, offset_type) \
    vector_type_name##Vector* vector_type_name##VectorCreate(); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector);\
    void vector_type_name##VectorInit(vector_type_name##Vector* arr); 
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    vector_type_name##Vector* vector_type_name##VectorCreate() { \
        return allocator##_Create(NULL, vector_type_name##Vector); \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* vector) { \
        allocator##_Release(vector, vector); \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr) { \
        arr->count = 0; \
    } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* arr) { \
        return sizeof(arr->obj_arr) / sizeof(arr->obj_arr[0]); \
    }
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC_DEFINE_2(vector_type_name, expand_count) \
    return NULL;
#define LIBYUC_CONTAINER_VECTOR_MODE_STATIC LIBYUC_CONTAINER_VECTOR_MODE_STATIC

#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DECLARATION_1(vector_type_name, element_type, static_count) \
    offset_type capacity; \
    element_type* obj_arr;
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DECLARATION_2(vector_type_name, offset_type) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* arr, offset_type capacity); \
    void vector_type_name##VectorExpand(vector_type_name##Vector* arr, offset_type add_count); \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, offset_type capacity); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr);
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* arr, offset_type capacity) { \
        element_type* new_buf = allocator##_CreateMultiple(arr, element_type, capacity); \
        if (arr->obj_arr) { \
            MemoryCopy(new_buf, arr->obj_arr, sizeof(element_type) * arr->count); \
            allocator##_Release(arr, arr->obj_arr); \
        } \
        arr->obj_arr = new_buf; \
        arr->capacity = capacity; \
    } \
    void vector_type_name##VectorExpand(vector_type_name##Vector* arr, offset_type add_count) { \
        offset_type old_capacity = vector_type_name##VectorGetCapacity(arr); \
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
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, offset_type capacity) { \
        arr->obj_arr = NULL; \
        arr->capacity = capacity; \
        if (capacity != 0) { \
            vector_type_name##VectorResetCapacity(arr, capacity); \
        } \
        arr->count = 0; \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr) { \
        if (arr->obj_arr) { \
            allocator##_Release(arr, arr->obj_arr); \
            arr->obj_arr = NULL; \
        } \
        arr->capacity = 0; \
        arr->count = 0; \
    } \
    offset_type vector_type_name##VectorGetCapacity(vector_type_name##Vector* arr) { \
        return vector_type_name##VectorGetCapacity(arr); \
    }
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC_DEFINE_2(vector_type_name, expand_count) \
    vector_type_name##VectorExpand(arr, expand_count);
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC

#define LIBYUC_CONTAINER_VECTOR_DECLARATION(vector_type_name, mode, offset_type, id_type, element_type, static_count) \
    typedef struct _##vector_type_name##Vector { \
        offset_type count; \
        mode##_DECLARATION_1(vector_type_name, element_type, static_count) \
    } vector_type_name##Vector; \
    typedef struct _##vector_type_name##VectorIterator { \
        offset_type cur_index; \
    } vector_type_name##VectorIterator; \
    mode##_DECLARATION_2(vector_type_name, offset_type) \
    element_type* vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, offset_type count); \
    element_type* vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj); \
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

#define LIBYUC_CONTAINER_VECTOR_DEFINE(vector_type_name, mode, offset_type, id_type, element_type, accessor, allocator, callbacker, comparer, referencer) \
    mode##_DEFINE_1(vector_type_name, offset_type, element_type, allocator, callbacker) \
    element_type* vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj) { \
        if (vector_type_name##VectorGetCapacity(arr) <= arr->count) { \
            mode##_DEFINE_2(vector_type_name, 1) \
        } \
        MemoryCopy(&arr->obj_arr[arr->count++], obj, sizeof(element_type)); \
        return vector_type_name##VectorIndex(arr, arr->count - 1); \
    } \
    element_type* vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, offset_type count) { \
        if (vector_type_name##VectorGetCapacity(arr) <= arr->count + count) { \
            mode##_DEFINE_2(vector_type_name, count) \
        } \
        MemoryCopy(&arr->obj_arr[arr->count], obj, sizeof(element_type) * count); \
        arr->count+=count; \
        return vector_type_name##VectorIndex(arr, arr->count - count); \
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

#define LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT_InvalidId LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT_InvalidId
#define LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_VECTOR_H_