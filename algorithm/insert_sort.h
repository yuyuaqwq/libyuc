/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_ALGORITHM_INSERT_SORT_H_
#define CUTILS_ALGORITHM_INSERT_SORT_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* 插入排序
*/
#define CUTILS_ALGORITHM_INSERT_SORT_DEFINE(insert_sort_name, element_type, id_type) \
    void insert_sort_name##InsertSort(element_type* arr, id_type lenght) { \
        /* 将数组分成两个区域，一边是排序后数组，一边是未排序数组；
        * 最开始时，已排序数组区间只有一个成员，长度是1，未排序数组区间则是整个数组长度-1；
        * 从未排序数组区域逐个取出成员，然后在排序后数组区域中一一比对；
        * 找到比当前成员的值更大的值后，将其放到该成员前面，此位置后的成员就往后挪；
        * 整个排序过程保证排序后数组区域是有序的，直到排序结束。
        * 稳定排序算法，查找到相等的元素，将后来的插入到后面
        * 时间复杂度O(n^2)，空间复杂度O(1) */ \
        element_type key; \
        for (id_type i = 1; i < lenght; ++i) { \
            for (id_type j = i; j; --j) { \
                if (arr[j] < arr[j - 1]) { \
                    key = arr[j]; \
                    arr[j] = arr[j - 1]; \
                    arr[j - 1] = key; \
                } else { \
                    break; \
                } \
            } \
        } \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_INSERT_SORT_H_