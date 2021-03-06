#include "minunit.h"
#include <lcthw/darray_algos.h>
#include <time.h>

// Variables for performance test. N - number of elements in DArray for the first repeat.
// It will be multiplied by 10 until it's less than MAX_N
#define MAX_N 100
int N = 10;

int testcmp(char **a, char **b)
{
    return strcmp(*a, *b);
}

DArray *create_words()
{
    DArray *result = DArray_create(0, N);
    char *words[] = {"asdfasfd", "werwar", "13234", "asdfasdf", "oioj"};
    int i = 0;
    int rand_index = 0;
    
    for (i = 0; i < N; i++) {
        rand_index = rand() % 5;        
        DArray_push(result, words[rand_index]);
    }

    return result;
}

int is_sorted(DArray *array)
{
    int i = 0;

    for(i = 0; i < DArray_count(array) - 1; i++) {
        if(strcmp(DArray_get(array, i), DArray_get(array, i+1)) > 0) {
            return 0;
        }
    }

    return 1;
}

char *run_sort_test(int (*func)(DArray *, DArray_compare), const char *name)
{

    DArray *words = create_words();
    mu_assert(!is_sorted(words), "Words should start not sorted.");
    
    debug("---Testing %s sorting algorithm.", name);
    int rc = func(words, (DArray_compare)testcmp);
    mu_assert(rc == 0, "sort_failed");
    mu_assert(is_sorted(words), "didn't sort it");

    DArray_destroy(words);

    return NULL;

} 

char *test_gnu_qsort()
{
    return run_sort_test(DArray_gnu_qsort, "gnu_qsort");
}

char *test_qsort()
{
    return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort()
{
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort()
{
    return run_sort_test(DArray_mergesort, "mergesort");
}

char *test_darray_sort_add_and_darray_find()
{
    DArray *words = create_words();
    int old_end = words->end;
    char *test_word = "zuzuzuz";
    int rc = DArray_sort_add(words, (DArray_compare)testcmp, test_word); 
    mu_assert(rc == 0, "DArray_sort_add_failed");
    mu_assert(old_end + 1 == words->end, "The new element wasn't added to DArray.");
    mu_assert(is_sorted(words), "didnt sort it");   
    
    rc = DArray_find(words, (DArray_compare)testcmp, test_word);

    mu_assert(rc == words->end - 1, "DArray_find found wrong element.");

    DArray_destroy(words);       

    return NULL;
}

char * all_tests()
{
    mu_suite_start();
    srand(time(NULL));
    init_timer();

    // units
    mu_run_test(test_darray_sort_add_and_darray_find);

    // performance
    while (N < MAX_N) { 
        log_info("N: %d", N);
        time_it_with_args(mu_run_test(test_gnu_qsort), "gnu_qsort");
        time_it_with_args(mu_run_test(test_qsort), "qsort");
        time_it_with_args(mu_run_test(test_heapsort), "heapsort");
        time_it_with_args(mu_run_test(test_mergesort), "mergesort");
        N = N * 10;
    }
    
    return NULL;
}

RUN_TESTS(all_tests);

