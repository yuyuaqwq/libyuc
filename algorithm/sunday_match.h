/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_SUNDAY_MATCH_H_
#define LIBYUC_ALGORITHM_SUNDAY_MATCH_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* sunday思想：
* 提前记录字符最后一次出现的位置(从后算起)，即构建shift数组
* 从后向前匹配，出现失配字符时，从shift中查找当前main中匹配子串末尾的下一字符，如果该字符未记录，则可以直接跳过当前匹配部分，i指向该字符的下一字符，否则将j对齐到失配字符，i回溯
* 
* 可以结合kmp算法降低最坏时间复杂度
*/
int SundayMatch(const char* main, size_t mainLen, const char* pattern, size_t patternLen) {
    int shift[256];
    for (int i = 0; i < sizeof(shift) / sizeof(int); i++) {
        shift[i] = patternLen + 1;        // 先假设所有字符都不存在，即跳过patternLen+1个字符
    }
    for (int i = 0; i < patternLen; i++) {
        shift[(unsigned char)(pattern[i])] = patternLen - i;
    }
    int i = 0;
    int j = 0;
    while (i < mainLen && i + patternLen <= mainLen) {
        if (main[i + j] == pattern[j]) {
            if (++j == patternLen) {
                return i;
            }
            continue;
        }
        i += shift[(unsigned char)(main[i + patternLen])];
        j = 0;
    }
    return -1;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_SUNDAY_MATCH_H_