#include <lcthw/bstree.h>
#include <lcthw/bstrlib.h>
#include "minunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("xest data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

BSTree *map = NULL;

char *test_create()
{

    map = BSTree_create(NULL, NULL);

    BSTree_set(map, &test1, &expect1);
    BSTree_set(map, &test2, &expect2);
    BSTree_set(map, &test3, &expect3);

    return NULL;
}

char *test_destroy()
{
    BSTree_destroy(map);
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
