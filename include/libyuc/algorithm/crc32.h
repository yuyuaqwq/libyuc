/*
https://blog.csdn.net/gongmin856/article/details/77101397
*/

#ifndef LIBYUC_ALGORITHM_KMP_MATCH_H_
#define LIBYUC_ALGORITHM_KMP_MATCH_H_

#include <libyuc/basic.h>

uint32_t Crc32(const void*buf, size_t size);
uint32_t Crc32Start();
uint32_t Crc32Continue(uint32_t crc, const void* buf, size_t size);
uint32_t Crc32End(uint32_t crc);


#endif // LIBYUC_ALGORITHM_MERGE_SORT_H_