/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_VECTOR_H_
#define CUTILS_CONTAINER_VECTOR_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif


#define CUTILS_CONTAINER_VECTOR_DECLARATION(vector_type_name, element_type) \
    typedef struct _##vector_type_name##Vector { \
        size_t count; \
        size_t capacity; \
        element_type* obj_arr; \
    } vector_type_name##Vector; \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, size_t count, bool create); \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr); \
    ptrdiff_t vector_type_name##VectorPushTail(vector_type_name##Vector* arr, element_type* obj); \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* arr); \

#define CUTILS_CONTAINER_VECTOR_DEFINE(vector_type_name, element_type, allocator) \
    void vector_type_name##VectorResetCapacity(vector_type_name##Vector* arr, size_t capacity) { \
        element_type* new_buf = allocator##_CreateMultiple(element_type, capacity); \
        if (arr->obj_arr) { \
            MemoryCopy(new_buf, arr->obj_arr, sizeof(element_type) * arr->count); \
            allocator##_Delete(arr->obj_arr); \
        } \
        arr->obj_arr = new_buf; \
        arr->capacity = capacity; \
    } \
    void vector_type_name##VectorExpand(vector_type_name##Vector* arr, size_t add_count) { \
        size_t cur_capacity = arr->capacity; \
        size_t target_count = cur_capacity + add_count; \
        if (cur_capacity == 0) { \
            cur_capacity = 1; \
        } \
        while (cur_capacity < target_count) { \
            cur_capacity *= 2; \
        } \
        vector_type_name##VectorResetCapacity(arr, cur_capacity); \
    } \
    void vector_type_name##VectorInit(vector_type_name##Vector* arr, size_t count, bool create) { \
        arr->count = count; \
        arr->obj_arr = NULL; \
        if (count != 0 && create) { \
            vector_type_name##VectorResetCapacity(arr, count); \
        } \
        else { \
            arr->capacity = count; \
        } \
    } \
    void vector_type_name##VectorRelease(vector_type_name##Vector* arr) { \
        if (arr->obj_arr) { \
            allocator##_Delete(arr->obj_arr); \
            arr->obj_arr = NULL; \
        } \
        arr->capacity = 0; \
        arr->count = 0; \
    } \
    ptrdiff_t vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj) { \
        if (arr->capacity <= arr->count) { \
            vector_type_name##VectorExpand(arr, 1); \
        } \
        MemoryCopy(&arr->obj_arr[arr->count++], obj, sizeof(element_type)); \
        return arr->count - 1; \
    } \
    element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* arr) { \
        if (arr->count == 0) { \
            return NULL; \
        } \
        return &arr->obj_arr[--arr->count]; \
    } \


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_VECTOR_H_