/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_ALGORITHM_BRUTE_FORCE_MATCH_H_
#define CUTILS_ALGORITHM_BRUTE_FORCE_MATCH_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* BruteForce
*/
int BruteForceMatch(const char* main, size_t mainLen, const char* pattern, size_t patternLen) {
	for (int i = 0; i < mainLen && i + patternLen <= mainLen; i++) {
		int j = 0;
		for (; j < patternLen; j++) {
			if (main[i+j] != pattern[j]) {
				break;
			}
		}
		if (j == patternLen) {
			return i;
		}
	}
	return -1;
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_BRUTE_FORCE_MATCH_H_