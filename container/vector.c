#include "vector.h"


void VectorExpand(Vector* vector, size_t targetCount) {
	size_t oldCapacity = vector->capacity;

	if (vector->capacity == 0) {
		vector->capacity = 1;
	}
	while (vector->capacity < targetCount) {
		vector->capacity *= 2;
	}

	if (oldCapacity != vector->capacity) {
		void* newBuf = CreateMultipleObjByCount(void*, vector->capacity);
		if (vector->objPtrArr) {
			MemoryCopy(newBuf, vector->objPtrArr, sizeof(uintptr_t) * vector->size);
			DeleteObject_(vector->objPtrArr);
		}
		vector->objPtrArr = newBuf;
	}
}

void VectorInit(Vector* vector, size_t capacity) {
	VectorRelease(vector);
	if (capacity != 0) {
		VectorExpand(vector, capacity);
	}
}

void VectorRelease(Vector* vector) {
	if (vector->objPtrArr) {
		for (int i = 0; i < vector->size; i++) {
			DeleteObject_(vector->objPtrArr[i]);
		}
		DeleteObject_(vector->objPtrArr);
		vector->objPtrArr = NULL;
	}
	vector->capacity = 0;
	vector->size = 0;
}

void VectorPushTail(Vector* vector, void* obj) {
	if (vector->capacity <= vector->size) {
		VectorExpand(vector, vector->size + 1);
	}
	vector->objPtrArr[vector->size++] = obj;
}

void* VectorPopTail(Vector* vector) {
	return vector->objPtrArr[vector->size--];
}