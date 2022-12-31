#include "queue.h"

static void ArrayQueueExpand(ArrayQueue* queue) {
	Array* arr = &queue->arr;
	ArrayExpand(arr, arr->count + 2);
	if (queue->head > queue->tail) {
		size_t copyCount = arr->count - queue->head;
		if (copyCount != 0) {
			void* src = ArrayAt(arr, queue->head, void);
			queue->head = arr->capacity - copyCount;
			void* dst = ArrayAt(arr, queue->head, void);
			MemoryCopyR(dst, src, copyCount * arr->objSize);
		}
	}
	arr->count = arr->capacity;
}

void ArrayQueueInit(ArrayQueue* queue, size_t capacity, int objByteCount) {
	if (capacity == 1) {
		capacity = 2;		// 长度不能是1，否则会出现队满=队空的情况
	}
	ArrayInit(&queue->arr, capacity, objByteCount);
	queue->arr.count = capacity;
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
	return queue->arr.count == 0 || (queue->tail + 1) % queue->arr.count == queue->head;
}

void ArrayQueueEnqueue(ArrayQueue* queue, void* obj) {
	if (ArrayQueueIsFull(queue)) {
		ArrayQueueExpand(queue);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), obj, queue->arr.objSize);
	queue->tail++;
	queue->tail %= queue->arr.count;
}

void ArrayQueueEnqueueByCount(ArrayQueue* queue, void* obj, size_t count) {
	if (ArrayQueueIsFull(queue)) {
		ArrayQueueExpand(queue);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), obj, queue->arr.objSize);
	queue->tail += count;
	queue->tail %= queue->arr.count;
}

void* ArrayQueueDequeue(ArrayQueue* queue) {
	if (ArrayQueueIsEmpty(queue)) {
		return NULL;
	}
	int index = queue->head;
	void* ret = ArrayAt(&queue->arr, index, void);
	queue->head++;
	queue->head %= queue->arr.count;
	return ret;
}

size_t ArrayQueueGetCount(ArrayQueue* queue) {
	if (queue->tail >= queue->head) {
		return queue->tail - queue->head;
	} else {
		return queue->tail + (queue->arr.count - queue->head);
	}
}