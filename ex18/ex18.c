#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

void die(const char *message, int *numbers)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    
    if(numbers) free(numbers);

    exit(1);
}

typedef int (*compare_cb)(int a, int b);

typedef int *(*sorting_cb) (int *numbers, int count, compare_cb cmp);

int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    //Never trust the input
    assert(numbers != NULL);    

    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));
    int step = 0;

    if(!target) die("Memory error", numbers);
    if(count == 1) return target;

    //Copy numbers to target
    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
            step++;
        }
        // Speed up by insreasing iterations.
        count--;
    }
    printf("Bubble sort done in %d steps\n", step);
    return target;
}

int *insertion_sort(int *numbers, int count, compare_cb cmp)
{
    assert(numbers != NULL);
    
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));
    int step = 0;     
    if(!target) die("Memory error", numbers);

    memcpy(target, numbers, count * sizeof(int));



    for(i = 1; i < count; i++) {
        j = i;
        while(j > 0 && cmp(target[j-1], target[j]) > 0) {
            temp = target[j];
            target[j] = target[j-1];
            target[j-1] = temp;
            j--;
            step++;
        }
    }

    printf("Insertion sort done in %d steps\n", step);
    return target;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}


void test_sorting(int *numbers, int count, sorting_cb srt, compare_cb cmp)
{
    int i = 0;
    int *sorted = srt(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.", numbers);

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }

    printf("\n");

    free(sorted);

    printf("\n");
}

int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6", NULL);
    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;

    int *numbers = malloc(count *sizeof(int));
    if(!numbers) die("Memory error.", NULL);

    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, bubble_sort, sorted_order);

    test_sorting(numbers, count, insertion_sort, sorted_order);

    test_sorting(numbers, count, insertion_sort, reverse_order);

    free(numbers);

    return 0;
}
