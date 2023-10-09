#ifndef LIBYUC_ALGORITHM_TWO_H_
#define LIBYUC_ALGORITHM_TWO_H_

#include <libyuc/basic.h>


#define LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(x) (!((x)&((x)-1)))

/*
* ��������ָ��
*/
/* ����2��ָ������ */
#define LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(exponent) (1 << (exponent))

/* ����2������ָ�� */
static forceinline size_t LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(size_t power) {
    int32_t exponent = 0;
    while (power != 0) {
        exponent++;
        power >>= 1;
    }
    return exponent - 1;
}

/*
* ���뵽2����
*/
static forceinline size_t LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(size_t num) {
    for (int i = 1; i < sizeof(num) * 8 / 2 + 1; i *= 2) {
        num |= num >> i;
    }
    return num + 1;
}

#endif // LIBYUC_ALGORITHM_TWO_H_