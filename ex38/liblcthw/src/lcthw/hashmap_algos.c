#include <lcthw/hashmap_algos.h>
#include <lcthw/bstrlib.h>

// settings taken from
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_fnv1a_hash(void *data)
{
    bstring s = (bstring) data;
    uint32_t hash = FNV_OFFSET_BASIS;
    int i = 0;

    for(i = 0; i < blength(s); i++) {
        hash ^= bchare(s, i, 0);
        hash *= FNV_PRIME;
    }

    return hash;
}

const int MOD_ADLER = 65521;

uint32_t Hashmap_adler32_hash(void *data)
{
    bstring s = (bstring)data;
    uint32_t a = 1, b = 0;
    int i = 0;

    for(i = 0; i < blength(s); i++) {
        a = (a + bchare(s, i, 0)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

uint32_t Hashmap_djb_hash(void *data)
{
    bstring s = (bstring)data;
    uint32_t hash = 5381;
    int i = 0;

    for(i = 0; i < blength(s); i++) {
        hash = ((hash << 5) + hash) + bchare(s, i, 0);
    }
    return hash;
}

uint32_t Hashmap_jenkins_hash(void *data)
{
    size_t len = blength((bstring)data);
    char *key = bdata((bstring)data);
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i)
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

uint32_t Hashmap_pjw_hash(void *data)
{
    bstring s = (bstring) data;

    uint32_t hash = 0;
    uint32_t test = 0;
    int i = 0;

    for(i = 0; i < blength(s); i++) {
        hash = (hash << 4) + bchare(s, i, 0);

        if((test = hash & 0xf0000000) != 0) {
            hash = ((hash ^ (test >> 24)) & (0xfffffff));
        }
    }

    return hash;
}
