/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_ALGORITHM_HEAP_SORT_H_
#define CUTILS_ALGORITHM_HEAP_SORT_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* 下沉
*/
void Down(int* arr, int lenght, int temp){
	int child = temp * 2 + 1;
	int key;
	while (child < lenght) {
		if (arr[temp] < arr[child]) {
			if (lenght > child + 1 && arr[child] < arr[child + 1]) {
				++child;
			}
		}
		else if (lenght > child + 1 && arr[temp] < arr[++child]);
		else break;
		key = arr[temp];
		arr[temp] = arr[child];
		arr[child] = key;
		temp = child;
		child = child * 2 + 1;
	}
}

/*
* 堆排序
*/
void HeapSort(int * arr, int lenght) {
	// 基于原数组建立一个堆，对下标n/2(n为数组成员)-1～0的数组成员依次进行自上而下的堆化，n/2～n是叶子节点，无需堆化，堆化完成后，数组满足大顶堆的特性，再将堆顶元素和最后一个元素交换，然后将[此时]的堆顶元素进行自上而下的堆化，放到合适的位置，再将[此时]的堆顶元素与最后一个元素-1交换，重复该步骤，直到所有元素交换且堆化完成。
	// 不稳定排序算法，堆化的交换可能导致等值的元素前后位置关系就可能发生改变。
	// 时间复杂度O(nlogn)，空间复杂度O(1)
	// 
	// 堆化建堆
	int key;
	for (int i = lenght / 2 - 1; i >= 0; --i) {
		Down(arr, lenght, i);
	}

	//排序
	for (int i = lenght - 1; i > 0; --i) {
		key = arr[0];
		arr[0] = arr[i];
		arr[i] = key;
		Down(arr, i, 0);
	}
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_HASH_MAP_H_