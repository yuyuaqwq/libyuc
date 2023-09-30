/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_SELECT_SORT_H_
#define LIBYUC_ALGORITHM_SELECT_SORT_H_

#include <libyuc/basic.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* 选择排序
*/
void SelectSort(int* arr, int lenght) {
    // 每轮循环先取下标为0的成员当做基准值，往后比对，如果有比当前基准值更大的成员，就选其作为新基准值，到末端为止，将末端成员和基准值交换位置，末端-1，此时末端值就是该数组中最大的成员，重复上述步骤直到排序完成。
    // 不稳定排序算法，交换位置时可能导致被交换位置的元素和其他相等的元素的先后顺序改变
    // 时间复杂度O(n^2)，空间复杂度O(1)
    int key;
    int n;
    for (int i = 0; i < lenght; ++i) {
        n = 0;
        for (int j = 1; j < lenght - i; ++j) {
            if (arr[n] < arr[j]) {
                n = j;
            }
        }
        if (arr[n] > arr[lenght - i - 1]) {
            //防止两值相同时交换位置 
            key = arr[n];
            arr[n] = arr[lenght - i - 1];
            arr[lenght - i - 1] = key;
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_SELECT_SORT_H_