/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_VECTOR_H_
#define LIBYUC_CONTAINER_VECTOR_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif


#define LIBYUC_CONTAINER_VECTOR_DECLARATION(vector_type_name, element_type) \
  typedef struct _##vector_type_name##Vector { \
    size_t count; \
    size_t capacity; \
    bool is_vector_alloc; \
    element_type* obj_arr; \
  } vector_type_name##Vector; \
  void vector_type_name##VectorInit(vector_type_name##Vector* arr, size_t capacity, element_type* external_arr); \
  void vector_type_name##VectorRelease(vector_type_name##Vector* arr); \
  ptrdiff_t vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, size_t count); \
  ptrdiff_t vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj); \
  element_type* vector_type_name##VectorGetTail(vector_type_name##Vector* arr); \
  element_type* vector_type_name##VectorPopTail(vector_type_name##Vector* arr); \

#define LIBYUC_CONTAINER_VECTOR_DEFINE(vector_type_name, element_type, allocator, callbacker) \
  void vector_type_name##VectorResetCapacity(vector_type_name##Vector* arr, size_t capacity) { \
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
  void vector_type_name##VectorExpand(vector_type_name##Vector* arr, size_t add_count) { \
    size_t old_capacity = arr->capacity; \
    size_t cur_capacity = old_capacity; \
    size_t target_count = cur_capacity + add_count; \
    if (cur_capacity == 0) { \
      cur_capacity = 1; \
    } \
    while (cur_capacity < target_count) { \
      cur_capacity *= 2; \
    } \
    vector_type_name##VectorResetCapacity(arr, cur_capacity); \
    callbacker##_Expand(arr, old_capacity, cur_capacity); \
  } \
  void vector_type_name##VectorInit(vector_type_name##Vector* arr, size_t capacity, element_type* external_arr) { \
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
  ptrdiff_t vector_type_name##VectorPushTail(vector_type_name##Vector* arr, const element_type* obj) { \
    if (arr->capacity <= arr->count) { \
      vector_type_name##VectorExpand(arr, 1); \
    } \
    MemoryCopy(&arr->obj_arr[arr->count++], obj, sizeof(element_type)); \
    return arr->count - 1; \
  } \
  ptrdiff_t vector_type_name##VectorPushMultipleTail(vector_type_name##Vector* arr, const element_type* obj, size_t count) { \
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

#define LIBYUC_CONTAINER_VECTOR_DEFAULT_CALLBACKER_Expand(ARR, OLD_CAPACITY, NEW_CAPACITY)
#define LIBYUC_CONTAINER_VECTOR_DEFAULT_CALLBACKER LIBYUC_CONTAINER_VECTOR_DEFAULT_CALLBACKER

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_VECTOR_H_