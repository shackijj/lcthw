#ifndef _lcthw_stack_h
#define _lcthw_stack_h

#include <lcthw/list.h>

typedef struct List Stack;

#define Stack_create() (List_create())

#define Stack_destroy(LIST) (List_destroy(LIST))

#define Stack_push(LIST, NODE) (List_unshift(LIST, NODE))

#define Stack_pop(LIST) (List_shift(LIST))

#define Stack_peek(LIST) (List_first(LIST))

#define Stack_count(LIST) (List_count(LIST))

#define STACK_FOREACH(LIST, CUR) LIST_FOREACH(LIST, first, next, CUR)

#endif
