/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "object.h"

void MemorySwap(void* buf1_, void* buf2_, size_t size) {
    uint8_t* buf1 = (uint8_t*)buf1_;
    uint8_t* buf2 = (uint8_t*)buf2_;
    for (size_t i = 0; i < size; i++) {
        uint8_t temp = buf1[i];
        buf1[i] = buf2[i];
        buf2[i] = temp;
    }
}

void MemoryCopyR(void* dst_, void* src_, size_t size) {
    uint8_t* dst = dst_;
    uint8_t* src = src_;
    while (size-- != 0) {
        dst[size] = src[size];
    }
}

int MemoryCmp(const void* buf1_, const void* buf2_, size_t size) {
    return memcmp(buf1_, buf2_, size);
}

int MemoryCmpR(const void* buf1_, const void* buf2_, size_t size) {
    uint8_t* buf1 = buf1_;
    uint8_t* buf2 = buf2_;
    while (size-- != 0) {
        int res = buf1[size] - buf2[size];
        if (res != 0) {
            return res;
        }
    }
    return 0;
}
