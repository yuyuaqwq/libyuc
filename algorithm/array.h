/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_ARRAY_H_
#define LIBYUC_ALGORITHM_ARRAY_H_

#include <libyuc/basic.h>


#ifdef __cplusplus
extern "C" {
#endif


#define LIBYUC_ALGORITHM_ARRAY_DEFINE(array_name, id_type, element_type, key_type, accessor, comparer, referencer) \
    id_type array_name##ArrayFind(element_type* array, id_type count, const key_type* key) { \
        for (id_type i = 0; i < count; i++) { \
            if (comparer##_Equal(array, accessor##_GetKey(array, &array[i]), key)) return (id_type)i; \
        } \
        return referencer##_InvalidId; \
    } \
    void array_name##ArrayInsert(element_type* array, id_type count, id_type index, element_type* element) { \
        for(id_type i = count; i > index; i--) { \
            array[i] = array[i - 1]; \
        } \
        array[index] = *element; \
    } \
    void array_name##ArrayDelete(element_type* array, id_type count, id_type index) { \
        for(id_type i = index; i < count - 1; i++) { \
            array[i] = array[i + 1]; \
        } \
    } \
    /* 有序数组相关 */ \
    /* 基于BinarySearch的查找 */ \
    /*
    * 找到了返回正确下标，否则返回-1
    */ \
    id_type array_name##ArrayOrderFind(element_type* array, id_type first, id_type last, const key_type* key) { \
        id_type mid; \
        while (first <= last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Greater(array, accessor##_GetKey(array, &array[mid]), key)) { \
                last = mid - 1; \
            } \
            else if (comparer##_Less(array, accessor##_GetKey(array, &array[mid]), key)) { \
                first = mid + 1; \
            } \
            else { \
                return mid; \
            } \
        } \
        return -1; \
    } \
    /*
    * 找到了返回正确下标，否则返回第一个小于或大于key的元素下标
    * (通常返回第一个大于等于key的元素下标，若所有元素都小于key，则返回最后一个元素的下标)
    */ \
    id_type array_name##ArrayOrderFind_Range(element_type* array, id_type first, id_type last, const key_type* key) { \
        id_type mid = 0; \
        while (first < last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Less(array, accessor##_GetKey(array, &array[mid]), key)) first = mid + 1; \
            else last = mid; \
        } \
        return first; \
    } \
    void array_name##ArrayOrderPut(element_type* array, id_type count, element_type* element) { \
        id_type index = array_name##ArrayOrderFind_Range(array, 0, count - 1, (const key_type*)accessor##_GetKey(array, element)); \
    } \
    bool array_name##ArrayOrderDelete(element_type* array, id_type count, const key_type* key) { \
        id_type id = array_name##ArrayOrderFind(array, 0, count - 1, key); \
        if (id == referencer##_InvalidId) return false; \
        array_name##ArrayDelete(array, count, id); \
        return true; \
    }


#define LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT_GetKey(ARR, OBJ) (OBJ)
#define LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT

#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID_Equal(ARR, OBJ1, OBJ2) (false)
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID_Less(ARR, OBJ1, OBJ2) (false)
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID_Greater(ARR, OBJ1, OBJ2) (false)
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID

#define LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT_InvalidId (-1)
#define LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_INSERT_SORT_H_