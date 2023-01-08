#include "search.h"

/*
* 找到了返回正确下标，否则返回-1
*/
int BinarySearch(int* array, int low, int high, int key) {
	int mid = 0;
	while (low < high) {
		mid = low + (high - low) / 2;
		if (array[mid] < key) low = mid + 1;
		else if (array[mid] == key) return mid;
		else high = mid;
	}
	return -1;
}

/*
* 找到了返回正确下标，否则返回第一个小于或大于key的元素下标，(通常返回第一个大于key的元素下标，若所有元素都小于key，则返回最后一个元素的下标)
*/
int BinarySearch_Range(int* array, int low, int high, int key) {
	int mid = 0;
	while (low < high) {
		mid = low + (high - low) / 2;
		if (array[mid] < key) low = mid + 1;
		else high = mid;
	}
	return low;
}


int BinarySearch_Custom(void* objArr, int objSize, int keyFieldOffset, int keyFieldSize, int low, int high, void* key, CmpFunc cmpFunc) {
	int mid = 0;
	while (low < high) {
		mid = low + (high - low) / 2;
		void* curObj = ObjArrAt(objArr, mid, objSize);
		void* curKey = GetFieldByFieldOffset(curObj, keyFieldOffset, void);
		int res = cmpFunc(curKey, key, keyFieldSize);
		if (res < 0) low = mid + 1;
		else if (res == 0) return mid;
		else high = mid;
	}
	return -1;
}

int BinarySearch_Range_Custom(void* objArr, int objSize, int keyFieldOffset, int keyFieldSize, int low, int high, void* key, CmpFunc cmpFunc) {
	int mid = 0;
	while (low < high) {
		mid = low + (high - low) / 2;
		void* curObj = ObjArrAt(objArr, mid, objSize);
		void* curKey = GetFieldByFieldOffset(curObj, keyFieldOffset, void);
		if (cmpFunc(curKey, key, keyFieldSize) < 0) low = mid + 1;
		else high = mid;
	}
	return low;
}
