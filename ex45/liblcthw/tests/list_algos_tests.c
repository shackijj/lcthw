#include "minunit.h"
#include <lcthw/list.h>
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

char *values[] = {"XXXXX", "1234", "abcd", "xjvef", "NDSS"};

#define NUM_VALUES 500
#define REPEAT 10

List *create_words()
{
    int i = 0;
    List *words = List_create();

    for(i = 0; i < NUM_VALUES; i++) {
        int random = rand() % 5;
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

char *test_list_copy() 
{
    List *A = create_words();
    List *B = create_words();
    List_copy(A, B);

    int rc = strcmp(A->first->value, B->first->value);
    mu_assert(rc == 0, "First values not equal.");
    rc = strcmp(A->last->value, B->last->value);
    mu_assert(rc == 0, "Last values not equal.");
    mu_assert(B->count == A->count, "Counts not equal.");

    List_destroy(A);
    List_destroy(B); 
    return NULL;
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

char *test_bottom_up_merge_sort()
{
    List *words = create_words();
    List_bottom_up_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(words), "Words are not sorted after bottom_up_merge sort.");
    
    List_bottom_up_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(words), "Should be still sorted after bottom_up_merge sort.");

    List_destroy(words);
    return NULL;
}

char *test_list_insert_sorted() 
{
    List *words = List_create();
    List_push(words, "b");
    List_push(words, "d");

    char *min_value = "a";
    char *max_value = "e";
    char *middle_value = "c";
 
    List_insert_sorted(words, min_value, (List_compare)strcmp);
    List_insert_sorted(words, max_value, (List_compare)strcmp);
    List_insert_sorted(words, middle_value, (List_compare)strcmp);

    LIST_FOREACH(words, first, next, cur) {
        if (strcmp(cur->value, "c") == 0) {
            mu_assert(strcmp(cur->prev->value, "b") == 0, "Wrong insert in a middle. Bad prev");
            mu_assert(strcmp(cur->next->value, "d") == 0, "Wrong insert data in a middle. Bad next");
            mu_assert(strcmp(words->first->value, min_value) == 0, "Wrong min value.");
            mu_assert(strcmp(words->last->value, max_value) == 0, "Wrong max value.");
            mu_assert(words->count == 5, "Wrong words count");             
        }
    }
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
    List_merge_sort(words, (List_compare)strcmp);
    List_destroy(words);
}

void merge_bu_sort() {
    List *words = create_words();
    List_bottom_up_merge_sort(words, (List_compare)strcmp);
    List_destroy(words);
}

void get_node() {
    List *words = create_words();
    ListNode *node = List_get_node(words, NUM_VALUES - 1);
    if (!node) log_info("List_get_node returned empty node.");
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
    
    counter = 0;
    gettimeofday(&tval_before, NULL);
    
    while (counter < REPEAT) {
        merge_bu_sort();
        counter++;
    } 
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    log_info("Merge_bottom_up_sort take Time elapsed: %ld.%06ld\n", 
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);
    
    counter = 0;
    gettimeofday(&tval_before, NULL);
 
    while (counter < REPEAT) {
        get_node();
        counter++;
    }
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    log_info("Get_node take Time elapsed: %ld.%06ld\n",
    (long int)tval_result.tv_sec, (long int) tval_result.tv_usec);

    return NULL;
}


char *all_tests()
{
    mu_suite_start();
    mu_run_test(test_list_copy);
    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);
    mu_run_test(test_list_insert_sorted);
    mu_run_test(test_bottom_up_merge_sort);
    mu_run_test(test_timing);
    return NULL;
}

RUN_TESTS(all_tests);
