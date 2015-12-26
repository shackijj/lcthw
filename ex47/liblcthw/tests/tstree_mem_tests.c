#include <lcthw/tstree.h>
#include <lcthw/bstrlib.h>
#include "minunit.h"
#include <string.h>

char *key1 = "test 1";
char *key2 = "test 2";
char *key3 = "test 3";

char *value1 = "test data 1";
char *value2 = "test data 2";
char *value3 = "test data 3";

TSTree *map = NULL;

char *test_create()
{

    map = TSTree_insert(map, key1, strlen(key1), value1);
    map = TSTree_insert(map, key2, strlen(key2), value2);
    map = TSTree_insert(map, key3, strlen(key3), value3);

    return NULL;
}

char *test_destroy()
{
    TSTree_destroy(map);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
