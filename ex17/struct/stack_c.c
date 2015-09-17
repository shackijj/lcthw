#include <stdio.h>
#include <stdlib.h>

struct Stack {
    int size;
    int *array;
    int **ptr;
};

struct Stack *Stack_create()
{
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->array = malloc(sizeof(int));
    stack->size = 0;
    stack->ptr = &stack->array;
    return stack;
}

void Stack_delete(struct Stack *stack)
{
    if(stack) {
        free(stack->array);
        free(stack);
    }
}

void Push(struct Stack *stack, int i) 
{
    stack->size++;
    stack = realloc(stack->array, sizeof(int) * stack->size);

    stack->ptr  = &stack->array + stack->size;
}

void Pop(struct Stack *stack) {

    //int i;
    //if(!stack->size) return &stack->array;
    stack->size--;
    printf("Stack size: %d", stack->size);
    if(stack->size) {
        stack = realloc(stack->array, sizeof(int) * stack->size);
    }
    printf("Pushed: %p\n", stack->ptr);
    stack->ptr = &stack->array - stack->size;
    //return i;
}

int main(int argc, char *argv[])
{
    struct Stack *s = Stack_create();

    Push(s, 1);
    Pop(s);
    Stack_delete(s);
    return 0;
}
