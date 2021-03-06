#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lcthw/dbg.h>
#include <lcthw/posixringbuffer.h>
#include <sys/mman.h>

PosixRingBuffer *PosixRingBuffer_create(size_t ring_size)
{
    PosixRingBuffer *buffer = calloc(1, sizeof(PosixRingBuffer));
    check_mem(buffer);

    buffer->start = 0;
    buffer->end = 0;
    // initial size of buffer
    buffer->size = ring_size;
    buffer->length = (int) sysconf(_SC_PAGE_SIZE) * SYSPAGE_M;    

    // the underlying buffer‘s length must then equal some multiple of the system’s page size.
    // Ok, then multiple of 2 
    buffer->mapping = mmap(NULL, buffer->length, 
        PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    check(buffer->mapping != MAP_FAILED, "mmap failed.");

    buffer->buffer =  (char *)mmap(buffer->mapping, buffer->size, 
        PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);    

    check(buffer->mapping != MAP_FAILED, "mmap failed.");

    return buffer;
error:
    return NULL;
}

void PosixRingBuffer_destroy(PosixRingBuffer *buffer)
{
    if(buffer) {
        munmap(buffer->buffer, buffer->size);
        munmap(buffer->mapping, sysconf(_SC_PAGE_SIZE) * SYSPAGE_M);
        free(buffer);
    }
}

int PosixRingBuffer_write(PosixRingBuffer *buffer, char *data, int length)
{
    if(PosixRingBuffer_available_data(buffer) == 0) {
        buffer->start = buffer->end = 0;
    }

    check(length <= PosixRingBuffer_available_space(buffer),
            "Not enough space: %d request, %d available",
            PosixRingBuffer_available_data(buffer), length);

    check(PosixRingBuffer_ends_at(buffer) + length <= buffer->buffer + buffer->length,
        "Write out of buffer.");
    void *result = memcpy(PosixRingBuffer_ends_at(buffer), data, length);
    check(result != NULL, "Failed to write data into buffer.");

    PosixRingBuffer_commit_write(buffer, length);

    return length;
error:
    return -1;
}

int PosixRingBuffer_read(PosixRingBuffer *buffer, char *target, int amount)
{
    check_debug(amount <= PosixRingBuffer_available_data(buffer),
            "Not enough in the buffer: has %d, needs %d",
            PosixRingBuffer_available_data(buffer), amount);

    check(PosixRingBuffer_starts_at(buffer) + amount <= buffer->buffer + buffer->length, 
        "Reading outside buffer."); 
    void *result = memcpy(target, PosixRingBuffer_starts_at(buffer), amount);
    check(result != NULL, "Failed to write buffer into data.");

    PosixRingBuffer_commit_read(buffer, amount);

    if(buffer->end == buffer->start) {
        buffer->start = buffer->end = 0;
    }

    return amount;
error:
    return -1;
}

bstring PosixRingBuffer_gets(PosixRingBuffer *buffer, int amount)
{
    check(amount > 0, "Need more than 0 for gets, you gave: %d ", amount);
    check_debug(amount <= PosixRingBuffer_available_data(buffer),
            "Not enough in the buffer.");

    bstring result = blk2bstr(PosixRingBuffer_starts_at(buffer), amount);
    check(result != NULL, "Failed to create gets result.");
    check(blength(result) == amount, "Wrong result length.");

    PosixRingBuffer_commit_read(buffer, amount);
    assert(PosixRingBuffer_available_data(buffer) >= 0 && "Error in read commit.");

    return result;
error:
    return NULL;
}
