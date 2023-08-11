#ifndef LIBYUC_ALGORITHM_TWO_H_
#define LIBYUC_ALGORITHM_TWO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/basic.h>


#define LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(x) (!((x)&((x)-1)))

/*
* 根据幂求指数
*/
/* 根据2的指数求幂 */
#define LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(exponent) (1 << (exponent))

/* 根据2的幂求指数 */
static forceinline int32_t LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(uint32_t power) {
    int32_t exponent = 0;
    while (power != 0) {
        exponent++;
        power >>= 1;
    }
    return exponent - 1;
}

/*
* 对齐到2的幂
*/
static forceinline int32_t LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(uint32_t num) {
    for (int i = 1; i < sizeof(num) * 8 / 2 + 1; i *= 2) {
        num |= num >> i;
    }
    return num + 1;
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_TWO_H_