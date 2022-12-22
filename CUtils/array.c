#include "array.h"

static void ArrayExpand(Array* arr, size_t targetCapacity) {
	size_t oldCapacity = arr->capacity;

	if (arr->capacity == 0) {
		arr->capacity = 1;
	}
	while (arr->capacity > targetCapacity) {
		arr->capacity *= 2;
	}

	if (oldCapacity != arr->capacity) {
		void* newBuf = CreateMultipleObjFromByteCount(arr->objByteCount, arr->capacity);
		if (arr->objArr) {
			MemoryCopy(newBuf, arr->objArr, arr->objByteCount * arr->size);
			DeleteObj(arr->objArr);
		}
		arr->objArr = newBuf;
	}
}

void ArrayInit(Array* arr, int typeSize, size_t capacity) {
	ArrayRelease(arr);
	arr->objByteCount = typeSize;
	if (capacity != 0) {
		ArrayExpand(arr, capacity);
	}
}

void ArrayRelease(Array* arr) {
	if (arr->objArr) {
		DeleteObj(arr->objArr);
		arr->objArr = NULL;
	}
	arr->capacity = 0;
	arr->size = 0;
}

void ArrayPushBack(Array* arr, void* obj) {
	if (arr->capacity <= arr->size) {
		ArrayExpand(arr, arr->size + 1);
	}
	MemoryCopy(ArrayAt(arr, void, arr->size++), obj, arr->objByteCount);
}

void* ArrayPopBack(Array* arr) {
	return ArrayAt(arr, void, arr->size--);
}