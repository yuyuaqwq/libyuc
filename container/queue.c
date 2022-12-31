#include "queue.h"

static void ArrayQueueExpand(ArrayQueue* queue, size_t targetSize) {
	Array* arr = &queue->arr;
	ArrayExpand(arr, targetSize + 1);
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

inline int ArrayQueueIndexRewind(ArrayQueue* queue, int index) {
	return index % queue->arr.count;
}

bool ArrayQueueIsEmpty(ArrayQueue* queue) {
	return queue->head == queue->tail;
}

bool ArrayQueueIsFull(ArrayQueue* queue) {
	return queue->arr.count == 0 || (queue->tail + 1) % queue->arr.count == queue->head;
}

size_t ArrayQueueGetEntryCount(ArrayQueue* queue) {
	if (queue->tail >= queue->head) {
		return queue->tail - queue->head;
	}
	else {
		return queue->tail + (queue->arr.count - queue->head);
	}
}

size_t ArrayQueueGetFreeCount(ArrayQueue* queue) {
	if (queue->arr.count == 0) {
		return 0;
	}
	return queue->arr.count - ArrayQueueGetEntryCount(queue) - 1;
}

void ArrayQueueEnqueue(ArrayQueue* queue, void* entry) {
	if (ArrayQueueIsFull(queue)) {
		ArrayQueueExpand(queue, ArrayQueueGetEntryCount(queue) + 1);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), entry, queue->arr.objSize);
	queue->tail++;
	queue->tail = ArrayQueueIndexRewind(queue, queue->tail);
}

void ArrayQueueEnqueueByCount(ArrayQueue* queue, void* entry, size_t count) {
	if (ArrayQueueGetFreeCount(queue) < count) {
		ArrayQueueExpand(queue, ArrayQueueGetEntryCount(queue) + count);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), entry, queue->arr.objSize);
	queue->tail += count;
	queue->tail = ArrayQueueIndexRewind(queue, queue->tail);
}

void* ArrayQueueDequeue(ArrayQueue* queue) {
	if (ArrayQueueIsEmpty(queue)) {
		return NULL;
	}
	int index = queue->head;
	void* entry = ArrayAt(&queue->arr, index, void);
	queue->head++;
	queue->head = ArrayQueueIndexRewind(queue, queue->head);
	return entry;
}
