#ifndef CUTILS_QUEUE_H_
#define CUTILS_QUEUE_H_

#include "CUtils/container/object.h"
#include "CUtils/container/array.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ArrayQueue {
	Array arr;
	int head;		// head指向即将出队的元素索引
	int tail;		// tail指向最近入队的元素索引的下一个索引
} ArrayQueue;

//        t=h
// 队空：|   |   |   |
//             t   h
// 队满：| 3 |   | 1 | 2 |

void ArrayQueueInit(ArrayQueue* queue, size_t capacity, int objSize);
void ArrayQueueRelease(ArrayQueue* queue);
int ArrayQueueIndexRewind(ArrayQueue* queue, int index);
bool ArrayQueueIsEmpty(ArrayQueue* queue);
bool ArrayQueueIsFull(ArrayQueue* queue);
size_t ArrayQueueGetEntryCount(ArrayQueue* queue);
size_t ArrayQueueGetFreeCount(ArrayQueue* queue);

void ArrayQueueEnqueue(ArrayQueue* queue, void* obj);
void ArrayQueueEnqueueByCount(ArrayQueue* queue, void* obj, size_t count);
void* ArrayQueueDequeue(ArrayQueue* queue);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_VECTOR_H_