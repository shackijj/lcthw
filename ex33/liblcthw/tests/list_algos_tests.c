#include "minunit.h"
#include <lcthw/list.h>
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

char *values[] = {"XXXXX", "1234", "abcd", "xjvef", "NDSS"};

#define NUM_VALUES 5
#define REPEAT 100

List *create_words()
{
    int i = 0;
    List *words = List_create();

    for(i = 0; i < NUM_VALUES; i++) {
        int random = rand() % 5;
        log_info("%s", values[random]);
        List_push(words, values[random]);
    }

    return words;
}

int is_sorted(List *words)
{
    LIST_FOREACH(words, first, next, cur) {
        if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
                return 0;
        }
    }

    return 1;
}

char *test_bubble_sort()
{
    List *words = create_words();
    
    int rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");
    
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");
    
    List_destroy(words);

    words = List_create(words);
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(rc == 0, "Words should be sorted if empty.");

    List_destroy(words);

    return NULL;
}


char *test_merge_sort()
{
    List *words = create_words();

    List *res = List_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");
    
    List *res2 = List_merge_sort(res, (List_compare)strcmp);
    mu_assert(is_sorted(res2), "Should be stilee sorted after merge sort.");
    List_destroy(res2);
    List_destroy(res);

    List_destroy(words);
    return NULL;
}

void bubble_sort() {
    List *words = create_words();
    List_bubble_sort(words, (List_compare)strcmp);
    List_destroy(words);
}

void merge_sort() {
    List *words = create_words();
    List_bubble_sort(words, (List_compare)strcmp);
    List_destroy(words);
}

char *test_timing()
{
    int counter = 0;
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
    
    while (counter < REPEAT) {
        bubble_sort();
        counter++;
    } 
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    log_info("Bubble_sort take Time elapsed: %ld.%06ld\n", 
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);

    counter = 0;
    gettimeofday(&tval_before, NULL);
    
    while (counter < REPEAT) {
        merge_sort();
        counter++;
    } 
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    log_info("Merge_sort take Time elapsed: %ld.%06ld\n", 
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);



    

    return NULL;
}


char *all_tests()
{
    mu_suite_start();
    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);
    mu_run_test(test_timing);
    return NULL;
}

RUN_TESTS(all_tests);
