#include "queue.h"

static void QueueExpand(Queue* queue, size_t targetSize) {
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

void QueueInit(Queue* queue, size_t capacity, int objByteCount) {
	if (capacity == 1) {
		capacity = 2;		// 长度不能是1，否则会出现队满=队空的情况
	}
	ArrayInit(&queue->arr, capacity, objByteCount);
	queue->arr.count = capacity;
	queue->head = 0;
	queue->tail = 0;
}

void QueueRelease(Queue* queue) {
	ArrayRelease(&queue->arr);
	queue->head = 0;
	queue->tail = 0;
}

inline int QueueIndexRewind(Queue* queue, int index) {
	return index % queue->arr.count;
}

bool QueueIsEmpty(Queue* queue) {
	return queue->head == queue->tail;
}

bool QueueIsFull(Queue* queue) {
	return queue->arr.count == 0 || (queue->tail + 1) % queue->arr.count == queue->head;
}

size_t QueueGetEntryCount(Queue* queue) {
	if (queue->tail >= queue->head) {
		return queue->tail - queue->head;
	}
	else {
		return queue->tail + (queue->arr.count - queue->head);
	}
}

size_t QueueGetFreeCount(Queue* queue) {
	if (queue->arr.count == 0) {
		return 0;
	}
	return queue->arr.count - QueueGetEntryCount(queue) - 1;
}

void QueueEnqueue(Queue* queue, void* entry) {
	if (QueueIsFull(queue)) {
		QueueExpand(queue, QueueGetEntryCount(queue) + 1);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), entry, queue->arr.objSize);
	queue->tail++;
	queue->tail = QueueIndexRewind(queue, queue->tail);
}

void QueueEnqueueByCount(Queue* queue, void* entry, size_t count) {
	if (QueueGetFreeCount(queue) < count) {
		QueueExpand(queue, QueueGetEntryCount(queue) + count);
	}
	MemoryCopy(ArrayAt(&queue->arr, queue->tail, void), entry, queue->arr.objSize);
	queue->tail += count;
	queue->tail = QueueIndexRewind(queue, queue->tail);
}

void* QueueDequeue(Queue* queue) {
	if (QueueIsEmpty(queue)) {
		return NULL;
	}
	int index = queue->head;
	void* entry = ArrayAt(&queue->arr, index, void);
	queue->head++;
	queue->head = QueueIndexRewind(queue, queue->head);
	return entry;
}
