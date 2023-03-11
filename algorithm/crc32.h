/*
https://blog.csdn.net/gongmin856/article/details/77101397
*/

#ifndef CUTILS_ALGORITHM_KMP_MATCH_H_
#define CUTILS_ALGORITHM_KMP_MATCH_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

int KmpMatch(const char* main, size_t mainLen, const char* pattern, size_t patternLen);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_MERGE_SORT_H_