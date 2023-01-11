#ifndef CUTILS_CONTAINER_QUEUE_H_
#define CUTILS_CONTAINER_QUEUE_H_

#include "CUtils/object.h"
#include "CUtils/container/array.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Queue {
	Array arr;
	int head;		// head指向即将出队的元素索引
	int tail;		// tail指向最近入队的元素索引的下一个索引
} Queue;

//        t=h
// 队空：|   |   |   |
//             t   h
// 队满：| 3 |   | 1 | 2 |

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

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_QUEUE_H_