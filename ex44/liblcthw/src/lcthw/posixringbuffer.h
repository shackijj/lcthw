#ifndef _lcthw_PosixRingBuffer_h
#define _lcthw_PosixRingBuffer_h

#include <lcthw/bstrlib.h>

typedef struct {
    char *buffer;
    char *ul_buffer;
    int length;
    int start;
    int end;
} PosixRingBuffer;


PosixRingBuffer *PosixRingBuffer_create(int length);

void PosixRingBuffer_destroy(PosixRingBuffer *buffer);

int PosixRingBuffer_read(PosixRingBuffer *buffer, char *target, int amount);

int PosixRingBuffer_write(PosixRingBuffer *buffer, char *data, int length);

int PosixRingBuffer_empty(PosixRingBuffer *buffer);

int PosixRingBuffer_full(PosixRingBuffer *buffer);

int PosixRingBuffer_available_data(PosixRingBuffer *buffer);

int PosixRingBuffer_available_space(PosixRingBuffer *buffer);

bstring PosixRingBuffer_gets(PosixRingBuffer *buffer, int amount);

#define PosixRingBuffer_available_data(B) (((B)->end + 1) % (B)->length - (B)->start - 1)

#define PosixRingBuffer_available_space(B) ((B)->length - (B)->end - 1)

#define PosixRingBuffer_full(B) (PosixRingBuffer_available_data((B)) - (B)->length == 0)

#define PosixRingBuffer_empty(B) (PosixRingBuffer_available_data((B)) == 0)

#define PosixRingBuffer_puts(B, D) PosixRingBuffer_write((B), bdata((D)), blength((D)))

#define PosixRingBuffer_get_all(B) PosixRingBuffer_gets((B), PosixRingBuffer_available_data((B)))

#define PosixRingBuffer_starts_at(B) ((B)->buffer + (B)->start)

#define PosixRingBuffer_ends_at(B) ((B)->buffer + (B)->end)

#define PosixRingBuffer_commit_read(B, A) ((B)->start = ((B)->start + (A)) % (B)->length)

#define PosixRingBuffer_commit_write(B, A) ((B)->end = ((B)->end + (A)) % (B)->length)

#endif
