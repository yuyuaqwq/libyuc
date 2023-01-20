/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "array.h"

void ArrayExpand(Array* arr, size_t targetCount) {
    size_t oldCapacity = arr->capacity;

    if (arr->capacity == 0) {
        arr->capacity = 1;
    }
    while (arr->capacity < targetCount) {
        arr->capacity *= 2;
    }

    if (oldCapacity != arr->capacity) {
        void* newBuf = CreateObjArrByObjSize(arr->objSize, arr->capacity);
        if (arr->objArr) {
            MemoryCopy(newBuf, arr->objArr, arr->objSize * arr->count);
            DeleteObject_(arr->objArr);
        }
        arr->objArr = newBuf;
    }
}

void ArrayInit(Array* arr, size_t capacity, int objByteCount) {
    arr->capacity = 0;
    arr->count = 0;
    arr->objArr = NULL;
    arr->objSize = objByteCount;
    if (capacity != 0) {
        ArrayExpand(arr, capacity);
    }
}

void ArrayRelease(Array* arr) {
    if (arr->objArr) {
        DeleteObject_(arr->objArr);
        arr->objArr = NULL;
    }
    arr->capacity = 0;
    arr->count = 0;
}

int ArrayPushTail(Array* arr, void* obj) {
    if (arr->capacity <= arr->count) {
        ArrayExpand(arr, arr->count + 1);
    }
    MemoryCopy(ArrayAt(arr, arr->count++, void), obj, arr->objSize);
    return arr->count - 1;
}

void* ArrayPopTail(Array* arr) {
    if (arr->count == 0) {
        return NULL;
    }
    return ArrayAt(arr, --arr->count, void);
}

void ArraySwapEntry(Array* arr, int index1, int index2) {
    void* obj1 = ArrayAt(arr, index1, void);
    void* obj2 = ArrayAt(arr, index2, void);
    MemorySwap(obj1, obj2, arr->objSize);
}

size_t ArrayGetCount(Array* arr) {
    return arr->count;
}

void ArraySetCount(Array* arr, size_t count) {
    arr->count = count;
}

size_t ArrayGetCapacity(Array* arr) {
    return arr->capacity;
}

void ArraySetCapacity(Array* arr, size_t capacity) {
    arr->capacity = capacity;
}