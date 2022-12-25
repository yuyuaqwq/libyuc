#include "array.h"

void ArrayExpand(Array* arr, size_t targetSize) {
	size_t oldCapacity = arr->capacity;

	if (arr->capacity == 0) {
		arr->capacity = 1;
	}
	while (arr->capacity < targetSize) {
		arr->capacity *= 2;
	}

	if (oldCapacity != arr->capacity) {
		void* newBuf = CreateMultipleObjByByteCount(arr->objSize, arr->capacity);
		if (arr->objArr) {
			MemoryCopy(newBuf, arr->objArr, arr->objSize * arr->count);
			DeleteObject(arr->objArr);
		}
		arr->objArr = newBuf;
	}
}

void ArrayInit(Array* arr, size_t capacity, int objByteCount) {
	ArrayRelease(arr);
	arr->objSize = objByteCount;
	if (capacity != 0) {
		ArrayExpand(arr, capacity);
	}
}

void ArrayRelease(Array* arr) {
	if (arr->objArr) {
		DeleteObject(arr->objArr);
		arr->objArr = NULL;
	}
	arr->capacity = 0;
	arr->count = 0;
}

void ArrayPushTail(Array* arr, void* obj) {
	if (arr->capacity <= arr->count) {
		ArrayExpand(arr, arr->count + 1);
	}
	MemoryCopy(ArrayAt(arr, void, arr->count++), obj, arr->objSize);
}

void* ArrayPopTail(Array* arr) {
	if (arr->count == 0) {
		return NULL;
	}
	return ArrayAt(arr, void, arr->count--);
}

void ArraySwapEntry(Array* arr, int index1, int index2) {
	void* obj1 = ArrayAt(arr, void, index1);
	void* obj2 = ArrayAt(arr, void, index2);
	MemorySwap(obj1, obj2, arr->objSize);
}