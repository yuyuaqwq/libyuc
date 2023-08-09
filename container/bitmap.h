/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_BITMAP_H_
#define LIBYUC_CONTAINER_BITMAP_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 位图
*/
#define LIBYUC_CONTAINER_BIT_MAP_DECLARATION(bit_map_type_name, offset_type) \
    typedef struct _##bit_map_type_name##Bitmap { \
        offset_type count; \
        uint8_t byte_arr[]; \
    } bit_map_type_name##Bitmap; \

#define LIBYUC_CONTAINER_BIT_MAP_DEFINE(bit_map_type_name, offset_type) \
    /*
    * 必须是8的倍数
    */ \
    void bit_map_type_name##BitmapInit(bit_map_type_name##Bitmap* bitmap, offset_type bit_count) { \
        offset_type byte_count = bit_count / 8 + (bit_count % 8 ? 1 : 0); \
        for (offset_type i = 0; i < byte_count; i++) { \
            bitmap->byte_arr[i] = 0; \
        } \
    } \
    bool bit_map_type_name##BitmapGet(bit_map_type_name##Bitmap* bitmap, offset_type bit_idx) { \
        uint8_t offset = bit_idx % 8; \
        return bitmap->byte_arr[bit_idx / 8] & ((uint8_t)0x80 >> offset); \
    } \
    void bit_map_type_name##BitmapSet(bit_map_type_name##Bitmap* bitmap, offset_type bit_idx, bool value) { \
        uint8_t offset = bit_idx % 8; \
        if (value) { \
            bitmap->byte_arr[bit_idx / 8] |= (uint8_t)0x80 >> offset; \
        } else { \
            bitmap->byte_arr[bit_idx / 8] &= ~((uint8_t)0x80 >> offset); \
        } \
    } \
    void bit_map_type_name##BitmapSetMultiple(bit_map_type_name##Bitmap* bitmap, offset_type bit_idx, offset_type bit_count, bool val) { \
        offset_type byte_idx = bit_idx / 8; \
        bit_idx += (bit_count / 8) * 8; \
        while (bit_count > 8 && byte_idx < bitmap->count) { \
            bitmap->byte_arr[byte_idx] = (val ? 0xff : 0); \
            bit_count--; \
            byte_idx++; \
        } \
        while (bit_count > 0 && bit_idx < (bitmap->count * 8)) { \
            bit_map_type_name##BitmapSet(bitmap, bit_idx, val); \
            bit_count--; \
            bit_idx++; \
        } \
    } \
    offset_type bit_map_type_name##BitmapFindBit(bit_map_type_name##Bitmap* bitmap, offset_type bit_start, bool val) { \
        offset_type bit_end = bit_start % 8; \
        if (bit_end > 0) { \
            bit_end = bit_start - bit_end + 8; \
        } else { \
            bit_end = bit_start; \
        } \
            offset_type byte_start = bit_start / 8; \
        if (bit_end > bit_start) { \
            byte_start++; \
        } \
        /* 先单独比较前面的位 */ \
        while (bit_start < bit_end) { \
            if (bit_map_type_name##BitmapGet(bitmap, bit_start) == val) { \
                return bit_start; \
            } \
            bit_start++; \
        } \
        offset_type byte_end = bitmap->count; \
        /* 再进行字节比较 */ \
        while (byte_start < byte_end) { \
            if (bitmap->byte_arr[byte_start] != (val ? 0 : 0xff)) { \
                break; \
            } \
            byte_start++; \
        } \
        if (byte_start == byte_end) { \
            return -1; \
        } \
        offset_type bit_off = 0; \
        uint8_t byte = bitmap->byte_arr[byte_start]; \
        /* 不断移位有效位，并比较，直到找到第一个有/无效位(从左往右) */ \
        while (((uint8_t)(0x80 >> bit_off) & byte) == (val ? 0 : (0x80 >> bit_off))) { \
            bit_off++; \
        } \
        bit_start = (byte_start * 8 + bit_off); \
        return bit_start; \
    } \
    offset_type bit_map_type_name##BitmapAlloc(bit_map_type_name##Bitmap* bitmap, offset_type bit_count) { \
        offset_type bit_start = 0; \
        bit_start = bit_map_type_name##BitmapFindBit(bitmap, bit_start, false); \
        if (bit_start == -1) { \
            return -1; \
        } \
        if (bit_count == 1) { \
            bit_map_type_name##BitmapSet(bitmap, bit_start, true); \
            return bit_start; \
        } \
        offset_type bit_end = (bitmap->count * 8) - bit_start;        /* 剩余可检查位数 */ \
        offset_type bit_cur = bit_start; \
        offset_type free_count = 1; \
        /* 简化实现的算法，逐位比较，性能较低 */ \
        while (bit_end-- > 0) { \
            if (bit_map_type_name##BitmapGet(bitmap, ++bit_cur) == false) { \
                free_count++; \
            } else { \
                free_count = 0;        /* 被有效位阻断，重新开始记录 */ \
            } \
            if (bit_count == free_count) { \
                bit_start = bit_cur - bit_count + 1; \
                bit_map_type_name##BitmapSetMultiple(bitmap, bit_start, bit_count, true); \
                return bit_start; \
            } \
        } \
        return -1; \
    } \
    void bit_map_type_name##BitmapFree(bit_map_type_name##Bitmap* bitmap, offset_type bit_idx, offset_type bit_count) { \
        bit_map_type_name##BitmapSetMultiple(bitmap, bit_idx, bit_count, false); \
    } \
    offset_type bit_map_type_name##BitmapGetMaxFreeCount(bit_map_type_name##Bitmap* bitmap) { \
        offset_type bit_start = 0; \
        bit_start = bit_map_type_name##BitmapFindBit(bitmap, bit_start, false); \
        if (bit_start == -1) { \
            return 0; \
        } \
        offset_type bit_end = (bitmap->count * 8) - bit_start;        /* 剩余可检查位数 */ \
        offset_type bit_cur = bit_start; \
        offset_type free_count = 1; \
        offset_type max_free_count = 1; \
        /* 简化实现的算法，逐位比较，性能较低 */ \
        while (bit_end-- > 0) { \
            if (bit_map_type_name##BitmapGet(bitmap, ++bit_cur) == false) { \
                free_count++; \
            } \
            else { \
                if (free_count > max_free_count) { \
                    max_free_count = free_count; \
                } \
                free_count = 0;        /* 被有效位阻断，重新开始记录 */ \
            } \
        } \
        if (free_count > max_free_count) { \
            max_free_count = free_count; \
        } \
        return max_free_count; \
    } \

LIBYUC_CONTAINER_BIT_MAP_DECLARATION(, size_t)

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_BITMAP_H_