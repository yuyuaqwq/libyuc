/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
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