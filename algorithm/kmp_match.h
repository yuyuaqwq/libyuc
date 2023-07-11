/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_KMP_MATCH_H_
#define LIBYUC_ALGORITHM_KMP_MATCH_H_

#include <libyuc/object.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
* https://www.bilibili.com/video/BV1AY4y157yL/?spm_id_from=333.337.search-card.all.click&vd_source=2f16cbe6e2d0a9708b5b399c0b862fe3
*/


// 构建next：
// 字符串为 abababc
// a
// next[0] = 0，长度为1不可能有前缀后缀
// ab
	// next[1] = 0，长度为2，不分前缀后缀
// aba
	// next[2] = 0，长度为3，a为前缀后缀的最长相等子串
// abab
	// next[3] = 1，长度为4，ab为前缀后缀的最长相等子串

// 快速构建：
static int* BuildNext(const char* pattern, size_t len) {
	int* next = ObjectCreateArray(int, len);
	next[0] = 0;
	int i = 1;		// i指向当前构建的好前缀末尾(也是后缀子串的末尾)
	int prefixLen = 0;		// 当前好前缀的最长相等前后缀长度，也用于访问当前前缀子串的末尾
	while (i < len) {
		if (pattern[prefixLen] == pattern[i]) {
			// 好前缀长度+1之后，后缀子串末尾能够与前缀子串末尾匹配，即 当前好前缀 的 最长相等前后缀 等于 上一好前缀 的 最长相等前后缀+1
			// 如 abcab+c，ab c ab c，c匹配c，2+1=3，组成abc abc
			next[i++] = ++prefixLen;
			continue;
		}
		// 即好前缀长度+1之后，新后缀子串末尾无法与新前缀子串末尾匹配了
		// 如 abcab+b，ab c ab b，b无法匹配c
		if (prefixLen == 0) {
			next[i++] = 0;
		}
		else {
			// 原则上是根据 可能存在更短相等前后缀长度 以及 原先的已经计算好的结果 来进行计算
			// 如aba c aba b，由于aba(前)必定等于aba(后)，aba(前)+b如果存在最长相等前后缀子串，也能反映到整个串中(即忽视中间的字符)(ab ab == ab .... ab)
			// 而aba的最长相等前后缀长度已经计算过了
			prefixLen = next[prefixLen - 1];
			// 等待下一次循环检查 a b a b 中 b == b
		}
	}
	return next;
}

// kmp思想：
// 在进行字符串匹配时匹配出好前缀，然后将好前缀截取出来(得到长度)，获取该长度的好前缀中的最长可匹配前缀/后缀子串
// next存放的就是各长度的好前缀中的最长可匹配前缀子串的长度
// 这样子一旦出现好前缀时，直接通过该好前缀的长度作为next的下标，即可获得该好前缀中最长可匹配前缀子串的长度了


// 匹配示例
// kmp是不回头的，所以匹配是O(n)的时间复杂度，加上O(m)构建next数组就是O(m+n)
// 113113113112
// ↑
// 112

// 113113113112
//  ↑
// 112

// 113113113112
//   ↑
// 112

// 此时不匹配，根据好前缀next[1](len2)移动子串指针为1：
// 113113113112
//   ↑
//  112

// 发现不匹配，好前缀next[0]=0，移动子串指针为0：
// 113113113112
//    ↑
//    112

// 继续


int KmpMatch(const char* main, size_t mainLen, const char* pattern, size_t patternLen) {
	int* next = BuildNext(pattern, patternLen);
	int i = 0;		// 主串指针
	int j = 0;		// 模式串指针
	int pos = -1;
	while (i < mainLen) {
		if (main[i] == pattern[j]) {
			i++; j++;
			if (j == patternLen) {
				pos = i - j;
				break;
			}
		}
		else if (j > 0) {
			j = next[j - 1];		// 根据next跳过可以跳过的字符
		}
		else {
			i++;		// 第一个字符就不匹配
		}
	}
	ObjectDelete(next);
	return pos;

}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_MERGE_SORT_H_