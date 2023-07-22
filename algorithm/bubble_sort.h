/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_BUBBLE_SORT_H_
#define LIBYUC_ALGORITHM_BUBBLE_SORT_H_

#include <libyuc/object.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* 冒泡排序
*/
void BubbleSort(int * arr, int lenght)  {
  // 每轮循环都比对相邻的两个成员，如果前面的成员大于后面的成员，就交换他们的位置，重复上述步骤直到排序完成。
  // 可以自己选择等大是否交换位置，因此是稳定排序算法
  // 时间复杂度O(n^2)，空间复杂度O(1)
  int key;
  bool exchange = true; //增加一个标记，若整轮都没有出现交换位置的情况，说明是有序，可以直接结束循环。
  for (int i = 0; i < lenght; ++i) {
    for (int j = 0; j < lenght - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        key = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = key;
        if (exchange) exchange = false; 
      }
    }
    if (exchange) break;
  }
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_HASH_MAP_H_