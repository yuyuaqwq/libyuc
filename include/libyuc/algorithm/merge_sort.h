/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_MERGE_SORT_H_
#define LIBYUC_ALGORITHM_MERGE_SORT_H_

#include <libyuc/basic.h>

/*
* 归并排序
* left填0，right填数组成员数 - 1
*/
void MergeSort(int * arr, int left, int right) {
    // 将数组折半，再分别对两边数组再次折半，重复这个步骤，直到两边都只有1个成员，创建一个两边数组未分半时长度的数组，随后逐个比较两边数组的成员，小的一边就放进去，直到两个数组的成员全部都放到新数组中，新数组有序，将其放回两边数组未分半的数组，继续往上合并，直到排序完成。
    // 稳定排序算法，合并时左半边数组优先放入即可
    // 时间复杂度O(nlogn)，空间复杂度O(n)

    if (left >= right) return;
    
    // 递归折半数组
    int half = (left + right) / 2;
    MergeSort(arr, left, half);
    MergeSort(arr, half+1, right);
    
    // 折半完成，开始归并
    int lenght = right - left + 1;
    int* temp = ObjectCreateArray(int, lenght);
    int i = left;
    int j = half + 1;
    for (int n = 0; n < lenght; ++n) {
        //根据i和j指向的成员的大小选择将哪个放入临时数组
        if (j > right || i <= half && arr[i] <= arr[j]) {
            temp[n] = arr[i];
            ++i;
        }
        else {
            temp[n] = arr[j];
            ++j;
        }
    }
    for (int n = 0,i = left; n < lenght; ++n,++i) {
        arr[i] = temp[n];
    }
    ObjectDelete(temp);
}

#endif // LIBYUC_ALGORITHM_MERGE_SORT_H_