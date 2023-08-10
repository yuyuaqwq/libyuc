#ifndef LIBYUC_ALGORITHM_BIT_H_
#define LIBYUC_ALGORITHM_BIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/basic.h>

forceinline static bool BitGet(uint8_t* bit_arr, size_t bit_idx) {
    uint8_t offset = bit_idx & 7/* % 8 */;
    return bit_arr[bit_idx >> 3/* / 8 */] & ((uint8_t)0x80 >> offset);
}
forceinline static void BitSet(uint8_t* bit_arr, size_t bit_idx, bool value) {
    uint8_t offset = bit_idx & 7;
    if (value) {
        bit_arr[bit_idx >> 3] |= (uint8_t)0x80 >> offset;
    } else {
        bit_arr[bit_idx >> 3] &= ~((uint8_t)0x80 >> offset);
    }
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_BIT_H_