#include "queue.h"

static void ArrayQueueExpand(ArrayQueue* queue) {
	Array* arr = &queue->arr;
	ArrayExpand(arr, arr->size + 2);
	if (queue->head > queue->tail) {
		size_t copyCount = arr->size - queue->head;
		if (copyCount != 0) {
			void* src = ArrayAt(arr, void, queue->head);
			queue->head = arr->capacity - copyCount;
			void* dst = ArrayAt(arr, void, queue->head);
			MemoryCopyR(dst, src, copyCount * arr->objByteCount);
		}
	}
	arr->size = arr->capacity;
}

void ArrayQueueInit(ArrayQueue* queue, size_t capacity, int objByteCount) {
	if (capacity == 1) {
		capacity = 2;		// 长度不能是1，否则会出现队满=队空的情况
	}
	ArrayInit(&queue->arr, capacity, objByteCount);
	queue->arr.size = capacity;
	queue->head = 0;
	queue->tail = 0;
}

void ArrayQueueRelease(ArrayQueue* queue) {
	ArrayRelease(&queue->arr);
	queue->head = 0;
	queue->tail = 0;
}

bool ArrayQueueIsEmpty(ArrayQueue* queue) {
	return queue->head == queue->tail;
}

bool ArrayQueueIsFull(ArrayQueue* queue) {
	return queue->arr.size == 0 || (queue->tail + 1) % queue->arr.size == queue->head;
}

void ArrayQueueEnqueue(ArrayQueue* queue, void* obj) {
	if (ArrayQueueIsFull(queue)) {
		ArrayQueueExpand(queue);
	}
	MemoryCopy(ArrayAt(&queue->arr, void, queue->tail), obj, queue->arr.objByteCount);
	queue->tail++;
	queue->tail %= queue->arr.size;
}

void* ArrayQueueDequeue(ArrayQueue* queue) {
	if (ArrayQueueIsEmpty(queue)) {
		return NULL;
	}
	int index = queue->head;
	void* ret = ArrayAt(&queue->arr, void, index);
	queue->head++;
	queue->head %= queue->arr.size;
	return ret;
}
