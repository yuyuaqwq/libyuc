/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "HashMap.h"

/*
* 主要哈希算法来源：笨方法学C
*/

/**
 * Jenkins hash
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
uint32_t Hashmap_jenkins_hash(const void* buf1_, size_t size) {
    char* key = (char*)buf1_;
    uint32_t hash = 0;
    uint32_t i = 0;

    for (hash = i = 0; i < size; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}



// settings taken from
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;
uint32_t Hashmap_fnv1a_hash(const void* data, size_t size)
{
    char* s = (char*)data;
    uint32_t hash = FNV_OFFSET_BASIS;
    int i = 0;

    for (i = 0; i < size; i++) {
        hash ^= s[i];
        hash *= FNV_PRIME;
    }

    return hash;
}


const int MOD_ADLER = 65521;
uint32_t Hashmap_adler32_hash(const void* data, size_t size)
{
    char* s = (char*)data;
    uint32_t a = 1, b = 0;
    int i = 0;

    for (i = 0; i < size; i++)
    {
        a = (a + s[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}


uint32_t Hashmap_djb_hash(const void* data, size_t size)
{
    char* s = (char*)data;
    uint32_t hash = 5381;
    int i = 0;

    for (i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + s[i]; /* hash * 33 + c */
    }

    return hash;
}



/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014-2022 joseph werle <joseph.werle@gmail.com>
 */
uint32_t Hashmap_murmurhash(const void* key_, uint32_t len/*, uint32_t seed*/) {
    uint32_t seed = 0;

    char* key = (char*)key_;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    uint8_t* d = (uint8_t*)key; // 32 bit extract from `key'
    const uint32_t* chunks = NULL;
    const uint8_t* tail = NULL; // tail - last 8 bytes
    int i = 0;
    int l = len / 4; // chunk length

    h = seed;

    chunks = (const uint32_t*)(d + l * 4); // body
    tail = (const uint8_t*)(d + l * 4); // last 8 byte chunk of `key'

    // for each 4 byte chunk of `key'
    for (i = -l; i != 0; ++i) {
        // next 4 byte chunk of `key'
        k = chunks[i];

        // encode next 4 byte chunk of `key'
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        // append to hash
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    k = 0;

    // remainder
    switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
    case 2: k ^= (tail[1] << 8);

    case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
    }

    h ^= len;

    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);

    return h;
}
