/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_BRUTE_FORCE_MATCH_H_
#define LIBYUC_ALGORITHM_BRUTE_FORCE_MATCH_H_

#include <libyuc/basic.h>

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


#endif // LIBYUC_ALGORITHM_BRUTE_FORCE_MATCH_H_