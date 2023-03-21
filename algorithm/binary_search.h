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

#define CUTILS_ALGORITHM_SEARCH_DECLARATION(binary_search_func_name, obj_type, key_type) \
	int32_t binary_search_func_name##BinarySearch(obj_type* arr, int32_t first, int32_t last, key_type element); \
    int32_t binary_search_func_name##BinarySearch_Range(obj_type* arr, int32_t first, int32_t last, key_type key); \

// 访问器需提供_GetKey方法
#define CUTILS_ALGORITHM_SEARCH_DEFINE(binary_search_func_name, obj_type, key_type, accessor, indexer, comparer) \
    /*
    * 找到了返回正确下标，否则返回-1
    */ \
	int32_t binary_search_func_name##BinarySearch(obj_type* arr, int32_t first, int32_t last, key_type* key) { \
        int32_t mid; \
        while (first <= last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Greater(accessor##_GetKey(indexer(arr, mid)), *key)) { \
                last = mid - 1; \
            } \
            else if (comparer##_Less(accessor##_GetKey(indexer(arr, mid)), *key)) { \
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
    int32_t binary_search_func_name##BinarySearch_Range(element_type* arr, int32_t first, int32_t last, key_type* key) { \
        int mid = 0; \
        while (first < last) { \
            mid = first + (last - first) / 2; \
            if (comparer##_Less(accessor##_GetKey(indexer(arr, mid)), *key)) first = mid + 1; \
            else last = mid; \
        } \
        return first; \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_SEARCH_H_