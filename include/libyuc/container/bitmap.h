/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_BITMAP_H_
#define LIBYUC_CONTAINER_BITMAP_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/bit.h>
#include <libyuc/container/vector.h>



/*
* 位图
*/

typedef struct Bitmap {
    size_t count;
    uint8_t byte_array[];
} Bitmap;
/*
* 必须是8的倍数
*/
void BitmapInit(Bitmap* bitmap, size_t bit_count) {
    size_t byte_count = bit_count / 8 + (bit_count % 8 ? 1 : 0);
    for (size_t i = 0; i < byte_count; i++) {
        bitmap->byte_array[i] = 0;
    }
}
void BitSetMultiple(Bitmap* bitmap, size_t bit_idx, size_t bit_count, bool val) {
    size_t byte_idx = bit_idx / 8;
    bit_idx += (bit_count / 8) * 8;
    while (bit_count > 8 && byte_idx < bitmap->count) {
        bitmap->byte_array[byte_idx] = (val ? 0xff : 0);
        bit_count--;
        byte_idx++;
    }
    while (bit_count > 0 && bit_idx < (bitmap->count * 8)) {
        BitSet(bitmap->byte_array, bit_idx, val);
        bit_count--;
        bit_idx++;
    }
}
size_t BitmapFindBit(Bitmap* bitmap, size_t bit_start, bool val) {
    size_t bit_end = bit_start % 8;
    if (bit_end > 0) {
        bit_end = bit_start - bit_end + 8;
    } else {
        bit_end = bit_start;
    }
        size_t byte_start = bit_start / 8;
    if (bit_end > bit_start) {
        byte_start++;
    }
    /* 先单独比较前面的位 */
    while (bit_start < bit_end) {
        if (BitGet(bitmap->byte_array, bit_start) == val) {
            return bit_start;
        }
        bit_start++;
    }
    size_t byte_end = bitmap->count;
    /* 再进行字节比较 */
    while (byte_start < byte_end) {
        if (bitmap->byte_array[byte_start] != (val ? 0 : 0xff)) {
            break;
        }
        byte_start++;
    }
    if (byte_start == byte_end) {
        return -1;
    }
    size_t bit_off = 0;
    uint8_t byte = bitmap->byte_array[byte_start];
    /* 不断移位有效位，并比较，直到找到第一个有/无效位(从左往右) */
    while (((uint8_t)(0x80 >> bit_off) & byte) == (val ? 0 : (0x80 >> bit_off))) {
        bit_off++;
    }
    bit_start = (byte_start * 8 + bit_off);
    return bit_start;
}
size_t BitmapAlloc(Bitmap* bitmap, size_t bit_count) {
    size_t bit_start = 0;
    bit_start = BitmapFindBit(bitmap, bit_start, false);
    if (bit_start == -1) {
        return -1;
    }
    if (bit_count == 1) {
        BitSet(bitmap->byte_array, bit_start, true);
        return bit_start;
    }
    size_t bit_end = (bitmap->count * 8) - bit_start;        /* 剩余可检查位数 */
    size_t bit_cur = bit_start;
    size_t free_count = 1;
    /* 简化实现的算法，逐位比较，性能较低 */
    while (bit_end-- > 0) {
        if (BitGet(bitmap->byte_array, ++bit_cur) == false) {
            free_count++;
        } else {
            free_count = 0;        /* 被有效位阻断，重新开始记录 */
        }
        if (bit_count == free_count) {
            bit_start = bit_cur - bit_count + 1;
            BitSetMultiple(bitmap, bit_start, bit_count, true);
            return bit_start;
        }
    }
    return -1;
}
void BitmapFree(Bitmap* bitmap, size_t bit_idx, size_t bit_count) {
    BitSetMultiple(bitmap, bit_idx, bit_count, false);
}
size_t BitmapGetMaxFreeCount(Bitmap* bitmap) {
    size_t bit_start = 0;
    bit_start = BitmapFindBit(bitmap, bit_start, false);
    if (bit_start == -1) {
        return 0;
    }
    size_t bit_end = (bitmap->count * 8) - bit_start;        /* 剩余可检查位数 */
    size_t bit_cur = bit_start;
    size_t free_count = 1;
    size_t max_free_count = 1;
    /* 简化实现的算法，逐位比较，性能较低 */
    while (bit_end-- > 0) {
        if (BitGet(bitmap->byte_array, ++bit_cur) == false) {
            free_count++;
        }
        else {
            if (free_count > max_free_count) {
                max_free_count = free_count;
            }
            free_count = 0;        /* 被有效位阻断，重新开始记录 */
        }
    }
    if (free_count > max_free_count) {
        max_free_count = free_count;
    }
    return max_free_count;
}


#endif // LIBYUC_CONTAINER_BITMAP_H_