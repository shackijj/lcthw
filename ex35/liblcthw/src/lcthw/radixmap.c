#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max)
{
    RadixMap *map = calloc(sizeof(RadixMap), 1);
    check_mem(map);

    map->contents = calloc(sizeof(RMElement), max + 1);
    check_mem(map->contents);

    map->temp = calloc(sizeof(RMElement), max + 1);
    check_mem(map->temp);

    map->max = max;
    map->end = 0;
    map->min_value = 0;
    map->max_key = 0;

    return map;
error:
    return NULL;
}

void RadixMap_destroy(RadixMap *map)
{
    if(map) {
        free(map->contents);
        free(map->temp);
        free(map);
    }
}

#define ByteOf(x,y) (((uint8_t *)x)[(y)])

static inline void radix_sort(short offset, uint64_t start, uint64_t max, uint64_t *source, uint64_t *dest)
{
    uint64_t count[256] = {0};
    uint64_t *cp = NULL;
    uint64_t *sp = NULL;
    uint64_t *end = NULL;
    uint64_t s = 0;
    uint64_t c = 0;

    //log_info("Start with: %u Max: %u", (unsigned int)start, (unsigned int)max);
    //count occurences of every byte value
    for(sp = source + start, end = source + max; sp < end; sp++) {
        count[ByteOf(sp, offset)]++;
    }
 
    //transform count into index by summing elements and storing into same array
    for(s = start, cp = count, end = count + 256; cp < end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }

    // fill dest with the right values in right place
    for (sp = source + start, end = source + max; sp < end; sp++) {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}

void RadixMap_sort(RadixMap *map, size_t start)
{
    uint64_t *source = &map->contents[0].raw;
    uint64_t *temp = &map->temp[0].raw;

    // it saves few loops sometime
    if (map->max_key > (UINT32_MAX >> 7)) radix_sort(0, start, map->end, source, temp);
    radix_sort(1, start, map->end, temp, source);
    radix_sort(2, start, map->end, source, temp);
    radix_sort(3, start, map->end, temp, source);
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find)
{
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high) {
        int middle = low + (high - low) / 2;
        uint32_t key = data[middle].data.key;
        
        if (to_find < key) {
            high = middle - 1;
        } else if (to_find > key) {
            low = middle + 1;
        } else {
            return &data[middle];
        }
    }

    return NULL;
}

size_t RadixMap_find_minimum(RadixMap *map, uint32_t to_find)
{   
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while(low <= high) {
        int middle = low + (high - low) / 2;
        uint32_t key = data[middle].data.key;
        
        if (to_find < key) {
            high = middle - 1;
        } else if (to_find > key) {
            return middle + 1;
        } else {
            return middle;
        }
    }

    return 0;

}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value)
{
    check(key < UINT32_MAX, "Key can't be equal UINT32_MAX.");

    RMElement element = {.data = {.key = key, .value = value}};
    check(map->end + 1 < map->max, "RadixMap is full.");

    map->contents[map->end++] = element;
    size_t minimum = RadixMap_find_minimum(map, key);
 
    //log_info("Key to add: %u, Value to add: %u, MaxKey: %u", key, value, map->max_key);
    //RadixMap_sort(map, minimum);
 
    if(map->max_key < key) {
        map->max_key = key;
    } else {
        RadixMap_sort(map, minimum);
    }

    return 0;
error:
    return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el)
{
    check(map->end > 0, "There is nothing to delete.");
    check(el != NULL, "Can't delete a NULL element.");

    el->data.key = UINT32_MAX;

    if(map->end > 1) {
        RadixMap_sort(map, 0);
    }

    map->end--;

    return 0;
error:
    return -1;
}
