/*
* @yuyuaqwq - ÓãÓã
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* Çë±£Áô´ËÉùÃ÷
*/

#include <CUtils/algorithm/brute_force_match.h>

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