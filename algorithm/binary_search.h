/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_ALGORITHM_SEARCH_H_
#define CUTILS_ALGORITHM_SEARCH_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_ALGORITHM_SEARCH_DECLARATION(binary_search_func_name, obj_type, key_type, id_type) \
	id_type binary_search_func_name##BinarySearch(obj_type* arr, id_type first, id_type last, key_type element); \
    id_type binary_search_func_name##BinarySearch_Range(obj_type* arr, id_type first, id_type last, key_type key); \

// 访问器需提供_GetKey方法
#define CUTILS_ALGORITHM_SEARCH_DEFINE(binary_search_func_name, obj_type, key_type, id_type, accessor, indexer, comparer) \
    /*
    * 找到了返回正确下标，否则返回-1
    */ \
	id_type binary_search_func_name##BinarySearch(obj_type* arr, id_type first, id_type last, key_type* key) { \
        id_type mid; \
        while (first <= last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Greater(arr, accessor##_GetKey(arr, &arr[mid]), key)) { \
                last = mid - 1; \
            } \
            else if (comparer##_Less(arr, accessor##_GetKey(arr, &arr[mid]), key)) { \
                first = mid + 1; \
            } \
            else { \
                return mid; \
            } \
        } \
        return -1; \
	} \
    /*
    * 找到了返回正确下标，否则返回第一个小于或大于key的元素下标，(通常返回第一个大于key的元素下标，若所有元素都小于key，则返回最后一个元素的下标)
    */ \
    id_type binary_search_func_name##BinarySearch_Range(obj_type* arr, id_type first, id_type last, key_type* key) { \
        id_type mid = 0; \
        while (first < last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Less(, accessor##_GetKey(arr, &arr[mid]), key)) first = mid + 1; \
            else last = mid; \
        } \
        return first; \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_SEARCH_H_