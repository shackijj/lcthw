#ifndef _lcthw_queue_h
#define _lcthw_queue_h

#include <lcthw/list.h>

typedef struct List Queue;

#define Queue_create() (List_create())

#define Queue_destroy(Q) (List_destroy(Q))

#define Queue_send(Q, V) (List_push(Q, V))

#define Queue_peek(Q) (List_first(Q))

#define Queue_count(Q) (List_count(Q))

#define QUEUE_FOREACH(Q, CUR) LIST_FOREACH(Q, first, next, CUR)

#define Queue_recv(Q) (List_shift(Q))


#endif
