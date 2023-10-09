/*
* 算法来源：网络
*/

#ifndef LIBYUC_ALGORITHM_HASH_CODE_H_
#define LIBYUC_ALGORITHM_HASH_CODE_H_

#include <libyuc/basic.h>

uint32_t HashCode_hashint(uint32_t a);

uint32_t HashCode_jenkins_hash(const void* buf1_, size_t size);

uint32_t HashCode_fnv1a_hash(const void* data, size_t size);

uint32_t HashCode_adler32_hash(const void* data, size_t size);

uint32_t HashCode_djb_hash(const void* data, size_t size);

uint32_t HashCode_murmurhash(const void* key_, size_t len);

uint32_t HashCode_murmur3_fmix32(uint32_t h);

uint64_t HashCode_murmur3_fmix64(uint64_t k);


#endif // LIBYUC_ALGORITHM_HASH_CODE_H_