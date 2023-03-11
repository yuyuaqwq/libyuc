/*
* 算法来源：网络
*/

#ifndef CUTILS_ALGORITHM_HASH_MAP_H_
#define CUTILS_ALGORITHM_HASH_MAP_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif

uint32_t Hashmap_jenkins_hash(const void* buf1_, size_t size);

uint32_t Hashmap_fnv1a_hash(const void* data, size_t size);

uint32_t Hashmap_adler32_hash(const void* data, size_t size);

uint32_t Hashmap_djb_hash(const void* data, size_t size);

uint32_t Hashmap_murmurhash(const void* key_, size_t len);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_HASH_MAP_H_