/*
* 算法来源：网络
*/

#ifndef CUTILS_ALGORITHM_HASH_CODE_H_
#define CUTILS_ALGORITHM_HASH_CODE_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

uint32_t HashCode_hashint(uint32_t a);

uint32_t HashCode_jenkins_hash(const void* buf1_, size_t size);

uint32_t HashCode_fnv1a_hash(const void* data, size_t size);

uint32_t HashCode_adler32_hash(const void* data, size_t size);

uint32_t HashCode_djb_hash(const void* data, size_t size);

uint32_t HashCode_murmurhash(const void* key_, size_t len);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_HASH_CODE_H_