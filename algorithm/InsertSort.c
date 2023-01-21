/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

/*
* 插入排序
*/
void InsertSort(int * arr, int lenght) {
	// 将数组分成两个区域，一边是排序后数组，一边是未排序数组；最开始时，已排序数组区间只有一个成员，长度是1，未排序数组区间则是整个数组-1；从未排序数组区域逐个取出成员，然后在排序后数组区域中一一比对，找到比当前成员的值更大的值后，将其放到该成员前面，此位置后的成员就往后挪，整个排序过程保证排序后数组区域是有序的，直到排序结束。
	// 稳定排序算法，查找到相等的元素，将后来的插入到后面
	// 时间复杂度O(n^2)，空间复杂度O(1)
	int key;
	for (int i = 1; i < lenght; ++i) {
		for (int j = i; j; --j) {
			if (arr[j] < arr[j - 1]) {
				key = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = key;
			} else {
				break;
			}
		}
	}
}