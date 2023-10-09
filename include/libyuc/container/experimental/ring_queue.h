/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_QUEUE_H_
#define LIBYUC_CONTAINER_QUEUE_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

/*
* 环形队列
* 单读单写支持无锁实现
*/

typedef struct _Queue {
    Vector arr;
    int head;        // head指向即将出队的元素索引
    int tail;        // tail指向最近入队的元素索引的下一个索引
} Queue;

//        t=h
// 队空：|     |     |     |
//             t     h
// 队满：| 3 |     | 1 | 2 |

void QueueInit(Queue* queue, size_t capacity, int objSize);
void QueueRelease(Queue* queue);
int QueueIndexRewind(Queue* queue, int index);
bool QueueIsEmpty(Queue* queue);
bool QueueIsFull(Queue* queue);
size_t QueueGetEntryCount(Queue* queue);
size_t QueueGetFreeCount(Queue* queue);

void QueueEnqueue(Queue* queue, void* obj);
void QueueEnqueueByCount(Queue* queue, void* obj, size_t count);
void* QueueDequeue(Queue* queue);

static void QueueExpand(Queue* queue, size_t addCount) {
    Vector* arr = &queue->arr;
    VectorExpand(arr, addCount);
    if (queue->head > queue->tail) {
        size_t copyCount = arr->count - queue->head;
        if (copyCount != 0) {
            void* src = VectorAt(arr, queue->head);
            queue->head = arr->capacity - copyCount;
            void* dst = VectorAt(arr, queue->head);
            MemoryCopyR(dst, src, copyCount * arr->objSize);
        }
    }
    arr->count = arr->capacity;
}

void QueueInit(Queue* queue, size_t capacity, int objByteCount) {
    if (capacity == 1) {
        capacity = 2;        // 长度不能是1，否则会出现队满=队空的情况
    }
    VectorInit(&queue->arr, capacity, objByteCount);
    queue->arr.count = capacity;
    queue->head = 0;
    queue->tail = 0;
}

void QueueRelease(Queue* queue) {
    VectorRelease(&queue->arr);
    queue->head = 0;
    queue->tail = 0;
}

forceinline static int QueueIndexRewind(Queue* queue, int index) {
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

bool QueueEnqueue(Queue* queue, void* entry) {
    if (QueueIsFull(queue)) {
        return false;
    }
    MemoryCopy(VectorAt(&queue->arr, queue->tail), entry, queue->arr.objSize);
    queue->tail = QueueIndexRewind(queue, queue->tail + 1);
}

void* QueueDequeue(Queue* queue) {
    if (QueueIsEmpty(queue)) {
        return NULL;
    }
    int index = queue->head;
    void* entry = VectorAt(&queue->arr, index);
    queue->head = QueueIndexRewind(queue, queue->head + 1);
    return entry;
}


#endif // LIBYUC_CONTAINER_QUEUE_H_