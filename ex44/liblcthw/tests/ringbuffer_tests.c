#include "minuint.h"
#include <lcthw/ringbuffer.h>

#define BUFFER_LEN 10

char *test_data = "fooboozoo";
RingBuffer *buff;

char *test_create()
{
    buff = RingBuffer_create(10);
    mu_assert(buff != NULL, "Can't create buffer");

    return NULL;
}

