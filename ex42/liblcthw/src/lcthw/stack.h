#ifndef _lcthw_stack_h
#define _lcthw_stack_h

#include <lcthw/darray.h>

#define STACK_SIZE 30

typedef struct DArray Stack;

typedef struct StackValue {
    void *value;
} StackValue;

#define Stack_create() (DArray_create(sizeof(char *), STACK_SIZE))

#define Stack_destroy(S) (DArray_destroy(S))

#define Stack_push(S, NODE) (DArray_push(S, NODE))

#define Stack_pop(S) (DArray_pop(S))

#define Stack_peek(S) (DArray_last(S))

#define Stack_count(S) (DArray_count(S))

#define STACK_FOREACH(S, CUR) int SC = 0; StackValue H = {.value=S->contents[0]}; StackValue *CUR = &H;\
    for(SC = 0; i < DArray_end(S); CUR->value=S->contents[++SC])
#endif
